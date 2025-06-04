import sys 
import os 
import subprocess 
import readline 
import string 

UL17={

 
 
"UL17_ZGToLLG":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p3/TTbarPowheg_ZG/v1/UL17_ZGToLLG'], 'mc', 'none', '2017', 'none', '55.78', '41.48', '18731142.8501'],
"UL17_ttHnobb":[['awightma/skims/mc/new-lepMVA-v2/central_sgnl/FullRun2/v1/UL17_ttHnobb'], 'mc', 'none', '2017', 'none', '0.2151', '41.48', '3077771.06037'],
"UL17_tbarW":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p1/FullRun2/v2/UL17_tbarW'], 'mc', 'none', '2017', 'none', '35.85', '41.48', '5673700.19707'],
"UL17_WZZ":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p1/FullRun2/v2/UL17_WZZ'], 'mc', 'none', '2017', 'none', '0.05565', '41.48', '9254639.55042'],
"UL17_ST_top_t_channel":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p1/FullRun2/v2/UL17_ST_top_t_channel'], 'mc', 'none', '2017', 'none', '136.02', '41.48', '121728253.441'],
"UL17_WWZ_4F":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p5/add_ext_bkg/v1/UL17_WWZ_4F'], 'mc', 'none', '2017', 'none', '0.1651', '41.48', '9175332.13889'],
"UL17_ZZTo4L":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p1/FullRun2/v2/UL17_ZZTo4L'], 'mc', 'none', '2017', 'none', '1.256 ', '41.48', '98378103.5373'],
"UL17_TTTo2L2Nu":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p3/TTbarPowheg_ZG/v1/UL17_TTTo2L2Nu'], 'mc', 'none', '2017', 'none', '87.31', '41.48', '105859985.307'],
"UL17_WJetsToLNu":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p1/FullRun2/v2/UL17_WJetsToLNu'], 'mc', 'none', '2017', 'none', '61526.7', '41.48', '105267898.574'],
"UL17_TTWJetsToLNu":[['awightma/skims/mc/new-lepMVA-v2/central_sgnl/FullRun2/v1/UL17_TTWJetsToLNu'], 'mc', 'none', '2017', 'none', '0.2043', '41.48', '3871055.12414'],
"UL17_tZq":[['awightma/skims/mc/new-lepMVA-v2/central_sgnl/FullRun2/v1/UL17_tZq'], 'mc', 'none', '2017', 'none', '0.0758', '41.48', '2578040.00753'],
"UL17_ZZZ":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p1/FullRun2/v2/UL17_ZZZ'], 'mc', 'none', '2017', 'none', '0.01398', '41.48', '8644726.14543'],
"UL17_DY50":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p1/FullRun2/v2/UL17_DY50'], 'mc', 'none', '2017', 'none', '6077.22', '41.48', '131552424.895'],
"UL17_TTGamma_Dilept":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p6/jsons_for_lo_ttgamma/v1/UL17_TTGamma_Dilept'], 'mc', 'none', '2017', 'none', '1.513', '41.48', '10018000.0'],
"UL17_TTGamma_SingleLept":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p6/jsons_for_lo_ttgamma/v1/UL17_TTGamma_SingleLept'], 'mc', 'none', '2017', 'none', '5.121', '41.48', '18906000.0'],
"UL17_TTZToLLNuNu_M_10":[['awightma/skims/mc/new-lepMVA-v2/central_sgnl/FullRun2/v1/UL17_TTZToLLNuNu_M_10'], 'mc', 'none', '2017', 'none', '0.2529', '41.48', '6911466.01119'],
"UL17_DY10to50":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p1/FullRun2/v2/UL17_DY10to50'], 'mc', 'none', '2017', 'none', '18610', '41.48', '68480179.0'],
"UL17_tW":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p1/FullRun2/v2/UL17_tW'], 'mc', 'none', '2017', 'none', '35.85', '41.48', '5648711.86506'],
"UL17_WWTo2L2Nu":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p1/FullRun2/v2/UL17_WWTo2L2Nu'], 'mc', 'none', '2017', 'none', '12.178', '41.48', '7071358.09406'],
"UL17_WWW_4F":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p1/FullRun2/v2/UL17_WWW_4F'], 'mc', 'none', '2017', 'none', '0.2086', '41.48', '9087470.1924'],
"UL17_ST_antitop_t_channel":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p1/FullRun2/v2/UL17_ST_antitop_t_channel'], 'mc', 'none', '2017', 'none', '80.95', '41.48', '65701156.9409'],
"UL17_tttt":[['awightma/skims/mc/new-lepMVA-v2/central_sgnl/FullRun2/v1/UL17_tttt'], 'mc', 'none', '2017', 'none', '0.009103', '41.48', '4526556.16103'],
"UL17_ST_top_s_channel":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p1/FullRun2/v2/UL17_ST_top_s_channel'], 'mc', 'none', '2017', 'none', '3.68', '41.48', '8866570.27604'],
"UL17_WZTo3LNu":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p4/WZTo3LNuPowheg/v2/UL17_WZTo3LNu'], 'mc', 'none', '2017', 'none', '5.28', '41.48', '1972408.00938'],
"UL17_TTToSemiLeptonic":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p3/TTbarPowheg_ZG/v1/UL17_TTToSemiLeptonic'], 'mc', 'none', '2017', 'none', '364.35', '41.48', '343257664.066'],
"UL17_TWZToLL_tlept_Whad":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p7/TWZToLL/v1/UL17_TWZToLL_tlept_Whad'], 'mc', 'none', '2017', 'none', '0.003004', '41.48', '1233755.94901', '0', '1', '0'],
"UL17_TWZToLL_tlept_Wlept":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p7/TWZToLL/v1/UL17_TWZToLL_tlept_Wlept'], 'mc', 'none', '2017', 'none', '0.0015', '41.48', '422595.981816', '0', '1', '0'],
"UL17_TWZToLL_thad_Wlept":[['awightma/skims/mc/new-lepMVA-v2/central_bkgd_p7/TWZToLL/v1/UL17_TWZToLL_thad_Wlept'], 'mc', 'none', '2017', 'none', '0.003004', '41.48', '1249617.94835', '0', '1', '0'],

 
 
"data_UL17_F_DoubleEG":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/DoubleEG_F_UL2017'], 'data', 'DoubleEG', '2017', 'F', '1', '41.48', '1'],
"data_UL17_C_MuonEG":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/MuonEG_C_UL2017'], 'data', 'MuonEG', '2017', 'C', '1', '41.48', '1'],
"data_UL17_D_SingleMuon":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/SingleMuon_D_UL2017'], 'data', 'SingleMuon', '2017', 'D', '1', '41.48', '1'],
"data_UL17_F_MuonEG":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/MuonEG_F_UL2017'], 'data', 'MuonEG', '2017', 'F', '1', '41.48', '1'],
"data_UL17_D_DoubleEG":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/DoubleEG_D_UL2017'], 'data', 'DoubleEG', '2017', 'D', '1', '41.48', '1'],
"data_UL17_C_DoubleMuon":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/DoubleMuon_C_UL2017'], 'data', 'DoubleMuon', '2017', 'C', '1', '41.48', '1'],
"data_UL17_F_SingleMuon":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/SingleMuon_F_UL2017'], 'data', 'SingleMuon', '2017', 'F', '1', '41.48', '1'],
"data_UL17_B_SingleMuon":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/SingleMuon_B_UL2017'], 'data', 'SingleMuon', '2017', 'B', '1', '41.48', '1'],
"data_UL17_D_MuonEG":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/MuonEG_D_UL2017'], 'data', 'MuonEG', '2017', 'D', '1', '41.48', '1'],
"data_UL17_C_SingleMuon":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/SingleMuon_C_UL2017'], 'data', 'SingleMuon', '2017', 'C', '1', '41.48', '1'],
"data_UL17_E_DoubleMuon":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/DoubleMuon_E_UL2017'], 'data', 'DoubleMuon', '2017', 'E', '1', '41.48', '1'],
"data_UL17_B_SingleElectron":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/SingleElectron_B_UL2017'], 'data', 'SingleElectron', '2017', 'B', '1', '41.48', '1'],
"data_UL17_C_SingleElectron":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/SingleElectron_C_UL2017'], 'data', 'SingleElectron', '2017', 'C', '1', '41.48', '1'],
"data_UL17_E_MuonEG":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/MuonEG_E_UL2017'], 'data', 'MuonEG', '2017', 'E', '1', '41.48', '1'],
"data_UL17_F_DoubleMuon":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/DoubleMuon_F_UL2017'], 'data', 'DoubleMuon', '2017', 'F', '1', '41.48', '1'],
"data_UL17_E_DoubleEG":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/DoubleEG_E_UL2017'], 'data', 'DoubleEG', '2017', 'E', '1', '41.48', '1'],
"data_UL17_B_DoubleEG":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/DoubleEG_B_UL2017'], 'data', 'DoubleEG', '2017', 'B', '1', '41.48', '1'],
"data_UL17_B_MuonEG":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/MuonEG_B_UL2017'], 'data', 'MuonEG', '2017', 'B', '1', '41.48', '1'],
"data_UL17_C_DoubleEG":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/DoubleEG_C_UL2017'], 'data', 'DoubleEG', '2017', 'C', '1', '41.48', '1'],
"data_UL17_E_SingleMuon":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/SingleMuon_E_UL2017'], 'data', 'SingleMuon', '2017', 'E', '1', '41.48', '1'],
"data_UL17_B_DoubleMuon":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/DoubleMuon_B_UL2017'], 'data', 'DoubleMuon', '2017', 'B', '1', '41.48', '1'],
"data_UL17_E_SingleElectron":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/SingleElectron_E_UL2017'], 'data', 'SingleElectron', '2017', 'E', '1', '41.48', '1'],
"data_UL17_D_SingleElectron":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/SingleElectron_D_UL2017'], 'data', 'SingleElectron', '2017', 'D', '1', '41.48', '1'],
"data_UL17_D_DoubleMuon":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/DoubleMuon_D_UL2017'], 'data', 'DoubleMuon', '2017', 'D', '1', '41.48', '1'],
"data_UL17_F_SingleElectron":[['awightma/skims/data/NAOD_ULv9_new-lepMVA-v2/FullRun2/v3/SingleElectron_F_UL2017'], 'data', 'SingleElectron', '2017', 'F', '1', '41.48', '1'],
}
