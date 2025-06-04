#ifndef Objects
#define Objects

#include <TROOT.h>
#include<cmath>
#include<string>
#include<iostream>
#include<vector>
#include<complex>
#include <TLorentzVector.h>

using namespace std;

bool isTightJet(Float_t, Float_t, Int_t, Float_t);

bool ttH_idEmu_cuts_E3(int*, Float_t, Float_t, Float_t, Float_t, Float_t);

Float_t coneptElec(Float_t, Float_t, Float_t);

Float_t smoothBFlav(Float_t, Float_t, Float_t, TString);

bool isPresElec(Float_t, Float_t,Float_t,Float_t,Float_t,Float_t,Bool_t);

bool isPresMuon(Float_t, Float_t, Float_t, Float_t, Float_t, Float_t);

bool isLooseElec(Float_t, Float_t, UChar_t);

bool isLooseMuon(Float_t, Float_t, Bool_t);

bool isFOElec(Float_t, Float_t, Float_t, Bool_t, Bool_t, UChar_t, Float_t, Float_t, Bool_t, TString);

bool isFOMuon(Float_t, Float_t, Float_t, Float_t, Float_t, TString);

bool tightSelElec(bool, Float_t);

bool tightSelMuon(bool, Int_t, Float_t);

Float_t coneptMuon(Float_t,Float_t,Float_t,Int_t);

void muonCuts(int*, Float_t, Float_t, Float_t, Float_t, Float_t, TString, Int_t);

void electronCuts(bool*, Float_t, Float_t, Float_t, Bool_t, Bool_t, UChar_t, Float_t, Float_t, Bool_t, bool, bool, Int_t, TString);

std::vector<int> electronCuts(int, int, Float_t, Float_t, Float_t, Bool_t, Bool_t, UChar_t, Float_t, Float_t, Bool_t, bool, bool, Int_t, TString);

std::vector<int> muonCuts(int, int, Float_t, Float_t, Float_t, Float_t, Float_t, bool, bool, TString, Int_t);

#endif
