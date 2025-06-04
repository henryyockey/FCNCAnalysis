#include "muon_candidate.h" 

muon_candidate::muon_candidate(
        float pt_in,
        float eta_in,
        float dxy_in,
        float dz_in,
        float sip3d_in,
        float miniPFRelIso_all_in,
        int pfIsoId_in,
	float segmentComp_in,
	float jetPtRelv2_in,
        float nearbyJetDeepCSV_in,
        float mvaTTH_in,
        float phi_in,
        int charge_in,
        int ind_in,
        int lep_in,
	bool isWellMeasured_in
){
  pt = pt_in;
  eta = eta_in;
  dxy = dxy_in;
  dz = dz_in;
  sip3d = sip3d_in;
  miniPFRelIso_all = miniPFRelIso_all_in;
  pfIsoId = pfIsoId_in;
  segmentComp = segmentComp_in;
  jetPtRelv2 = jetPtRelv2_in;
  nearbyJetDeepCSV = nearbyJetDeepCSV_in;
  mvaTTH = mvaTTH_in;
  phi = phi_in;
  charge = charge_in;
  indice = ind_in;
  lep = lep_in;
  isWellMeasured = isWellMeasured_in;
}


muon_candidate::~muon_candidate(){}

bool muon_candidate::isTightMuon() {
  bool a = pt > 10;
  bool b = abs(eta) < 2.4;
  bool c = abs(dxy) < 0.05;
  bool d = abs(dz) < 0.1;
  bool e = miniPFRelIso_all < 8;
  bool f = sip3d < 0.4*pt;
  bool g =  pfIsoId >= 3;
  bool h = nearbyJetDeepCSV < 0.4941;
  bool i = mvaTTH > 0.90;


  return a && b && c && d && e && f && g && h && i;

}

bool muon_candidate::isFakeableMuon() {
  bool a = pt > 10;
  bool b = abs(eta) < 2.4;
  bool c = abs(dxy) < 0.05;
  bool d = abs(dz) < 0.1;
  bool e = miniPFRelIso_all < 8;
  bool f = sip3d < 0.4*pt;
  bool g =  pfIsoId >= 2;
  bool h = nearbyJetDeepCSV < 0.07;
  bool i = mvaTTH < 0.90;
  bool j = segmentComp > 0.3;
  bool k = jetPtRelv2 > 0.6;


  return a && b && c && d && e && f && g && h && i && j && k;

}

bool muon_candidate::isLooseMuon() {
  bool a = pt > 10;
  bool b = abs(eta) < 2.4;
  bool c = abs(dxy) < 0.05;
  bool d = abs(dz) < 0.1;
  bool e = miniPFRelIso_all < 8;
  bool f = sip3d < 0.4*pt;
  bool g =  pfIsoId >= 2;

  return a && b && c && d && e && f && g;

}

int muon_candidate::leptonClass() {
  if (muon_candidate::isTightMuon()) return 3;
  else if (muon_candidate::isFakeableMuon()) return 2;
  else if (muon_candidate::isLooseMuon()) return 1;
  else return 0;
}

