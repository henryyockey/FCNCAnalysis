#include "JigsawRecTZFCNC.h"

using namespace RestFrames;
using namespace std;

JigsawRecTZFCNC::JigsawRecTZFCNC(){
  LAB=new LabRecoFrame("LAB","LAB");
  TZ=new DecayRecoFrame("TZ","tZ");
  T=new DecayRecoFrame("T","t");
  W=new DecayRecoFrame("W","W");
  B=new VisibleRecoFrame("B","b");
  L=new VisibleRecoFrame("L","#it{l}");
  NU=new InvisibleRecoFrame("NU","#nu");
  Z=new DecayRecoFrame("Z","Z");
  Lp=new VisibleRecoFrame("Lp","#it{l}^{+}");
  Lm=new VisibleRecoFrame("Lm","#it{l}^{-}");

  LAB->SetChildFrame(*TZ);
  TZ->AddChildFrame(*T);
  TZ->AddChildFrame(*Z);
  T->AddChildFrame(*B);
  T->AddChildFrame(*W);
  W->AddChildFrame(*L);
  W->AddChildFrame(*NU);
  Z->AddChildFrame(*Lp);
  Z->AddChildFrame(*Lm);

  LAB->InitializeTree(); 
  INV = new InvisibleGroup("INV","Invisible System");
  INV->AddFrame(*NU);

  // set the invisible system mass to zero
  InvMass = new SetMassInvJigsaw("InvMass","M_{#nu} = 0");
  INV->AddJigsaw(*InvMass);

  // set the invisible system rapidity to that of visible
  InvRapidity = new SetRapidityInvJigsaw("InvRapidity", "Invisible system rapidity Jigsaw");
  INV->AddJigsaw(*InvRapidity);
//  InvRapidity->AddVisibleFrames(TZ->GetListVisibleFrames());
  InvRapidity->AddVisibleFrame(*L);
  InvRapidity->AddVisibleFrame(*B);  
  LAB->InitializeAnalysis(); 
   ////////////// Jigsaw rules set-up /////////////////
}

void JigsawRecTZFCNC::Analyze(TLorentzVector l, TLorentzVector lp, TLorentzVector lm, TLorentzVector b, TLorentzVector nu){
  LAB->ClearEvent();                                   // clear the reco tree

  L->SetLabFrameFourVector(l);     // Set 4-vector
  Lp->SetLabFrameFourVector(lp);   // Set 4-vector
  Lm->SetLabFrameFourVector(lm);   // Set 4-vector
  B->SetLabFrameFourVector(b);     // Set 4-vector
  INV->SetLabFrameFourVector(nu);     // Set 4-vector
  LAB->AnalyzeEvent();                                 //analyze the event
}


JigsawRecTZFCNC::~JigsawRecTZFCNC(){
  delete LAB; 
  delete TZ;
  delete T;
  delete W;
  delete B;
  delete L;
  delete NU;
  delete Z;
  delete Lp;
  delete Lm;
  delete INV;
  delete InvMass;
  delete InvRapidity;
}


JigsawRecTTFCNC::JigsawRecTTFCNC(){
  LAB=new LabRecoFrame("LAB","LAB");
  TT=new DecayRecoFrame("TT","tt");
  Ta=new DecayRecoFrame("Ta","t");
  Tb=new DecayRecoFrame("Tb","t");
  Z=new DecayRecoFrame("Z","Z");
  W=new DecayRecoFrame("W","W");
  B=new VisibleRecoFrame("B","b");
  Lw=new VisibleRecoFrame("Lw","#it{l}");
  NU=new InvisibleRecoFrame("NU","#nu");
  Q = new VisibleRecoFrame("Q", "q");
  Lp=new VisibleRecoFrame("Lp","#it{l}^{+}");
  Lm=new VisibleRecoFrame("Lm","#it{l}^{-}");

  LAB->SetChildFrame(*TT);
  TT->AddChildFrame(*Ta);
  TT->AddChildFrame(*Tb);
  Ta->AddChildFrame(*B);
  Ta->AddChildFrame(*W);
  W->AddChildFrame(*Lw);
  W->AddChildFrame(*NU);
  Tb->AddChildFrame(*Z);
  Tb->AddChildFrame(*Q);
  Z->AddChildFrame(*Lp);
  Z->AddChildFrame(*Lm);

  LAB->InitializeTree();
  INV = new InvisibleGroup("INV","Invisible System");
  INV->AddFrame(*NU);

  // set the invisible system mass to zero
  InvMass = new SetMassInvJigsaw("InvMass","M_{#nu} = 0");
  INV->AddJigsaw(*InvMass);
  // set the invisible system rapidity to that of visible
  InvRapidity = new SetRapidityInvJigsaw("InvRapidity", "Invisible system rapidity Jigsaw");
  INV->AddJigsaw(*InvRapidity);
  InvRapidity->AddVisibleFrame(*Lw);
  InvRapidity->AddVisibleFrame(*B);
  LAB->InitializeAnalysis();
}

