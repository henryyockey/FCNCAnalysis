#include <map>
#include <string>
#include <iterator>

typedef std::map<std::string, std::vector<bool>> m1;
typedef std::map<std::string, dim1> m2;
typedef std::map<std::string, dim2> m3;

m3 dataset_map = {
    { 
	"2017", 
	{ 
	    "SingleMuon",
	    { 
		IsoMu24,
                IsoMu27
	    }
	},
	{ 
	"SingleElectron", 
	     { 
		Ele32_WPTight_Gsf,
                Ele35_WPTight_Gsf,
             }
	},
	{ "DoubleMuon"
	}
    }
} 


