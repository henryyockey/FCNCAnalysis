import sys 
import os 
import subprocess 
import readline 
import string 

mcFCNC_samples={'uHDecay': [['rgoldouz/FullProduction/FCNC/ntuple_tuFCNC_uHDecay'], 'mc', 'none', '2017', 'none', '1', '41.53', '500000.0', '1', '200.0'], 'tHProduction': [['rgoldouz/FullProduction/FCNC/ntuple_tuFCNC_tHProduction'], 'mc', 'none', '2017', 'none', '1', '41.53', '500000.0', '1', '200.0'], 'ullDecay_noH': [['rgoldouz/FullProduction/FCNC/ntuple_tuFCNC_ullDecay_noH'], 'mc', 'none', '2017', 'none', '1', '41.53', '204000.0', '1', '102.0'], 'tllProduction_noH': [['rgoldouz/FullProduction/FCNC/ntuple_tuFCNC_tllProduction_noH'], 'mc', 'none', '2017', 'none', '1', '41.53', '500000.0', '1', '200.0'],  'tllProduction_noH_Henry': [['hyockey/FullProduction/Ntuple_tuFCNC_tllProduction_noH/ntuple_tuFCNC_tllProduction_noH'], 'mc', 'none', '2017', 'none', '1', '41.53', '400000.0', '0', '200.0']}
