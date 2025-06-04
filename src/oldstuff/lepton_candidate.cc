#include "lepton_candidate.h"

lepton_candidate::lepton_candidate(float pt_in, float eta_in, float phi_in, int charge_in, int ind_in, int lep_in, bool temp ){
  pt_ = pt_in;
  eta_ = eta_in;
  phi_ = phi_in;
  charge_ = (charge_in > 0) ? 1 : ((charge_in < 0) ? -1 : 0);
  lep_ = lep_in;
  if(abs(charge_in) == 11)  p4_.SetPtEtaPhiM(pt_, eta_, phi_, 0.000511) ;
  if(abs(charge_in) == 13)  p4_.SetPtEtaPhiM(pt_, eta_, phi_, 0.10566) ;
  if(abs(charge_in) == 15)  p4_.SetPtEtaPhiM(pt_, eta_, phi_, 1.7) ;
  indice_ = ind_in;
  isWellMeasured_ = true;
}

lepton_candidate::lepton_candidate(electron_candidate ele ){
  pt_ = ele.pt;
  eta_ = ele.eta;
  phi_ = ele.phi;
  charge_ = (ele.charge > 0) ? 1 : ((ele.charge < 0) ? -1 : 0);
  lep_ = ele.lep;
  p4_.SetPtEtaPhiM(ele.pt, ele.eta, ele.phi, 0.000511) ;
  indice_ = ele.indice;
  isWellMeasured_ = ele.isWellMeasured;
  leptonClass_ = ele.leptonClass();
}


lepton_candidate::lepton_candidate(muon_candidate muo ){
  pt_ = muo.pt;
  eta_ = muo.eta;
  phi_ = muo.phi;
  charge_ = (muo.charge > 0) ? 1 : ((muo.charge < 0) ? -1 : 0);
  lep_ = muo.lep;
  p4_.SetPtEtaPhiM(muo.pt, muo.eta, muo.phi, 0.10566) ;
  indice_ = muo.indice;
  isWellMeasured_ = muo.isWellMeasured;
  leptonClass_ = muo.leptonClass();
}


lepton_candidate::~lepton_candidate(){}


