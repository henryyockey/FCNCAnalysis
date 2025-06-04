import ROOT
import os
from collections import defaultdict
import numpy as np
import math
import gc
import sys
import copy
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.gROOT.ProcessLine("gErrorIgnoreLevel = 1;")
ROOT.TH1.AddDirectory(ROOT.kFALSE)
ROOT.gStyle.SetOptStat(0)
from array import array
from ROOT import TColor
from ROOT import TGaxis
from ROOT import THStack
import gc
TGaxis.SetMaxDigits(2)


channels  = {
#    'LLpp'    : ['LFpp', 'FFpp'], 
#    'LLmm'    : ['LFmm', 'FFmm'], 
#    '3LonZ'   : ['LLFonZ', 'LFFonZ', 'FFFonZ'], 
    '3LoffZp' : ['LLFoffZp', 'LFFoffZp', 'FFFoffZp'], 
    '3LoffZm' : ['LLFoffZm', 'LFFoffZm', 'FFFoffZm'],
}
regions   = ['0Bjet', '1Bjet', 'G1Bjet', "All"]
variables = ["lep1Pt", "BDTOutput"
            #, "lep1Eta", "lep1Phi", "lep2Pt", "lep2Eta", 
            # "lep2Phi", "llM", "llPt", "llDr", "llDphi", "jet1Pt", 
            # "jet1Eta", "jet1Phi", "njet", "nbjet", "Met", "MetPhi", "nVtx"
]

f = ROOT.TFile.Open("Data.root")
g = ROOT.TFile('Fakes.root', 'recreate')

for ch, fch in channels.items():
    for reg in regions: 
        for var in variables:
            LF = f.Get("{}_{}_{}".format(fch[0], reg, var))
            FF = f.Get("{}_{}_{}".format(fch[1], reg, var))
            if '3L' in ch: FFF = f.Get("{}_{}_{}".format(fch[2], reg, var))    
            h = LF.Clone("{}_{}_{}".format(ch, reg, var))
            h.Add(FF,-1)
            if '3L' in ch: h.Add(FFF)
            h.Write()

g.Close()
                    
