#include "MyAnalysis.h"
int main(){
    TChain* ch    = new TChain("Events") ;
    TChain ch1("meta") ;
    ch->Add("/hadoop/store/user/awightma/skims/data/NanoAOD_ULv8_UL2017/2021_11_16/v1/SingleMuon_F_UL2017/output_1680.root");
    ch1.Add("/hadoop/store/user/awightma/skims/data/NanoAOD_ULv8_UL2017/2021_11_16/v1/SingleMuon_F_UL2017/output_1680.root");
    ch ->AddFriend("meta");
    MyAnalysis t1(ch);
    t1.Loop("SingleMuon_F_UL2017", "data" , "SingleMuon" , "2017" , "F" , 1 , 41.53 , 17668916);
}