#define MyAnalysis_cxx
#include "MyAnalysis.h"
#include "PU_reWeighting.h"
#include "lepton_candidate.h"
#include "electron_candidate.h"
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

bool ComparePtLep(electron_candidate *a, electron_candidate *b) { return a->pt > b->pt; }

int vInd(std::map<TString, std::vector<float>> V, TString name){
  return V.find(name)->second.at(0);
}

float deltar(float eta1, float eta2, float phi1, float phi2) {
   float deta = eta1-eta2;
   float dphi = phi1-phi2;
   return sqrt(deta*deta+dphi*dphi);
}

float nearbyJetDeepCSV(float e_eta, float e_phi, float *FatJet_eta, float *FatJet_phi, float *FatJet_pt, int nFatJet, float *FatJet_btagCSVV2 ) {
   float least_deltar = 0.4;
   int jet_idx = -1;
   for (int j=0; j<nFatJet; j++) {
      float j_eta = FatJet_eta[j];
      float j_phi = FatJet_phi[j];
      float dr = deltar(e_eta, j_eta, e_phi, j_phi);
      if (dr<least_deltar && FatJet_pt[j] > 15) {
         least_deltar = dr;
	 jet_idx = j; 
      }
   }
   return (jet_idx == -1) ? 0 : FatJet_btagCSVV2[jet_idx];
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
      {"ElectronPt", 				{0, 20, 0, 400}},
      {"ElectronEta", 				{1, 20, -3, 3}},
      {"ElectronDxy", 				{2, 20, -0.01, 0.01}},
      {"ElectronDz", 				{3, 20, -0.04, 0.04}},
      {"ElectronSip3d", 			{4, 20, 0, 10}},
      {"ElectronMiniRelIso", 			{5, 20, 0, 0.08}},
      {"ElectronMissingHits", 			{6, 3, 0, 3}},
      {"ElectronEGammaPOGMVA", 			{7, 20, -1, 1}},
      {"ElectronConversionRejection", 		{8, 2, -0.5, 1.5}},
      {"ElectronSieie", 			{9, 40, 0, 0.04}},
      {"ElectronHOE", 				{10, 20, 0, 0.1}},
      {"ElectronInvE_InvP", 			{11, 20, -0.005, 0.005}},
      {"ElectronJetPtRatio", 			{12, 20, 0, 15}},
      {"ElectronNearbyJetDeepCSV", 		{13, 20, 0, 1}},
      {"ElectronPrompteMVA", 			{14, 20, 0, 1}},
      {"TightElectronPt", 			{15, 20, 0, 400}},
      {"TightElectronEta", 			{16, 20, -3, 3}},
      {"TightElectronDxy", 			{17, 20, -0.01, 0.01}},
      {"TightElectronDz", 			{18, 20, -0.04, 0.04}},
      {"TightElectronSip3d", 			{19, 20, 0, 10}},
      {"TightElectronMiniRelIso", 		{20, 20, 0, 0.08}},
      {"TightElectronMissingHits", 		{21, 3, 0, 3}},
      {"TightElectronEGammaPOGMVA", 		{22, 20, -1, 1}},
      {"TightElectronConversionRejection",	{23, 2, -0.5, 1.5}},
      {"TightElectronSieie", 			{24, 40, 0, 0.04}},
      {"TightElectronHOE", 			{25, 20, 0, 0.1}},
      {"TightElectronInvE_InvP", 		{26, 20, -0.005, 0.005}},
      {"TightElectronJetPtRatio", 		{27, 20, 0, 15}},
      {"TightElectronNearbyJetDeepCSV", 	{28, 20, 0, 1}},
      {"TightElectronPrompteMVA", 		{29, 20, 0, 1}},
      {"Electron_mvanoIso", 			{30, 20, -1, 1}},
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

   std::vector<electron_candidate*> *electrons;

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

      electrons = new std::vector<electron_candidate*>();
      for (int i=0; i<nElectron; i++) {
	 electrons->push_back(new electron_candidate(
	        Electron_pt[i],
	        Electron_eta[i],
	        Electron_dxy[i],
	        Electron_dz[i],
	        Electron_sip3d[i],
	        Electron_miniPFRelIso_all[i],
	        Electron_lostHits[i],
	        Electron_mvaFall17V1noIso[i],
	        Electron_convVeto[i],
	        Electron_sieie[i],
	        Electron_hoe[i],
	        Electron_eInvMinusPInv[i],
	        Electron_jetPtRelv2[i],
	        nearbyJetDeepCSV(
			Electron_eta[i], 
			Electron_phi[i], 
			FatJet_eta, 
			FatJet_phi, 
			FatJet_pt,
			nFatJet,
			FatJet_btagCSVV2),
	        Electron_mvaTTH[i],
		Electron_phi[i],
		Electron_pdgId[i],
		i,
		Electron_pdgId[i]
         ));
      }

      sort(electrons->begin(),electrons->end(), ComparePtLep);

      for (int i=0; i<nElectron; i++) {
	 Hists[vInd(vars, "ElectronPt")]->Fill(electrons->at(i)->pt);
         Hists[vInd(vars, "ElectronEta")]->Fill(electrons->at(i)->eta);
         Hists[vInd(vars, "ElectronDxy")]->Fill(electrons->at(i)->dxy);
         Hists[vInd(vars, "ElectronDz")]->Fill(electrons->at(i)->dz);
         Hists[vInd(vars, "ElectronSip3d")]->Fill(electrons->at(i)->sip3d);
         Hists[vInd(vars, "ElectronMiniRelIso")]->Fill(electrons->at(i)->miniPFRelIso_all);
         Hists[vInd(vars, "ElectronMissingHits")]->Fill(electrons->at(i)->lostHits);
         Hists[vInd(vars, "ElectronEGammaPOGMVA")]->Fill(electrons->at(i)->mvaFall17V1noIso);
         Hists[vInd(vars, "ElectronConversionRejection")]->Fill(electrons->at(i)->convVeto);
         Hists[vInd(vars, "ElectronSieie")]->Fill(electrons->at(i)->sieie);
         Hists[vInd(vars, "ElectronHOE")]->Fill(electrons->at(i)->hoe);
         Hists[vInd(vars, "ElectronInvE_InvP")]->Fill(electrons->at(i)->eInvMinusPInv);
         Hists[vInd(vars, "ElectronNearbyJetDeepCSV")]->Fill(electrons->at(i)->nearbyJetDeepCSV);
         Hists[vInd(vars, "ElectronPrompteMVA")]->Fill(electrons->at(i)->mvaTTH);
	 //cout << electrons->at(i)->mvaFall17V1noIso << endl;
	 Hists[vInd(vars, "Electron_mvanoIso")]->Fill(Electron_mvaFall17V1noIso[i]);
	 if (electrons->at(i)->isTightElectron()) {
	    Hists[vInd(vars, "TightElectronPt")]->Fill(electrons->at(i)->pt);
	    Hists[vInd(vars, "TightElectronEta")]->Fill(electrons->at(i)->eta);
            Hists[vInd(vars, "TightElectronDxy")]->Fill(electrons->at(i)->dxy);
            Hists[vInd(vars, "TightElectronDz")]->Fill(electrons->at(i)->dz);
            Hists[vInd(vars, "TightElectronSip3d")]->Fill(electrons->at(i)->sip3d);
            Hists[vInd(vars, "TightElectronMiniRelIso")]->Fill(electrons->at(i)->miniPFRelIso_all);
            Hists[vInd(vars, "TightElectronMissingHits")]->Fill(electrons->at(i)->lostHits);
	    Hists[vInd(vars, "TightElectronEGammaPOGMVA")]->Fill(electrons->at(i)->mvaFall17V1noIso);
            Hists[vInd(vars, "TightElectronConversionRejection")]->Fill(electrons->at(i)->convVeto);
            Hists[vInd(vars, "TightElectronSieie")]->Fill(electrons->at(i)->sieie);
            Hists[vInd(vars, "TightElectronHOE")]->Fill(electrons->at(i)->hoe);
            Hists[vInd(vars, "TightElectronInvE_InvP")]->Fill(electrons->at(i)->eInvMinusPInv);
	    Hists[vInd(vars, "TightElectronNearbyJetDeepCSV")]->Fill(electrons->at(i)->nearbyJetDeepCSV);
      	    Hists[vInd(vars, "TightElectronPrompteMVA")]->Fill(electrons->at(i)->mvaTTH);
	 }
      }

/*      for (int i=0; i<nElectron; i++) {
	 Hists[vInd(vars, "ElectronPt")]->Fill(Electron_pt[i]);
	 Hists[vInd(vars, "ElectronEta")]->Fill(Electron_eta[i]);
	 Hists[vInd(vars, "ElectronDxy")]->Fill(Electron_dxy[i]);
	 Hists[vInd(vars, "ElectronDz")]->Fill(Electron_dz[i]);
	 Hists[vInd(vars, "ElectronSip3d")]->Fill(Electron_sip3d[i]);
	 Hists[vInd(vars, "ElectronMiniRelIso")]->Fill(Electron_miniPFRelIso_all[i]);
         Hists[vInd(vars, "ElectronMissingHits")]->Fill(Electron_lostHits[i]);
	 Hists[vInd(vars, "ElectronConversionRejection")]->Fill(Electron_convVeto[i]);
	 Hists[vInd(vars, "ElectronSieie")]->Fill(Electron_sieie[i]);
	 Hists[vInd(vars, "ElectronHOE")]->Fill(Electron_hoe[i]);
	 Hists[vInd(vars, "ElectronInvE_InvP")]->Fill(Electron_eInvMinusPInv[i]);
	 Hists[vInd(vars, "ElectronJetPtRatio")]->Fill(Electron_jetPtRelv2[i]);
      }
*/


   }
   for (int l=0;l<vars.size();++l) {
      Hists[l]->Write("",TObject::kOverwrite);
   }


   f->Close();

}
