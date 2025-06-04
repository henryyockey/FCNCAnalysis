#include "scalefactor.h"
#include<TROOT.h>
#include <TH2.h>
#include <TFile.h>
#include <iostream>

using namespace std;

ScaleFactor::ScaleFactor(TString fName, TString name, TString year) {
   TFile *fin = TFile::Open(fName);
   if (fin) cout << "file " << fName << " opened\n"; 
   hist_ = *(TH2F*)fin->Get(name);
   fin->Close(); 
}

Float_t ScaleFactor::getScaleFactor(Float_t X, Float_t Y, TString uncert="central"){
    int NbinsX=hist_.GetXaxis()->GetNbins();
    int NbinsY=hist_.GetYaxis()->GetNbins();
    float x_min=hist_.GetXaxis()->GetBinLowEdge(1);
    float x_max=hist_.GetXaxis()->GetBinLowEdge(NbinsX)+hist_.GetXaxis()->GetBinWidth(NbinsX);
    float y_min=hist_.GetYaxis()->GetBinLowEdge(1);
    float y_max=hist_.GetYaxis()->GetBinLowEdge(NbinsY)+hist_.GetYaxis()->GetBinWidth(NbinsY);
    TAxis *Xaxis = hist_.GetXaxis();
    TAxis *Yaxis = hist_.GetYaxis();
    Int_t binx=1;
    Int_t biny=1;
    if(x_min < X && X < x_max) binx = Xaxis->FindBin(X);
    else binx= (X<=x_min) ? 1 : NbinsX ;
    if(y_min < Y && Y < y_max) biny = Yaxis->FindBin(Y);
    else biny= (Y<=y_min) ? 1 : NbinsY ;
    if(uncert=="up") return (hist_.GetBinContent(binx, biny)+hist_.GetBinError(binx, biny));
    if(uncert=="down") return (hist_.GetBinContent(binx, biny)-hist_.GetBinError(binx, biny));
    if(uncert=="central") return  hist_.GetBinContent(binx, biny);


    //return hist_.GetBinContent(binx,biny);
}
