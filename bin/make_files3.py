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

diremc = '/hadoop/store/user/rgoldouz/NanoAodPostProcessingUL/UL17/v1/'

MCSAMPLES = {}

for root, dirs, files in os.walk(diremc):
    if len(files) > 0 and "DoubleEG" in root:
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
        if "rwgt" in root:
            MCSAMPLES[root.split('/')[-1][7:]]= [[root[19:]],    "mc",    "none",    "2017",    "none",  "1"  ,    "100",    str(neventsweight), '1', str(len(files))]
        else:
            MCSAMPLES[root.split('/')[-1][7:]]= [[root[19:]],    "mc",    "none",    "2017",    "none",  "1"  ,    "100",    str(neventsweight), '0', str(len(files))]


text += 'mcFCNC_samples='                
text += str(MCSAMPLES)
text += '\n'
open('Files_FCNC_new2.py', 'wt').write(text)
