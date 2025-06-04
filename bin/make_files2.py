import math
import gc
import sys
import ROOT
import numpy as np
import copy
import os
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.gROOT.ProcessLine("gErrorIgnoreLevel = 1;")
ROOT.TH1.AddDirectory(ROOT.kFALSE)
ROOT.gStyle.SetOptStat(0)
from array import array
from ROOT import TColor
from ROOT import TGaxis
from ROOT import THStack
import gc
import sys
import os
import subprocess
import readline
import string


text = ''
text += 'import sys \n'
text += 'import os \n'
text += 'import subprocess \n'
text += 'import readline \n'
text += 'import string \n'
text += '\n'

diremc = '/hadoop/store/user/rgoldouz/FullProduction/FCNC/'
#dire = '/hadoop/store/user/rgoldouz/ExitedTopSamplesMCJan2021/'
#couplingsName = ['ctp','cpQM','cpQ3','cpt','cptb','ctA','ctZ','cbW','ctG','cQl3','cQlM','cQe','ctl','cte','ctlS','ctlT','_randomRWpoint']
couplingsName = ['All','H','noH']
#couplingsName = ['H']
#xs = [0.031,0.049,0.0,0.095,0.0,0.46,1.35,0.0,8.62,0.0,0.176,0.176,0.176,0.176,0.151,3.277,0.0]

MCSAMPLES = {}
for k, key in enumerate(couplingsName):
    MCSAMPLES['tllFcncProductionU' +key] = [['rgoldouz/FullProduction/FCNC/ntuple_ttllFCNC'],    "mc",    "none",    "2017",    "none",  "1"  ,    "41.53",    "15961733", '0', '20']
for key, value in MCSAMPLES.items():
    value[8] = str(1)
    value[0] =[]
    value[7] = str(0)
    value[9] = str(0)
    

for root, dirs, files in os.walk(diremc):
    if 'ntuple' not in root:
        continue
    if 'tllFcncProductionU' not in root:
        continue
    if len(files) > 0:
        print root
        neventsweight = 0
        for fname in files:
            filename = root + '/' + fname
            if 'fail' in fname:
                continue
            f = ROOT.TFile.Open(filename)
            tree_in = f.Get('IIHEAnalysis')
            tree_meta = f.Get('meta')
            tree_meta.GetEntry(0)
            neventsweight += tree_meta.mc_nEventsWeighted
            f.Close()
        for key, value in MCSAMPLES.items():
            if key  in root:
                print key 
                value[0].append(root[19:])
                value[7] = str( float(value[7]) + neventsweight)
                value[9] = str( float(value[9]) + len(files))

for key, value in MCSAMPLES.items():
    if len(value[0])==0:
        del MCSAMPLES[key]

text += 'mcFCNC_samples='                
text += str(MCSAMPLES)
text += '\n'

#diredata = '/hadoop/store/user/rgoldouz/ExitedTopSamplesDataJan2021/'
#DATASAMPLES = {}
#DATASAMPLES.update(Files_2017.data2017_samples)
#
#for key, value in DATASAMPLES.items():
#    value[0] =[]
#for root, dirs, files in os.walk(diredata):
#    if len(files) > 0:
#        for key, value in DATASAMPLES.items():
#            if key in root:
#                value[0].append(root[19:])
#text += '\n'
#text += 'data2017_samples='
#text += str(DATASAMPLES)

open('Files_FCNC_new.py', 'wt').write(text)
