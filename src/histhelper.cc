#include "histhelper.h"
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>


Dim1 makeHists(std::map<TString, std::vector<float>> map){
    Dim1 hists(map.size());
    std::stringstream name;
    for (auto it = map.cbegin(); it!= map.cend(); ++it) {
	name<<it->first;
	TH1F * h_test = new TH1F((name.str()).c_str(), (name.str()).c_str(), it->second.at(1), it->second.at(2), it->second.at(3));
	h_test->StatOverflows(kTRUE);
        h_test->Sumw2(kTRUE);
	hists[it->second.at(0)] = h_test;
	name.str("");
    }
    return hists;
}
Dim2 makeHists(std::vector<TString> v1, std::map<TString, std::vector<float>> map) {
    Dim2 hists(v1.size(), Dim1(map.size()));
    std::stringstream name;
    for (int i=0; i<v1.size(); ++i) {
	for (auto it = map.cbegin(); it!= map.cend(); ++it) {
            name<<v1[i] << "_" << it->first;
            TH1F * h_test = new TH1F((name.str()).c_str(), (name.str()).c_str(), it->second.at(1), it->second.at(2), it->second.at(3));
            h_test->StatOverflows(kTRUE);
            h_test->Sumw2(kTRUE);
            hists[i][it->second.at(0)] = h_test;
            name.str("");
        }
    }
    return hists;
}
Dim3 makeHists(std::vector<TString> v2, std::vector<TString> v1, std::map<TString, std::vector<float>> map) {
    Dim3 hists(v2.size(), Dim2(v1.size(), Dim1(map.size())));
    std::stringstream name;
    for (int j=0; j<v2.size(); ++j) {
        for (int i=0; i<v1.size(); ++i) {
            for (auto it = map.cbegin(); it!= map.cend(); ++it) {
                name<<v2[j] << "_" << v1[i] << "_" << it->first;
                TH1F * h_test = new TH1F((name.str()).c_str(), (name.str()).c_str(), it->second.at(1), it->second.at(2), it->second.at(3));
                h_test->StatOverflows(kTRUE);
                h_test->Sumw2(kTRUE);
                hists[j][i][it->second.at(0)] = h_test;
                name.str("");
            }
        }
    }
    return hists;
}
Dim4 makeHists(std::vector<TString> v3, std::vector<TString> v2, std::vector<TString> v1, std::map<TString, std::vector<float>> map) {
    Dim4 hists(v3.size(), Dim3(v2.size(), Dim2(v1.size(), Dim1(map.size()))));
    std::stringstream name;
    for (int k=0; k<v3.size(); ++k) {
        for (int j=0; j<v2.size(); ++j) {
            for (int i=0; i<v1.size(); ++i) {
                for (auto it = map.cbegin(); it!= map.cend(); ++it) {
                    name<<v3[k] << "_" << v2[j] << "_" << v1[i] << "_" << it->first;
                    TH1F * h_test = new TH1F((name.str()).c_str(), (name.str()).c_str(), it->second.at(1), it->second.at(2), it->second.at(3));
                    h_test->StatOverflows(kTRUE);
                    h_test->Sumw2(kTRUE);
                    hists[k][j][i][it->second.at(0)] = h_test;
                    name.str("");
		}
	    }
	}
    }
    return hists;
}

void writeHists(TFile *f, Dim1 hists) {
    f->cd();
    int v1  = hists.size();
    for (int i=0; i<v1; ++i) {
   	hists[i]->Write("", TObject::kOverwrite);
    } 
    f->Close();
} 

void writeHists(TFile *f, Dim2 hists) {
    f->cd();
    int v2 = hists.size();
    int v1 = hists[0].size();
    for (int j=0; j<v2; ++j) {
	for (int i=0; i<v1; ++i) {
	    hists[j][i]->Write("", TObject::kOverwrite);
	}
    }
    f->Close();
}

void writeHists(TFile *f, Dim3 hists) {
    f->cd();
    int v3 = hists.size();
    int v2 = hists[0].size();
    int v1 = hists[0][0].size();
    for (int k=0; k<v3; ++k) {
        for (int j=0; j<v2; ++j) {
            for (int i=0; i<v1; ++i) {
                hists[k][j][i]->Write("", TObject::kOverwrite);
            }
        }
    }
    f->Close();
}

void writeHists(TFile *f, Dim4 hists) {
    f->cd();
    int v4 = hists.size();
    int v3 = hists[0].size();
    int v2 = hists[0][0].size();
    int v1 = hists[0][0][0].size();
    for (int l=0; l<v4; ++l) {
        for (int k=0; k<v3; ++k) {
            for (int j=0; j<v2; ++j) {
                for (int i=0; i<v1; ++i) {
                    hists[l][k][j][i]->Write("", TObject::kOverwrite);
                }
            }
        }
    }
    f->Close();
}


