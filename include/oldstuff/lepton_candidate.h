#ifndef MY_lepton_candidate
#define MY_lepton_candidate

#include<cmath>
#include<string>
#include<iostream>
#include<vector>
#include<complex>
#include <TLorentzVector.h>
#include "electron_candidate.h"
#include "muon_candidate.h"

using namespace std;
//using namespace math;
class lepton_candidate {
  
public:
  lepton_candidate(float, float, float, int, int, int, bool );
  lepton_candidate(muon_candidate);
  lepton_candidate(electron_candidate);
  ~lepton_candidate();
  float pt_;
  float eta_;
  float phi_;
  int charge_;
  int indice_;
  int lep_;
  TLorentzVector p4_;
  bool isWellMeasured_;
  int leptonClass_;
private:
  
};

#endif

