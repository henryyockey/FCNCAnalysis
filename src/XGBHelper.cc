#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <fstream>
#include <stdexcept>

#include <iostream>

#include "XGBHelper.h"
#include "json.hpp"

//using json = nlohmann::json;

/*struct TreeNode {
    std::string split;	 		// name of feature eg lep1Pt
    float split_condition;		// value that branch splits at
    float leaf;	 			// for end nodes -- the value of the leaf
    bool isLeaf;	 		// True if is leaf
    std::vector<TreeNode> children; 	//
};


class Model {
    private:

	std::vector<TreeNode> baseNodes;	//base nodes of tree -- can be recursively searched
	TreeNode parseNode(const json& nodeJson);
	float predict(const TreeNode& node, const std::map<std::string, float>& v); // prediction from one tree
    public:
	// constructor
	Model(const std::string& filename);

	// prediction
	float predict_proba(const std::map<std::string, float>& v);
};
*/

Model::Model(const std::string& filename)
{
    std::ifstream file(filename);
    json treesJson;
    file >> treesJson;

    for (const auto& treeJson : treesJson) {
	baseNodes.push_back(parseNode(treeJson));
    }
}

TreeNode Model::parseNode(const json& nodeJson) 
{
    TreeNode node;

    //add children recursively
    if (nodeJson.find("children") != nodeJson.end()) {
    	node.split          = nodeJson["split"];
        node.split_condition= nodeJson["split_condition"];
	node.isLeaf 	    = false;
	for (const json& childJson : nodeJson["children"]) {
	    node.children.push_back(parseNode(childJson));
	}
    } else {
	node.isLeaf = true;
	node.leaf = nodeJson["leaf"];
    }

    return node;
}

float Model::predict(const TreeNode& node, const std::map<std::string, float>& features)
{
    if (node.isLeaf) {
	return node.leaf;
    } else {
    	auto it = features.find(node.split);
	if (it != features.end()) {
	    if (it->second < node.split_condition) {
	    	return predict(node.children[0], features); // node.yes
	    } else {
	    	return predict(node.children[1], features); // node.no
	    }
	} else {
	    throw std::runtime_error("Key '" + node.split + "' not found in features.");
	}
    }
}

float Model::predict_proba(const std::map<std::string, float>& features)
{
    float sum = 0;
    for (const auto& baseNode : baseNodes)
    {
	sum += predict(baseNode, features);
    }

    return 1 / (1 + exp(-sum));
}

//using MultiXGB = std::map<std::string, std::map<std::string,std::map<std::string,std::unique_ptr<Model>>>>;

MultiXGB loadModels(const std::vector<std::string>& regions, 
			const std::vector<std::string>& channels, 
			const std::vector<std::string>& wilsCoefs,
                        const std::string directory)
{
    MultiXGB multi;
    for (const auto& reg : regions)
    {
	for (const auto& ch : channels)
	{
	    for (const auto& wc : wilsCoefs) 
	    {
		std::string filename = directory+reg+"_"+ch+"_"+wc+".json";
                std::cout << "Loading XGB model at " << filename << std::endl;
		multi[reg][ch][wc] = std::make_unique<Model>(filename);
	    }
	}
    }
    return multi;
}

std::unique_ptr<Model> loadModel(std::string region, std::string channel, std::string wilson, std::string directory)
{
    std::string filename = directory+region+"_"+channel+"_"+wilson+".json";
//    std::cout << "Loading XGB model at " << filename << std::endl;
    return std::make_unique<Model>(filename);
}

bool modelLoaded(std::string region, std::string channel, std::string wilson, MultiXGB& multi)
{
   auto iR = multi.find(region);
   if (iR != multi.end()) 
   {
      auto iC = iR->second.find(channel);
      if (iC != iR->second.end())
      {
         auto iW = iC->second.find(wilson);
         if (iW != iC->second.end())
         {
            return true;
         }
      }
   }
   return false;
}

/*int main() 
{

    std::map<std::string, float> features;

    features["lep1Pt"] = 112.795319;
    features["lep1Eta"] = 112.795319;
    features["lep1Phi"] = -0.446716;
    features["lep2Pt"] = 77.550911;
    features["lep2Eta"] = 77.550911;
    features["lep2Phi"] = -1.462891;
    features["jet1Pt"] = 78.937500;
    features["jet1Eta"] = -1.463867;
    features["jet1Phi"] = 1.389160;
    features["llM"] = 91.076134	;
    features["llPt"] = 167.182693;
    features["llDr"] = 1.016915;
    features["llDphi"] = 1.016174;
    features["Met"] = 12.069560;
    features["MetPhi"] = -2.430176; 
    features["njet"] = 1;
    features["nbjet"] = 1;
    features["nVtx"] = 12;

    std::vector<std::string> reg = {"test"};
    std::vector<std::string> ch  = {"test"};
    std::vector<std::string> wc  = {"ctp", "ctG"};

    MultiXGB multi = loadModels(reg, ch, wc);

    std::cout << multi["test"]["test"]["ctp"]->predict_proba(features) << std::endl 
    	      << multi["test"]["test"]["ctG"]->predict_proba(features) << std::endl;
    return 0;
}*/
