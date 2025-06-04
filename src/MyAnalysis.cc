#define MyAnalysis_cxx
#include "MyAnalysis.h"
#include "PU_reWeighting.h"
#include "lepton_candidate.h"
#include "jet_candidate.h"
#include "objects.h"
#include "scalefactor.h"
#include "histhelper.h"
#include "TRandom.h"
#include "TRandom3.h"
#include "selection.h"
#include "correction.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TRandom3.h>
#include <TLorentzVector.h>
#include <time.h>
#include <iostream>
#include <cmath>
#include <vector>
#include "RoccoR.h"
#include "BTagCalibrationStandalone.h"
#include "WCPoint.h"
#include "WCFit.h"
#include "TH1EFT.h"
#include "utilities.h"
#include <memory>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>
#include <typeinfo>
#include "XGBHelper.h"
#include "JigsawRecTZFCNC.h"
/*
//#include "genLevelAnalysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "PU_reWeighting.h"
#include "sumOfWeights.h"
#include "sumOfWeightsSignal.h"
#include "TDirectory.h"
#include <time.h>
#include <cstdio>
#include <cmath>
#include <vector>
#include "RoccoR.h"
#include "BTagCalibrationStandalone.h"
#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#include "CondFormats/Serialization/interface/Archive.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
//#include "Archive.h"
//#include "JetCorrectorParameters.h"
//#include "JetCorrectionUncertainty.h"
#include "GEScaleSyst.h"
#include "Utils.h"
#include "WCPoint.h"
#include "WCFit.h"
#include "TH1EFT.h"
#include <map>
#include "sys/types.h"
#include "sys/sysinfo.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include <regex>
#endif
*/

#define COMPILER (!defined(__CINT__) && !defined(__CLING__))
#if defined(__MAKECINT__) || defined(__ROOTCLING__) || COMPILER
#include "RestFrames/RestFrames.hh"
#else
RestFrames::RFKey ensure_autoload(1);
#endif

using namespace correction;
using namespace RestFrames;


int getVecPos(std::vector<TString> vec, string element){
    int i;
    for(i = 0; i < vec.size(); i++){
        if(vec[i] == element){
            break;
        }
    }
    if(i == vec.size()){
        std::cout<<"No such element as "<<element<<" found. Please enter again: ";
        std::cin>>element;
        i = getVecPos(vec, element);
    }

    return i;
}


std::string printEvent(std::vector<lepton_candidate*> *leptons, std::string eventID){
   std::stringstream s;
   s << "eventID: " << eventID << endl; 
   for(auto *l: *leptons){
      if (l->lep_==1) s <<"e";
      else s << "mu";
      if (l->charge_>0) s<< "+";
      else s << "-";

      s << "\tisPres: " << l->isPresLep_ << "\tisLoose: " << l->isLooseLep_ << "\tisFO: " << l->isFOLep_ << "\tisTight: " << l->isTightLep_ << endl;
   }
   return s.str();
}

std::string printEvent(std::vector<std::vector<int>> *cuts, std::string eventID){
   std::stringstream s;
   s<< "eventID: " << eventID << endl;

   for (auto l: *cuts){
      if (!l[4]) continue; // if not loose continue;
      if(l[0]==1) {
         s<<"e" << ((l[1]>0)?"+":"-")<< ": ";
         s << "btag: "<< l[5] << "\tptReq: " << l[6] << "\tqualityReq: " << l[7] << "\tmvaReq: "<<l[8] << "\ttightCharge: "<<l[9] << "\ttightSel: " << l[10] <<endl;
      }
      else {
         s<<"mu" <<( (l[1]>0)?"+":"-")<< ": ";
         s<< "btag: "<< l[5] << "\tptReq: " << l[6] << "\tmvaReq: "<<l[7] <<  "\ttightSel: " << l[8] <<endl;
      }
   }

   return s.str();
}

bool isInList(std::string id) {
   std::string l[]={
      "297722:132:160939414",
      "297603:291:527055163",
      "299061:119:126427408",
      "297425:111:177737176",
      "297605:22:37641832",
      "299180:19:25209952",
      "297486:60:102246253",
      "297431:156:278746246",
      "299067:91:112406026",
      "297486:112:189289012",
      "299185:71:109175463",
      "297562:160:265702053",
      "297660:12:19100791",
      "297050:520:629564608",
      "299327:35:51257907",
      "297722:109:119504154",
      "298996:190:195967548",
      "297485:190:322518888",
      "297411:606:1004768065",
      "297411:55:38743428",
      "297485:74:129428827",
      "297503:493:905075117",
      "299000:22:14541904",
      "298997:27:12720341",
      "297675:339:579882978",
      "297503:257:468450032",
      "299062:105:178036210",
      "297219:2363:3277100857",
      "297503:426:779988413",
      "299067:182:229427903",
      "297620:99:113128593",
      "297674:41:66028997",
      "297557:130:207542131",
      "297486:177:298079873",
      "297431:127:228591970",
      "297057:845:976235413",
      "299178:64:28503635",
      "299149:110:115877420",
      "299318:16:28068529",
      "297604:332:534832495",
      "297225:17:13133147",
      "299329:40:68594856",
      "297467:119:179043561",
      "297425:18:27952569"
   };

   for(int i=0; i<44; i++){
      if (l[i]==id) return true;
   }
   return false;
   
}

void displayProgress(long current, long max){
  using std::cerr;
  if (max<500) return;
  if (current%(max/500)!=0 && current<max-1) return;

  int width = 52; // Hope the terminal is at least that wide.
  int barWidth = width - 2;
  cerr << "\x1B[2K"; // Clear line
  cerr << "\x1B[2000D"; // Cursor left
  cerr << '[';
  for(int i=0 ; i<barWidth ; ++i){ if(i<barWidth*current/max){ cerr << '=' ; }else{ cerr << ' ' ; } }
  cerr << ']';
  cerr << " " << Form("%8d/%8d (%5.2f%%)", (int)current, (int)max, 100.0*current/max) ;
  cerr.flush();
}

bool ComparePtLep(lepton_candidate *a, lepton_candidate *b) { 
   return a->pt_ > b->pt_; 
}

bool ComparePtJet(jet_candidate *a, jet_candidate *b) {
   return a->pt_ > b->pt_;
}

bool isBJet(jet_candidate *j) {
   return (bool) j->btag_;
}

int vInd(std::map<TString, std::vector<float>> V, TString name){
  return V.find(name)->second.at(0);
}

int getIndex(vector<TString> v, TString K)
{  
   auto it = find(v.begin(), v.end(), K);
   return (it != v.end()) ? it - v.begin() : -1;
}

Double_t deltaPhi(Double_t phi1, Double_t phi2) {
  Double_t dPhi = phi1 - phi2;
  if (dPhi > TMath::Pi()) dPhi -= 2.*TMath::Pi();
  if (dPhi < -TMath::Pi()) dPhi += 2.*TMath::Pi();
  return dPhi;
}

Double_t deltaPhi(TLorentzVector u, TLorentzVector v) {
   Double_t dPhi = u.Phi()-v.Phi();
  if (dPhi > TMath::Pi()) dPhi -= 2.*TMath::Pi();
  if (dPhi < -TMath::Pi()) dPhi += 2.*TMath::Pi();
  return dPhi;

}


Double_t deltaR(Double_t eta1, Double_t phi1, Double_t eta2, Double_t phi2) {
  Double_t dEta, dPhi ;
  dEta = eta1 - eta2;
  dPhi = deltaPhi(phi1, phi2);
  return sqrt(dEta*dEta+dPhi*dPhi);
}

Double_t deltaR(TLorentzVector u, TLorentzVector v) {
   Double_t dEta = u.Eta()-v.Eta();
   Double_t dPhi = u.Phi()-v.Phi();
   return sqrt(dEta*dEta+dPhi*dPhi);
}

//Int_t GenPartMotherPDGID(Int_t i) { // i is the gen part idx of the particle
//   return GenPart_pdgId[GenPart_genPartIdxMother[i]];
//}

