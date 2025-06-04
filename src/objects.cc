#include "objects.h"
#include "constants.h"
#include <TROOT.h>
#include <iostream>
#include <vector>
#include <complex>
#include <TLorentzVector.h>
#include <stdexcept>
#include <algorithm>

using namespace std;

bool isTightJet(Float_t pt, Float_t eta, Int_t jetId, Float_t jetPtCut = 25.0) {
    return ((pt > jetPtCut) && (abs(eta) < constants::eta_j_cut) && (jetId > constants::jet_id_cut));
}

bool ttH_idEmu_cuts_E3(int *cuts, Float_t hoe, Float_t eta, Float_t deltaEtaSC, Float_t eInvMinusPInv, Float_t sieie) {
    cuts[0] = hoe<0.10;
    cuts[1] = eInvMinusPInv>-0.04;
    cuts[2] = sieie<(0.011+0.019*(abs(eta+deltaEtaSC)>1.479));
    return cuts[0]&&cuts[1]&&cuts[2];
//    return ((hoe<0.10) && (eInvMinusPInv>-0.04) && (sieie<(0.011+0.019*(abs(eta+deltaEtaSC)>1.479))));
}


Float_t smoothBFlav(Float_t jetpt, Float_t ptmin, Float_t ptmax, TString year) {
    double wploose, wpmedium;
    if (year == "2017") {
	wploose = constants::btag_wp_loose_UL17;
	wpmedium = constants::btag_wp_medium_UL17;
    }
    else {
        throw std::invalid_argument("Invalid year provided");
    }

    double x = min(max(0.0, (double)jetpt-ptmin)/(ptmax-ptmin) , 1.0);
    return x*wploose + (1-x)*wpmedium;
}

bool isPresElec(Float_t pt, Float_t eta, Float_t dxy, Float_t dz, Float_t miniIso, Float_t sip3d, Bool_t eleId){
    bool pt_mask 	= pt		> constants::pres_e_pt_cut;
    bool eta_mask 	= abs(eta)	< constants::eta_e_cut;
    bool dxy_mask 	= abs(dxy)	< constants::dxy_cut;
    bool dz_mask 	= abs(dz)	< constants::dz_cut;
    bool iso_mask	= miniIso	< constants::iso_cut;
    bool sip3d_mask	= sip3d 	< constants::sip3d_cut;

    return pt_mask && eta_mask && dxy_mask && dz_mask && iso_mask && sip3d_mask && eleId;

}

bool isPresMuon(Float_t pt, Float_t eta, Float_t dxy, Float_t dz, Float_t miniRelIso, Float_t sip3d){                                                                  
    bool pt_mask        = pt            > constants::pres_m_pt_cut;                         
    bool eta_mask       = abs(eta)      < constants::eta_m_cut;                             
    bool dxy_mask       = abs(dxy)      < constants::dxy_cut;                               
    bool dz_mask        = abs(dz)       < constants::dz_cut;                                
    bool iso_mask       = miniRelIso    < constants::iso_cut;                               
    bool sip3d_mask     = sip3d         < constants::sip3d_cut;         
    return pt_mask && eta_mask && dxy_mask && dz_mask && iso_mask && sip3d_mask;   
} 

bool isLooseElec(Float_t miniPFRelIso_all,  Float_t sip3d, UChar_t lostHits){
    return (miniPFRelIso_all < constants::iso_cut && sip3d < constants::sip3d_cut && lostHits <= 1);
}

bool isLooseMuon(Float_t miniPFRelIso_all,  Float_t sip3d, Bool_t looseId){
    return (miniPFRelIso_all < constants::iso_cut && sip3d < constants::sip3d_cut && looseId);                                                                                    
}    


bool isFOElec(Float_t pt, Float_t conept, Float_t jetBtagDeepFlav, Bool_t ttH_idEmu_cuts_E3, Bool_t convVeto, UChar_t lostHits, Float_t mvaTTHUL, Float_t jetRelIso, Bool_t mvaFall17V2noIso_WP90, TString year) {
    double bTagCut;
    if (year == "2017") {
    	bTagCut = constants::btag_wp_medium_UL17;
    }
    else {
	throw std::invalid_argument("Invalid year provided");
    }
    bool btagReq = jetBtagDeepFlav < bTagCut;
    bool ptReq = pt > constants::fo_pt_cut;
    bool qualityReq = (ttH_idEmu_cuts_E3 && convVeto && (lostHits==0));
    bool mvaReq = (mvaTTHUL > constants::mva_TTH_e_cut) || 
	          (mvaFall17V2noIso_WP90 && 
		   jetBtagDeepFlav<smoothBFlav(0.9*pt*(1+jetRelIso), 20, 45, year) &&
		   jetRelIso < constants::fo_e_jetRelIso_cut);

    return btagReq && ptReq && qualityReq && mvaReq;
}