void JigsawRecTTFCNC::Analyze(TLorentzVector lw, TLorentzVector lp, TLorentzVector lm, TLorentzVector b, TLorentzVector q,  TLorentzVector nu){
  LAB->ClearEvent();                                   // clear the reco tree

  Lw->SetLabFrameFourVector(lw);     // Set 4-vector
  Lp->SetLabFrameFourVector(lp);   // Set 4-vector
  Lm->SetLabFrameFourVector(lm);   // Set 4-vector
  B->SetLabFrameFourVector(b);     // Set 4-vector
  Q->SetLabFrameFourVector(q);
  INV->SetLabFrameFourVector(nu);     // Set 4-vector
  LAB->AnalyzeEvent();                                 //analyze the event
}

JigsawRecTTFCNC::~JigsawRecTTFCNC(){
  delete LAB;
  delete TT;
  delete Ta;
  delete Tb;
  delete W;
  delete B;
  delete Lw;
  delete NU;
  delete Z;
  delete Lp;
  delete Lm;
  delete INV;
  delete InvMass;
  delete InvRapidity;
}

JigsawRecTHFCNC::JigsawRecTHFCNC() {
  LAB=new LabRecoFrame("LAB","LAB");
  TH=new DecayRecoFrame("TH","tH");
  T=new DecayRecoFrame("T","t");
  H=new DecayRecoFrame("H","H");
  W1=new DecayRecoFrame("W1","W");
  W2=new DecayRecoFrame("W2", "W");
  WT=new DecayRecoFrame("WT","W");
  B=new VisibleRecoFrame("B","b");
  La=new VisibleRecoFrame("La","#it{l}");
  NUa=new InvisibleRecoFrame("NUa","#nu");
  Lb=new VisibleRecoFrame("Lb","#it{l}");
  NUb=new InvisibleRecoFrame("NUb","#nu");
  Q1 = new VisibleRecoFrame("Q1", "q");
  Q2 = new VisibleRecoFrame("Q2", "q");

  LAB->SetChildFrame(*TH);
  TH->AddChildFrame(*T);
  TH->AddChildFrame(*H);
  T->AddChildFrame(*WT);
  T->AddChildFrame(*B);
  H->AddChildFrame(*W1);
  H->AddChildFrame(*W2);
  WT->AddChildFrame(*La);
  WT->AddChildFrame(*NUa);
  W1->AddChildFrame(*Q1);
  W1->AddChildFrame(*Q2);
  W2->AddChildFrame(*Lb);
  W2->AddChildFrame(*NUb);

  LAB->InitializeTree();
  INV = new InvisibleGroup("INV","Invisible System");
  INV->AddFrame(*NUa);
  INV->AddFrame(*NUb);

  NuNuM = new SetMassInvJigsaw("NuNuM", "M_{#nu#nu} ~ m_{#it{l}#it{l}}");
  INV->AddJigsaw(*NuNuM);
  
  NuNuR = new SetRapidityInvJigsaw("NuNuR", "#eta_{#nu#nu}= #eta_{#it{l}#it{l}}");
  INV->AddJigsaw(*NuNuR);
  NuNuR->AddVisibleFrames(*La+*Lb+*B);
 
  MinMW = new MinMassesSqInvJigsaw("MinMW", "min #Sigma M_{W}^{ 2}",2);
  INV->AddJigsaw(*MinMW);
  MinMW->AddVisibleFrames(WT->GetListVisibleFrames(),0);
  MinMW->AddVisibleFrames(W2->GetListVisibleFrames(),1);
  MinMW->AddInvisibleFrames(WT->GetListInvisibleFrames(),0);
  MinMW->AddInvisibleFrames(W2->GetListInvisibleFrames(),1);



  LAB->InitializeAnalysis();
}

void JigsawRecTHFCNC::Analyze(TLorentzVector la, TLorentzVector lb, TLorentzVector q1, TLorentzVector q2, TLorentzVector b, TLorentzVector met) {
  LAB->ClearEvent();

  La->SetLabFrameFourVector(la);
  Lb->SetLabFrameFourVector(lb);
  Q1->SetLabFrameFourVector(q1);
  Q2->SetLabFrameFourVector(q2);
  B ->SetLabFrameFourVector(b);
  INV->SetLabFrameFourVector(met);
  LAB->AnalyzeEvent();
}

JigsawRecTHFCNC::~JigsawRecTHFCNC() {
  delete LAB;
  delete TH;
  delete T;
  delete H;
  delete WT;
  delete W1;
  delete W2;
  delete La;
  delete Lb;
  delete Q1;
  delete Q2;
  delete B;
  delete INV;
  delete NuNuM;
  delete NuNuR;
  delete MinMW;
}
