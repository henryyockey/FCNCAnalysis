#ifndef XGBHELPER_H
#define XGBHELPER_H

#include "json.hpp"
#include <vector>
#include <map>
#include <string>

using json = nlohmann::json;

struct TreeNode {
    std::string split;                  // name of feature eg lep1Pt
    float split_condition;              // value that branch splits at
    float leaf;                         // for end nodes -- the value of the leaf
    bool isLeaf;                        // True if is leaf
    std::vector<TreeNode> children;     //
};


class Model {
    private:

        std::vector<TreeNode> baseNodes;        //base nodes of tree -- can be recursively searched
        TreeNode parseNode(const json& nodeJson);
        float predict(const TreeNode& node, const std::map<std::string, float>& v); // prediction from one tree
    public:
        // constructor
        Model(const std::string& filename);
        
        // prediction
        float predict_proba(const std::map<std::string, float>& v);
};

using MultiXGB = std::map<std::string, std::map<std::string,std::map<std::string,std::unique_ptr<Model>>>>;

MultiXGB loadModels(const std::vector<std::string>& regions,
                        const std::vector<std::string>& channels,
                        const std::vector<std::string>& wilsCoefs,
                        const std::string directory);
std::unique_ptr<Model> loadModel(std::string region, std::string channel, std::string wilson, std::string directory);
bool modelLoaded(std::string region, std::string channel, std::string wilson, MultiXGB& multi);

#endif
