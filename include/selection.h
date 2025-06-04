#ifndef Selection
#define Selection

#include "lepton_candidate.h"
#include "jet_candidate.h"
#include <TROOT.h>
#include<cmath>
#include<string>
#include<iostream>
#include<vector>
#include<complex>
#include <TLorentzVector.h>
//#include "MyAnalysis.h"

using namespace std;
bool isMatched(MyAnalysis *evt, int l, int pdgid, TString MC, bool ifChargedMatched); 
bool isOnZ(std::vector<lepton_candidate*>);
TString EventCategory(std::vector<lepton_candidate*>, std::vector<jet_candidate*>, bool);
#endif