bool isFOMuon(Float_t pt, Float_t conept, Float_t jetBtagDeepFlav, Float_t mvaTTHUL, Float_t jetRelIso, TString year) {
    double bTagCut;
    if (year == "2017") {
	bTagCut  = constants::btag_wp_medium_UL17; 
    }
    else {
	throw std::invalid_argument("Invalid year provided");
    }

    bool bTagReq = (jetBtagDeepFlav< bTagCut);
    bool ptReq = (conept > constants::fo_pt_cut);
    bool mvaReq = ((mvaTTHUL>constants::mva_TTH_m_cut) ||
	 	   ((jetBtagDeepFlav<smoothBFlav(0.9*pt*(1+jetRelIso), 20, 45, year) &&
		    (jetRelIso < constants::fo_m_jetRelIso_cut))));
    return bTagReq && ptReq && mvaReq;
}


bool tightSelElec(bool clean_and_FO_selection_TTH, Float_t mvaTTHUL)  {
    return (clean_and_FO_selection_TTH) && (mvaTTHUL > constants::mva_TTH_e_cut);
}

bool tightSelMuon(bool clean_and_FO_selection_TTH, Int_t mediumId, Float_t mvaTTHUL) {
    return (clean_and_FO_selection_TTH && (mediumId>0) && (mvaTTHUL > constants::mva_TTH_m_cut));
}

Float_t coneptElec(Float_t pt, Float_t mvaTTHUL, Float_t jetRelIso) {
    Float_t conePt =  (0.90*pt*(1+jetRelIso));
    return (mvaTTHUL>constants::mva_TTH_e_cut)? pt : conePt;
}

Float_t coneptMuon(Float_t pt, Float_t mvaTTHUL, Float_t jetRelIso, Int_t mediumId) { 
    Float_t conePt = (0.90*pt*(1+jetRelIso));
    return ((mvaTTHUL>constants::mva_TTH_m_cut) && (mediumId>0)) ? pt : conePt;
}

void electronCuts(bool *cuts, Float_t pt, Float_t conept, Float_t jetBtagDeepFlav, Bool_t ttH_idEmu_cuts_E3, Bool_t convVeto, UChar_t lostHits, Float_t mvaTTHUL, Float_t jetRelIso, Bool_t mvaFall17V2noIso_WP90, bool isPres, bool isLoose, Int_t Electron_tightCharge, TString year ) {
    double bTagCut;
    if (year == "2017") {
        bTagCut = constants::btag_wp_medium_UL17;
    }
    else {
        throw std::invalid_argument("Invalid year provided");
    }

    cuts[0] = isPres;                                                                   // pres
    cuts[1] = isLoose && cuts[0];                                                       // loose
    cuts[2] = jetBtagDeepFlav < bTagCut && cuts[1]; 					// btag req
    cuts[3] = conept > constants::fo_pt_cut && cuts[2];					// ptreq
    cuts[4] = (ttH_idEmu_cuts_E3 && convVeto && (lostHits==0)) && cuts[3];		// quality req
    cuts[5] = (mvaTTHUL > constants::mva_TTH_e_cut) ||					// mva req
              (mvaFall17V2noIso_WP90 &&
              jetBtagDeepFlav<smoothBFlav(0.9*pt*(1+jetRelIso), 20, 45, year) &&
              jetRelIso < constants::fo_e_jetRelIso_cut) && cuts[4];
    cuts[6] = Electron_tightCharge == 2 && cuts[5];                                     // FO sel
    cuts[7] = (mvaTTHUL > constants::mva_TTH_e_cut) && cuts[6];				// tight sel
    
}