//void MyAnalysis::Loop(TString fname, TString data, TString dataset ,TString year, TString run, float xs, float lumi, float Nevent, int iseft, int nRuns)
void MyAnalysis::Loop(TString fname, TString data, TString dataset ,string year, TString runTemp, float xs, float lumi, float Nevent, int iseft, int nRuns)
{
   ofstream fout;
   fout.open("fout.txt");
/*
   JigsawRecTZFCNC *jigsaw_tZFCNC;
   jigsaw_tZFCNC = new JigsawRecTZFCNC();
   TLorentzVector B_tZFCNC, L_tZFCNC, NU_tZFCNC, Lp_tZFCNC, Lm_tZFCNC;

   JigsawRecTTFCNC *jigsaw_ttFCNC;
   jigsaw_ttFCNC = new JigsawRecTTFCNC();
   TLorentzVector topa_ttFCNC, topb_ttFCNC, ba_ttFCNC, Wa_ttFCNC, la_ttFCNC, nua_ttFCNC, qb_ttFCNC, Zb_ttFCNC, lbp_ttFCNC, lbm_ttFCNC;

   JigsawRecTHFCNC *jigsaw_tHFCNC;
   jigsaw_tHFCNC = new JigsawRecTHFCNC();
   TLorentzVector T_tHFCNC, H_tHFCNC, WT_tHFCNC, W1_tHFCNC, W2_tHFCNC, B_tHFCNC, La_tHFCNC, Lb_tHFCNC, Q1_tHFCNC, Q2_tHFCNC, NUa_tHFCNC, NUb_tHFCNC;

   JigsawRecTHFCNC *jigsaw_tHFCNC_gen;
   jigsaw_tHFCNC_gen = new JigsawRecTHFCNC();
   TLorentzVector T_tHFCNC_gen, H_tHFCNC_gen, WT_tHFCNC_gen, W1_tHFCNC_gen, W2_tHFCNC_gen, B_tHFCNC_gen, 
                  La_tHFCNC_gen, Lb_tHFCNC_gen, Q1_tHFCNC_gen, Q2_tHFCNC_gen, NUa_tHFCNC_gen, NUb_tHFCNC_gen;

*/

 
   Bool_t isSignal;
//   Float_t weights[12];
   Float_t lep1Pt;
   Float_t lep1Eta;
   Float_t lep1Phi;
   Float_t lep2Pt;
   Float_t lep2Eta;
   Float_t lep2Phi;
   Float_t llM;
   Float_t llPt;
   Float_t llDr;
   Float_t llDphi;
   Int_t njet;
   Int_t nbjet;
   Float_t Met;
   Float_t MetPhi;
   Int_t nVtx;
   Float_t jet1Pt;
   Float_t jet1Phi;
   Float_t jet1Eta;
   Float_t llss_WT_mass, llss_T_mass, llss_H_mass, llss_tH_DR, llss_tH_DPhi;
   Int_t channel;
   Int_t dset;

   Float_t ml1H, nJet_, mbt, mHb, ptWhad, dRl1H, mWhad, ptl1, etal1, dRHWtop, dRl0l1, 
           ml1b, etab, dRl0t, MET, fll0, etal0, ptl0, dRl1t, mHWtop, dRl1Wtop, ml0H, ptb;

   Float_t wctp, wctlS, wcte, wctl, wctlT, wctZ, wcpt, wcpQM, wctA, wcQe, wctG, wcQlM;

//   TBranch *b_weights;

   TFile *f = new TFile("ANoutput.root","RECREATE");

/*   TTree *t1= new TTree("t1", "t1");
   t1->Branch("isSignal", &isSignal, "isSignal/O");
   t1->Branch("lep1Pt", &lep1Pt, "lep1Pt/F");
   t1->Branch("lep1Eta", &lep1Eta, "lep1Eta/F");
   t1->Branch("lep1Phi", &lep1Phi, "lep1Phi/F");
   t1->Branch("lep2Pt", &lep2Pt, "lep2Pt/F");
   t1->Branch("lep2Eta", &lep2Eta, "lep2Eta/F");
   t1->Branch("lep2Phi", &lep2Phi, "lep2Phi/F");
   t1->Branch("jet1Pt", &jet1Pt, "jet1Pt/F");
   t1->Branch("jet1Eta", &jet1Eta, "jet1Eta/F");
   t1->Branch("jet1Phi", &jet1Phi, "jet1Phi/F");
   t1->Branch("llM", &llM, "llM/F");
   t1->Branch("llPt", &llPt, "llPt/F");
   t1->Branch("llDr", &llDr, "llDr/F");
   t1->Branch("llDphi", &llDphi, "llDphi/F");
   t1->Branch("Met", &Met, "Met/F");
   t1->Branch("MetPhi", &MetPhi, "MetPhi/F");
   t1->Branch("njet", &njet, "njet/I");
   t1->Branch("nbjet", &nbjet, "nbjet/I");
   t1->Branch("nVtx", &nVtx, "nVtx/I");
   t1->Branch("channel", &channel, "channel/I");
//   t1->Branch("weights", weights, "weights[12]/F");
   t1->Branch("dataset", &dset, "dataset/I");
   t1->Branch("llss_WT_mass", &llss_WT_mass, "llss_WT_mass/F");
   t1->Branch("llss_T_mass", &llss_T_mass, "llss_T_mass/F");
   t1->Branch("llss_H_mass", &llss_H_mass, "llss_H_mass/F");
   t1->Branch("llss_tH_DR", &llss_tH_DR, "llss_tH_DR/F");
   t1->Branch("llss_tH_DPhi", &llss_tH_DPhi, "llss_tH_DPhi/F");

   t1->Branch("ml1H", &ml1H, "ml1H/F");
   t1->Branch("nJet_", &nJet_, "nJet_/F");
   t1->Branch("mbt", &mbt, "mbt/F");
   t1->Branch("mHb", &mHb, "mHb/F");
   t1->Branch("ptWhad", &ptWhad, "ptWhad/F");
   t1->Branch("dRl1H", &dRl1H, "dRl1H/F");
   t1->Branch("mWhad", &mWhad, "mWhad/F");
   t1->Branch("ptl1", &ptl1, "ptl1/F");
   t1->Branch("etal1", &etal1, "etal1/F");
   t1->Branch("dRHWtop", &dRHWtop, "dRHWtop/F");
   t1->Branch("dRl0l1", &dRl0l1, "dRl0l1/F");
   t1->Branch("ml1b", &ml1b, "ml1b/F");
   t1->Branch("etab", &etab, "etab/F");
   t1->Branch("dRl0t", &dRl0t, "dRl0t/F");
   t1->Branch("MET", &MET, "MET/F");
   t1->Branch("fll0", &fll0, "fll0/F");
   t1->Branch("etal0", &etal0, "etal0/F");
   t1->Branch("ptl0", &ptl0, "ptl0/F");
   t1->Branch("dRl1t", &dRl1t, "dRl1t/F");
   t1->Branch("mHWtop", &mHWtop, "mHWtop/F");
   t1->Branch("dRl1Wtop", &dRl1Wtop, "dRl1Wtop/F");
   t1->Branch("ml0H", &ml0H, "ml0H/F");
   t1->Branch("ptb", &ptb, "ptb/F");

   t1->Branch("wctp", &wctp, "wctp/F");
   t1->Branch("wctlS", &wctlS, "wctlS/F");
   t1->Branch("wcte", &wcte, "wcte/F");
   t1->Branch("wctl", &wctl, "wctl/F");
   t1->Branch("wctlT", &wctlT, "wctlT/F");
   t1->Branch("wctZ", &wctZ, "wctZ/F");
   t1->Branch("wcpt", &wcpt, "wcpt/F");
   t1->Branch("wcpQM", &wcpQM, "wcpQM/F");
   t1->Branch("wctA", &wctA, "wctA/F");
   t1->Branch("wcQe", &wcQe, "wcQe/F");
   t1->Branch("wctG", &wctG, "wctG/F");
   t1->Branch("wcQlM", &wcQlM, "wcQlM/F");
*/
   std::map<TString,int> DataSets {
      {"UL17_ZGToLLG",                  0       },
      {"UL17_ST_top_t_channel",         1       },
      {"UL17_ttHnobb",                  2       },
      {"UL17_tbarW",                    3       },
      {"UL17_WZZ",                      4       },
      {"UL17_TTGamma_Dilept",           5       },
      {"UL17_DY50",                     6       },
      {"UL17_WWZ_4F",                   7       },
      {"UL17_ZZTo4L",                   8       },
      {"UL17_TTTo2L2Nu",                9       },
      {"UL17_WJetsToLNu",               10      },
      {"UL17_TTWJetsToLNu",             11      },
      {"UL17_tZq",                      12      },
      {"UL17_TTToSemiLeptonic",         13      },
      {"UL17_TTGamma_SingleLept",       14      },
      {"UL17_TTZToLLNuNu_M_10",         15      },
      {"UL17_DY10to50",                 16      },
      {"UL17_ZZZ",                      17      },
      {"UL17_tW",                       18      },
      {"UL17_WWTo2L2Nu",                19      },
      {"UL17_WWW_4F",                   20      },
      {"UL17_ST_antitop_t_channel",     21      },
      {"UL17_tttt",                     22      },
      {"UL17_ST_top_s_channel",         23      },
      {"UL17_WZTo3LNu",                 24      },
   };




   TString tempData = data;
   cout << "Hello world" << endl;
//   TFile *f = new TFile("ANoutput.root","RECREATE");
//   TFile *g = new TFile("CutTable.root", "RECREATE");
//   f->cd();
//   if (!f) { 
//      cout << "file not opened" << endl;
//      exit(1);
//   }
//   else cout << "ANoutput opened" << endl;

//   ofstream fout;
//   fout.open ("fout.txt");

//   std::vector<TString> regions{"llonZ", "lloffZ"};
//   std::vector<TString> channels{"eeos", "eess+", "eess-", "emuos", "emuss+", "emuss-", "mumuos", "mumuss+", "mumuss-"};
   std::vector<TString> channels{
//      "EpEm", "EpmMUmp", "MUpMUm", 
//      "LLpp", "LLmm",// "LFpp", "LFmm", "FFpp", "FFmm",
//      "3LonZ", 
      "3LoffZp", "3LoffZm",
//      "4L",
//      "LLFonZ", "LFFonZ","FFFonZ",
//      "LLFoffZp", "LFFoffZp","FFFoffZp",
//      "LLFoffZm", "LFFoffZm","FFFoffZm"
   };
   
   if (data=="data") {
      channels.insert(channels.end(), {
//         "LFpp", "LFmm", "FFpp", "FFmm",
//         "LLFonZ", "LFFonZ", "FFFonZ",
         "LLFoffZp", "LFFoffZp", "FFFoffZp",
         "LLFoffZm", "LFFoffZm", "FFFoffZm"
      });
   }
   
   std::vector<TString> regions{"0Bjet","1Bjet", "G1Bjet", "All"};
   const std::map<TString, std::vector<float>> vars = 
   {
      {"lep1Pt",      	       	  	{0, 	10, 	0, 	300	}},
      {"lep1Eta",            	 	{1, 	20,    -3, 	3	}},
      {"lep1Phi",			{2, 	25,    -4, 	4	}},
      {"lep2Pt",                        {3,     25,     0,      1000    }},
      {"lep2Eta",                       {4,     20,    -3,      3       }},
      {"lep2Phi",                       {5,     25,    -4,      4       }},
      {"llM",                           {6,     20,     0, 	500 	}},
      {"llPt",                          {7,     20,     0, 	200 	}},
      {"llDr",                          {8,     25,     0, 	7   	}},
      {"llDphi",                        {9,     15,     0, 	4   	}},
      {"jet1Pt",                        {10,    20,     0, 	300 	}},
      {"jet1Eta",                       {11,    20,    -3, 	3  	}},
      {"jet1Phi",                       {12,    25,    -4, 	4  	}},
      {"njet",                          {13,    10,     0, 	10  	}},
      {"nbjet",                         {14,     6,     0, 	6   	}},
      {"Met",                           {15,    20,     0, 	210 	}},
      {"MetPhi",                        {16,    20,    -4, 	4  	}},
      {"nVtx",                          {17,    10,     0, 	70  	}},
      {"llMZw",                         {18,    10,    70, 	110	}},
      {"BDTOutput",                     {19,     10,     0,      1       }},
   };
   const std::map<TString, std::vector<float>> sys_vars =
   {
//      {"lep1Pt",                        {0,     10,     0,      300     }},
      {"BDTOutput",                     {0,     10,     0,      1       }},
   };

   const std::map<TString, std::vector<float>> vars_bdt = 
   {
      {"ctpTrainedBDT",                 {0,    40,     0,      1       }},
      {"ctlSTrainedBDT",                {1,    40,     0,      1       }},
      {"cteTrainedBDT",                 {2,    40,     0,      1       }},
      {"ctlTrainedBDT",                 {3,    40,     0,      1       }},
      {"ctlTTrainedBDT",                {4,    40,     0,      1       }},
      {"ctZTrainedBDT",                 {5,    40,     0,      1       }},
      {"cptTrainedBDT",                 {6,    40,     0,      1       }},
      {"cpQMTrainedBDT",                {7,    40,     0,      1       }},
      {"ctATrainedBDT",                 {8,    40,     0,      1       }},
      {"cQeTrainedBDT",                 {9,    40,     0,      1       }},
      {"ctGTrainedBDT",                 {10,    40,     0,      1       }},
      {"cQlMTrainedBDT",                {11,    40,     0,      1       }},

   };

   std::vector<TString> level{"genPart", "genJigsaw", "recoJigsaw"};
   std::vector<TString> particles {"T", "H", "WT", "W1", "W2"};
   const std::map<TString, std::vector<float>> vars_rjr = 
   {
      {"WTMass",                   {0, 20, 0, 400}},
      {"W1Mass",                   {1, 20, 0, 400}},
      {"W2Mass",                   {2, 20, 0, 400}},
      {"TMass",                    {3, 20, 0, 400}},
      {"HMass",                    {4, 20, 0, 400}},
      {"tHDR",                     {5, 20, 0, 7  }},
      {"tHDPhi",                   {6, 20, 0, 4  }},
//      {"Pt",                     {2, 20, 0, 500}},
//      {"Eta",                    {3, 20,-3.0, 3.0  }},
//      {"CosDecayAngle",          {4, 20, -1, 1}},
   };

   const std::map<TString, std::vector<float>> vars_llss_jigsaw = 
   {
      {"ml1H",                  {      0,     30,      0,    500}},
      {" nJet_",                {      1,     10,      0,     10}},
      {"mbt",                   {      2,     30,      0,    500}},
      {"mHb",                   {      3,     30,      0,    500}},
      {"ptWhad",                {      4,     30,      0,    500}},
      {"dRl1H",                 {      5,     30,      0,      7}},
      {"mWhad",                 {      6,     30,      0,    500}},
      {"ptl1",                  {      7,     30,      0,    500}},
      {"etal1",                 {      8,     30,     -3,      3}},
      {"dRHWtop",               {      9,     30,      0,      7}},
      {"dRl0l1",                {     10,     30,      0,      7}},
      {"ml1b",                  {     11,     30,      0,    500}},
      {"etab",                  {     12,     30,     -3,      3}},
      {"dRl0t",                 {     13,     30,      0,    500}},
      {"MET",                   {     14,     30,      0,    500}},
      {"fll0",                  {     15,      2,      0,      2}},
      {"etal0",                 {     16,     30,     -3,      3}},
      {"ptl0",                  {     17,     30,      0,    500}},
      {"dRl1t",                 {     18,     30,      0,      7}},
      {"mHWtop",                {     19,     30,      0,    500}},
      {"dRl1Wtop",              {     20,     30,      0,      7}},
      {"ml0H",                  {     21,     30,      0,    500}},
      {"ptb",                   {     22,     30,      0,    500}},
   };
   std::vector<TString> sys{"pu", "muIdIsoSf", "eleIDSf", "eleRecoSf", "prefiring", "bcTagSf", "bcTagSfUnCorr", "LTagSf", "LTagSfUnCorr"};
   std::vector<float> nominalWeights, sysUpWeights, sysDownWeights;
   nominalWeights.assign(sys.size(), 1);
   sysUpWeights.assign(sys.size(), 1);
   sysDownWeights.assign(sys.size(), 1);
   Dim3EFT Hists = makeHistsEFT(channels, regions, vars);
   Dim4EFT HistsSysUp, HistsSysDown;
   if (data=="mc"){
      HistsSysUp = makeHistsEFT(sys, channels, regions, sys_vars, (TString)"Up");
      HistsSysDown = makeHistsEFT(sys, channels, regions, sys_vars, (TString)"Down");
   }
//   Dim1 Hists_rjr = makeHists(vars_rjr);

   

//   Dim3EFT Hists = makeHistsEFT(channels, regions, vars);
   
   std::vector<string> wc_names_lst={};
   std::vector<string> wc_names_lst_FCNC={"ctp","ctlS","cte","ctl","ctlT","ctZ","cpt","cpQM","ctA","cQe","ctG","cQlM"};
   std::vector<string> wc_names_short={"ctlS","cte","ctl","ctlT","cQe","cQlM"};
   std::vector<TString> wc_names={"ctp","ctlS","cte","ctl","ctlT","ctZ","cpt","cpQM","ctA","cQe","ctG","cQlM"};
   if (fname.Contains("FCNC")) wc_names_lst = wc_names_lst_FCNC;


//   Dim3EFT HistsBDTOutput = makeHistsEFT(channels, regions, vars_bdt);


   MultiXGB XGBModels; 


   // set up  utput file
   //
   // declare lepton candidate vectors -- 
   std::vector<lepton_candidate*> *selectedLeptons;
   std::vector<jet_candidate*> *selectedJets;
   std::vector<std::vector<int>> *leptonCuts;
   WCFit *eft_fit;

   // declare triggers and other variable
   bool oppositeSign;
   bool sameSign;
   int onZ;
   Float_t pt, phi, eta, mll;
   Int_t lep1, lep2;
   int ch;
   TString cat;
   Double_t weight, weight_EFT;
   bool cleanJet;
   Int_t ind;
   Double_t mass, btag;
   // 3l triggers
   bool triggerPass3l;
   bool triggerPass3l_exclude;
   Float_t conept, jetBtagDeepFlav;
   bool idEmu, isPres, isLoose, isFO, isTight;
   int eCuts[5], muCuts[4], ttHCuts[3];
   bool print = false;
   float sumWeight=0;
   int numEpEm=0;
   RoccoR rc;
   string rochesterFile;
   double muPtSFRochester = 1;
   string eleSF, muSF, bSF;
   TString fName;
   float eleNominalWeight=1.0, muNominalWeight=1.0;

   PU wPU;
   Float_t weight_PU;
   Float_t weight_prefiring;
   Float_t sf_Ele_Reco;
   TLorentzVector highPtMu;
   Float_t sf_Mu_ID, sf_Mu_ISO, sf_Mu_RECO;
   float BJetSF;
   float CJetSF;
   float LJetSF;
   float BJetSF_UpCorr;
   float CJetSF_UpCorr;
   float LJetSF_UpCorr;
   float BJetSF_UpUnCorr;
   float CJetSF_UpUnCorr;
   float LJetSF_UpUnCorr;
   float BJetSF_DownCorr;
   float CJetSF_DownCorr;
   float LJetSF_DownCorr;
   float BJetSF_DownUnCorr;
   float CJetSF_DownUnCorr;
   float LJetSF_DownUnCorr;
   float BJetEff;
   float CJetEff;
   float LJetEff;   


   // Open Data Files
   TString fakeFactorFile = "/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/data/fr_2017_recorrected.root";
   ScaleFactor *mu_sf = new ScaleFactor(fakeFactorFile, "FR_mva085_mu_data_comb_recorrected", "2017");
   ScaleFactor *ele_sf = new ScaleFactor(fakeFactorFile, "FR_mva090_el_data_comb_NC_recorrected", "2017");

   // scale factors
   if (year == "2017") {
      rochesterFile = "/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/data/RoccoR2017UL.txt";
      eleSF="/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/data/POG/EGM/2017_UL/electron.json.gz";
      muSF= "/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/data/POG/MUO/2017_UL/muon_Z.json.gz";
      bSF="/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/data/POG/BTV/2017_UL/btagging.json.gz"; 
   }

   map<string,float> HEEPSF_B{ {"2016preVFP",0.985}, { "2016postVFP",0.985},{"2017",0.979},{"2018",0.973} };
   map<string,float> HEEPSF_E{ {"2016preVFP",0.990}, { "2016postVFP",0.990},{"2017",0.987},{"2018",0.980} };

   rc.init(rochesterFile);
   auto csetFileEleSF = CorrectionSet::from_file(eleSF);
   auto csetEleIdReco = csetFileEleSF->at("UL-Electron-ID-SF");

   auto csetFileMuSF = CorrectionSet::from_file(muSF);  
   auto csetMuTightId = csetFileMuSF->at("NUM_HighPtID_DEN_TrackerMuons");
   auto csetMuTightRelIso = csetFileMuSF->at("NUM_LooseRelTkIso_DEN_HighPtIDandIPCut");
   auto csetMuReco = csetFileMuSF->at("NUM_TrackerMuons_DEN_genTracks");
   auto csetMuLoose = csetFileMuSF->at("NUM_LooseID_DEN_TrackerMuons");

   auto csetFilebSF = CorrectionSet::from_file(bSF);
   auto csetLightJetSF = csetFilebSF->at("deepJet_incl");
   auto csetBcJetSF = csetFilebSF->at("deepJet_comb");    

   std::vector<TString> fNameEleSF{
      "/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/data/topCoffeaData/leptonSF/elec/egammaEffi" + year + "_recoToloose_EGM2D.root",
      "/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/data/topCoffeaData/leptonSF/elec/egammaEffi" + year + "_iso_EGM2D.root",
      "/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/data/topCoffeaData/leptonSF/elec/egammaEffi" + year + "_3l_EGM2D.root"
   };

   std::vector<TString> fNameMuSF{
      "/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/data/topCoffeaData/leptonSF/muon/egammaEffi" + year + "_EGM2D.root",
      "/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/data/topCoffeaData/leptonSF/muon/egammaEffi" + year + "_iso_EGM2D.root"
   };
   TString fNamebSF = "/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/data/btagEff.root"; 
   TString fNameHighPtMuRecoSF = "/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/data/HighPtMuRecoSF_" + year + ".root";
   std::vector<ScaleFactor*> *eleNominalSF = new std::vector<ScaleFactor*>();
   std::vector<ScaleFactor*> *muNominalSF = new std::vector<ScaleFactor*>();
   
   ScaleFactor *btagEff_b_H, *btagEff_c_H, *btagEff_udsg_H;
   ScaleFactor *highPtMuRecoSF_pVsAbsEta_H;

   if (data=="mc") {
      for (auto fName : fNameEleSF) {
         eleNominalSF->push_back(new ScaleFactor(fName, "EGamma_SF2D", year));
      }
      for (auto fName : fNameMuSF) {
         muNominalSF->push_back(new ScaleFactor(fName, "EGamma_SF2D", year));
      }
      btagEff_b_H = new ScaleFactor(fNamebSF, year+"_h2_BTaggingEff_b", year);
      btagEff_c_H = new ScaleFactor(fNamebSF, year+"_h2_BTaggingEff_c", year);
      btagEff_udsg_H = new ScaleFactor(fNamebSF, year+"_h2_BTaggingEff_udsg", year);
      highPtMuRecoSF_pVsAbsEta_H = new ScaleFactor(fNameHighPtMuRecoSF, "h2_HighPtMuRecoSF_pVsAbsEta", year);
   }

   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   string eventID;

   float sfsum = 0;
   float sum =0;
   int k=0;
// begin analysis loop
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      data = tempData;
//   for (Long64_t jentry=0; jentry<1000;jentry++) {
      if (jentry%10000==0) cout << jentry << " events processed" << endl;
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
    //  displayProgress(jentry,ntr);
      eventID = to_string(run)+":"+to_string(luminosityBlock)+":"+to_string(event);
      print = false;
      if(print) cout << eventID << endl;
      weight = 1;
      if (data == "mc") weight = ((1000*xs*lumi)/Nevent)*((genWeight>0)?1:-1);

      // initialize variables
      // trigger pass for each channel
      //

      oppositeSign = false;
      sameSign = false;
      triggerPass3l = false;
      triggerPass3l_exclude = false;

      sysUpWeights[getVecPos(sys,"eleIDSf")]=1;
      sysDownWeights[getVecPos(sys,"eleIDSf")]=1;
      nominalWeights[getVecPos(sys,"eleIDSf")]=1;
      sysUpWeights[getVecPos(sys,"eleRecoSf")]=1;
      sysDownWeights[getVecPos(sys,"eleRecoSf")]=1;
      nominalWeights[getVecPos(sys,"eleRecoSf")]=1;
      sysUpWeights[getVecPos(sys,"muIdIsoSf")]=1;
      sysDownWeights[getVecPos(sys,"muIdIsoSf")]=1;
      nominalWeights[getVecPos(sys,"muIdIsoSf")]=1;
      sysUpWeights[getVecPos(sys,"bcTagSf")]=1;
      sysDownWeights[getVecPos(sys,"bcTagSf")]=1;
      nominalWeights[getVecPos(sys,"bcTagSf")]=1;
      sysUpWeights[getVecPos(sys,"bcTagSfUnCorr")]=1;
      sysDownWeights[getVecPos(sys,"bcTagSfUnCorr")]=1;
      nominalWeights[getVecPos(sys,"bcTagSfUnCorr")]=1;
      sysUpWeights[getVecPos(sys,"LTagSf")]=1;
      sysDownWeights[getVecPos(sys,"LTagSf")]=1;
      nominalWeights[getVecPos(sys,"LTagSf")]=1;
      sysUpWeights[getVecPos(sys,"LTagSfUnCorr")]=1;
      sysDownWeights[getVecPos(sys,"LTagSfUnCorr")]=1;
      nominalWeights[getVecPos(sys,"LTagSfUnCorr")]=1;




      weight_prefiring = 1;
      sf_Ele_Reco=1;
      sf_Mu_ID=1;
      sf_Mu_ISO=1; 
      sf_Mu_RECO=1;
      weight_PU=1;

      BJetSF=1;
      CJetSF=1;
      LJetSF=1;
      BJetSF_UpCorr=1;
      CJetSF_UpCorr=1;
      LJetSF_UpCorr=1;
      BJetSF_UpUnCorr=1;
      CJetSF_UpUnCorr=1;
      LJetSF_UpUnCorr=1;
      BJetSF_DownCorr=1;
      CJetSF_DownCorr=1;
      LJetSF_DownCorr=1;
      BJetSF_DownUnCorr=1;
      CJetSF_DownUnCorr=1;
      LJetSF_DownUnCorr=1;
      BJetEff=1;
      CJetEff=1;
      LJetEff=1;

/*      topa_ttFCNC.SetPtEtaPhiM(0,0,0,0);
      topb_ttFCNC.SetPtEtaPhiM(0,0,0,0);
      ba_ttFCNC.SetPtEtaPhiM(0,0,0,0);
      Wa_ttFCNC.SetPtEtaPhiM(0,0,0,0);
      la_ttFCNC.SetPtEtaPhiM(0,0,0,0);
      qb_ttFCNC.SetPtEtaPhiM(0,0,0,0);
      Zb_ttFCNC.SetPtEtaPhiM(0,0,0,0);
      lbp_ttFCNC.SetPtEtaPhiM(0,0,0,0);
      lbm_ttFCNC.SetPtEtaPhiM(0,0,0,0);


      if( data=="mc") {
         T_tHFCNC_gen.SetPtEtaPhiM(0,0,0,0);
         H_tHFCNC_gen.SetPtEtaPhiM(0,0,0,0);
         WT_tHFCNC_gen.SetPtEtaPhiM(0,0,0,0);
         W1_tHFCNC_gen.SetPtEtaPhiM(0,0,0,0);
         W2_tHFCNC_gen.SetPtEtaPhiM(0,0,0,0);
         B_tHFCNC_gen.SetPtEtaPhiM(0,0,0,0);
         La_tHFCNC_gen.SetPtEtaPhiM(0,0,0,0);
         Lb_tHFCNC_gen.SetPtEtaPhiM(0,0,0,0);
         Q1_tHFCNC_gen.SetPtEtaPhiM(0,0,0,0);
         Q2_tHFCNC_gen.SetPtEtaPhiM(0,0,0,0);
         NUa_tHFCNC_gen.SetPtEtaPhiM(0,0,0,0);
         NUb_tHFCNC_gen.SetPtEtaPhiM(0,0,0,0);
         auto Mother_pdgId = [this](int i){return abs(GenPart_pdgId[GenPart_genPartIdxMother[i]]);};
         auto GrandMother_pdgId = [this](int i){
            int gmid = GenPart_genPartIdxMother[GenPart_genPartIdxMother[i]];
            if (gmid >=0 && gmid<nGenPart) return abs(GenPart_pdgId[gmid]);
            else return -1;
         };
         auto isBadGenPart = [this](int i){return isnan(GenPart_pt[i]) || isinf(GenPart_pt[i]) || GenPart_pt[i]==0;};
         int W1ID=-1;

         for (int l=nGenPart-1; l>=0; l--){
            if(isBadGenPart(l)) continue;
            if(abs(GenPart_pdgId[l])==11 || abs(GenPart_pdgId[l])==13 || abs(GenPart_pdgId[l])==15){
               if (Mother_pdgId(l) == 24 && GrandMother_pdgId(l) == 6) La_tHFCNC_gen.SetPtEtaPhiM(GenPart_pt[l], GenPart_eta[l], GenPart_phi[l], GenPart_mass[l]);
               if (Mother_pdgId(l) == 24 && GrandMother_pdgId(l) == 25) Lb_tHFCNC_gen.SetPtEtaPhiM(GenPart_pt[l], GenPart_eta[l], GenPart_phi[l], GenPart_mass[l]);
            }
            if(abs(GenPart_pdgId[l])==12 || abs(GenPart_pdgId[l])==14 || abs(GenPart_pdgId[l])==16){
               if (GrandMother_pdgId(l) == 6 && Mother_pdgId(l)==24) {
                   NUa_tHFCNC_gen.SetPtEtaPhiM(GenPart_pt[l], GenPart_eta[l], GenPart_phi[l], GenPart_mass[l]);
                   int w = GenPart_genPartIdxMother[l];
                   WT_tHFCNC_gen.SetPtEtaPhiM(GenPart_pt[w], GenPart_eta[w], GenPart_phi[w], GenPart_mass[w]);
               }
               if (GrandMother_pdgId(l) == 25&& Mother_pdgId(l)==24 ) {
                   NUb_tHFCNC_gen.SetPtEtaPhiM(GenPart_pt[l], GenPart_eta[l], GenPart_phi[l], GenPart_mass[l]);
                   int w = GenPart_genPartIdxMother[l];
                   W2_tHFCNC_gen.SetPtEtaPhiM(GenPart_pt[w], GenPart_eta[w], GenPart_phi[w], GenPart_mass[w]);
               }
            }
            if((abs(GenPart_pdgId[l])==1 || abs(GenPart_pdgId[l])==3) && Mother_pdgId(l)==24 && GrandMother_pdgId(l) == 25) {
                int w = GenPart_genPartIdxMother[l];
                Q1_tHFCNC_gen.SetPtEtaPhiM(GenPart_pt[l], GenPart_eta[l], GenPart_phi[l], GenPart_mass[l]);
                W1_tHFCNC_gen.SetPtEtaPhiM(GenPart_pt[w], GenPart_eta[w], GenPart_phi[w], GenPart_mass[w]);
            }
            if((abs(GenPart_pdgId[l])==2 || abs(GenPart_pdgId[l])==4) && Mother_pdgId(l)==24 && GrandMother_pdgId(l) == 25) {
                Q2_tHFCNC_gen.SetPtEtaPhiM(GenPart_pt[l], GenPart_eta[l], GenPart_phi[l], GenPart_mass[l]);
            }
            if(abs(GenPart_pdgId[l])==6) T_tHFCNC_gen.SetPtEtaPhiM(GenPart_pt[l], GenPart_eta[l], GenPart_phi[l], GenPart_mass[l]);
               if(abs(GenPart_pdgId[l])==25) H_tHFCNC_gen.SetPtEtaPhiM(GenPart_pt[l], GenPart_eta[l], GenPart_phi[l], GenPart_mass[l]);
               if(abs(GenPart_pdgId[l])==5) B_tHFCNC_gen.SetPtEtaPhiM(GenPart_pt[l], GenPart_eta[l], GenPart_phi[l], GenPart_mass[l]);
            }

         bool GenAllMatched = T_tHFCNC_gen.Pt()>0&& H_tHFCNC_gen.Pt()>0&& WT_tHFCNC_gen.Pt()>0&& W1_tHFCNC_gen.Pt()>0&& W2_tHFCNC_gen.Pt()>0&& B_tHFCNC_gen.Pt()>0&&
                  La_tHFCNC_gen.Pt()>0&& Lb_tHFCNC_gen.Pt()>0&& Q1_tHFCNC_gen.Pt()>0&& Q2_tHFCNC_gen.Pt()>0&& NUa_tHFCNC_gen.Pt()>0&& NUb_tHFCNC_gen.Pt()>0;

         TLorentzVector nuSum =  NUa_tHFCNC_gen+NUb_tHFCNC_gen;
         nuSum.SetPtEtaPhiM(nuSum.Pt(),0,nuSum.Phi(),0);
      }

//            jigsaw_tHFCNC_gen->Analyze(La_tHFCNC_gen, Lb_tHFCNC_gen, Q1_tHFCNC_gen, Q2_tHFCNC_gen, B_tHFCNC_gen, nuSum);
//            if (La_tHFCNC_gen.Pt()>15 && Lb_tHFCNC_gen.Pt()>15&&Q1_tHFCNC_gen.Pt()>15 && Q2_tHFCNC_gen.Pt()>10){
//               Hists_rjr[1][0][vInd(vars_rjr,"Mass")]->Fill(jigsaw_tHFCNC_gen->T->GetFourVector().M());
//               Hists_rjr[1][1][vInd(vars_rjr,"Mass")]->Fill(jigsaw_tHFCNC_gen->H->GetFourVector().M());
//               Hists_rjr[1][2][vInd(vars_rjr,"Mass")]->Fill(jigsaw_tHFCNC_gen->WT->GetFourVector().M());
//               Hists_rjr[1][3][vInd(vars_rjr,"Mass")]->Fill(jigsaw_tHFCNC_gen->W1->GetFourVector().M());
//               Hists_rjr[1][4][vInd(vars_rjr,"Mass")]->Fill(jigsaw_tHFCNC_gen->W2->GetFourVector().M());
//            }
*/           
 
/***********************************************
 *   Data triggers using topcoffea approach 
 ***********************************************/
      Bool_t triggers_2017_SingleMuon[] = {
	 HLT_IsoMu24,
	 HLT_IsoMu27
      };

      Bool_t triggers_2017_SingleElectron[] = {
	 HLT_Ele35_WPTight_Gsf,
	 HLT_Ele32_WPTight_Gsf_L1DoubleEG
      };

      Bool_t triggers_2017_DoubleMuon[] = {
	 HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ,
         HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8,
         HLT_TripleMu_12_10_5
      };

      Bool_t triggers_2017_DoubleEG[] = {
	 HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL,
	 HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL
//         HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ,
      };

      Bool_t triggers_2017_MuonEG[] = {
	 HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL,
         HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ,
         HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ,
         HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ,
         HLT_Mu8_DiEle12_CaloIdL_TrackIdL,
         HLT_Mu8_DiEle12_CaloIdL_TrackIdL_DZ,
         HLT_DiMu9_Ele9_CaloIdL_TrackIdL_DZ,
      };


      //exclude nothing
      Bool_t triggers_2017_SingleMuon_exclude[] = {};
      
      //exclude single muon triggers
      Bool_t triggers_2017_SingleElectron_exclude[] = {
         HLT_IsoMu24,
         HLT_IsoMu27
      }; 

      //exclude single muon + single electron triggers
      Bool_t triggers_2017_DoubleMuon_exclude[] = {
         HLT_IsoMu24,
         HLT_IsoMu27,
         HLT_Ele35_WPTight_Gsf,
	 HLT_Ele32_WPTight_Gsf_L1DoubleEG
      };

      //exclude single muon + single electron + DoubleMuon triggers
      Bool_t triggers_2017_DoubleEG_exclude[] = {
         HLT_IsoMu24,
         HLT_IsoMu27,
         HLT_Ele35_WPTight_Gsf,
	 HLT_Ele32_WPTight_Gsf_L1DoubleEG,
         HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ,
         HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8,
         HLT_TripleMu_12_10_5
      };

      //exclude single muon + single electron + DoubleMuon + DoubleEG triggers
      Bool_t triggers_2017_MuonEG_exclude[] = {
         HLT_IsoMu24,
         HLT_IsoMu27,
         HLT_Ele35_WPTight_Gsf,
	 HLT_Ele32_WPTight_Gsf_L1DoubleEG,
         HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ,
         HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8,
         HLT_TripleMu_12_10_5,
         HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL,
//         HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ,
         HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL
      };

      Bool_t triggers_2017_all[] = {
         HLT_IsoMu24,
         HLT_IsoMu27,
         HLT_Ele35_WPTight_Gsf,
	 HLT_Ele32_WPTight_Gsf_L1DoubleEG,
         HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ,
         HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8,
         HLT_TripleMu_12_10_5,
         HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL,
//         HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ,
         HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL,
         HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL,
         HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ,
         HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ,
         HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ,
         HLT_Mu8_DiEle12_CaloIdL_TrackIdL,
         HLT_Mu8_DiEle12_CaloIdL_TrackIdL_DZ,
         HLT_DiMu9_Ele9_CaloIdL_TrackIdL_DZ,
      };


      if (print) {
 //	 cout << "all triggers" << endl;
	 for (int i=0; i<16; i++) cout << triggers_2017_all[i] << " ";
	 cout << endl;
      }

      if (data=="mc") {
	 if (year=="2017") {
	    for (int i=0; i<16; i++) {
	       triggerPass3l = triggerPass3l || triggers_2017_all[i];
	    }
	 }
      }
      
      if (data=="data") {
	 if (year=="2017") {
	    if (dataset=="SingleMuon") {
	       for (int i=0; i<2;i++) {
                  triggerPass3l = triggerPass3l || triggers_2017_SingleMuon[i];
               }
               for (int i=0; i<0; i++) {
                  triggerPass3l_exclude = triggerPass3l_exclude || triggers_2017_SingleMuon_exclude[i];
               }
	       triggerPass3l = triggerPass3l && !triggerPass3l_exclude;
	    }
            if (dataset=="SingleElectron") {
               for (int i=0; i<2; i++) {
                  triggerPass3l = triggerPass3l || triggers_2017_SingleElectron[i];
               }
               for (int i=0; i<2; i++) {
                  triggerPass3l_exclude = triggerPass3l_exclude || triggers_2017_SingleElectron_exclude[i];
               }
               triggerPass3l = triggerPass3l && !triggerPass3l_exclude;
            }
            if (dataset=="DoubleMuon") {
               for (int i=0; i<3; i++) {
                  triggerPass3l = triggerPass3l || triggers_2017_DoubleMuon[i];
               }
               for (int i=0; i<4; i++) {
                  triggerPass3l_exclude = triggerPass3l_exclude || triggers_2017_DoubleMuon_exclude[i];
               }
               triggerPass3l = triggerPass3l && !triggerPass3l_exclude;
	    }
            if (dataset=="DoubleEG") {
               for (int i=0; i<2; i++) {
                  triggerPass3l = triggerPass3l || triggers_2017_DoubleEG[i];
               }
               for (int i=0; i<7; i++) {
                  triggerPass3l_exclude = (triggerPass3l_exclude || triggers_2017_DoubleEG_exclude[i]);
               }
	       
               triggerPass3l = triggerPass3l && !triggerPass3l_exclude;
            }
            if (dataset=="MuonEG") {
               for (int i=0; i<7; i++) {
                  triggerPass3l = triggerPass3l || triggers_2017_MuonEG[i];
               }
               for (int i=0; i<9; i++) {
                  triggerPass3l_exclude = triggerPass3l_exclude || triggers_2017_MuonEG_exclude[i];
               }
               triggerPass3l = triggerPass3l && !triggerPass3l_exclude;
            }
	 }	 	 
      }
      if (!triggerPass3l) continue;

/*******************************************************************************************/
      // initialize candidate vectors
      selectedLeptons = new std::vector<lepton_candidate*>();
      selectedJets = new std::vector<jet_candidate*>();
      leptonCuts = new std::vector<std::vector<int>>();
      float sf_Ele_ID = 1.0;
      // if criteria is passed, add electrons to appropriate vector(s)
      int nTight = 0;
      int nFake = 0;
      float eleNominalWeight = 1.0;
      float muNominalWeight = 1.0; 
      bool isWellMeasured;
      bool eCuts[8];
      for (int i=0; i<nElectron; i++) {
	 conept = coneptElec(Electron_pt[i], Electron_mvaTTHUL[i], Electron_jetRelIso[i]);
	 jetBtagDeepFlav = (Electron_jetIdx[i]==-1||Electron_jetIdx[i]>nJet) ? -99 : Jet_btagDeepFlavB[Electron_jetIdx[i]];
	 idEmu = ttH_idEmu_cuts_E3(
	    ttHCuts, Electron_hoe[i], Electron_eta[i], Electron_deltaEtaSC[i], 
	    Electron_eInvMinusPInv[i], Electron_sieie[i]
	 );
	 isPres = isPresElec(
	    Electron_pt[i], Electron_eta[i], Electron_dxy[i], Electron_dz[i],
	    Electron_miniPFRelIso_all[i], Electron_sip3d[i], Electron_mvaFall17V2noIso_WPL[i]
	 );
	 isLoose = isLooseElec(
	    Electron_miniPFRelIso_all[i], Electron_sip3d[i], Electron_lostHits[i]
	 ) && isPres;
	 isFO = isFOElec(
	    Electron_pt[i], conept, jetBtagDeepFlav, idEmu, Electron_convVeto[i],
	    Electron_lostHits[i], Electron_mvaTTHUL[i], Electron_jetRelIso[i],
	    Electron_mvaFall17V2noIso_WP90[i], year
	 ) && Electron_tightCharge[i]==2 && isLoose;
	 isTight = tightSelElec(isFO, Electron_mvaTTHUL[i]) && isLoose;

         electronCuts(eCuts, Electron_pt[i], conept, jetBtagDeepFlav, idEmu, Electron_convVeto[i],
            Electron_lostHits[i], Electron_mvaTTHUL[i], Electron_jetRelIso[i],
            Electron_mvaFall17V2noIso_WP90[i], isPres, isLoose, Electron_tightCharge[i], year); 

         std::vector<int> cuts = electronCuts(i, Electron_charge[i], Electron_pt[i], conept, jetBtagDeepFlav, idEmu, Electron_convVeto[i],
            Electron_lostHits[i], Electron_mvaTTHUL[i], Electron_jetRelIso[i],
            Electron_mvaFall17V2noIso_WP90[i], isPres, isLoose, Electron_tightCharge[i], year);
         leptonCuts->push_back(cuts);
         
	 if(!(isFO || isTight)) continue;
         if (data=="mc" && isTight && Electron_genPartFlav[i]==0) continue;
         if (isTight) { nTight++; if (print) cout << "tight electron" << endl;}
         Float_t fopt = Electron_pt[i];
	 if (isFO && !(isTight)) {nFake++; if (print) cout << "fo electron" << endl;}

	 selectedLeptons->push_back(
	    new lepton_candidate(
	       Electron_pt[i], Electron_eta[i], Electron_phi[i], Electron_charge[i],
	        i, 1, isPres, isLoose, isFO, isTight, 2.0, Electron_pdgId[i]
	    )
	 );
         if (data=="mc") {
            sf_Ele_Reco = sf_Ele_Reco * csetEleIdReco->evaluate({year, "sf", (Electron_pt[i]>20?"RecoAbove20":"RecoBelow20"), Electron_eta[i],Electron_pt[i]}); 
            nominalWeights[getVecPos(sys,"eleRecoSf")] = nominalWeights[getVecPos(sys,"eleRecoSf")] * csetEleIdReco->evaluate({year, "sf", (Electron_pt[i]>20?"RecoAbove20":"RecoBelow20"), Electron_eta[i],Electron_pt[i]});
            sysUpWeights[getVecPos(sys,"eleRecoSf")] = sysUpWeights[getVecPos(sys,"eleRecoSf")] * csetEleIdReco->evaluate({year, "sfup", (Electron_pt[i]>20?"RecoAbove20":"RecoBelow20"), Electron_eta[i],Electron_pt[i]});
            sysDownWeights[getVecPos(sys,"eleRecoSf")] = sysDownWeights[getVecPos(sys,"eleRecoSf")] * csetEleIdReco->evaluate({year, "sfdown", (Electron_pt[i]>20?"RecoAbove20":"RecoBelow20"), Electron_eta[i],Electron_pt[i]});
            if (abs(Electron_eta[i])< 1.4442) {
               sf_Ele_ID = sf_Ele_ID *HEEPSF_B[year];
               nominalWeights[getVecPos(sys,"eleIDSf")] = nominalWeights[getVecPos(sys,"eleIDSf")] * HEEPSF_B[year];
               if(Electron_pt[i] <100){
                  sysUpWeights[getVecPos(sys,"eleIDSf")] = sysUpWeights[getVecPos(sys,"eleIDSf")] * HEEPSF_B[year] * 1.01;
                  sysDownWeights[getVecPos(sys,"eleIDSf")] = sysDownWeights[getVecPos(sys,"eleIDSf")] * HEEPSF_B[year] * 0.99;
               }
               else{
                  sysUpWeights[getVecPos(sys,"eleIDSf")] = sysUpWeights[getVecPos(sys,"eleIDSf")] * HEEPSF_B[year] * (1 + (0.01 + (0.02/900)*(Electron_pt[i]-100)));
                  sysDownWeights[getVecPos(sys,"eleIDSf")] = sysDownWeights[getVecPos(sys,"eleIDSf")] * HEEPSF_B[year] * (1 - (0.01 + (0.02/900)*(Electron_pt[i]-100)));
               }
            }
            else {
               sf_Ele_ID = sf_Ele_ID * HEEPSF_E[year];
               nominalWeights[getVecPos(sys,"eleIDSf")] = nominalWeights[getVecPos(sys,"eleIDSf")] * HEEPSF_E[year];
               if(Electron_pt[i] <100){
                  sysUpWeights[getVecPos(sys,"eleIDSf")] = sysUpWeights[getVecPos(sys,"eleIDSf")] * HEEPSF_B[year] * 1.02;
                  sysDownWeights[getVecPos(sys,"eleIDSf")] = sysDownWeights[getVecPos(sys,"eleIDSf")] * HEEPSF_B[year] * 0.98;
               }
               else if(Electron_pt[i] <300){
                  sysUpWeights[getVecPos(sys,"eleIDSf")] = sysUpWeights[getVecPos(sys,"eleIDSf")] * HEEPSF_B[year] * (1 + (0.02 + (0.03/200)*(Electron_pt[i]-100)));
                  sysDownWeights[getVecPos(sys,"eleIDSf")] = sysDownWeights[getVecPos(sys,"eleIDSf")] * HEEPSF_B[year] * (1 - (0.02 + (0.03/200)*(Electron_pt[i]-100)));
               }
               else{
                  sysUpWeights[getVecPos(sys,"eleIDSf")] = sysUpWeights[getVecPos(sys,"eleIDSf")] * HEEPSF_B[year] * 1.05;
                  sysDownWeights[getVecPos(sys,"eleIDSf")] = sysDownWeights[getVecPos(sys,"eleIDSf")] * HEEPSF_B[year] * 0.95;
               }
            }
         }
         if (!isTight) continue;
         if (!(data=="mc"&& Electron_pt[i]>10)) continue;
         eleNominalWeight*=csetEleIdReco->evaluate({year, "sf", (Electron_pt[i]>20?"RecoAbove20":"RecoBelow20"), Electron_eta[i],Electron_pt[i]});
         for (ScaleFactor* sf : *eleNominalSF) eleNominalWeight*=sf->getScaleFactor(abs(Electron_eta[i]), Electron_pt[i], "central");
      }

      // If criteria is passed, add muons to appropriate vector
      double RoccorPt;  
      for (int i=0; i<nMuon; i++) {
         muPtSFRochester=1;
         if(data == "data" && Muon_pt[i]>20 && abs(Muon_eta[i])<2.4) muPtSFRochester = rc.kScaleDT(Muon_charge[i], Muon_pt[i],Muon_eta[i],Muon_phi[i], 0, 0);
         if (data == "mc" && Muon_pt[i]>20 && abs(Muon_eta[i])<2.4){
            if (Muon_genPartIdx[i]>=0 && Muon_genPartIdx[i]<=nGenPart) muPtSFRochester = rc.kSpreadMC(Muon_charge[i], Muon_pt[i],Muon_eta[i],Muon_phi[i], GenPart_pt[Muon_genPartIdx[i]],0, 0);
            if (Muon_genPartIdx[i]<0) muPtSFRochester = rc.kSmearMC(Muon_charge[i], Muon_pt[i],Muon_eta[i],Muon_phi[i], Muon_nTrackerLayers[i] , gRandom->Rndm(),0, 0);
         }

         RoccorPt = Muon_pt[i]*muPtSFRochester;

	 conept = coneptMuon(RoccorPt, Muon_mvaTTHUL[i], Muon_jetRelIso[i], Muon_mediumId[i]);
	 jetBtagDeepFlav = (Muon_jetIdx[i]<0 || Muon_jetIdx[i]>nJet) ? -99 : Jet_btagDeepFlavB[Muon_jetIdx[i]]; 
         isPres = isPresMuon(
	    RoccorPt, Muon_eta[i], Muon_dxy[i], Muon_dz[i], Muon_miniPFRelIso_all[i], Muon_sip3d[i]
	 ); // Muon pt rochester corrections?
	 isLoose = isLooseMuon(
	    Muon_miniPFRelIso_all[i], Muon_sip3d[i], Muon_looseId[i]
	 ) && isPres;
	 isFO = isFOMuon(
	    RoccorPt, conept, jetBtagDeepFlav, Muon_mvaTTHUL[i], Muon_jetRelIso[i], year
	 ) && isLoose;
	 isTight = tightSelMuon(
	    isFO, Muon_mediumId[i], Muon_mvaTTHUL[i]
	 ) && isLoose;

	 muonCuts(muCuts, RoccorPt, conept, jetBtagDeepFlav, Muon_mvaTTHUL[i], Muon_jetRelIso[i], year, Muon_mediumId[i]);
         std::vector<int> cuts = muonCuts(i, Muon_charge[i], RoccorPt, conept, jetBtagDeepFlav, Muon_mvaTTHUL[i], Muon_jetRelIso[i], isPres, isLoose, year, Muon_mediumId[i]);
         leptonCuts->push_back(cuts);
	 if (!(isFO || isTight)) continue;
         if (data=="mc" && isTight && Muon_genPartFlav[i]==0) continue;
         if (isTight) { nTight++;if (print)cout << "tight muon" << endl;}
         if (isFO && !isTight){ nFake++; if (print) cout << "fo muon" << endl;}
         selectedLeptons->push_back(
	    new lepton_candidate(
	       RoccorPt, Muon_eta[i], Muon_phi[i], Muon_charge[i],
 	       i, 10, isPres, isLoose, isFO, isTight, 2.0, Muon_pdgId[i]
	    )
	 );

         if (!(isTight)) continue;
         if (!(data=="mc" && RoccorPt > 15)) continue;
         muNominalWeight *= csetMuReco->evaluate({year + "_UL", abs(Muon_eta[i]),  RoccorPt, "sf"});
         muNominalWeight *= csetMuLoose->evaluate({year + "_UL", abs(Muon_eta[i]),  RoccorPt, "sf"});
         for (ScaleFactor* sf : *muNominalSF) muNominalWeight*=sf->getScaleFactor(abs(Muon_eta[i]), RoccorPt, "central");
         highPtMu.SetPtEtaPhiM(Muon_tunepRelPt[i]*Muon_pt[i], Muon_eta[i],Muon_phi[i], 0.10566) ;
         sf_Mu_RECO=highPtMuRecoSF_pVsAbsEta_H->getScaleFactor(highPtMu.E(), abs(highPtMu.Eta()),"central");
         sf_Mu_ID = sf_Mu_ID * csetMuTightId->evaluate({year + "_UL", abs(Muon_eta[i]),  RoccorPt, "sf"})*sf_Mu_RECO;
         sf_Mu_ISO = sf_Mu_ISO * csetMuTightRelIso->evaluate({year + "_UL", abs(Muon_eta[i]),  RoccorPt, "sf"});

         nominalWeights[getVecPos(sys,"muIdIsoSf")] = 
            nominalWeights[getVecPos(sys,"muIdIsoSf")] * 
            csetMuTightRelIso->evaluate({year + "_UL", abs(Muon_eta[i]),  RoccorPt, "sf"}) * 
            csetMuTightId->evaluate({year + "_UL", abs(Muon_eta[i]),  RoccorPt, "sf"})*sf_Mu_RECO;

         sysUpWeights[getVecPos(sys,"muIdIsoSf")] = 
            sysUpWeights[getVecPos(sys,"muIdIsoSf")] * 
            csetMuTightRelIso->evaluate({year + "_UL", abs(Muon_eta[i]),  RoccorPt, "systup"}) * 
            csetMuTightId->evaluate({year + "_UL", abs(Muon_eta[i]),  RoccorPt, "systup"})*sf_Mu_RECO;

         sysDownWeights[getVecPos(sys,"muIdIsoSf")] = 
            sysDownWeights[getVecPos(sys,"muIdIsoSf")] * 
            csetMuTightRelIso->evaluate({year + "_UL", abs(Muon_eta[i]),  RoccorPt, "systdown"}) * 
            csetMuTightId->evaluate({year + "_UL", abs(Muon_eta[i]),  RoccorPt, "systdown"})*sf_Mu_RECO;
      }

      for (int i=0; i<nJet; i++) {
         pt = Jet_pt[i];
         eta = Jet_eta[i];
         phi = Jet_phi[i];
         mass = Jet_mass[i];
         btag = Jet_btagDeepB[i];
         ind = Jet_partonFlavour[i];
         cleanJet = true;
         for (int i=0; i<selectedLeptons->size(); i++) {
            Double_t lep_eta = (*selectedLeptons)[i]->eta_;
            Double_t lep_phi = (*selectedLeptons)[i]->phi_ ;
            if (deltaR(eta, phi, lep_eta, lep_phi) < 0.4) cleanJet = false;
         }
         if (cleanJet && pt>30 && abs(eta)<2.4) selectedJets->push_back(new jet_candidate(pt, eta, phi, mass, btag, year, ind));
      }
      // sort selected leptons
      
      sort(selectedLeptons->begin(), selectedLeptons->end(),ComparePtLep);
      sort(selectedJets->begin(), selectedJets->end(),ComparePtJet);
      // 2l categorization
      // Must have 2 leptons -- tight or fakeable

      int nLepTF = nFake+nTight;

      // 2l Fake channels
      // channel by number of fakeable leptons
      Float_t fakefactor =  1.0;
      for (auto it = (*selectedLeptons).begin(); it!=(*selectedLeptons).end(); ++it) {
	 if ((**it).lep_==1)  {
	    fakefactor*=((**it).isTightLep_) ? 1.0 : ele_sf->getScaleFactor((**it).pt_, (**it).eta_, "central");
	 }
	 else if ((**it).lep_==10)  {
	    fakefactor*= ((**it).isTightLep_) ? 1.0 : mu_sf->getScaleFactor((**it).pt_, (**it).eta_, "central");
	 }
      }

   // pile up
      if (data == "mc" && year == "2017") {
         weight_PU = wPU.PU_2017(int(Pileup_nTrueInt),"nominal");
         nominalWeights[getVecPos(sys,"pu")] = wPU.PU_2017(int(Pileup_nTrueInt),"nominal");
         sysUpWeights[getVecPos(sys,"pu")] = wPU.PU_2017(int(Pileup_nTrueInt),"up");
         sysDownWeights[getVecPos(sys,"pu")] = wPU.PU_2017(int(Pileup_nTrueInt),"down");
      }
   // prefiring
     if (data == "mc"){
        weight_prefiring = L1PreFiringWeight_Nom;
        nominalWeights[getVecPos(sys,"prefiring")] = L1PreFiringWeight_Nom;
        sysUpWeights[getVecPos(sys,"prefiring")] = L1PreFiringWeight_Up;
        sysDownWeights[getVecPos(sys,"prefiring")] = L1PreFiringWeight_Dn;
    }
         

      // btag scale factors
      float P_bjet_mc = 1.0, P_bjet_data = 1.0;
      for (int l=0; l<selectedJets->size(); l++) {
         if (data=="data") continue;
         jet_candidate *jet = (*selectedJets)[l];
         // get jet scale factors
         BJetSF = csetBcJetSF->evaluate({"central", "M", 5, abs((jet->eta_)),(jet->pt_)});
         CJetSF = csetBcJetSF->evaluate({"central", "M", 4, abs((jet->eta_)),(jet->pt_)});
         LJetSF = csetLightJetSF->evaluate({"central", "M", 0, abs((jet->eta_)),(jet->pt_)});
         BJetSF_UpCorr=csetBcJetSF->evaluate({"up_correlated", "M", 5, abs((*selectedJets)[l]->eta_),(*selectedJets)[l]->pt_});
         CJetSF_UpCorr=csetBcJetSF->evaluate({"up_correlated", "M", 4, abs((*selectedJets)[l]->eta_),(*selectedJets)[l]->pt_});
         LJetSF_UpCorr=csetLightJetSF->evaluate({"up_correlated", "M", 0, abs((*selectedJets)[l]->eta_),(*selectedJets)[l]->pt_});
         BJetSF_UpUnCorr=csetBcJetSF->evaluate({"up_uncorrelated", "M", 5, abs((*selectedJets)[l]->eta_),(*selectedJets)[l]->pt_});
         CJetSF_UpUnCorr=csetBcJetSF->evaluate({"up_uncorrelated", "M", 4, abs((*selectedJets)[l]->eta_),(*selectedJets)[l]->pt_});
         LJetSF_UpUnCorr=csetLightJetSF->evaluate({"up_uncorrelated", "M", 0, abs((*selectedJets)[l]->eta_),(*selectedJets)[l]->pt_});
         BJetSF_DownCorr=csetBcJetSF->evaluate({"down_correlated", "M", 5, abs((*selectedJets)[l]->eta_),(*selectedJets)[l]->pt_});
         CJetSF_DownCorr=csetBcJetSF->evaluate({"down_correlated", "M", 4, abs((*selectedJets)[l]->eta_),(*selectedJets)[l]->pt_});
         LJetSF_DownCorr=csetLightJetSF->evaluate({"down_correlated", "M", 0, abs((*selectedJets)[l]->eta_),(*selectedJets)[l]->pt_});
         BJetSF_DownUnCorr=csetBcJetSF->evaluate({"down_uncorrelated", "M", 5, abs((*selectedJets)[l]->eta_),(*selectedJets)[l]->pt_});
         CJetSF_DownUnCorr=csetBcJetSF->evaluate({"down_uncorrelated", "M", 4, abs((*selectedJets)[l]->eta_),(*selectedJets)[l]->pt_});
         LJetSF_DownUnCorr=csetLightJetSF->evaluate({"down_uncorrelated", "M", 0, abs((*selectedJets)[l]->eta_),(*selectedJets)[l]->pt_});
         BJetEff = btagEff_b_H->getScaleFactor(jet->pt_, abs(jet->eta_), "central");
         CJetEff = btagEff_c_H->getScaleFactor(jet->pt_, abs(jet->eta_),"central");
         LJetEff = btagEff_udsg_H->getScaleFactor(jet->pt_, abs(jet->eta_),"central");
         // b-quark
         if (abs(jet->flavor_)==5) {
            if ((*selectedJets)[l]->btag_) {
               P_bjet_mc *=  (BJetEff) ; 
               P_bjet_data *= (BJetEff*BJetSF);
               nominalWeights[getVecPos(sys,"bcTagSf")] = nominalWeights[getVecPos(sys,"bcTagSf")] * BJetEff * BJetSF;
               sysUpWeights[getVecPos(sys,"bcTagSf")] = sysUpWeights[getVecPos(sys,"bcTagSf")] * BJetEff * BJetSF_UpCorr;
               sysDownWeights[getVecPos(sys,"bcTagSf")] = sysDownWeights[getVecPos(sys,"bcTagSf")] * BJetEff * BJetSF_DownCorr;
               nominalWeights[getVecPos(sys,"LTagSf")] = nominalWeights[getVecPos(sys,"LTagSf")] * BJetEff * BJetSF;
               sysUpWeights[getVecPos(sys,"LTagSf")] = sysUpWeights[getVecPos(sys,"LTagSf")] * BJetEff * BJetSF;
               sysDownWeights[getVecPos(sys,"LTagSf")] = sysDownWeights[getVecPos(sys,"LTagSf")] * BJetEff * BJetSF;

               nominalWeights[getVecPos(sys,"bcTagSfUnCorr")] = nominalWeights[getVecPos(sys,"bcTagSfUnCorr")] * BJetEff * BJetSF;
               sysUpWeights[getVecPos(sys,"bcTagSfUnCorr")] = sysUpWeights[getVecPos(sys,"bcTagSfUnCorr")] * BJetEff * BJetSF_UpUnCorr;
               sysDownWeights[getVecPos(sys,"bcTagSfUnCorr")] = sysDownWeights[getVecPos(sys,"bcTagSfUnCorr")] * BJetEff * BJetSF_DownUnCorr;
               nominalWeights[getVecPos(sys,"LTagSfUnCorr")] = nominalWeights[getVecPos(sys,"LTagSfUnCorr")] * BJetEff * BJetSF;
               sysUpWeights[getVecPos(sys,"LTagSfUnCorr")] = sysUpWeights[getVecPos(sys,"LTagSfUnCorr")] * BJetEff * BJetSF;
               sysDownWeights[getVecPos(sys,"LTagSfUnCorr")] = sysDownWeights[getVecPos(sys,"LTagSfUnCorr")] * BJetEff * BJetSF;
            } else {
               P_bjet_mc = P_bjet_mc * (1 - BJetEff);
               P_bjet_data = P_bjet_data * (1- (BJetEff * BJetSF));
               nominalWeights[getVecPos(sys,"bcTagSf")] = nominalWeights[getVecPos(sys,"bcTagSf")]* (1- (BJetEff * BJetSF));
               sysUpWeights[getVecPos(sys,"bcTagSf")] = sysUpWeights[getVecPos(sys,"bcTagSf")]* (1- (BJetEff * BJetSF_UpCorr));
               sysDownWeights[getVecPos(sys,"bcTagSf")] = sysDownWeights[getVecPos(sys,"bcTagSf")]* (1- (BJetEff * BJetSF_DownCorr));
               nominalWeights[getVecPos(sys,"LTagSf")] = nominalWeights[getVecPos(sys,"LTagSf")]* (1- (BJetEff * BJetSF));
               sysUpWeights[getVecPos(sys,"LTagSf")] = sysUpWeights[getVecPos(sys,"LTagSf")]* (1- (BJetEff * BJetSF));
               sysDownWeights[getVecPos(sys,"LTagSf")] = sysDownWeights[getVecPos(sys,"LTagSf")]* (1- (BJetEff * BJetSF));

               nominalWeights[getVecPos(sys,"bcTagSfUnCorr")] = nominalWeights[getVecPos(sys,"bcTagSfUnCorr")]* (1- (BJetEff * BJetSF));
               sysUpWeights[getVecPos(sys,"bcTagSfUnCorr")] = sysUpWeights[getVecPos(sys,"bcTagSfUnCorr")]* (1- (BJetEff * BJetSF_UpUnCorr));
               sysDownWeights[getVecPos(sys,"bcTagSfUnCorr")] = sysDownWeights[getVecPos(sys,"bcTagSfUnCorr")]* (1- (BJetEff * BJetSF_DownUnCorr));
               nominalWeights[getVecPos(sys,"LTagSfUnCorr")] = nominalWeights[getVecPos(sys,"LTagSfUnCorr")]* (1- (BJetEff * BJetSF));
               sysUpWeights[getVecPos(sys,"LTagSfUnCorr")] = sysUpWeights[getVecPos(sys,"LTagSfUnCorr")]* (1- (BJetEff * BJetSF));
               sysDownWeights[getVecPos(sys,"LTagSfUnCorr")] = sysDownWeights[getVecPos(sys,"LTagSfUnCorr")]* (1- (BJetEff * BJetSF));
            }
         }
         // c quark
         else if (abs(jet->flavor_)==4) {
            if( (*selectedJets)[l]->btag_) {
               P_bjet_mc = P_bjet_mc * CJetEff;
               P_bjet_data = P_bjet_data * CJetEff * CJetSF;
               nominalWeights[getVecPos(sys,"bcTagSf")] = nominalWeights[getVecPos(sys,"bcTagSf")] * CJetEff * CJetSF;
               sysUpWeights[getVecPos(sys,"bcTagSf")] = sysUpWeights[getVecPos(sys,"bcTagSf")] * CJetEff * CJetSF_UpCorr;
               sysDownWeights[getVecPos(sys,"bcTagSf")] = sysDownWeights[getVecPos(sys,"bcTagSf")] * CJetEff * CJetSF_DownCorr;
               nominalWeights[getVecPos(sys,"LTagSf")] = nominalWeights[getVecPos(sys,"LTagSf")] * CJetEff * CJetSF;
               sysUpWeights[getVecPos(sys,"LTagSf")] = sysUpWeights[getVecPos(sys,"LTagSf")] * CJetEff * CJetSF;
               sysDownWeights[getVecPos(sys,"LTagSf")] = sysDownWeights[getVecPos(sys,"LTagSf")] * CJetEff * CJetSF;

               nominalWeights[getVecPos(sys,"bcTagSfUnCorr")] = nominalWeights[getVecPos(sys,"bcTagSfUnCorr")] * CJetEff * CJetSF;
               sysUpWeights[getVecPos(sys,"bcTagSfUnCorr")] = sysUpWeights[getVecPos(sys,"bcTagSfUnCorr")] * CJetEff * CJetSF_UpUnCorr;
               sysDownWeights[getVecPos(sys,"bcTagSfUnCorr")] = sysDownWeights[getVecPos(sys,"bcTagSfUnCorr")] * CJetEff * CJetSF_DownUnCorr;
               nominalWeights[getVecPos(sys,"LTagSfUnCorr")] = nominalWeights[getVecPos(sys,"LTagSfUnCorr")] * CJetEff * CJetSF;
               sysUpWeights[getVecPos(sys,"LTagSfUnCorr")] = sysUpWeights[getVecPos(sys,"LTagSfUnCorr")] * CJetEff * CJetSF;
               sysDownWeights[getVecPos(sys,"LTagSfUnCorr")] = sysDownWeights[getVecPos(sys,"LTagSfUnCorr")] * CJetEff * CJetSF;
            }
            if( !(*selectedJets)[l]->btag_ ) {
               P_bjet_mc = P_bjet_mc * (1 - CJetEff);
               P_bjet_data = P_bjet_data * (1- (CJetEff * CJetSF));
               nominalWeights[getVecPos(sys,"bcTagSf")] = nominalWeights[getVecPos(sys,"bcTagSf")]* (1- (CJetEff * CJetSF));
               sysUpWeights[getVecPos(sys,"bcTagSf")] = sysUpWeights[getVecPos(sys,"bcTagSf")]* (1- (CJetEff * CJetSF_UpCorr));
               sysDownWeights[getVecPos(sys,"bcTagSf")] = sysDownWeights[getVecPos(sys,"bcTagSf")]* (1- (CJetEff * CJetSF_DownCorr));
               nominalWeights[getVecPos(sys,"LTagSf")] = nominalWeights[getVecPos(sys,"LTagSf")]* (1- (CJetEff * CJetSF));
               sysUpWeights[getVecPos(sys,"LTagSf")] = sysUpWeights[getVecPos(sys,"LTagSf")]* (1- (CJetEff * CJetSF));
               sysDownWeights[getVecPos(sys,"LTagSf")] = sysDownWeights[getVecPos(sys,"LTagSf")]* (1- (CJetEff * CJetSF));

               nominalWeights[getVecPos(sys,"bcTagSfUnCorr")] = nominalWeights[getVecPos(sys,"bcTagSfUnCorr")]* (1- (CJetEff * CJetSF));
               sysUpWeights[getVecPos(sys,"bcTagSfUnCorr")] = sysUpWeights[getVecPos(sys,"bcTagSfUnCorr")]* (1- (CJetEff * CJetSF_UpUnCorr));
               sysDownWeights[getVecPos(sys,"bcTagSfUnCorr")] = sysDownWeights[getVecPos(sys,"bcTagSfUnCorr")]* (1- (CJetEff * CJetSF_DownUnCorr));
               nominalWeights[getVecPos(sys,"LTagSfUnCorr")] = nominalWeights[getVecPos(sys,"LTagSfUnCorr")]* (1- (CJetEff * CJetSF));
               sysUpWeights[getVecPos(sys,"LTagSfUnCorr")] = sysUpWeights[getVecPos(sys,"LTagSfUnCorr")]* (1- (CJetEff * CJetSF));
               sysDownWeights[getVecPos(sys,"LTagSfUnCorr")] = sysDownWeights[getVecPos(sys,"LTagSfUnCorr")]* (1- (CJetEff * CJetSF));
            }
         }
         
         // light quark / gluon
         else {
            if( (*selectedJets)[l]->btag_) {
               P_bjet_mc = P_bjet_mc * LJetEff;
               P_bjet_data = P_bjet_data * LJetEff * LJetSF;
               nominalWeights[getVecPos(sys,"bcTagSf")] = nominalWeights[getVecPos(sys,"bcTagSf")]* LJetEff * LJetSF;
               sysUpWeights[getVecPos(sys,"bcTagSf")] = sysUpWeights[getVecPos(sys,"bcTagSf")]* LJetEff * LJetSF;
               sysDownWeights[getVecPos(sys,"bcTagSf")] = sysDownWeights[getVecPos(sys,"bcTagSf")]* LJetEff * LJetSF;
               nominalWeights[getVecPos(sys,"LTagSf")] = nominalWeights[getVecPos(sys,"LTagSf")] * LJetEff * LJetSF;
               sysUpWeights[getVecPos(sys,"LTagSf")] = sysUpWeights[getVecPos(sys,"LTagSf")] * LJetEff * LJetSF_UpCorr;
               sysDownWeights[getVecPos(sys,"LTagSf")] = sysDownWeights[getVecPos(sys,"LTagSf")] * LJetEff * LJetSF_DownCorr;

               nominalWeights[getVecPos(sys,"bcTagSfUnCorr")] = nominalWeights[getVecPos(sys,"bcTagSfUnCorr")]* LJetEff * LJetSF;
               sysUpWeights[getVecPos(sys,"bcTagSfUnCorr")] = sysUpWeights[getVecPos(sys,"bcTagSfUnCorr")]* LJetEff * LJetSF;
               sysDownWeights[getVecPos(sys,"bcTagSfUnCorr")] = sysDownWeights[getVecPos(sys,"bcTagSfUnCorr")]* LJetEff * LJetSF;
               nominalWeights[getVecPos(sys,"LTagSfUnCorr")] = nominalWeights[getVecPos(sys,"LTagSfUnCorr")] * LJetEff * LJetSF;
               sysUpWeights[getVecPos(sys,"LTagSfUnCorr")] = sysUpWeights[getVecPos(sys,"LTagSfUnCorr")] * LJetEff * LJetSF_UpUnCorr;
               sysDownWeights[getVecPos(sys,"LTagSfUnCorr")] = sysDownWeights[getVecPos(sys,"LTagSfUnCorr")] * LJetEff * LJetSF_DownUnCorr;
            }
            if( !(*selectedJets)[l]->btag_ ) {
               P_bjet_mc = P_bjet_mc * (1 - LJetEff);
               P_bjet_data = P_bjet_data * (1- (LJetEff * LJetSF));
               nominalWeights[getVecPos(sys,"bcTagSf")] = nominalWeights[getVecPos(sys,"bcTagSf")]* (1- (LJetEff * LJetSF));
               sysUpWeights[getVecPos(sys,"bcTagSf")] = sysUpWeights[getVecPos(sys,"bcTagSf")]* (1- (LJetEff * LJetSF));
               sysDownWeights[getVecPos(sys,"bcTagSf")] = sysDownWeights[getVecPos(sys,"bcTagSf")]* (1- (LJetEff * LJetSF));
               nominalWeights[getVecPos(sys,"LTagSf")] = nominalWeights[getVecPos(sys,"LTagSf")]* (1- (LJetEff * LJetSF));
               sysUpWeights[getVecPos(sys,"LTagSf")] = sysUpWeights[getVecPos(sys,"LTagSf")]* (1- (LJetEff * LJetSF_UpCorr));
                  sysDownWeights[getVecPos(sys,"LTagSf")] = sysDownWeights[getVecPos(sys,"LTagSf")]* (1- (LJetEff * LJetSF_DownCorr));

               nominalWeights[getVecPos(sys,"bcTagSfUnCorr")] = nominalWeights[getVecPos(sys,"bcTagSfUnCorr")]* (1- (LJetEff * LJetSF));
               sysUpWeights[getVecPos(sys,"bcTagSfUnCorr")] = sysUpWeights[getVecPos(sys,"bcTagSfUnCorr")]* (1- (LJetEff * LJetSF));
               sysDownWeights[getVecPos(sys,"bcTagSfUnCorr")] = sysDownWeights[getVecPos(sys,"bcTagSfUnCorr")]* (1- (LJetEff * LJetSF));
               nominalWeights[getVecPos(sys,"LTagSfUnCorr")] = nominalWeights[getVecPos(sys,"LTagSfUnCorr")]* (1- (LJetEff * LJetSF));
               sysUpWeights[getVecPos(sys,"LTagSfUnCorr")] = sysUpWeights[getVecPos(sys,"LTagSfUnCorr")]* (1- (LJetEff * LJetSF_UpUnCorr));
               sysDownWeights[getVecPos(sys,"LTagSfUnCorr")] = sysDownWeights[getVecPos(sys,"LTagSfUnCorr")]* (1- (LJetEff * LJetSF_DownUnCorr));
            }
         }
      }

/*      if (iseft) weight_EFT = lumi*(1.0/nRuns)*eleNominalWeight*muNominalWeight*fakefactor*P_bjet_data/P_bjet_mc;
      if (iseft) eft_fit = new WCFit(nWCnames, wc_names_lst, nEFTfitCoefficients, EFTfitCoefficients, weight_EFT);
      if (k<10) {
         fout << eventID << endl;
         for (int i=0; i<12; i++) {
            string text{"EFTrwgt4_"+ wc_names_lst[i] + "_1.0"};
            WCPoint *wc = new WCPoint(text);
            Float_t weightA = eft_fit->evalPoint(wc);
            fout << wc_names_lst[i] <<":" << weightA << endl;
            delete wc;
         }
         k++;
      }
*/
      if (print) cout << "nLep == 2  " << (nLepTF ==2) << endl;
      if (nLepTF > 1) {
         // 2l lepton channels
         // channel by type of leptons and charge
         bool allMatched = true;
         for (int i=0; i<selectedLeptons->size(); i++)
         {
            if (!isMatched(this, (*selectedLeptons)[i]->indice_,(*selectedLeptons)[i]->pdgid_,data, false)) {
                  allMatched=false;
                  break;
            }
         }
         cat = EventCategory((*selectedLeptons), (*selectedJets),allMatched);
         ch = getIndex(channels, cat);
         int nbJets = 0;
         std::vector<TLorentzVector> bjets,ljets;
	 for (int i = 0; i<(*selectedJets).size(); i++) {
	    nbJets+=(*selectedJets)[i]->btag_;
            if ((*selectedJets)[i]->btag_) bjets.push_back((*selectedJets)[i]->p4_);
            else ljets.push_back((*selectedJets)[i]->p4_);
	 }
	 int reg = (nbJets>1) ? 2 : nbJets;

	 if (print) cout << cat << "  " << ch << endl;
	 mll = ((*selectedLeptons)[0]->p4_+(*selectedLeptons)[1]->p4_).M();
         onZ = isOnZ(*selectedLeptons);
         // if 2l fill fake_lepton_region_var hist [fakeChannel][ch][onZ]
        
 
         if (ch < 0) continue;
	 weight*=eleNominalWeight*muNominalWeight*fakefactor*P_bjet_data/P_bjet_mc*weight_PU*sf_Ele_ID*weight_prefiring*sf_Ele_Reco;
         if (iseft) weight_EFT = lumi*(1000.0/nRuns)*eleNominalWeight*muNominalWeight*fakefactor*P_bjet_data/P_bjet_mc*weight_PU*sf_Ele_ID*weight_prefiring*sf_Ele_Reco;
         if (iseft) eft_fit = new WCFit(nWCnames, wc_names_lst, nEFTfitCoefficients, EFTfitCoefficients, weight_EFT);
         else eft_fit = new WCFit(0, wc_names_lst,1,  &genWeight, 1.0); 
         if (iseft) weight = weight_EFT;
         isSignal = fname.Contains("FCNC");
         lep1Pt = (*selectedLeptons)[0]->pt_;
         lep1Eta = (*selectedLeptons)[0]->eta_;
         lep1Phi =(*selectedLeptons)[0]->phi_;
         lep2Pt = (*selectedLeptons)[1]->pt_;
         lep2Eta = (*selectedLeptons)[1]->eta_;
         lep2Phi = (*selectedLeptons)[1]->phi_;
         llM = ((*selectedLeptons)[0]->p4_+(*selectedLeptons)[1]->p4_).M();
         llPt = ((*selectedLeptons)[0]->p4_+(*selectedLeptons)[1]->p4_).Pt();
         llDr = deltaR((*selectedLeptons)[0]->eta_,(*selectedLeptons)[0]->phi_,(*selectedLeptons)[1]->eta_,(*selectedLeptons)[1]->phi_);
         llDphi = abs(deltaPhi((*selectedLeptons)[0]->phi_,(*selectedLeptons)[1]->phi_));
         njet = selectedJets->size();
         nbjet = nbJets;
         Met = MET_pt;
         MetPhi = MET_phi;
         nVtx = PV_npvs;
         jet1Pt = (njet>0) ? (*selectedJets)[0]->pt_ : -1;
         jet1Eta = (njet>0) ? (*selectedJets)[0]->eta_ : -4;
         jet1Phi = (njet>0) ? (*selectedJets)[0]->phi_ : -4;
         channel = ch;
         dset = (!iseft) ? DataSets[fname] : -1;
         float wgt=weight;

//         WCFit *wcfit=eft_fit;

         std::map<std::string,float> features;
         features["lep1Pt"] = lep1Pt;
         features["lep1Eta"] = lep1Eta;
         features["lep1Phi"] = lep1Phi;
         features["lep2Pt"] = lep2Pt;
         features["lep2Eta"] = lep2Eta;
         features["lep2Phi"] = lep2Phi;
         features["jet1Pt"] = jet1Pt;
         features["jet1Eta"] = jet1Eta;
         features["jet1Phi"] = jet1Phi;
         features["llM"] = llM ;
         features["llPt"] = llPt;
         features["llDr"] = llDr;
         features["llDphi"] = llDphi;
         features["Met"] = Met;
         features["MetPhi"] = MetPhi;
         features["njet"] = njet;
         features["nbjet"] = nbjet;
         features["nVtx"] = nVtx;
         float max_bdt=0;
         for (auto wc : wc_names_short){
            string c = cat.Data();
            if (c=="LFpp" || c=="FFpp") c = "LLpp";
            if (c=="LFmm" || c=="FFmm") c = "LLmm";
            if (c=="LLFonZ" || c== "LFFonZ" || c=="FFFonZ") c = "3LonZ";
            if (c=="LLFoffZp"||c== "LFFoffZp"||c== "FFFoffZp") c = "3LoffZp";
            if (c=="LLFoffZm"||c== "LFFoffZm"||c== "FFFoffZm") c = "3LoffZm";
            string r = regions[reg].Data();
            string w = wc;
            if (c=="3LonZ") c = "3Lonz";
            if (!modelLoaded(c,r,w,XGBModels))XGBModels[c][r][w] = loadModel(c,r,w,"/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/bin/savedjson/");
//            auto model = loadModel(c,r,w,"/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/bin/savedjson/");
            float pred = XGBModels[c][r][w]->predict_proba(features);
            if (pred > max_bdt) max_bdt = pred;
         }

         Hists[ch][reg][vInd(vars, "BDTOutput")]->Fill(max_bdt, wgt, *eft_fit);
         Hists[ch][3][vInd(vars, "BDTOutput")]->Fill(max_bdt, wgt, *eft_fit);

         Hists[ch][reg][vInd(vars, "lep1Pt")]->Fill(lep1Pt,wgt,*eft_fit);
         Hists[ch][reg][vInd(vars, "lep1Eta")]->Fill(lep1Eta,wgt,*eft_fit);
         Hists[ch][reg][vInd(vars, "lep1Phi")]->Fill(lep1Phi,wgt,*eft_fit);
         Hists[ch][reg][vInd(vars, "lep2Pt")]->Fill(lep2Pt,wgt,*eft_fit);
         Hists[ch][reg][vInd(vars, "lep2Eta")]->Fill(lep2Eta,wgt,*eft_fit);
         Hists[ch][reg][vInd(vars, "lep2Phi")]->Fill(lep2Phi,wgt,*eft_fit);
         Hists[ch][reg][vInd(vars, "llM")]->Fill(llM,wgt,*eft_fit);
         Hists[ch][reg][vInd(vars, "llPt")]->Fill(llPt,wgt,*eft_fit);
         Hists[ch][reg][vInd(vars, "llDr")]->Fill(llDr,wgt,*eft_fit);
         Hists[ch][reg][vInd(vars, "llDphi")]->Fill(llDphi,wgt,*eft_fit);
         Hists[ch][reg][vInd(vars, "njet")]->Fill(njet,wgt,*eft_fit);
         Hists[ch][reg][vInd(vars, "nbjet")]->Fill(nbjet,wgt,*eft_fit);
         Hists[ch][reg][vInd(vars, "Met")]->Fill(Met,wgt,*eft_fit);
         Hists[ch][reg][vInd(vars, "MetPhi")]->Fill(MetPhi,wgt,*eft_fit);
         Hists[ch][reg][vInd(vars, "nVtx")]->Fill(nVtx,wgt,*eft_fit);
         if(njet>0) {
            Hists[ch][reg][vInd(vars, "jet1Pt")]->Fill(jet1Pt,wgt,*eft_fit);
            Hists[ch][reg][vInd(vars, "jet1Eta")]->Fill(jet1Eta,wgt,*eft_fit);
            Hists[ch][reg][vInd(vars, "jet1Phi")]->Fill(jet1Phi,wgt,*eft_fit);
         }

         Hists[ch][3][vInd(vars, "lep1Pt")]->Fill(lep1Pt,wgt,*eft_fit);
         Hists[ch][3][vInd(vars, "lep1Eta")]->Fill(lep1Eta,wgt,*eft_fit);
         Hists[ch][3][vInd(vars, "lep1Phi")]->Fill(lep1Phi,wgt,*eft_fit);
         Hists[ch][3][vInd(vars, "lep2Pt")]->Fill(lep2Pt,wgt,*eft_fit);
         Hists[ch][3][vInd(vars, "lep2Eta")]->Fill(lep2Eta,wgt,*eft_fit);
         Hists[ch][3][vInd(vars, "lep2Phi")]->Fill(lep2Phi,wgt,*eft_fit);
         Hists[ch][3][vInd(vars, "llM")]->Fill(llM,wgt,*eft_fit);
         Hists[ch][3][vInd(vars, "llPt")]->Fill(llPt,wgt,*eft_fit);
         Hists[ch][3][vInd(vars, "llDr")]->Fill(llDr,wgt,*eft_fit);
         Hists[ch][3][vInd(vars, "llDphi")]->Fill(llDphi,wgt,*eft_fit);
         Hists[ch][3][vInd(vars, "njet")]->Fill(njet,wgt,*eft_fit);
         Hists[ch][3][vInd(vars, "nbjet")]->Fill(nbjet,wgt,*eft_fit);
         Hists[ch][3][vInd(vars, "Met")]->Fill(Met,wgt,*eft_fit);
         Hists[ch][3][vInd(vars, "MetPhi")]->Fill(MetPhi,wgt,*eft_fit);
         Hists[ch][3][vInd(vars, "nVtx")]->Fill(nVtx,wgt,*eft_fit);
         if(njet>0) {
            Hists[ch][3][vInd(vars, "jet1Pt")]->Fill(jet1Pt,wgt,*eft_fit);
            Hists[ch][3][vInd(vars, "jet1Eta")]->Fill(jet1Eta,wgt,*eft_fit);
            Hists[ch][3][vInd(vars, "jet1Phi")]->Fill(jet1Phi,wgt,*eft_fit);
         }
         if (data=="mc") {
            for (int n=0; n<sys.size(); n++) {
               delete eft_fit;
               if (iseft) eft_fit = new WCFit(nWCnames, wc_names_lst, nEFTfitCoefficients, EFTfitCoefficients, weight_EFT* (sysUpWeights[n]/nominalWeights[n]));
               else eft_fit = new WCFit(0,wc_names_lst,1, &genWeight, 1.0);
               wgt = weight * (sysUpWeights[n]/nominalWeights[n]);
               HistsSysUp[n][ch][reg][vInd(sys_vars, "BDTOutput")]->Fill(max_bdt,wgt,*eft_fit); 
               HistsSysUp[n][ch][3][vInd(sys_vars, "BDTOutput")]->Fill(max_bdt,wgt,*eft_fit);
            }
            for (int n=0; n<sys.size(); n++) {
               delete eft_fit;
               if (iseft) eft_fit = new WCFit(nWCnames, wc_names_lst, nEFTfitCoefficients, EFTfitCoefficients, weight_EFT* (sysDownWeights[n]/nominalWeights[n]));
               else eft_fit = new WCFit(0,wc_names_lst,1, &genWeight, 1.0);
               wgt = weight * (sysDownWeights[n]/nominalWeights[n]);
               HistsSysDown[n][ch][reg][vInd(sys_vars, "BDTOutput")]->Fill(max_bdt,wgt,*eft_fit);
               HistsSysDown[n][ch][3][vInd(sys_vars, "BDTOutput")]->Fill(max_bdt,wgt,*eft_fit);
            }
         } // end filling systematics
/*
         if(iseft){
            WCPoint *WCctp = new WCPoint("EFTrwgt4_ctp_1.0");
            WCPoint *WCctlS= new WCPoint("EFTrwgt4_ctlS_1.0");
            WCPoint *WCcte= new WCPoint("EFTrwgt4_cte_1.0");
            WCPoint *WCctl= new WCPoint("EFTrwgt4_ctl_1.0");
            WCPoint *WCctlT= new WCPoint("EFTrwgt4_ctlT_1.0");
            WCPoint *WCctZ= new WCPoint("EFTrwgt4_ctZ_1.0");
            WCPoint *WCcpt= new WCPoint("EFTrwgt4_cpt_1.0");
            WCPoint *WCcpQM= new WCPoint("EFTrwgt4_cpQM_1.0");
            WCPoint *WCctA= new WCPoint("EFTrwgt4_ctA_1.0");
            WCPoint *WCcQe= new WCPoint("EFTrwgt4_cQe_1.0");
            WCPoint *WCctG= new WCPoint("EFTrwgt4_ctG_1.0");
            WCPoint *WCcQlM= new WCPoint("EFTrwgt4_cQlM_1.0");

            wctp = eft_fit->evalPoint(WCctp);
            wctlS = eft_fit->evalPoint(WCctlS);
            wcte = eft_fit->evalPoint(WCcte);
            wctl = eft_fit->evalPoint(WCctl);
            wctlT = eft_fit->evalPoint(WCctlT);
            wctZ = eft_fit->evalPoint(WCctZ);
            wcpt = eft_fit->evalPoint(WCcpt);
            wcpQM = eft_fit->evalPoint(WCcpQM);
            wctA = eft_fit->evalPoint(WCctA);
            wcQe = eft_fit->evalPoint(WCcQe);
            wctG = eft_fit->evalPoint(WCctG);
            wcQlM = eft_fit->evalPoint(WCcQlM);

            delete WCctp, WCctlS, WCcte, WCctl, WCctlT, WCctZ, WCcpt, WCcpQM, WCctA, WCcQe, WCctG, WCcQlM;
         } else {
            wctp = weight;
            wctlS = weight;
            wcte = weight;
            wctl = weight;
            wctlT = weight;
            wctZ = weight;
            wcpt = weight;
            wcpQM = weight;
            wctA = weight;
            wcQe = weight;
            wctG = weight;
            wcQlM = weight;

         }
*/
//            for (int i=0; i<wc_names_lst_FCNC.size(); i++){
//               string text{"EFTrwgt4_"+wc_names_lst_FCNC[i]+"_1.0"};
//               WCPoint *wc = new WCPoint(text);
//               if (wc_names_lst[i]=="ctp") wctp = eft_fit->evalPoint(wc);
//               weights[i] = eft_fit->evalPoint(WCPoint("EFTrwgt4_"+wc_names_lst_FCNC[i]+"_1.0"));
//               delete wc;
//            }
//         }
//         else {
//            for (int i=0; i<wc_names_lst_FCNC.size(); i++) weights[i]=weight;
//         }
         
/*         std::map<std::string,float> features;
         f`eatures["lep1Pt"] = lep1Pt;
         features["lep1Eta"] = lep1Eta;
         features["lep1Phi"] = lep1Phi;
         features["lep2Pt"] = lep2Pt;
         features["lep2Eta"] = lep2Eta;
         features["lep2Phi"] = lep2Phi;
         features["jet1Pt"] = jet1Pt;
         features["jet1Eta"] = jet1Eta;
         features["jet1Phi"] = jet1Phi;
         features["llM"] = llM ;
         features["llPt"] = llPt;
         features["llDr"] = llDr;
         features["llDphi"] = llDphi;
         features["Met"] = Met;
         features["MetPhi"] = MetPhi;
         features["njet"] = njet;
         features["nbjet"] = nbjet;
         features["nVtx"] = nVtx;
         float pred;
         for (auto wc : wc_names)
         {
            string c = cat.Data();
            string r = regions[reg].Data();
            string w = wc.Data();
            if (c=="3LonZ") c = "3Lonz";
            if (!modelLoaded(c,r,w,XGBModels))XGBModels[c][r][w] = loadModel(c,r,w,"/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/bin/savedjson/");
            pred = XGBModels[c][r][w]->predict_proba(features);
            HistsBDTOutput[ch][reg][vInd(vars_bdt, wc+"TrainedBDT")]->Fill(pred,weight,*eft_fit);
         }
*/
         // jigsaw reco tZ
/*         if (cat == "3LonZ" && nbjet==1) {
            for(int i=0; i<selectedLeptons->size(); i++)
            {
               for(int j=i+1; j<selectedLeptons->size(); j++)
               {
                  bool sameFlavor = (*selectedLeptons)[i]->lep_==(*selectedLeptons)[j]->lep_;
                  bool oppoSign = (*selectedLeptons)[i]->charge_==-(*selectedLeptons)[j]->charge_;
                  bool onZ = abs((((*selectedLeptons)[i]->p4_+(*selectedLeptons)[j]->p4_).M())-91.1876)<10;
                  if (sameFlavor && oppoSign && onZ)
                  {
                     if((*selectedLeptons)[i]->charge_>0)
                     {
                        Lp_tZFCNC = (*selectedLeptons)[i]->p4_;
                        Lm_tZFCNC = (*selectedLeptons)[j]->p4_;
                        L_tZFCNC  = (*selectedLeptons)[3-i-j]->p4_;
                     } else {
                        Lp_tZFCNC = (*selectedLeptons)[i]->p4_;
                        Lm_tZFCNC = (*selectedLeptons)[j]->p4_;
                        L_tZFCNC  = (*selectedLeptons)[3-i-j]->p4_;
                     }
                  }
               }
            B_tZFCNC  = bjets[0];
            NU_tZFCNC.SetPtEtaPhiM(MET_pt, 0, MET_phi, 0);     
            jigsaw_tZFCNC->Analyze(L_tZFCNC, Lp_tZFCNC, Lm_tZFCNC, B_tZFCNC, NU_tZFCNC);
         }
*/
         // Jigsaw Reconstruction tt with reco level objects
/*         if (cat == "3LonZ" && nbjet==1 && njet>1)
         {
            for(int i=0; i<selectedLeptons->size(); i++) 
            {
               for(int j=i+1; j<selectedLeptons->size(); j++) 
               {
                  bool sameFlavor = (*selectedLeptons)[i]->lep_==(*selectedLeptons)[j]->lep_;
                  bool oppoSign = (*selectedLeptons)[i]->charge_==-(*selectedLeptons)[j]->charge_;
                  bool onZ = abs((((*selectedLeptons)[i]->p4_+(*selectedLeptons)[j]->p4_).M())-91.1876)<10;
                  if (sameFlavor && oppoSign && onZ)
                  {
                     if((*selectedLeptons)[i]->charge_>0)
                     {
                        lbp_ttFCNC = (*selectedLeptons)[i]->p4_;
                        lbm_ttFCNC = (*selectedLeptons)[j]->p4_;
                        la_ttFCNC  = (*selectedLeptons)[3-i-j]->p4_;
                     } else {
                        lbm_ttFCNC = (*selectedLeptons)[i]->p4_;
                        lbp_ttFCNC = (*selectedLeptons)[j]->p4_;
                        la_ttFCNC  = (*selectedLeptons)[3-i-j]->p4_;
                     }
                  }
               }
            }
            qb_ttFCNC = (*selectedJets)[lightJetIdx]->p4_;
            ba_ttFCNC  = bjets[0];
            nua_ttFCNC.SetPtEtaPhiM(MET_pt, 0, MET_phi, 0);
 
            jigsaw_ttFCNC->Analyze(la_ttFCNC, lbp_ttFCNC, lbm_ttFCNC, ba_ttFCNC, qb_ttFCNC, nua_ttFCNC);
         } //end tt reco
*/
/*         // jigsaw reconstruction for tH
         if ((cat== "LLpp"||cat=="LLmm") && nbjet==1 && njet>=3) 
         {
            // reco reconstruction
            Q1_tHFCNC = ljets[0];
            Q2_tHFCNC = ljets[1];
            float bestMass = 10000;
            for (int i=0; i<selectedJets->size(); i++) {
               for (int j=i+1; j<selectedJets->size(); j++) {
                  if((*selectedJets)[i]->btag_ ||(*selectedJets)[j]->btag_) continue;
                  if (abs(((*selectedJets)[i]->p4_+(*selectedJets)[j]->p4_).M()-80)<bestMass){
                     Q1_tHFCNC=(*selectedJets)[i]->p4_; Q2_tHFCNC=(*selectedJets)[j]->p4_;
                     bestMass=((*selectedJets)[i]->p4_ + (*selectedJets)[j]->p4_).M();

                  } 
               }
            }

            B_tHFCNC  = bjets[0];
            NUa_tHFCNC.SetPtEtaPhiM(MET_pt, 0, MET_phi, 0);   

            float minDR = 100.0, DR;
            int La_idx=0, Lb_idx=1;
            for (int i=0; i<selectedLeptons->size(); i++){
               DR = deltaR(B_tHFCNC, (*selectedLeptons)[i]->p4_);
               if (DR<minDR){
                  minDR=DR;
                  La_idx=i;
               }
            }
            Lb_idx = (La_idx==0) ? 1 : 0;
            La_tHFCNC = (*selectedLeptons)[La_idx]->p4_;
            Lb_tHFCNC = (*selectedLeptons)[Lb_idx]->p4_;
            // gen reconstruction
            if(La_tHFCNC.Pt()>0 &&  Lb_tHFCNC.Pt()>0 &&  Q1_tHFCNC.Pt()>0 && Q2_tHFCNC.Pt()>0 && B_tHFCNC.Pt()>0){
               jigsaw_tHFCNC->Analyze(La_tHFCNC, Lb_tHFCNC, Q1_tHFCNC, Q2_tHFCNC, B_tHFCNC, NUa_tHFCNC);     
               TLorentzVector H = jigsaw_tHFCNC->H->GetFourVector();
               TLorentzVector T = jigsaw_tHFCNC->T->GetFourVector();
               TLorentzVector WT = jigsaw_tHFCNC->WT->GetFourVector();
               TLorentzVector W1 = jigsaw_tHFCNC->W1->GetFourVector();
               TLorentzVector W2 = jigsaw_tHFCNC->W2->GetFourVector();
               TLorentzVector L0 = (*selectedLeptons)[0]->p4_;
               TLorentzVector L1 = (*selectedLeptons)[1]->p4_;
               //Hists_rjr[vInd(vars_rjr,"WTMass")]->Fill(WT.M(), weight);
               //Hists_rjr[vInd(vars_rjr,"W1Mass")]->Fill(W1.M(), weight);
               //Hists_rjr[vInd(vars_rjr,"W2Mass")]->Fill(W2.M(), weight);
               //Hists_rjr[vInd(vars_rjr,"TMass")]->Fill(T.M(), weight);
               //Hists_rjr[vInd(vars_rjr,"HMass")]->Fill(H.M(), weight);
               //Hists_rjr[vInd(vars_rjr,"tHDR")]->Fill(deltaR(H,T), weight);
               //Hists_rjr[vInd(vars_rjr,"tHDPhi")]->Fill(deltaPhi(H,T), weight);

               llss_WT_mass = WT.M();
               llss_T_mass = T.M();
               llss_H_mass = H.M();
               llss_tH_DR = deltaR(H,T);
               llss_tH_DPhi = deltaPhi(H,T);

               ml1H = (H+L1).M();
               nJet_ = selectedJets->size();
               mbt = (T+B_tHFCNC).M();
               mHb = (H+B_tHFCNC).M();
               ptWhad = W1.Pt();
               dRl1H = deltaR(L1,H);
               mWhad = W1.M();
               ptl1 = L1.Pt();
               etal1 = L1.Eta();
               dRHWtop = deltaR(H,WT);
               dRl0l1 = deltaR(L0,L1);
               ml1b = (L1 + B_tHFCNC).M();
               etab = B_tHFCNC.Eta();
               dRl0t = deltaR(L0, T);
               MET = MET_pt;
               fll0 = (*selectedLeptons)[0]->lep_;
               etal0 = L0.Eta();
               ptl0 = L0.Pt();
               dRl1t = deltaR(L1, T);
               mHWtop = (H+WT).M();
               dRl1Wtop = deltaR(L1, WT);
               ml0H = (L0+H).M();
               ptb = B_tHFCNC.Pt();
               //t1->Fill();
            }          
         }// end tH 
*/
         delete eft_fit;
	 
      }
   }
   f->Write();
   writeHists(f, Hists, "Nominal");
   if (data=="mc") {
      writeHists(f, HistsSysDown, "SysDown");
      writeHists(f, HistsSysUp, "SysUp");
   }

   f->Close();


}
