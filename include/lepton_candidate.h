#ifndef MY_lepton_candidate
#define MY_lepton_candidate

#include<cmath>
#include<string>
#include<iostream>
#include<vector>
#include<complex>
#include <TLorentzVector.h>

using namespace std;
//using namespace math;
class lepton_candidate {
  
public:
  lepton_candidate(float pt_in, float eta_in, float phi_in, int charge_in, int ind_in, int lep_in);
  lepton_candidate(float pt_in, float eta_in, float phi_in, int charge_in, int ind_in, int lep_in, bool isPresLep, bool isLooseLep, bool isFOLep, bool isTightLep, float scalefactor, int pdgid_in);
  ~lepton_candidate();
  float pt_;
  float eta_;
  float phi_;
  int charge_;
  int indice_;
  int lep_;
  TLorentzVector p4_;
  bool isPresLep_;
  bool isLooseLep_;
  bool isFOLep_;
  bool isTightLep_;
  float scalefactor_;
  int pdgid_;

private:
  
};

//class electron_candidate : public lepton_candidate {

//}

#endif