//eft hists
//
Dim1EFT makeHistsEFT(std::map<TString, std::vector<float>> map){
    Dim1EFT hists(map.size());
    std::stringstream name;
    for (auto it = map.cbegin(); it!= map.cend(); ++it) {
        name<<it->first;
        TH1EFT * h_test = new TH1EFT((name.str()).c_str(), (name.str()).c_str(), it->second.at(1), it->second.at(2), it->second.at(3));
        h_test->StatOverflows(kTRUE);
        h_test->Sumw2(kTRUE);
        hists[it->second.at(0)] = h_test;
        name.str("");
    }
    return hists;
}
Dim2EFT makeHistsEFT(std::vector<TString> v1, std::map<TString, std::vector<float>> map) {
    Dim2EFT hists(v1.size(), Dim1EFT(map.size()));
    std::stringstream name;
    for (int i=0; i<v1.size(); ++i) {
        for (auto it = map.cbegin(); it!= map.cend(); ++it) {
            name<<v1[i] << "_" << it->first;
            TH1EFT * h_test = new TH1EFT((name.str()).c_str(), (name.str()).c_str(), it->second.at(1), it->second.at(2), it->second.at(3));
            h_test->StatOverflows(kTRUE);
            h_test->Sumw2(kTRUE);
            hists[i][it->second.at(0)] = h_test;
            name.str("");
        }
    }
    return hists;
}
Dim3EFT makeHistsEFT(std::vector<TString> v2, std::vector<TString> v1, std::map<TString, std::vector<float>> map) {
    Dim3EFT hists(v2.size(), Dim2EFT(v1.size(), Dim1EFT(map.size())));
    std::stringstream name;
    for (int j=0; j<v2.size(); ++j) {
        for (int i=0; i<v1.size(); ++i) {
            for (auto it = map.cbegin(); it!= map.cend(); ++it) {
                name<<v2[j] << "_" << v1[i] << "_" << it->first;
                TH1EFT * h_test = new TH1EFT((name.str()).c_str(), (name.str()).c_str(), it->second.at(1), it->second.at(2), it->second.at(3));
                h_test->StatOverflows(kTRUE);
                h_test->Sumw2(kTRUE);
                hists[j][i][it->second.at(0)] = h_test;
                name.str("");
            }
        }
    }
    return hists;
}

Dim4EFT makeHistsEFT(std::vector<TString> v3, std::vector<TString> v2, std::vector<TString> v1, std::map<TString, std::vector<float>> map, TString optionalName="") {
    Dim4EFT hists(v3.size(), Dim3EFT(v2.size(), Dim2EFT(v1.size(), Dim1EFT(map.size()))));
    std::stringstream name;
    for (int k=0; k<v3.size(); ++k) {
    for (int j=0; j<v2.size(); ++j) {
        for (int i=0; i<v1.size(); ++i) {
            for (auto it = map.cbegin(); it!= map.cend(); ++it) {
                name<< v2[j] << "_" << v1[i] << "_" << it->first<< "_" << v3[k] << optionalName;
                TH1EFT * h_test = new TH1EFT((name.str()).c_str(), (name.str()).c_str(), it->second.at(1), it->second.at(2), it->second.at(3));
                h_test->StatOverflows(kTRUE);
                h_test->Sumw2(kTRUE);
                hists[k][j][i][it->second.at(0)] = h_test;
                name.str("");
            }
        }
    }
    }
    return hists;
}


void writeHists(TFile *f, Dim1EFT hists) {
    f->cd();
    int v1  = hists.size();
    for (int i=0; i<v1; ++i) {
        hists[i]->Write("", TObject::kOverwrite);
    }
    f->Close();
}

void writeHists(TFile *f, Dim2EFT hists) {
    f->cd();
    int v2 = hists.size();
    int v1 = hists[0].size();
    for (int j=0; j<v2; ++j) {
        for (int i=0; i<v1; ++i) {
            hists[j][i]->Write("", TObject::kOverwrite);
        }
    }
    f->Close();
}

void writeHists(TFile *f, const Dim3EFT& hists, const TString& dirName) {
    f->cd();
    
    if (!f->GetDirectory(dirName)) {
        f->mkdir(dirName);
    }
    f->cd(dirName);

    int v4 = hists.size();
    int v3 = hists[0].size();
    int v2 = hists[0][0].size();

    for (int k = 0; k < v4; ++k) {
        for (int j = 0; j < v3; ++j) {
            for (int i = 0; i < v2; ++i) {
                hists[k][j][i]->Write("", TObject::kOverwrite);
            }
        }
    }
    f->cd();
}


void writeHists(TFile *f, const Dim4EFT& hists, const TString& dirName) {
    f->cd();
    
    if (!f->GetDirectory(dirName)) {
        f->mkdir(dirName);
    }
    f->cd(dirName);

    int v4 = hists.size();
    int v3 = hists[0].size();
    int v2 = hists[0][0].size();
    int v1 = hists[0][0][0].size();

    for (int k = 0; k < v4; ++k) {
        for (int j = 0; j < v3; ++j) {
            for (int i = 0; i < v2; ++i) {
                for (int h = 0; h < v1; ++h) {
                    hists[k][j][i][h]->Write("", TObject::kOverwrite);
                }
            }
        }
    }
    f->cd();
}


/*void writeHists(TFile *f, Dim3EFT hists) {
    f->cd();
    int v3 = hists.size();
    int v2 = hists[0].size();
    int v1 = hists[0][0].size();
    for (int k=0; k<v3; ++k) {
        for (int j=0; j<v2; ++j) {
            for (int i=0; i<v1; ++i) {
                hists[k][j][i]->Write("", TObject::kOverwrite);
            }
        }
    }
    f->Close();
}

void writeHists(TFile *f, Dim4EFT hists) {
    f->cd();
    int v4 = hists.size();
    int v3 = hists[0].size();
    int v2 = hists[0][0].size();
    int v1 = hists[0][0][0].size();
    for (int k=0; k<v4; ++k) {
        for (int j=0; j<v3; ++j) {
            for (int i=0; i<v2; ++i) {
                for (int h=0; i<v1; ++h) {
                    hists[k][j][i][h]->Write("", TObject::kOverwrite);
                }
            }
        }
    }
    f->Close();
}
*/
