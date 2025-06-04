import sys 
import os 
import subprocess 
import readline 
import string 

UL17={
"UL17_DY50":[['rgoldouz/NanoAodPostProcessingUL/UL17/v1/UL17_DY50/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/crab_UL17_DY50/211203_184109/0000'], 'mc', 'none', '2017', 'none', '6077.22', '41.53', '131552424.895'],
"data_UL17_C_DoubleEG":[['rgoldouz/NanoAodPostProcessingUL/UL17/v1/data_UL17_C_DoubleEG/DoubleEG/crab_data_UL17_C_DoubleEG/211109_183239/0000'], 'data', 'DoubleEG', '2017', 'C', '1', '41.53', '1'],
"data_UL17_F_DoubleEG":[['rgoldouz/NanoAodPostProcessingUL/UL17/v1/data_UL17_F_DoubleEG/DoubleEG/crab_data_UL17_F_DoubleEG/211109_184307/0000'], 'data', 'DoubleEG', '2017', 'F', '1', '41.53', '1'],
"data_UL17_D_DoubleEG":[['rgoldouz/NanoAodPostProcessingUL/UL17/v1/data_UL17_D_DoubleEG/DoubleEG/crab_data_UL17_D_DoubleEG/211109_183003/0000'], 'data', 'DoubleEG', '2017', 'D', '1', '41.53', '1'],
"data_UL17_E_DoubleEG":[['rgoldouz/NanoAodPostProcessingUL/UL17/v1/data_UL17_E_DoubleEG/DoubleEG/crab_data_UL17_E_DoubleEG/211109_185056/0000'], 'data', 'DoubleEG', '2017', 'E', '1', '41.53', '1'],
"data_UL17_B_DoubleEG":[['rgoldouz/NanoAodPostProcessingUL/UL17/v1/data_UL17_B_DoubleEG/DoubleEG/crab_data_UL17_B_DoubleEG/211109_185412/0000'], 'data', 'DoubleEG', '2017', 'B', '1', '41.53', '1'],
}
