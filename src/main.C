#include "MyAnalysis.h"
int main(){
    TChain* ch    = new TChain("Events") ;
//    ch->Add("/cms/cephfs/data/store/user/awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/SingleMuon_B_UL2017/output_4208.root");
//    ch->Add("/hadoop/store/user/awightma/skims/mc/new-lepMVA-v2/central_bkgd_p1/FullRun2/v2/UL17_DY50/output_3831.root");
//    ch->Add("/hadoop/store/user/awightma/skims/mc/new-lepMVA-v2/central_bkgd_p1/FullRun2/v2/UL17_WJetsToLNu/output_3724.root");
//    ch->Add("/cms/cephfs/data/store/user/awightma/skims/mc/new-lepMVA-v2/central_bkgd_p1/FullRun2/v2/UL17_DY50/output_1090.root");  
    ch->Add("/cms/cephfs/data/store/user/rgoldouz/FullProduction/FullR2/UL17/FullSimFCNC/postLHE_step/v1/UL17_tuFCNC_tllProduction_noH_skimmed/output_15.root");
//    ch->Add("/hadoop/store/user/rgoldouz/FullProduction/FullR2/UL17/FullSimFCNC/postLHE_step/v1/UL17_tuFCNC_ullDecay_noH_skimmed/*");

//   ch->Add("/cms/cephfs/data/store/user/rgoldouz/FullProduction/FullR2/UL17/FullSimFCNC/postLHE_step/v1/UL17_tuFCNC_tHProduction_skimmed/*");

//    ch->Add("/cms/cephfs/data/store/user/rgoldouz/FullProduction/FullR2/UL17/FullSimFCNC/postLHE_step/v1/UL17_tuFCNC_tllProduction_noH_skimmed/output_7.root");
    MyAnalysis t1(ch);
//    t1.Loop("UL17_TWZToLL_tlept_Wlept", "mc", "", "2017", "", 0.0015, 41.48, 422595.981816, 0, 0);
//      'data', 'MuonEG', '2017', 'B', '1', '41.48', '1', '0', '1', '0'
//    t1.Loop("UL17_data", "data","SingleMuon", "2017", "B", 1, 41.48, 1, 0, 1);
//    t1.Loop("UL17_DY50", "mc", "", "2017", "", 6077.22, 41.48, 131552424.895, 0, 0);
    t1.Loop("UL17_tuFCNC_tllProduction_noH_skimmed","mc", "", "2017", "", 1, 1, 200000, 1, 1);
//    t1.Loop("UL17_tuFCNC_ullDecay_noH_skimmed","mc", "", "2017", "", 1, 1, 200000, 1, 1);
//    t1.Loop("UL17_WJetsToLNu", "mc", "", "2017", "", 6077.22, 41.48, 131552424.895, 1, 0);

//      ch->Add("/hadoop/store/user/awightma/skims/mc/new-lepMVA-v2/central_bkgd_p1/FullRun2/v2/UL17_WWTo2L2Nu/*");
//      MyAnalysis t1(ch);
//      t1.Loop("UL17_WWTo2L2Nu", "mc" , "none" , "2017" , "none" , 12.178 , 41.48 , 7071358.09406 , 0 , 1);

//    ch->Add("/cms/cephfs/data/store/user/awightma/skims/mc/new-lepMVA-v2/central_bkgd_p1/FullRun2/v2/UL17_WJetsToLNu/output_3724.root");
//    MyAnalysis t1(ch);
//    t1.Loop("UL17_WJetsToLNu", "mc" , "none" , "2017" , "none" , 61526.7 , 41.48 , 105267898.574 , 0 , 1);

//   ch->Add("/cms/cephfs/data/store/user/awightma/skims/mc/new-lepMVA-v2/central_bkgd_p3/TTbarPowheg_ZG/v1/UL17_TTTo2L2Nu/output_10*");
//   MyAnalysis t1(ch);
//   t1.Loop("UL17_TTTo2L2Nu", "mc" , "none" , "2017" , "none" , 87.31 , 41.48 , 105859985.307 , 0 , 1);


}

/*
#include "MyAnalysis.h"
int main(){
    TChain* ch    = new TChain("Events") ;
    TChain ch1("meta") ;
    ch->Add("/hadoop/store/user/awightma/skims/mc/new-lepMVA-v2/central_bkgd_p1/FullRun2/v2/UL17_DY50/output_3801.root");
    ch1.Add("/hadoop/store/user/awightma/skims/mc/new-lepMVA-v2/central_bkgd_p1/FullRun2/v2/UL17_DY50/output_3801.root");
    MyAnalysis t1(ch);
    t1.Loop("UL17_DY50", "mc" , "" , "2017" , "" , 1 , 50.0 , 1);
}

"UL17_tuFCNC_tHProduction_skimmed":[['rgoldouz/FullProduction/FullR2/UL17/FullSimFCNC/postLHE_step/v1/UL17_tuFCNC_tHProduction_skimmed'], 'mc', 'none', '2017', 'none', '1', '41.48', '200000', '1', '200', '0'],

*/
