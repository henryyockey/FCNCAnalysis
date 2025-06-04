import sys
import os
import subprocess
import readline
import string
import glob
sys.path.append('/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/bin')
import Files_2017_nano
SAMPLES = {}
SAMPLES.update(Files_2017_nano.UL17)



#os.system('rm *.root')
dist = "/hadoop/store/user/hyockey/FullProduction/FCNC_nano_analysis8/Analysis_" 

for key, value in SAMPLES.items():
    year = value[3]
    hadd='hadd ' + key + '.root '
    for filename in os.listdir(dist + key):
        hadd += dist + key + '/' + filename + ' '
    os.system(hadd)


#for key, value in SAMPLES.items():
#    os.system('cp ' + glob.glob(dist  + key + '/*.root')[0] + ' ' + key + '.root')

