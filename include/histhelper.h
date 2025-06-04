#ifndef HISTHELPER_H
#define HISTHELPER_H

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include  "TH1EFT.h"

typedef std::vector<TH1F*> Dim1;
typedef std::vector<Dim1> Dim2;
typedef std::vector<Dim2> Dim3;
typedef std::vector<Dim3> Dim4;

Dim1 makeHists(std::map<TString, std::vector<float>>);
Dim2 makeHists(std::vector<TString>, std::map<TString, std::vector<float>>);
Dim3 makeHists(std::vector<TString>, std::vector<TString>, std::map<TString, std::vector<float>>);
Dim4 makeHists(std::vector<TString>, std::vector<TString>, std::vector<TString>, std::map<TString, std::vector<float>>);

void writeHists(TFile*, Dim1);
void writeHists(TFile*, Dim2);
void writeHists(TFile*, Dim3);
void writeHists(TFile*, Dim4);

typedef std::vector<TH1EFT*> Dim1EFT;
typedef std::vector<Dim1EFT> Dim2EFT;
typedef std::vector<Dim2EFT> Dim3EFT;
typedef std::vector<Dim3EFT> Dim4EFT;

Dim1EFT makeHistsEFT(std::map<TString, std::vector<float>>);
Dim2EFT makeHistsEFT(std::vector<TString>, std::map<TString, std::vector<float>>);
Dim3EFT makeHistsEFT(std::vector<TString>, std::vector<TString>, std::map<TString, std::vector<float>>);
Dim4EFT makeHistsEFT(std::vector<TString>, std::vector<TString>, std::vector<TString>, std::map<TString, std::vector<float>>, TString optionalName);

void writeHists(TFile*, Dim1EFT);
void writeHists(TFile*, Dim2EFT);
//void writeHists(TFile*, Dim3EFT);
//void writeHists(TFile*, Dim4EFT);
void writeHists(TFile*, const Dim3EFT&, const TString&);
void writeHists(TFile*, const Dim4EFT&, const TString&);


#endif
