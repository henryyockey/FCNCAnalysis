#define MyAnalysis_cxx
#include "MyAnalysis.h"
#include "PU_reWeighting.h"
#include "lepton_candidate.h"
#include "jet_candidate.h"
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

int vInd(std::map<TString, std::vector<float>> V, TString name){
  return V.find(name)->second.at(0);
}

void MyAnalysis::Loop(TString fname, TString data, TString dataset ,TString year, TString run, float xs, float lumi, float Nevent, int iseft, int nRuns)
{
   TFile *f = new TFile("ANoutput.root","RECREATE");
   f->cd();

   typedef vector<TH1F*> Dim1;
   typedef vector<Dim1> Dim2;
   typedef vector<Dim2> Dim3;
   typedef vector<Dim3> Dim4;

   const std::map<TString, std::vector<float>> vars = 
   {
      {"ElectronPt", {0, 20, 0, 400}},
      {"LeptonPt", {1, 20, 0, 400}},
      {"LeadingLeptonPt", {2, 20, 0, 400}},
      {"LeptonEta", {3, 20, -3, 3}},
      {"LeadingLeptonEta", {4, 20, -3, 3}}, 
      {"ElectronPt_mvanoIso_WP80", {5, 20, 0, 400}},
      {"ElectronPt_mvanoIso_WP90", {6, 20, 0, 400}},
      {"ElectronPt_mvanoIso_WPL", {7, 20, 0, 400}},
      {"LeadingElectronPt_mvanoIso_WP80", {8, 20, 0, 400}},
      {"LeadingElectronPt_mvanoIso_WP90", {9, 20, 0, 400}},
      {"LeadingElectronPt_mvanoIso_WPL", {10, 20, 0, 400}},
//      {"ElectronPt_mvaIso_WP80", {11, 20, 0, 400}},
//      {"ElectronPt_mvaIso_WP90", {12, 20, 0, 400}},
//      {"ElectronPt_mvaIso_WPL", {13, 20, 0, 400}},
//      {"LeadingElectronPt_mvaIso_WP80", {14, 20, 0, 400}},
//      {"LeadingElectronPt_mvaIso_WP90", {15, 20, 0, 400}},
//      {"LeadingElectronPt_mvaIso_WPL", {16, 20, 0, 400}},
      {"Electron_mvanoIso_WP80", {11, 20, -1, 1}},
      {"Electron_mvanoIso_WP90", {12, 20, -1, 1}},
      {"Electron_mvanoIso_WPL", {13, 20, -1, 1}},
      {"Electron_mvanoIso", {14, 20, -1, 1}},
      {"ElectronPt_mvanoIso_gt50", {15, 20, 0, 400}},
      {"ElectronPt_mvanoIso_gt75", {16, 20, 0, 400}},
      {"ElectronPt_mvanoIso_gt90", {17, 20, 0, 400}},
      {"ElectronPt_mvanoIso_gt99", {18, 20, 0, 400}},
      {"LeadingElectronPt_mvanoIso_gt50", {19, 20, 0, 400}},
      {"LeadingElectronPt_mvanoIso_gt75", {20, 20, 0, 400}},
      {"LeadingElectronPt_mvanoIso_gt90", {21, 20, 0, 400}},
      {"LeadingElectronPt_mvanoIso_gt99", {22, 20, 0, 400}},
      {"Electron_mvaTTH", {23, 20, -1, 1}},
      {"Muon_mvaTTH", {24, 20, -1, 1}},
      {"LeadingElectronPt_mvaTTH_gt50", {25, 20, 0, 400}},
      {"LeadingElectronPt_mvaTTH_gt75", {26, 20, 0, 400}},
      {"LeadingElectronPt_mvaTTH_gt90", {27, 20, 0, 400}},
      {"LeadingElectronPt_mvaTTH_gt99", {28, 20, 0, 400}},
      {"LeadingElectronEta_mvaTTH_gt50", {29, 20, -3, 3}},
      {"LeadingElectronEta_mvaTTH_gt75", {30, 20, -3, 3}},
      {"LeadingElectronEta_mvaTTH_gt90", {31, 20, -3, 3}},
      {"LeadingElectronEta_mvaTTH_gt99", {32, 20, -3, 3}},
   };

   std::stringstream name;
   TH1F *h_test;
   Dim1 Hists(vars.size());
   for( auto it = vars.cbegin() ; it != vars.cend() ; ++it ){
      name<<it->first;
      h_test = new TH1F((name.str()).c_str(),(name.str()).c_str(),it->second.at(1), it->second.at(2), it->second.at(3));
      h_test->StatOverflows(kTRUE);
      h_test->Sumw2(kTRUE);
      Hists[it->second.at(0)] = h_test;
      name.str("");
   }


   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   Float_t maxfalse = 0;
   Float_t mintrue = 1;

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;


      vector<Float_t> leptonPt;
      vector<Float_t> leptonEta;
      for (int i=0; i<nElectron; i++) {
	 if (Electron_pt[i] < 400) Hists[vInd(vars, "ElectronPt")]->Fill(Electron_pt[i]);
	 if (Electron_pt[i] < 400) Hists[vInd(vars, "LeptonPt")]->Fill(Electron_pt[i]);
	 leptonPt.push_back(Electron_pt[i]);
	 leptonEta.push_back(Electron_eta[i]);
      }
      for (int i=0; i<nMuon; i++) {
         if (Muon_pt[i] < 400) Hists[vInd(vars, "LeptonPt")]->Fill(Muon_pt[i]);
	 leptonPt.push_back(Muon_pt[i]);
	 leptonEta.push_back(Muon_eta[i]);
      }

      // leading lepton index
      int llidx = max_element(leptonPt.begin(), leptonPt.end()) - leptonPt.begin(); 

      if (leptonPt.size()>0) Hists[vInd(vars,"LeadingLeptonPt")]->Fill(leptonPt[llidx]);
      if (leptonEta.size()>0) Hists[vInd(vars,"LeadingLeptonEta")]->Fill(leptonEta[llidx]);
      for (std::vector<Float_t>::iterator it=leptonEta.begin(); it!=leptonEta.end(); ++it){
	 Hists[vInd(vars, "LeptonEta")]->Fill(*it);
      }

      // Fill mva noIso electron hists
      for (int i=0; i<nElectron; i++) {
	 if (Electron_mvaFall17V1noIso_WP80[i]) {
	    Hists[vInd(vars, "ElectronPt_mvanoIso_WP80")]->Fill(Electron_pt[i]);
	 }
	 if (Electron_mvaFall17V1noIso_WP90[i]) {
            Hists[vInd(vars, "ElectronPt_mvanoIso_WP90")]->Fill(Electron_pt[i]);
         }
	 if (Electron_mvaFall17V1noIso_WPL[i]) {
            Hists[vInd(vars, "ElectronPt_mvanoIso_WPL")]->Fill(Electron_pt[i]);
         }

      }

      // Fill mva Iso electron hists
/*      for (int i=0; i<nElectron; i++) {
         if (Electron_mvaFall17V1Iso_WP80[i]) {
            Hists[vInd(vars, "ElectronPt_mvaIso_WP80")]->Fill(Electron_pt[i]);
         }
         if (Electron_mvaFall17V1Iso_WP90[i]) {
            Hists[vInd(vars, "ElectronPt_mvaIso_WP90")]->Fill(Electron_pt[i]);
         }
         if (Electron_mvaFall17V1Iso_WPL[i]) {
            Hists[vInd(vars, "ElectronPt_mvaIso_WPL")]->Fill(Electron_pt[i]);
         }

      }
*/

      
      // fill leading electron pt mva noIso hists
      vector<Float_t> mvanoIso_WP80_electronPt;
      vector<Float_t> mvanoIso_WP90_electronPt;
      vector<Float_t> mvanoIso_WPL_electronPt;
      for (int i=0; i<nElectron; i++) {
         if (Electron_mvaFall17V1noIso_WP80[i]) {
            mvanoIso_WP80_electronPt.push_back(Electron_pt[i]);
         }
         if (Electron_mvaFall17V1noIso_WP90[i]) {
            mvanoIso_WP90_electronPt.push_back(Electron_pt[i]);
         }
         if (Electron_mvaFall17V1noIso_WPL[i]) {
            mvanoIso_WPL_electronPt.push_back(Electron_pt[i]);
         }
      }

      if ( mvanoIso_WP80_electronPt.size() > 0) 
	  Hists[vInd(vars, "LeadingElectronPt_mvanoIso_WP80")]->Fill(*max_element(mvanoIso_WP80_electronPt.begin(), mvanoIso_WP80_electronPt.end()));
      if ( mvanoIso_WP90_electronPt.size() > 0) 
	  Hists[vInd(vars, "LeadingElectronPt_mvanoIso_WP90")]->Fill(*max_element(mvanoIso_WP90_electronPt.begin(), mvanoIso_WP90_electronPt.end()));
      if ( mvanoIso_WPL_electronPt.size() > 0) 
	  Hists[vInd(vars, "LeadingElectronPt_mvanoIso_WPL")]->Fill(*max_element(mvanoIso_WPL_electronPt.begin(), mvanoIso_WPL_electronPt.end()));

      // fill leading electron pt mva Iso hists
/*      vector<Float_t> mvaIso_WP80_electronPt;
      vector<Float_t> mvaIso_WP90_electronPt;
      vector<Float_t> mvaIso_WPL_electronPt;
      for (int i=0; i<nElectron; i++) {
         if (Electron_mvaFall17V1Iso_WP80[i]) {
            mvaIso_WP80_electronPt.push_back(Electron_pt[i]);
         }
         if (Electron_mvaFall17V1Iso_WP90[i]) {
            mvaIso_WP90_electronPt.push_back(Electron_pt[i]);
         }
         if (Electron_mvaFall17V1Iso_WPL[i]) {
            mvaIso_WPL_electronPt.push_back(Electron_pt[i]);
         }
      }

      if ( mvaIso_WP80_electronPt.size() > 0) Hists[vInd(vars, "LeadingElectronPt_mvaIso_WP80")]->Fill(*max_element(mvaIso_WP80_electronPt.begin(), mvaIso_WP80_electronPt.end()));
      if ( mvaIso_WP90_electronPt.size() > 0) Hists[vInd(vars, "LeadingElectronPt_mvaIso_WP90")]->Fill(*max_element(mvaIso_WP90_electronPt.begin(), mvaIso_WP90_electronPt.end()));
      if ( mvaIso_WPL_electronPt.size() > 0) Hists[vInd(vars, "LeadingElectronPt_mvaIso_WPL")]->Fill(*max_element(mvaIso_WPL_electronPt.begin(), mvaIso_WPL_electronPt.end()));
*/

      // Fill electron mva hist
      for (int i=0; i<nElectron; i++) {
         if (Electron_mvaFall17V1noIso_WP80[i]) {
            Hists[vInd(vars, "Electron_mvanoIso_WP80")]->Fill(Electron_mvaFall17V1noIso[i]);
         }
         if (Electron_mvaFall17V1noIso_WP90[i]) {
            Hists[vInd(vars, "Electron_mvanoIso_WP90")]->Fill(Electron_mvaFall17V1noIso[i]);
         }
	 if (Electron_mvaFall17V1noIso_WPL[i]) {
            Hists[vInd(vars, "Electron_mvanoIso_WPL")]->Fill(Electron_mvaFall17V1noIso[i]);
         }
	 Hists[vInd(vars, "Electron_mvanoIso")]->Fill(Electron_mvaFall17V1noIso[i]);
      }

      // Fill Electron PT mvanoIso greater than hists
      for (int i=0; i<nElectron; i++) {
         if (Electron_mvaFall17V1noIso[i]>.50) {
            Hists[vInd(vars, "ElectronPt_mvanoIso_gt50")]->Fill(Electron_pt[i]);
         }
         if (Electron_mvaFall17V1noIso[i]>.75) {
            Hists[vInd(vars, "ElectronPt_mvanoIso_gt75")]->Fill(Electron_pt[i]);
         }
         if (Electron_mvaFall17V1noIso[i]>.90) {
            Hists[vInd(vars, "ElectronPt_mvanoIso_gt90")]->Fill(Electron_pt[i]);
         }
	 if (Electron_mvaFall17V1noIso[i]>.99) {
            Hists[vInd(vars, "ElectronPt_mvanoIso_gt99")]->Fill(Electron_pt[i]);
         }
      }

      // Fill leading lepton pt mvanoIso greater than hists
      vector<Float_t> mvanoIso_gt50_electronPt;
      vector<Float_t> mvanoIso_gt75_electronPt;
      vector<Float_t> mvanoIso_gt90_electronPt;
      vector<Float_t> mvanoIso_gt99_electronPt;
      for (int i=0; i<nElectron; i++) {
         if (Electron_mvaFall17V1noIso[i]>.50) {
            mvanoIso_gt50_electronPt.push_back(Electron_pt[i]);
         }
         if (Electron_mvaFall17V1noIso[i]>.75) {
            mvanoIso_gt75_electronPt.push_back(Electron_pt[i]);
         }
	 if (Electron_mvaFall17V1noIso[i]>.90) {
            mvanoIso_gt90_electronPt.push_back(Electron_pt[i]);
         }
	 if (Electron_mvaFall17V1noIso[i]>.99) {
            mvanoIso_gt99_electronPt.push_back(Electron_pt[i]);
         }

      }
      if ( mvanoIso_gt50_electronPt.size() > 0)
          Hists[vInd(vars, "LeadingElectronPt_mvanoIso_gt50")]->Fill(*max_element(mvanoIso_gt50_electronPt.begin(), mvanoIso_gt50_electronPt.end()));
      if ( mvanoIso_gt75_electronPt.size() > 0)
          Hists[vInd(vars, "LeadingElectronPt_mvanoIso_gt75")]->Fill(*max_element(mvanoIso_gt75_electronPt.begin(), mvanoIso_gt75_electronPt.end()));
      if ( mvanoIso_gt90_electronPt.size() > 0)
          Hists[vInd(vars, "LeadingElectronPt_mvanoIso_gt90")]->Fill(*max_element(mvanoIso_gt90_electronPt.begin(), mvanoIso_gt90_electronPt.end()));
      if ( mvanoIso_gt99_electronPt.size() > 0)
          Hists[vInd(vars, "LeadingElectronPt_mvanoIso_gt99")]->Fill(*max_element(mvanoIso_gt99_electronPt.begin(), mvanoIso_gt99_electronPt.end()));


      // fill electron/muon mva TTH hists
      for (int i=0; i<nElectron; i++) {
	 Hists[vInd(vars, "Electron_mvaTTH")]->Fill(Electron_mvaTTH[i]);
      }
      for (int i=0; i<nMuon; i++) {
         Hists[vInd(vars, "Muon_mvaTTH")]->Fill(Muon_mvaTTH[i]);
      }

      // Fill leading electron pt mvaTTH greater than hists
      vector<Float_t> mvaTTH_gt50_electronPt;
      vector<Float_t> mvaTTH_gt75_electronPt;
      vector<Float_t> mvaTTH_gt90_electronPt;
      vector<Float_t> mvaTTH_gt99_electronPt;
      vector<Float_t> mvaTTH_gt50_electronIdx;
      vector<Float_t> mvaTTH_gt75_electronIdx;
      vector<Float_t> mvaTTH_gt90_electronIdx;
      vector<Float_t> mvaTTH_gt99_electronIdx;
      for (int i=0; i<nElectron; i++) {
         if (Electron_mvaTTH[i]>.50) {
            mvaTTH_gt50_electronPt.push_back(Electron_pt[i]);
	    mvaTTH_gt50_electronIdx.push_back(i);
         }
         if (Electron_mvaTTH[i]>.75) {
            sts[vInd(vars, "LeadingElectronEta_mvaTTH_gt99")]->Fill(Electron_eta[llidx_mvaTTH_gt99]);mvaTTH_gt75_electronPt.push_back(Electron_pt[i]);
	    mvaTTH_gt75_electronIdx.push_back(i);
         }
         if (Electron_mvaTTH[i]>.90) {
            mvaTTH_gt90_electronPt.push_back(Electron_pt[i]);
	    mvaTTH_gt90_electronIdx.push_back(i);
         }
         if (Electron_mvaTTH[i]>.99) {
            mvaTTH_gt99_electronPt.push_back(Electron_pt[i]);
	    mvaTTH_gt99_electronIdx.push_back(i);
         }

      }
      if ( mvaTTH_gt50_electronPt.size() > 0)
          Hists[vInd(vars, "LeadingElectronPt_mvaTTH_gt50")]->Fill(*max_element(mvaTTH_gt50_electronPt.begin(), mvaTTH_gt50_electronPt.end()));
      if ( mvaTTH_gt75_electronPt.size() > 0)
          Hists[vInd(vars, "LeadingElectronPt_mvaTTH_gt75")]->Fill(*max_element(mvaTTH_gt75_electronPt.begin(), mvaTTH_gt75_electronPt.end()));
      if ( mvaTTH_gt90_electronPt.size() > 0)
          Hists[vInd(vars, "LeadingElectronPt_mvaTTH_gt90")]->Fill(*max_element(mvaTTH_gt90_electronPt.begin(), mvaTTH_gt90_electronPt.end()));
      if ( mvaTTH_gt99_electronPt.size() > 0)
          Hists[vInd(vars, "LeadingElectronPt_mvaTTH_gt99")]->Fill(*max_element(mvaTTH_gt99_electronPt.begin(), mvaTTH_gt99_electronPt.end()));
      
      //leading electron pt index
      if (mvaTTH_gt50_electronIdx.size()>0) {
	 int llidx_mvaTTH_gt50 = mvaTTH_gt50_electronIdx[max_element(mvaTTH_gt50_electronPt.begin(), mvaTTH_gt50_electronPt.end()) - mvaTTH_gt50_electronPt.begin()];
	 Hists[vInd(vars, "LeadingElectronEta_mvaTTH_gt50")]->Fill(Electron_eta[llidx_mvaTTH_gt50]);
      }   
      if (mvaTTH_gt75_electronIdx.size()>0) {
         int llidx_mvaTTH_gt75 = mvaTTH_gt75_electronIdx[max_element(mvaTTH_gt75_electronPt.begin(), mvaTTH_gt75_electronPt.end()) - mvaTTH_gt75_electronPt.begin()];
	 Hists[vInd(vars, "LeadingElectronEta_mvaTTH_gt75")]->Fill(Electron_eta[llidx_mvaTTH_gt75]);
      }
      if (mvaTTH_gt90_electronIdx.size()>0) {
	 int llidx_mvaTTH_gt90 = mvaTTH_gt90_electronIdx[max_element(mvaTTH_gt90_electronPt.begin(), mvaTTH_gt90_electronPt.end()) - mvaTTH_gt90_electronPt.begin()];
	 Hists[vInd(vars, "LeadingElectronEta_mvaTTH_gt90")]->Fill(Electron_eta[llidx_mvaTTH_gt90]); 
      }
      if (mvaTTH_gt99_electronIdx.size()>0) {
         int llidx_mvaTTH_gt99 = mvaTTH_gt99_electronIdx[max_element(mvaTTH_gt99_electronPt.begin(), mvaTTH_gt99_electronPt.end()) - mvaTTH_gt99_electronPt.begin()];
	 Hists[vInd(vars, "LeadingElectronEta_mvaTTH_gt99")]->Fill(Electron_eta[llidx_mvaTTH_gt99]);
      }

/*      // Fill leading electron eta hists
      if ( mvaTTH_gt50_electronIdx.size() > 0) {
//          Hists[vInd(vars, "LeadingElectronEta_mvaTTH_gt50")]->Fill(Electron_eta[llidx_mvaTTH_gt50]);
	  cout << mvaTTH_gt50_electronIdx.size() << " " << llidx_mvaTTH_gt50 << " " << nElectron << endl;
      }
      if ( mvaTTH_gt75_electronIdx.size() > 0) {
//          Hists[vInd(vars, "LeadingElectronEta_mvaTTH_gt75")]->Fill(Electron_eta[llidx_mvaTTH_gt75]);
	  cout << mvaTTH_gt50_electronIdx.size() << " " << llidx_mvaTTH_gt50 << " " << nElectron << endl;
      }
      if ( mvaTTH_gt90_electronIdx.size() > 0) {
//          Hists[vInd(vars, "LeadingElectronEta_mvaTTH_gt90")]->Fill(Electron_eta[llidx_mvaTTH_gt90]);
	  cout << mvaTTH_gt50_electronIdx.size() << " " << llidx_mvaTTH_gt50 << " " << nElectron << endl;
      }
      if ( mvaTTH_gt99_electronIdx.size() > 0) {
//          Hists[vInd(vars, "LeadingElectronEta_mvaTTH_gt99")]->Fill(Electron_eta[llidx_mvaTTH_gt99]);
	  cout << mvaTTH_gt50_electronIdx.size() << " " << llidx_mvaTTH_gt50 << " " << nElectron << endl;
      }
*/
   }  

   for (int l=0;l<vars.size();++l) {
      Hists[l]->Write("",TObject::kOverwrite);
   }


   f->Close();

}
