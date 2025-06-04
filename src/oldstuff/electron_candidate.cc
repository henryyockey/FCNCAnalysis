#include "electron_candidate.h"

electron_candidate::electron_candidate(
	float pt_in, 
	float eta_in, 
	float dxy_in, 
	float dz_in, 
	float sip3d_in, 
	float miniPFRelIso_all_in, 
	int lostHits_in, 
	float mvaFall17V1noIso_in, 
	bool convVeto_in, 
	float sieie_in, 
	float hoe_in, 
	float eInvMinusPInv_in, 
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
  lostHits = lostHits_in;
  mvaFall17V1noIso = mvaFall17V1noIso_in;
  convVeto = convVeto_in;
  sieie = sieie_in;
  hoe = hoe_in;
  eInvMinusPInv = eInvMinusPInv_in;
  jetPtRelv2 = jetPtRelv2_in;
  nearbyJetDeepCSV = nearbyJetDeepCSV_in;
  mvaTTH = mvaTTH_in;
  phi = phi_in;
  charge = charge_in;
  indice = ind_in;
  lep = lep_in;
  isWellMeasured = isWellMeasured_in;
}


electron_candidate::~electron_candidate(){}

bool electron_candidate::isTightElectron() {
  bool a = pt > 10;
  bool b = abs(eta) < 2.5;
  bool c = abs(dxy) < 0.05;
  bool d = abs(dz) < 0.1;
  bool e = sip3d < 0.4*pt;
  bool f = lostHits == 0;
  bool g;
  if (abs(eta) < 0.8) {
    if (pt < 10) g = mvaFall17V1noIso > -0.13;
    else g = mvaFall17V1noIso > -0.86;
  }
  else if (abs(eta) > 1.44) {
    if (pt < 10) g = mvaFall17V1noIso > -0.32;
    else g = mvaFall17V1noIso > -0.81;
  }
  else {
    if (pt < 10) g = mvaFall17V1noIso > -0.08;
    else g = mvaFall17V1noIso > -0.72;
  }
  bool h = convVeto;
  bool i;
  if (abs(eta) < 0.8) {
    i = sieie<0.011;
  }
  else if (abs(eta) > 1.44) {
    i = sieie<0.030;
  }
  else {
    i = sieie<0.011;
  }
  bool j = hoe<0.10;
  bool k = eInvMinusPInv > -0.04;
  bool l = nearbyJetDeepCSV < 0.4941;
  bool m = mvaTTH > 0.90;
  
  
  return a && b && c && d && e && f && g && h && i && j && k && l && m;

}

bool electron_candidate::isFakeableElectron() {
  bool a = pt > 10;
  bool b = abs(eta) < 2.5;
  bool c = abs(dxy) < 0.05;
  bool d = abs(dz) < 0.1;
  bool e = sip3d < 0.4*pt;
  bool f = lostHits == 0;
  bool g = mvaFall17V1noIso > 0.50;
  bool h = convVeto;
  bool i;
  if (abs(eta) < 0.8) {
    i = sieie<0.011;
  }
  else if (abs(eta) > 1.44) {
    i = sieie<0.030;
  }
  else {
    i = sieie<0.011;
  }
  bool j = hoe<0.10;
  bool k = eInvMinusPInv > -0.04;
  bool l = nearbyJetDeepCSV < 0.07;
  bool m = mvaTTH < 0.90;
  bool n = jetPtRelv2 > 0.6;


  return a && b && c && d && e && f && g && h && i && j && k && l && m && n;

}

bool electron_candidate::isLooseElectron() {
  bool a = pt > 7;
  bool b = abs(eta) < 2.5;
  bool c = abs(dxy) < 0.05;
  bool d = abs(dz) < 0.1;
  bool e = sip3d < 0.4*pt;
  bool f = lostHits <= 1;
  bool g;
  if (abs(eta) < 0.8) {
    if (pt < 10) g = mvaFall17V1noIso > -0.13;
    else g = mvaFall17V1noIso > -0.86;
  }
  else if (abs(eta) > 1.44) {
    if (pt < 10) g = mvaFall17V1noIso > -0.32;
    else g = mvaFall17V1noIso > -0.81;
  }
  else {
    if (pt < 10) g = mvaFall17V1noIso > -0.08;
    else g = mvaFall17V1noIso > -0.72;
  }

  return a && b && c && d && e && f && g;

}

int electron_candidate::leptonClass() {
  if (electron_candidate::isTightElectron()) return 3;
  else if (electron_candidate::isFakeableElectron()) return 2;
  else if (electron_candidate::isLooseElectron()) return 1;
  else return 0;

  
}

bool electron_candidate::isTightElectron(TH1F * cuts) {
  bool a = pt > 10; 		
  bool b = abs(eta) < 2.5; 	
  bool c = abs(dxy) < 0.05; 	
  bool d = abs(dz) < 0.1; 		
  bool e = sip3d < 0.4*pt; 		
  bool f = lostHits == 0; 		
  bool g;
  if (abs(eta) < 0.8) {
    if (pt < 10) g = mvaFall17V1noIso > -0.13;
    else g = mvaFall17V1noIso > -0.86;
  }
  else if (abs(eta) > 1.44) {
    if (pt < 10) g = mvaFall17V1noIso > -0.32;
    else g = mvaFall17V1noIso > -0.81;
  }
  else {
    if (pt < 10) g = mvaFall17V1noIso > -0.08;
    else g = mvaFall17V1noIso > -0.72;
  } 					
  bool h = convVeto; 			
  bool i;
  if (abs(eta) < 0.8) {
    i = sieie<0.011;
  }
  else if (abs(eta) > 1.44) {
    i = sieie<0.030;
  }
  else {
    i = sieie<0.011;
  } 					
  bool j = hoe<0.10; 			
  bool k = eInvMinusPInv > -0.04; 	
  bool l = nearbyJetDeepCSV < 0.4941; 	
  bool m = mvaTTH > 0.90; 		

  cuts->AddBinContent(1);
  if (a) {cuts->AddBinContent(2);
  if (b) {cuts->AddBinContent(3);
  if (c) {cuts->AddBinContent(4);
  if (d) {cuts->AddBinContent(5);
  if (e) {cuts->AddBinContent(6);
  if (f) {cuts->AddBinContent(7);
  if (g) {cuts->AddBinContent(8);
  if (h) {cuts->AddBinContent(9);
  if (i) {cuts->AddBinContent(10);
  if (j) {cuts->AddBinContent(11);
  if (k) {cuts->AddBinContent(12);
  if (l) {cuts->AddBinContent(13);
  if (m) {cuts->AddBinContent(14);
  }}}}}}}}}}}}}

  return a && b && c && d && e && f && g && h && i && j && k && l && m;

}                                                    	
