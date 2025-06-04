#ifndef MY_JigsawRecTZFCNC_h
#define MYJigsawRecTZFCNC_h
#include <iostream>
#include <cstring>
#include "RestFrames/RestFrames.hh"

using namespace RestFrames;
using namespace std;
class JigsawRecTZFCNC {
  public:
    JigsawRecTZFCNC();
    virtual ~JigsawRecTZFCNC();
    void Analyze(TLorentzVector l, TLorentzVector lp, TLorentzVector lm, TLorentzVector b, TLorentzVector nu); 
  /////////////////////////////////////////////////////////////////////////////////////////
    //Reco Frames
    LabRecoFrame           *LAB;
    DecayRecoFrame         *TZ;
    DecayRecoFrame         *T;
    DecayRecoFrame         *W;
    VisibleRecoFrame       *B;
    VisibleRecoFrame       *L;
    InvisibleRecoFrame     *NU;
    DecayRecoFrame         *Z;
    VisibleRecoFrame       *Lp;
    VisibleRecoFrame       *Lm;
    InvisibleGroup         *INV;
    SetMassInvJigsaw       *InvMass;
    SetRapidityInvJigsaw   *InvRapidity;
};

class JigsawRecTTFCNC {
public:
   JigsawRecTTFCNC();
   virtual ~JigsawRecTTFCNC();
   void Analyze(TLorentzVector lw, TLorentzVector lp, TLorentzVector lm, TLorentzVector b, TLorentzVector q, TLorentzVector nu );

   LabRecoFrame                 *LAB;
   DecayRecoFrame               *TT;
   DecayRecoFrame               *Ta;
   DecayRecoFrame               *Tb;
   DecayRecoFrame               *W;
   DecayRecoFrame               *Z;
   VisibleRecoFrame             *B;
   VisibleRecoFrame             *Lw;
   VisibleRecoFrame             *Q;
   VisibleRecoFrame             *Lp;
   VisibleRecoFrame             *Lm;
   InvisibleRecoFrame           *NU;

   InvisibleGroup               *INV;
   SetMassInvJigsaw             *InvMass;
   SetRapidityInvJigsaw         *InvRapidity;
};

class JigsawRecTHFCNC {
public:
   JigsawRecTHFCNC();
   virtual ~JigsawRecTHFCNC();
   void Analyze(TLorentzVector la, TLorentzVector lb, TLorentzVector q1, TLorentzVector q2, TLorentzVector b, TLorentzVector met );

   LabRecoFrame                 *LAB;
   DecayRecoFrame               *TH;
   DecayRecoFrame               *T;
   DecayRecoFrame               *H;
   DecayRecoFrame               *WT;
   DecayRecoFrame               *W1;
   DecayRecoFrame               *W2;
   VisibleRecoFrame             *B;
   VisibleRecoFrame             *La;
   VisibleRecoFrame             *Lb;
   VisibleRecoFrame             *Q1;
   VisibleRecoFrame             *Q2;
   InvisibleRecoFrame           *NUa;
   InvisibleRecoFrame           *NUb;

   InvisibleGroup               *INV;
   SetMassInvJigsaw             *NuNuM;
   SetRapidityInvJigsaw         *NuNuR;
   MinMassesSqInvJigsaw         *MinMW;
};


#endif





