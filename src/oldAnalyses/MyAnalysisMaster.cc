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
#include <TLorentzVector.h>
#include <map>
#include <string>
#include <algorithm>

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

float deltar(float eta1, float eta2, float phi1, float phi2) {
   float deta = eta1-eta2;
   float dphi = phi1-phi2;
   return sqrt(deta*deta+dphi*dphi);
}



//void MyAnalysis::Loop(TString fname, TString data, TString dataset ,TString year, TString run, float xs, float lumi, float Nevent, int iseft, int nRuns)
void MyAnalysis::Loop(TString fname, TString data, TString dataset ,TString year, TString run, float xs, float lumi, float Nevent)
{
   TFile *f = new TFile("ANoutput.root","RECREATE");
   f->cd();

   std::vector<TString> regions{"llonZ", "lloffZ"};
   std::vector<TString> channels{"eeos", "eess+", "eess-", "emuos", "emuss+", "emuss-", "mumuos", "mumuss+", "mumuss-"};
   const std::map<TString, std::vector<float>> vars = 
   {
      {"LeadingLeptonPt",      	        {0, 	20, 	0, 	200	}},
      {"LeadingLeptonEta",              {1, 	20, 	-3, 	3	}},
      {"Mll",				{2, 	40, 	0, 	150	}},
//      {"Mll_Zwindow",			{3, 	40,	70,	110	}},
//      {"nJets", 			{4, 	15, 	0, 	15	}},
//      {"SubLeadingLeptonPt",            {5,     20,     0,      200     }},
//      {"SubLeadingLeptonEta",           {6,     20,     -3,     3       }},
   };

   Dim3 Hists = makeHists(channels, regions, vars);
   
   // 3l category
   // 
   std::vector<TString> regions_3l{"llonZ", "lloffZ"};
   std::vector<TString> channels_3l{   "eee+",    "eee-",
				      "eemu+",   "eemu-", 
				     "emumu+",  "emumu-",
				    "mumumu+", "mumumu-",}; 
   const std::map<TString, std::vector<float>> vars_3l =
   {
      {"l1pt",               { 0,     20,      0,      200     }},
      {"l1eta",              { 1,     20,     -3,        3     }},
      {"l2pt",               { 2,     20,      0,      150     }},
      {"l2eta",              { 3,     20,     -3,        3     }},
      {"j1pt",               { 4,     20,      0,      150     }},
      {"j1eta",              { 5,     20,     -3,        3     }},
      {"b1pt",               { 6,     20,      0,      150     }},
      {"b1eta",              { 7,     20,     -3,        3     }},
      {"nLeptons",           { 8,      5,      0,        5     }},
      {"nJets",              { 9,     15,      0,       15     }},
      {"nbJets",             {10,     15,      0,       15     }},
      {"totalLeptonCharge",  {11,      7,     -3,        4     }},
      {"cat3l", 	     {12,      5,      0, 	 5     }},
      {"electroncuts",       {13,      2,      0,        2     }},
      {"muoncuts",           {14,      5,      0,        5     }},
      {"nElectrons",         {15,      6,      0,        6     }},
      {"nMuons",             {16,      6,      0,        6     }},
      {"nElectrons_cut",     {17,      6,      0,        6     }},
      {"nMuons_cut",         {18,      6,      0,        6     }},

   };
   
   Dim3 Hists_3l = makeHists(channels_3l, regions_3l, vars_3l);
  
   const std::map<TString, std::vector<float>> vars_cuts =
   {
      {"cat3l",              {0,      5,      0,        5     }},
      {"electroncuts",       {1,      2,      0,        2     }},
      {"muoncuts",           {2,      5,      0,        5     }},
      {"nElectrons",         {3,      6,      0,        6     }},
      {"nMuons",             {4,      6,      0,        6     }},
      {"nElectrons_cut",     {5,      6,      0,        6     }},
      {"nMuons_cut",         {6,      6,      0,        6     }},
   };

   Dim1 cuts = makeHists(vars_cuts);

   const std::map<TString, std::vector<float>> vars_yeilds =
   {
      {"3l(+)1b",              {0,      4,      0,        4     }},
      {"3l(-)1b",              {1,      4,      0,        4     }},
      {"3l(+)2b",              {2,      4,      0,        4     }},
      {"3l(-)2b",              {3,      4,      0,        4     }},
   };
   
   Dim1 yeilds = makeHists(vars_yeilds);

   std::vector<TString> regions_2l_fakes {"ll", "lf", "ff"};
   Dim4 Hists_2l_fakes = makeHists(regions_2l_fakes, channels, regions, vars);

   // set up  utput file
   //
   // declare lepton candidate vectors -- 
   std::vector<lepton_candidate*> *selectedLeptons;
   std::vector<jet_candidate*> *selectedJets;



   // declare triggers and other variable
   bool triggerPass;
   bool triggerPassEE;
   bool triggerPassEMu;
   bool triggerPassMuMu;
   bool oppositeSign;
   bool sameSign;
   int onZ;
   Float_t pt, phi, eta, mll;
   Int_t charge;
   Int_t lep1, lep2;
   int ch;
   Double_t weight;
   bool cleanJet;
   Int_t ind;
   Double_t mass, btag;
   // 3l triggers
   bool triggerPass3l;
   bool triggerPass3l_exclude;
   bool triggerPassEEE;
   bool triggerPassEEMu;
   bool triggerPassEMuMu;
   bool triggerPassMuMuMu;
   bool positive3l;
   bool negative3l;
   int onZ3l;
   bool trilep;
   bool pt3l;
   bool pt251510;
   bool cat3l;
   int channel_lep;
   int channel_sign;
   bool DeepCSVbtag;
   int nbJets, sumlcharge, nCleanJets;
   int no;
   double i;
   Float_t conept, jetBtagDeepFlav;
   bool idEmu, isPres, isLoose, isFO, isTight;


   ScaleFactor *mu_sf = new ScaleFactor("FR_mva085_mu_data_comb_recorrected", "2017");
   ScaleFactor *ele_sf = new ScaleFactor("FR_mva090_el_data_comb_NC_recorrected", "2017");

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

//   for (Long64_t jentry=0; jentry<nentries;jentry++) {
   for (Long64_t jentry=0; jentry<1000;jentry++) {
      if (jentry%10000==0) cout << jentry << " events processed" << endl;
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
    //  displayProgress(jentry,ntr);
      
      // assign weight
      weight = 1;
      if (data == "mc") weight = ((1000*xs*lumi)/Nevent)*((genWeight>0)?1:-1);
      
      // initialize variables
      // trigger pass for each channel
      triggerPass = false;
      triggerPassEE= false;
      triggerPassEMu = false;
      triggerPassMuMu = false;
      oppositeSign = false;
      sameSign = false;

      triggerPass3l = false;
      triggerPass3l_exclude = false;
      triggerPassEEE = false;
      triggerPassEEMu = false;
      triggerPassEMuMu = false;
      triggerPassMuMuMu = false;
      positive3l = false;
      negative3l = false;
      pt3l = false;
      pt251510 = false;
      cat3l = false;
      trilep = false;
      DeepCSVbtag = false;
/*
      // trigger MC
      if(data == "mc" && year == "2017"){
         if(HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL || HLT_Ele35_WPTight_Gsf) triggerPassEE =true;
         if(HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ || HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL || HLT_Ele35_WPTight_Gsf || HLT_IsoMu27) triggerPassEMu =true;
         if(HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8 || HLT_IsoMu27) triggerPassMuMu =true;

	 if (HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL) triggerPassEEE = true;
	 if (HLT_Mu8_DiEle12_CaloIdL_TrackIdL) triggerPassEEMu = true;
 	 if (HLT_DiMu9_Ele9_CaloIdL_TrackIdL_DZ) triggerPassEMuMu = true;
	 if (HLT_TripleMu_12_10_5) triggerPassMuMuMu = true;
      }
   
      // trigger Data

      if (data == "data") {
         if(year == "2017"){
	    if (HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL) triggerPassEEE = true;
            if (HLT_Mu8_DiEle12_CaloIdL_TrackIdL) triggerPassEEMu = true;
            if (HLT_DiMu9_Ele9_CaloIdL_TrackIdL_DZ) triggerPassEMuMu = true;
            if (HLT_TripleMu_12_10_5) triggerPassMuMuMu = true;

            if(dataset=="MuonEG"){
               if(HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ || HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL ) triggerPassEMu =true;
            }
            if(dataset=="SingleElectron"){
               if(!HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL && HLT_Ele35_WPTight_Gsf) triggerPassEE =true;
               if(!(HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ || HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL) && HLT_Ele35_WPTight_Gsf) triggerPassEMu =true;
            }
            if(dataset=="SingleMuon"){
               if(!HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8 && HLT_IsoMu27) triggerPassMuMu =true;
               if(!(HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ || HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL || HLT_Ele35_WPTight_Gsf) && HLT_IsoMu27) triggerPassEMu =true;
            }
            if(dataset=="DoubleEG"){
               if(HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL) triggerPassEE =true;
            }
            if(dataset=="DoubleMuon"){
               if(HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8)triggerPassMuMu =true;
       HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL,
         HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL,
         HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ,
         HLT_M     }
         }  

      }
*/
/***********************************************
 *   Data triggers using topcoffea approach 
 ***********************************************/

      Bool_t triggers_2017_SingleMuon[] = {
	 HLT_IsoMu24,
	 HLT_IsoMu27
      };

      Bool_t triggers_2017_SingleElectron[] = {
	 HLT_Ele35_WPTight_Gsf
      };

      Bool_t triggers_2017_DoubleMuon[] = {
	 HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ,
         HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8,
         HLT_TripleMu_12_10_5
      };

      Bool_t triggers_2017_DoubleEG[] = {
	 HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL,
         HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ,
         HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL
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
         HLT_Ele35_WPTight_Gsf
      };

      //exclude single muon + single electron + DoubleMuon triggers
      Bool_t triggers_2017_DoubleEG_exclude[] = {
         HLT_IsoMu24,
         HLT_IsoMu27,
         HLT_Ele35_WPTight_Gsf,
         HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ,
         HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8,
         HLT_TripleMu_12_10_5
      };

      //exclude single muon + single electron + DoubleMuon + DoubleEG triggers
      Bool_t triggers_2017_MuonEG_exclude[] = {
         HLT_IsoMu24,
         HLT_IsoMu27,
         HLT_Ele35_WPTight_Gsf,
         HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ,
         HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8,
         HLT_TripleMu_12_10_5,
         HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL,
         HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ,
         HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL
      };

      Bool_t triggers_2017_all[] = {
         HLT_IsoMu24,
         HLT_IsoMu27,
         HLT_Ele35_WPTight_Gsf,
         HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ,
         HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8,
         HLT_TripleMu_12_10_5,
         HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL,
         HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ,
         HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL,
         HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL,
         HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ,
         HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ,
         HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ,
         HLT_Mu8_DiEle12_CaloIdL_TrackIdL,
         HLT_Mu8_DiEle12_CaloIdL_TrackIdL_DZ,
         HLT_DiMu9_Ele9_CaloIdL_TrackIdL_DZ,
      };


      if (data=="mc") {
	 if (year=="2017") {
	    for (int i=0; i<16; i++) {
	       triggerPass3l = triggerPass3l || triggers_2017_all[i];
	       cout << triggers_2017_all[i];
	    }
	 }
      }
      cout << endl;
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
               for (int i=0; i<1; i++) {
                  triggerPass3l = triggerPass3l || triggers_2017_SingleElectron[i];
               }
               for (int i=0; i<2; i++) {
                  triggerPass3l_exclude = triggerPass3l_exclude || triggers_2017_SingleElectron_exclude[i];
               }
               triggerPass3l = triggerPass3l && !triggerPass3l_exclude;
            }
            if (dataset=="DoubleMuon") {
               for (int i=0; i<3; i++) {
                  triggerPass3l = triggerPass3l || triggers_2017_DoubleMuon[0];
               }
               for (int i=0; i<3; i++) {
                  triggerPass3l_exclude = triggerPass3l_exclude || triggers_2017_DoubleMuon_exclude[i];
               }
               triggerPass3l = triggerPass3l && !triggerPass3l_exclude;
	    }
            if (dataset=="DoubleEG") {
               for (int i=0; i<3; i++) {
                  triggerPass3l = triggerPass3l || triggers_2017_DoubleEG[i];
               }
               for (int i=0; i<6; i++) {
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
/*
      // if triggers not passed then continue
      if(!(triggerPassEE || triggerPassEMu || triggerPassMuMu ||
	   triggerPassEEE || triggerPassEEMu || triggerPassEMuMu || triggerPassMuMuMu)) continue;
*/
      // initialize candidate vectors
      selectedLeptons = new std::vector<lepton_candidate*>();
      selectedJets = new std::vector<jet_candidate*>();
      // if criteria is passed, add electrons to appropriate vector(s)
      int nTight = 0;
      int nFake = 0;
      Float_t sf;
      for (int i=0; i<nElectron; i++) {

	 conept = coneptElec(Electron_pt[i], Electron_mvaTTH[i], Electron_jetRelIso[i]);
	 jetBtagDeepFlav = (Electron_jetIdx[i]==-1) ? -99 : Jet_btagDeepFlavB[Electron_jetIdx[i]];
	 idEmu = ttH_idEmu_cuts_E3(
	    Electron_hoe[i], Electron_eta[i], Electron_deltaEtaSC[i], 
	    Electron_eInvMinusPInv[i], Electron_sieie[i]
	 );
	 isPres = isPresElec(
	    Electron_pt[i], Electron_eta[i], Electron_dxy[i], Electron_dz[i],
	    Electron_miniPFRelIso_all[i], Electron_sip3d[i], Electron_mvaFall17V2noIso_WPL[i]
	 );
	 isLoose = isLooseElec(
	    Electron_miniPFRelIso_all[i], Electron_sip3d[i], Electron_lostHits[i]
	 );
	 isFO = isFOElec(
	    Electron_pt[i], conept, jetBtagDeepFlav, idEmu, Electron_convVeto[i],
	    Electron_lostHits[i], Electron_mvaTTH[i], Electron_jetRelIso[i],
	    Electron_mvaFall17V2noIso_WP90[i], year
	 );
	 isTight = tightSelElec(isFO, Electron_mvaTTH[i]);

	 if(!(isFO || isTight)) continue;
         if (isTight) nTight++;
	 if (isFO && !isTight) nFake++;
         sf = (isTight) ? 1.0 : ele_sf->getScaleFactor(Electron_pt[i], Electron_eta[i]);
//         cout << isTight << " " << eta << " " << pt << " " << sf << endl;
	 selectedLeptons->push_back(
	    new lepton_candidate(
	       Electron_pt[i], Electron_eta[i], Electron_phi[i], Electron_charge[i],
	        i, 1, isPres, isLoose, isFO, isTight, 2.0
	    )
	 );
      }
      // If criteria is passed, add muons to appropriate vector
      for (int i=0; i<nMuon; i++) {

	 conept = coneptMuon(Muon_pt[i], Muon_mvaTTH[i], Muon_jetRelIso[i], Muon_mediumId[i]);
	 jetBtagDeepFlav = (Muon_jetIdx[i]==-1) ? -99 : Jet_btagDeepFlavB[Muon_jetIdx[i]]; 
	 isPres = isPresMuon(
	    Muon_pt[i], Muon_eta[i], Muon_dxy[i], Muon_dz[i], Muon_miniPFRelIso_all[i], Muon_sip3d[i]
	 ); // Muon pt rochester corrections?
	 isLoose = isLooseMuon(
	    Muon_miniPFRelIso_all[i], Muon_sip3d[i], Muon_looseId[i]
	 );
	 isFO = isFOMuon(
	    Muon_pt[i], conept, jetBtagDeepFlav, Muon_mvaTTH[i], Muon_jetRelIso[i], year
	 );
	 isTight = tightSelMuon(
	    isFO, Muon_mediumId[i], Muon_mvaTTH[i]
	 );

	 if (!(isFO || isTight)) continue;
         if (isTight) { nTight++;}
         if (isFO && !isTight){ nFake++; }
         sf = (isTight) ? 1.0 : mu_sf->getScaleFactor(Muon_pt[i], Muon_eta[i]);
//         cout << isTight << " " << eta << " " << pt << " " << sf << endl;
         selectedLeptons->push_back(
	    new lepton_candidate(
	       Muon_pt[i], Muon_eta[i], Muon_phi[i], Muon_charge[i],
 	       i, 10, isPres, isLoose, isFO, isTight, 2.0
	    )
	 );
         
      }
      for (int i=0; i<selectedLeptons->size(); i++) {
//	 cout << (*selectedLeptons)[i]->scalefactor_ << "\t";
      }
//      cout << endl;
      // if clean, add jet candidate
      for (int i=0; i<nJet; i++) {
	 pt = Jet_pt[i];
	 eta = Jet_eta[i];
	 phi = Jet_phi[i];
	 mass = Jet_mass[i];
	 btag = Jet_btagCSVV2[i];
	 ind = Jet_partonFlavour[i];
	 cleanJet = true;
	 for (int i=0; i<selectedLeptons->size(); i++) {
	    Double_t lep_eta = (*selectedLeptons)[i]->eta_;
	    Double_t lep_phi = (*selectedLeptons)[i]->phi_ ;
	    if (deltar(eta, lep_eta, phi, lep_phi) < 0.4) cleanJet = false;	    
	 } 
	 if (cleanJet) selectedJets->push_back(new jet_candidate(pt, eta, phi, mass, btag, year, ind));
      }
      // sort selected leptons
      
      sort(selectedLeptons->begin(), selectedLeptons->end(),ComparePtLep);
      sort(selectedJets->begin(), selectedJets->end(), ComparePtJet);

      // 2l categorization
      // Must have 2 leptons -- tight or fakeable

      int nLepTF = nFake+nTight;

      // 2l Fake channels
      // channel by number of fakeable leptons
      Float_t fakefactor =  1.0;
      for (auto it = (*selectedLeptons).begin(); it!=(*selectedLeptons).end(); ++it) {
	 if ((**it).lep_==1)  {
	    fakefactor*=((**it).isTightLep_) ? 1.0 : ele_sf->getScaleFactor((**it).pt_, (**it).eta_);
	    cout << "electron scale factor: " << ele_sf->getScaleFactor((**it).pt_, (**it).eta_) << endl;
	 }
	 else if ((**it).lep_==10)  {
	    fakefactor*= ((**it).isTightLep_) ? 1.0 : mu_sf->getScaleFactor((**it).pt_, (**it).eta_);
	     cout << "muon scale factor: " << mu_sf->getScaleFactor((**it).pt_, (**it).eta_) << endl;
	 }
	 cout << jentry<< " " << (**it).lep_ << " " << fakefactor << "     " << (**it).pt_ << "   " << (**it).eta_ << endl;
      }
      //cout << fakefactor << endl;
      int fakeChannel;
      //cout << nFake << "  " << nTight << "  " << nLepTF << endl;   
      if (nLepTF == 2) {
//	 cout << "nLepTF == 2" << endl;

         if (nTight==2 && nFake==0) fakeChannel = 0; // ll
         if (nTight==1 && nFake==1) fakeChannel = 1; // lf
         if (nTight==0 && nFake==2) fakeChannel = 2; // ff

//	 if (nTight==2 && nFake==0) fakefactor = 1.0; // ll
//         if (nTight==1 && nFake==1) fakefactor = 0.1; // lf
//         if (nTight==0 && nFake==2) fakefactor = 0.01; // ff

         // 2l lepton channels
         // channel by type of leptons and charge

      
         if ((*selectedLeptons)[0]->charge_ == -(*selectedLeptons)[1]->charge_) oppositeSign= true;
         if ((*selectedLeptons)[0]->charge_ == (*selectedLeptons)[1]->charge_) sameSign= true;
         lep1 = (*selectedLeptons)[0]->lep_;
         lep2 = (*selectedLeptons)[1]->lep_;
         if  (lep1+lep2 == 2 && oppositeSign) ch=0;        // "eeos"
         if  (lep1+lep2 == 2 && sameSign) {                // "eess"
            ch = ((*selectedLeptons)[0]->charge_ >0) ? 1:2;
         }
         if  (lep1+lep2 == 11 && oppositeSign) ch=3;       // "emuos"
         if  (lep1+lep2 == 11 && sameSign) {               // "emuss"
            ch = ((*selectedLeptons)[0]->charge_ >0) ? 4:5;
         }
         if  (lep1+lep2 == 20 && oppositeSign) ch=6;       // "mumuos"
         if  (lep1+lep2 == 20 && sameSign) {               // "mumuss"
            ch = ((*selectedLeptons)[0]->charge_ >0) ? 7:8;
         }

         // 2l regions
         // on or off Z peak

         mll = ((*selectedLeptons)[0]->p4_+(*selectedLeptons)[1]->p4_).M();
         onZ = (mll>76 && mll<106) ? 0 : 1;

         cout << "weight / sf / weight*sf: " << weight << " " << fakefactor << " " << weight*fakefactor << endl;
         // if 2l fill fake_lepton_region_var hist
         Hists_2l_fakes[fakeChannel][ch][onZ][0]->Fill((*selectedLeptons)[0]->pt_, weight*fakefactor);
         Hists_2l_fakes[fakeChannel][ch][onZ][1]->Fill((*selectedLeptons)[0]->eta_, weight*fakefactor);
//         Hists_2l_fakes[fakeChannel][ch][onZ][5]->Fill((*selectedLeptons)[1]->pt_, weight);
//         Hists_2l_fakes[fakeChannel][ch][onZ][6]->Fill((*selectedLeptons)[1]->eta_, weight);
         Hists_2l_fakes[fakeChannel][ch][onZ][2]->Fill(mll, weight*fakefactor);
//         Hists_2l_fakes[fakeChannel][ch][onZ][3]->Fill(mll, weight);
//         Hists_2l_fakes[fakeChannel][ch][onZ][4]->Fill(nJet, weight);
//	 cout << fakeChannel << ch << onZ << endl;
         
      }
      // 3l categorization
/*      trilep = selectedLeptons->size()==3;
      DeepCSVbtag = count_if(selectedJets->begin(), selectedJets->end(), isBJet)>0;
      if (trilep) {
	 pt3l = ((*selectedLeptons)[2]->lep_==1) ? (*selectedLeptons)[2]->pt_ > 15 : (*selectedLeptons)[2]->pt_ > 10;
	 pt251510 = (*selectedLeptons)[0]->pt_>25 && (*selectedLeptons)[1]->pt_>15 && pt3l;
      }

      cat3l = triggerPass3l && trilep && pt251510 && DeepCSVbtag;
//      if (triggerPass3l) cuts[0]->Fill(0., weight);
//      if (trilep) cuts[0]->Fill(1., weight);
//      if (pt251510) cuts[0]->Fill(2., weight);
//      if (DeepCSVbtag) cuts[0]->Fill(3., weight);
//      if (cat3l) cuts[0]->Fill(4., weight);
      //3l channels
      if (cat3l) {
	 channel_lep = (*selectedLeptons)[0]->lep_+(*selectedLeptons)[1]->lep_+(*selectedLeptons)[2]->lep_;
	 channel_sign = (*selectedLeptons)[0]->charge_ + (*selectedLeptons)[1]->charge_ + (*selectedLeptons)[2]->charge_;
	 if (channel_lep==3 && channel_sign>0) ch=9;		//eee+
	 if (channel_lep==3 && channel_sign<0) ch=10;		//eee-
         if (channel_lep==12 && channel_sign>0) ch=11;		//eemu+
         if (channel_lep==12 && channel_sign<0) ch=12;		//eemu-
         if (channel_lep==21 && channel_sign>0) ch=13;		//emumu+
         if (channel_lep==21 && channel_sign<0) ch=14;		//emumu-
         if (channel_lep==30 && channel_sign>0) ch=15;		//mumumu+
         if (channel_lep==30 && channel_sign<0) ch=16;		//mumumu-

      }

      // 3l regions
      bool sameFlavor, oppositeCharge, massOnZ;
      if (cat3l) {
	 for (int i=0; i<3; i++) {
	    sameFlavor = (*selectedLeptons)[i]->lep_==(*selectedLeptons)[(i+1)%3]->lep_;
	    oppositeCharge = (*selectedLeptons)[i]->charge_==-(*selectedLeptons)[(i+1)%3]->charge_;
	    mll = ((*selectedLeptons)[i]->p4_+(*selectedLeptons)[(i+1)%3]->p4_).M();
	    massOnZ = mll>76 && mll<106;
	    onZ3l = (sameFlavor && oppositeCharge && massOnZ) ? 0: 1;
	 }
      }

      // plotting yeilds 3l
      //
      if (cat3l) {
	 nbJets = count_if(selectedJets->begin(), selectedJets->end(), isBJet);
         nCleanJets = selectedJets->size();
         channel_sign = (*selectedLeptons)[0]->charge_ + (*selectedLeptons)[1]->charge_ + (*selectedLeptons)[2]->charge_;
	 
	 if (nbJets == 1 && channel_sign>0) no=0;
	 if (nbJets == 1 && channel_sign<0) no=1;
 	 if (nbJets > 1 && channel_sign>0) no=2;
         if (nbJets > 1 && channel_sign<0) no=3;
	 i = min(3, nCleanJets-2); // bin 0 = 2 jets -- bin 3 = 5+ jets
	 cout << no <<endl;	 
	 yeilds[no]->Fill(i, weight);
      }

      // 3l Plotting
      //
//      cuts[0]->Fill((cat3l)?1:0, weight);
      if (cat3l) {
*/ /*        Hists_3l[ch-9][onZ3l][0]->Fill((*selectedLeptons)[0]->pt_, weight);
         Hists_3l[ch-9][onZ3l][1]->Fill((*selectedLeptons)[0]->eta_, weight);
	 Hists_3l[ch-9][onZ3l][2]->Fill((*selectedLeptons)[1]->pt_, weight);
         Hists_3l[ch-9][onZ3l][3]->Fill((*selectedLeptons)[1]->eta_, weight);
         if (selectedJets->size()>0) {
            Hists_3l[ch-9][onZ3l][4]->Fill((*selectedJets)[0]->pt_, weight);
            Hists_3l[ch-9][onZ3l][5]->Fill((*selectedJets)[0]->eta_, weight);
	 }
         nbJets=0;
	 bool f=false;
	 int b1=-1;
	 for (int i=0; i<selectedJets->size(); i++) {
	    if ((*selectedJets)[i]->btag_) {
	       nbJets++;
	       if(!f) {
		  f=true;
		  b1=i;
	       }
	    }
	 }
         if (b1>=0){
 	    Hists_3l[ch-9][onZ3l][6]->Fill((*selectedJets)[b1]->pt_, weight);
            Hists_3l[ch-9][onZ3l][7]->Fill((*selectedJets)[b1]->eta_, weight);
	 }
	 Hists_3l[ch-9][onZ3l][8]->Fill(selectedLeptons->size(), weight);
         Hists_3l[ch-9][onZ3l][9]->Fill(selectedJets->size(), weight);
	 Hists_3l[ch-9][onZ3l][10]->Fill(nbJets, weight);  
	 Hists_3l[ch-9][onZ3l][11]->Fill((*selectedLeptons)[0]->charge_ + (*selectedLeptons)[1]->charge_ +(*selectedLeptons)[2]->charge_, weight);                    
*/	 
//      }
/*      // check is analysis cuts are passed and then categorize channels
      if (selectedLeptons->size() < 2) continue;
      if ((*selectedLeptons)[0]->charge_ == -(*selectedLeptons)[1]->charge_) oppositeSign= true;
      if ((*selectedLeptons)[0]->charge_ == (*selectedLeptons)[1]->charge_) sameSign= true;
      lep1 = (*selectedLeptons)[0]->lep_;
      lep2 = (*selectedLeptons)[1]->lep_;   
      if  (lep1+lep2 == 2 && oppositeSign) ch=0;	// "eeos"
      if  (lep1+lep2 == 2 && sameSign) { 		// "eess"
	 ch = ((*selectedLeptons)[0]->charge_ >0) ? 1:2;
      }
      if  (lep1+lep2 == 11 && oppositeSign) ch=3; 	// "emuos"
      if  (lep1+lep2 == 11 && sameSign) { 		// "emuss"
	 ch = ((*selectedLeptons)[0]->charge_ >0) ? 4:5;
      }
      if  (lep1+lep2 == 20 && oppositeSign) ch=6; 	// "mumuos"
      if  (lep1+lep2 == 20 && sameSign) { 		// "mumuss"
	 ch = ((*selectedLeptons)[0]->charge_ >0) ? 7:8;
      }
      if 
      // if (channel and trigger match) triggerPass = true
      if (triggerPassEE && (ch==0||ch==1||ch==2)) triggerPass = true;
      if (triggerPassEMu && (ch==3||ch==4||ch==5)) triggerPass = true;
      if (triggerPassMuMu && (ch==6||ch==7||ch==8)) triggerPass = true;

      mll = ((*selectedLeptons)[0]->p4_+(*selectedLeptons)[1]->p4_).M();
      onZ = (mll>76 && mll<106);

      // fill histograms
      //
      if (triggerPass) {
	 if (onZ) {
	    Hists[ch][0][0]->Fill((*selectedLeptons)[0]->pt_, weight);
	    Hists[ch][0][1]->Fill((*selectedLeptons)[0]->eta_, weight);
            Hists[ch][0][5]->Fill((*selectedLeptons)[1]->pt_, weight);
            Hists[ch][0][6]->Fill((*selectedLeptons)[1]->eta_, weight); 
	    Hists[ch][0][2]->Fill(mll, weight);
	    Hists[ch][0][3]->Fill(mll, weight);
	    Hists[ch][0][4]->Fill(nJet, weight);
	 } else {
            Hists[ch][1][0]->Fill((*selectedLeptons)[0]->pt_, weight);
            Hists[ch][1][1]->Fill((*selectedLeptons)[0]->eta_, weight);
            Hists[ch][1][5]->Fill((*selectedLeptons)[1]->pt_, weight);
            Hists[ch][1][6]->Fill((*selectedLeptons)[1]->eta_, weight);
            Hists[ch][1][2]->Fill(mll, weight);
            Hists[ch][1][4]->Fill(nJet, weight);
	 }
      }
*/
   }
//   for (int i=0;i<channels.size();++i) {
//      for (int k=0;k<regions.size();++k) {
//         for (int l=0;l<vars.size();++l) {
//            Hists[i][k][l]  ->Write("",TObject::kOverwrite);
//         }
//      }
//   }

//   for (int i=0;i<channels_3l.size();++i) {
//      for (int k=0;k<regions_3l.size();++k) {
//         for (int l=0;l<vars_3l.size();++l) {
//            Hists_3l[i][k][l]  ->Write("",TObject::kOverwrite);
//         }
//      }
//   }
//   for (int l=0;l<vars_yeilds.size();++l) {
//      yeilds[l]->Write("",TObject::kOverwrite);
//   }
//   f->Close();
//
   writeHists(f, Hists_2l_fakes);

}
