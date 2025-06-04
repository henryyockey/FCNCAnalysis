#include "objects.h"
#include "constants.h"
#include "selection.h"
#include "lepton_candidate.h"
#include "jet_candidate.h"
#include <TROOT.h>
#include <iostream>
#include <vector>
#include <complex>
#include <TLorentzVector.h>
#include <stdexcept>
#include <algorithm>

using namespace std;


/*
TString EventCategory(std::vector<lepton_candidate*> leptons, std::vector<jet_candidate*> jets) {
    
    int lep0, lep1;
    bool sameSign;
    TString sign, flavors;
    if (leptons.size()==2) {
	if (leptons.at(0)->pt_ < 25 || leptons.at(1)->pt_ < 15) return "None";
	lep0 = leptons.at(0)->lep_;
	lep1 = leptons.at(1)->lep_;
	sameSign = (leptons.at(0)->charge_ == leptons.at(1)->charge_);
  	if (sameSign) sign = (leptons.at(0)->charge_>0) ? "ss+" : "ss-";
	else sign = "os";
        if (lep0 == 1 && lep1 == 1) flavors = "ee";
	else if (lep0 == 10 && lep1 == 10) flavors = "mumu";
	else if ((lep0 == 1 && lep1 == 10) || (lep1 == 10 && lep0 == 1)) flavors = "emu";
	else return "None";
	return flavors+sign;
    }
    return "None";
}
*/

bool isMatched(MyAnalysis *evt, int l, int pdgid, TString MC, bool ifChargedMatched){
  MyAnalysis *eI=evt;
  if (MC=="data") return true;
  if(abs(pdgid) == 11){
    if(!ifChargedMatched){
      if(eI->Electron_genPartFlav[l]==1 || eI->Electron_genPartFlav[l]==15) return true;
    }
    if(ifChargedMatched){
      if((eI->Electron_genPartFlav[l]==1 || eI->Electron_genPartFlav[l]==15) && (eI->GenPart_pdgId[eI->Electron_genPartIdx[l]]*eI->Electron_pdgId[l]>0)) return true;
    }
  }
  if(abs(pdgid) == 13){
    if(eI->Muon_genPartFlav[l]==1 || eI->Muon_genPartFlav[l]==15) return true;
  }
  return false;
}


bool isOnZ(std::vector<lepton_candidate*> leptons){
   if (leptons.size()<2) return false;
   for(int i=0; i<leptons.size(); i++) {
      for(int j=i+1; j<leptons.size(); j++) {
         bool sameFlavor = leptons[i]->lep_==leptons[j]->lep_;
         bool oppoSign = leptons[i]->charge_==-leptons[j]->charge_;
         bool onZ = abs(((leptons[i]->p4_+leptons[j]->p4_).M())-91.1876)<10;
         if (sameFlavor && oppoSign && onZ) return true;
      }
   }
   return false;
}

TString EventCategory(std::vector<lepton_candidate*> leptons, std::vector<jet_candidate*> jets, bool matched) {
    if (leptons.size()<2) return "None";
    bool onZ = isOnZ(leptons);
    vector<int> TID, FID; 	//indices of tight and fakeable leptons
    bool leptonPass = false;
    int TsumFlavor = 0, TsumCharge = 0, FsumFlavor = 0, FsumCharge = 0;
    for (auto l = leptons.begin(); l!=leptons.end(); ++l) {
	if ((*l)->isTightLep_) {
	    TID.push_back(l-leptons.begin());
	    TsumCharge += (*l)->charge_;
	    TsumFlavor += (*l)->lep_;
	}
        if ((*l)->isFOLep_ && !((*l)->isTightLep_)) {
	    FID.push_back(l-leptons.begin());
            FsumCharge += (*l)->charge_;
            FsumFlavor += (*l)->lep_;
	}
    }
    int sumCharge = TsumCharge + FsumCharge;
    if (TID.size()==2 && FID.size()==0) {
	if ((leptons[TID[0]])->pt_>25 && (leptons[TID[1]])->pt_>15) leptonPass = true;
	if (leptonPass && TsumFlavor==2 && TsumCharge==0) return "EpEm";
        if (leptonPass && TsumFlavor==11 && TsumCharge==0) return "EpmMUmp";
        if (leptonPass && TsumFlavor==20 && TsumCharge==0) return "MUpMUm";
        if (leptonPass && TsumCharge > 0) return "LLpp";
    	if (leptonPass && TsumCharge < 0) return "LLmm";
    }
    if (TID.size()==1 && FID.size()==1) {
	if ((leptons)[0]->pt_ > 25 && (leptons)[1]->pt_ > 15) leptonPass=true;
	if (leptonPass && (TsumCharge+FsumCharge)>0) return "LFpp";
        if (leptonPass && (TsumCharge+FsumCharge)<0) return "LFmm";
    }
    if (TID.size()==0 && FID.size()==2) {
        if ((leptons)[0]->pt_ > 25 && (leptons)[1]->pt_ > 15) leptonPass=true;
        if (leptonPass && (TsumCharge+FsumCharge)>0) return "FFpp";
        if (leptonPass && (TsumCharge+FsumCharge)<0) return "FFmm";
    }

    if (TID.size()+FID.size()==3) {
        if (leptons[0]->pt_>25 &&leptons[1]->pt_>15) leptonPass =  true;
        if (leptons[2]->lep_==1 && leptons[2]->pt_<15) leptonPass = false;

        string sFake;
        if (TID.size()==3 && matched) sFake = "3L";
        if (TID.size()==2) sFake = "LLF";
        if (TID.size()==1) sFake = "LFF";
        if (TID.size()==0) sFake = "FFF";

        string sOnZ;
        if (onZ) sOnZ = "onZ";
        if (!onZ && sumCharge>0) sOnZ = "offZp";
        if (!onZ && sumCharge<0) sOnZ = "offZm";
         
        if (leptonPass) return sFake+sOnZ; 
    }

    if (TID.size()>3) {
        if (leptons[0]->pt_>25 &&leptons[1]->pt_>15) leptonPass =  true;
        if (leptons[2]->lep_==1 && leptons[2]->pt_<15) leptonPass = false;
        if (leptons[3]->lep_==1 && leptons[3]->pt_<15) leptonPass = false;

        if (leptonPass) return "4L";
    }







    return "None"; 
}
