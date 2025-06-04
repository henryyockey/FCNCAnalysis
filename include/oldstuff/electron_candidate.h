#ifndef MY_electron_candidate
#define MY_electron_candidate

#include<cmath>
#include<string>
#include<iostream>
#include<vector>
#include<complex>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TH1F.h>

using namespace std;
//using namespace math;
class electron_candidate {
  
public:
  electron_candidate(float, float, float, float, float, float, int, float, bool, float, float, float, float, float, float, float, int, int, int, bool);
  ~electron_candidate();
  float pt;
  float eta;
  float dxy;
  float dz;
  float sip3d;
  float miniPFRelIso_all;
  int lostHits;
  float mvaFall17V1noIso;
  bool convVeto;
  float sieie;
  float hoe;
  float eInvMinusPInv;
  float jetPtRelv2;
  float nearbyJetDeepCSV;
  float mvaTTH;
  float phi;
  int charge;
  int indice;
  int lep; 
  bool isWellMeasured;
  
  bool isTightElectron();
  bool isFakeableElectron();
  bool isLooseElectron();
  int leptonClass();
  bool isTightElectron(TH1F *cuts);
private:
  
};




#endif

