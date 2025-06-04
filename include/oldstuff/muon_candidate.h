#ifndef MY_muon_candidate
#define MY_muon_candidate

#include<cmath>
#include<string>
#include<iostream>
#include<vector>
#include<complex>
#include <TLorentzVector.h>

using namespace std;
//using namespace math;
class muon_candidate {
  
public:
  muon_candidate(float, float, float, float, float, float, int, float, float, float, float, float, int, int, int, bool);
  ~muon_candidate();
  float pt;
  float eta;
  float dxy;
  float dz;
  float sip3d;
  float miniPFRelIso_all;
  int pfIsoId;
  float segmentComp;  
  float jetPtRelv2;
  float nearbyJetDeepCSV;
  float mvaTTH;
  float phi;
  int charge;
  int indice;
  int lep; 
  bool isWellMeasured;
  
  bool isTightMuon();
  bool isFakeableMuon();
  bool isLooseMuon();
  int leptonClass();
private:
  
};




#endif

