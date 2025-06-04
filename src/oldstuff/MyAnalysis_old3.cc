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

bool ComparePtLep(lepton_candidate *a, lepton_candidate *b) { 
   return a->pt_ > b->pt_; 
}

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

/*bool fakeableWellMeasured(std::vector<std::vector<lepton_candidate*>> *leptons) {
   std::vector<lepton_candidate*> *fakeable = leptons->at(1);
   for (int i=0; i<fakeable->size(); i++) {
      if (!(fakeable->at(i)->isWellMeasured_)) return false;
   }
   return true;
}*/

bool hasTheJets(float *Jet_pt, float *Jet_eta, int nJet) {
   int count = 0;
   for (int i=0; i<nJet; i++) {
      if (Jet_pt[i] > 30 && Jet_eta[i] < 2.4) count++;
   }
   return count>=4;
}

bool is2lss(std::vector<lepton_candidate*> *leptons) {
   
   bool a = leptons->size()==2; 
   bool b = false, c = false;
   if (leptons->size()>=2) { 
      b = leptons->at(0)->lep_ == leptons->at(1)->lep_ ;
      c = leptons->at(0)->pt_>25 && leptons->at(1)->pt_>15; 
   }
   return a && b && c;
}

bool is2los(std::vector<lepton_candidate*> *leptons) {
   bool a = leptons->size()==2;
   bool b = false, c = false, d= false;
   if (leptons->size()>=2) {
      b = leptons->at(0)->lep_ == leptons->at(1)->lep_ ;
      c = leptons->at(0)->pt_>25 && leptons->at(1)->pt_>15;
      d = leptons->at(0)->charge_==-leptons->at(1)->charge_;
   }
   return a && b && c && d;

}

bool is3l(std::vector<lepton_candidate*> *leptons) {
   return leptons->size()==3;
}

void MyAnalysis::test() {
   cout << nElectron << endl;
}

//void MyAnalysis::Loop(TString fname, TString data, TString dataset ,TString year, TString run, float xs, float lumi, float Nevent, int iseft, int nRuns)
void MyAnalysis::Loop(TString fname, TString data, TString dataset ,TString year, TString run, float xs, float lumi, float Nevent)
{
   TFile *f = new TFile("ANoutput.root","RECREATE");
   f->cd();
   typedef vector<TH1F*> Dim1;
   typedef vector<Dim1> Dim2;
   typedef vector<Dim2> Dim3;
   typedef vector<Dim3> Dim4;

   const std::map<TString, std::vector<float>> vars = 
   {
      {"2los_LeadingLeptonPt",                  {0, 20, 0, 400}},
      {"2los_LeadingLeptonEta",                 {1, 20, -3, 3}},
      {"2los_LeptonPDGID",			{2, 43, -21, 22}},
      {"2los_Mll",				{3, 40, 0, 150}},
      {"2los_nJets", 				{4, 40, 0, 150}},
      {"2lss_LeadingLeptonPt",                  {5, 20, 0, 400}},
      {"2lss_LeadingLeptonEta",                 {6, 20, -3, 3}},
      {"2lss_LeptonPDGID",                      {7, 43, -21, 22}},
      {"2lss_Mll",                              {8, 40, 0, 150}},
      {"2lss_nJets",                            {9, 15, 0, 15}},
      {"raw_cuts", 				{10, 3, 0,3}},
      {"positive_weighted_cuts",		{11, 3, 0, 3}},
      {"negative_weighted_cuts",                {12, 3, 0, 3}},
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

   std::vector<lepton_candidate*> *selected_leptons;

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
      Double_t weight = 1;
      if (data == "mc") weight = ((1000*xs*lumi)/Nevent)*((genWeight>0)?1:-1);    
      Hists[vInd(vars, "raw_cuts")]->Fill(0.5); // bin 1 is uncut data
      if (weight>0) {
	  Hists[vInd(vars, "positive_weighted_cuts")]->Fill(0.5, weight);
      } else {
	   Hists[vInd(vars, "negative_weighted_cuts")]->Fill(0.5, -weight);
      }
      if (HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL || HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ) {
         Hists[vInd(vars, "raw_cuts")]->Fill(1.5); // bin 2 is after trigger
         if (weight>0) {
             Hists[vInd(vars, "positive_weighted_cuts")]->Fill(1.5, weight);
         } else {
             Hists[vInd(vars, "negative_weighted_cuts")]->Fill(1.5, -weight);
         }

         //cout << weight << "\t" << ((genWeight>0)?1:-1) << "\t" << genWeight << endl;
	 selected_leptons = new std::vector<lepton_candidate*>();
	 for (int i=0; i<nElectron; i++) {
	    if(Electron_cutBased[i]==4) {
		selected_leptons->push_back(new lepton_candidate(Electron_pt[i],Electron_eta[i],Electron_phi[i],Electron_charge[i],i,1));
	    }
	 }

         sort(selected_leptons->begin(),selected_leptons->end(), ComparePtLep);

         bool areElectrons = (selected_leptons->size()>0) ? abs(selected_leptons->at(0)->lep_)==1 : false;
	 if (selected_leptons->size()>1) cout << selected_leptons->at(0)->charge_ << "\t" << selected_leptons->at(1)->charge_ << endl;
         if (areElectrons) {
	    cout << "areElectrons" << endl;
            if (is2los(selected_leptons)) {
		cout << "is2los" << endl;
	       Hists[vInd(vars, "raw_cuts")]->Fill(2.5); // bin 3 selects electrons
               if (weight>0) {
	          Hists[vInd(vars, "positive_weighted_cuts")]->Fill(2.5, weight);
               } else {
                  Hists[vInd(vars, "negative_weighted_cuts")]->Fill(2.5, -weight);
               }


	       //Fill 2los hists
	       Hists[vInd(vars, "2los_LeadingLeptonPt")]->Fill(selected_leptons->at(0)->pt_, weight);
	       Hists[vInd(vars, "2los_LeadingLeptonEta")]->Fill(selected_leptons->at(0)->eta_, weight);
	       Hists[vInd(vars, "2los_LeptonPDGID")]->Fill(selected_leptons->at(0)->lep_);
	       Hists[vInd(vars, "2los_Mll")]->Fill(((*selected_leptons)[0]->p4_+(*selected_leptons)[1]->p4_).M(), weight);
	       Hists[vInd(vars, "2los_nJets")]->Fill((selected_leptons->at(0)->p4_+selected_leptons->at(1)->p4_).M(), weight);
            }

            if (is2lss(selected_leptons)) {
               //Fill 2lss hists
               Hists[vInd(vars, "2lss_LeadingLeptonPt")]->Fill(selected_leptons->at(0)->pt_, weight);
               Hists[vInd(vars, "2lss_LeadingLeptonEta")]->Fill(selected_leptons->at(0)->eta_, weight);
               Hists[vInd(vars, "2lss_LeptonPDGID")]->Fill(selected_leptons->at(0)->lep_);
               Hists[vInd(vars, "2lss_Mll")]->Fill((selected_leptons->at(0)->p4_+selected_leptons->at(1)->p4_).M(), weight);
               Hists[vInd(vars, "2lss_nJets")]->Fill(nJet, weight);

            }
         }
      }
   
   
   }
   for (int l=0;l<vars.size();++l) {
      Hists[l]->Write("",TObject::kOverwrite);
   }


   f->Close();

}