std::vector<int> electronCuts(int index, int charge, Float_t pt, Float_t conept, Float_t jetBtagDeepFlav, Bool_t ttH_idEmu_cuts_E3, Bool_t convVeto, UChar_t lostHits, Float_t mvaTTHUL, Float_t jetRelIso, Bool_t mvaFall17V2noIso_WP90, bool isPres, bool isLoose, Int_t Electron_tightCharge, TString year){
double bTagCut;
    if (year == "2017") {
        bTagCut = constants::btag_wp_medium_UL17;
    }
    else {
        throw std::invalid_argument("Invalid year provided");
    }
    std::vector<int> cuts;
    cuts.push_back(1);
    cuts.push_back(charge);
    cuts.push_back(index); 
    cuts.push_back(isPres);                                                                   // 3 pres
    cuts.push_back(isLoose && cuts[3]);                                                       // 4 loose
    cuts.push_back(jetBtagDeepFlav < bTagCut && cuts[4]);                                     // 5 btag req
    cuts.push_back(conept > constants::fo_pt_cut && cuts[5]);                                 // 6 ptreq
    cuts.push_back((ttH_idEmu_cuts_E3 && convVeto && (lostHits==0)) && cuts[6]);              // 7 quality req
    cuts.push_back((mvaTTHUL > constants::mva_TTH_e_cut) ||                                   // 8 mva req
              (mvaFall17V2noIso_WP90 &&
              jetBtagDeepFlav<smoothBFlav(0.9*pt*(1+jetRelIso), 20, 45, year) &&
              jetRelIso < constants::fo_e_jetRelIso_cut) && cuts[7]);
    cuts.push_back(Electron_tightCharge == 2 && cuts[8]);                                     // 9 FO sel
    cuts.push_back((mvaTTHUL > constants::mva_TTH_e_cut) && cuts[9]);                         // 10 tight sel

    return cuts;
}

void muonCuts(int *cuts, Float_t pt, Float_t conept, Float_t jetBtagDeepFlav, Float_t mvaTTHUL, Float_t jetRelIso, TString year, Int_t mediumId) {
    double bTagCut;
    if (year == "2017") {
        bTagCut  = constants::btag_wp_medium_UL17;
    }
    else {
        throw std::invalid_argument("Invalid year provided");
    }

    cuts[0] = (jetBtagDeepFlav< bTagCut);							// bTagReq
    cuts[1] = (conept > constants::fo_pt_cut);							// ptReq
    cuts[2] = ((mvaTTHUL>constants::mva_TTH_m_cut) ||						// mvaReq
              ((jetBtagDeepFlav<smoothBFlav(0.9*pt*(1+jetRelIso), 20, 45, year) &&
              (jetRelIso < constants::fo_m_jetRelIso_cut))));
    cuts[3] = cuts[0] && cuts[1] && cuts[2] && (mediumId>0) && (mvaTTHUL > constants::mva_TTH_m_cut);			// tight sel
    
}

std::vector<int> muonCuts(int index, int charge, Float_t pt, Float_t conept, Float_t jetBtagDeepFlav, Float_t mvaTTHUL, Float_t jetRelIso, bool isPres, bool isLoose, TString year, Int_t mediumId) {
    double bTagCut;
    if (year == "2017") {
        bTagCut  = constants::btag_wp_medium_UL17;
    }
    else {
        throw std::invalid_argument("Invalid year provided");
    }
    std::vector<int> cuts;    
    cuts.push_back(10);
    cuts.push_back(charge);
    cuts.push_back(index);
    cuts.push_back(isPres);                                                                   // 3 pres
    cuts.push_back(isLoose && cuts[3]);                                                       // 4 loose
    cuts.push_back (jetBtagDeepFlav< bTagCut && cuts[4]);                                     // 5 bTagReq
    cuts.push_back(conept > constants::fo_pt_cut && cuts[5]);                                 // 6 ptReq
    cuts.push_back((mvaTTHUL>constants::mva_TTH_m_cut) ||                                     // 7 mvaReq
              ((jetBtagDeepFlav<smoothBFlav(0.9*pt*(1+jetRelIso), 20, 45, year) &&
              (jetRelIso < constants::fo_m_jetRelIso_cut))) && cuts[6]) ;
    cuts.push_back((mediumId>0) && (mvaTTHUL > constants::mva_TTH_m_cut) && cuts[7]);         // 8 tight sel

    return cuts;
}
