#ifndef SCALEFACTOR_H
#define SCALEFACTOR_H
#include <TROOT.h>
#include <TH2.h>

class ScaleFactor
{
    public:
	ScaleFactor(TString fName, TString Name, TString year);
	TH2F hist_;
	Float_t getScaleFactor(Float_t x, Float_t y, TString );
};

#endif
