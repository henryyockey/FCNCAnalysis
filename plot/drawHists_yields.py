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
from StackPlots import stackPlots
TGaxis.SetMaxDigits(2)



def drawHistallW(A,AN, R, I):
    canvas = ROOT.TCanvas(R+I,R+I,10,10,1100,628)
    canvas.cd()

    pad_name = "pad"
    pad1=ROOT.TPad(pad_name, pad_name, 0, 0, 1, 1 , 0)
    pad1.Draw()
    pad1.cd()
#    pad1.SetLogy()
    A[0].SetTitle('')
    A[0].GetXaxis().SetTitle(R)
    A[0].GetYaxis().SetTitle('N events (2017)')

    maxi=A[0].GetMaximum()
    mini=A[0].GetMinimum()
    for i in range(len(A)):
        A[i].SetLineColor(i+1)
        if A[i].GetMaximum()>maxi:
            maxi=A[i].GetMaximum()
        if A[i].GetMinimum()<mini:
            mini=A[i].GetMinimum()
        if i==9:
            A[i].SetMarkerColor(12);

    A[0].SetMinimum(mini);
    A[0].SetMaximum(maxi * 1.2);

    leg = ROOT.TLegend(0.65,0.70,0.88,0.88)

    for i in range(len(A)):
        A[i].Draw("SAME")
        leg.AddEntry(A[i], AN[i]                           , "L");

#    hs = ROOT.THStack("hs","")
#    for num in range(1,len(A)):
#        hs.Add(A[num])
#    A[0].Draw("e HIST SAME")
#    hs.Draw("e HIST SAME")


    leg.SetBorderSize(0)
    leg.SetTextFont(42)
    leg.SetTextSize(0.04)
    leg.Draw("same")

    canvas.Print(R+"_"+I +".png")
    del canvas
    gc.collect()

 
variables = ["3l(+)1b", "3l(-)1b", "3l(+)2b", "3l(-)2b"]
variablesName= ["3l(+)1b", "3l(-)1b", "3l(+)2b", "3l(-)2b"]

HistAddress = '/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/hists/'



colors =  [ROOT.kBlack,ROOT.kYellow,ROOT.kGreen,ROOT.kBlue-3,ROOT.kRed-4,ROOT.kOrange-3, ROOT.kOrange-6, ROOT.kCyan-6,ROOT.kViolet, ROOT.kBlue-9, ROOT.kYellow-2]


year=['2017']
Samples = [
	   "Data.root", 
	   "UL17_DY.root", 
	   "UL17_tw.root", 	
	   "UL17_diboson.root",
	   "UL17_ttX.root"
	  ]
SamplesName = ["Data", "DY", "tW", "diboson", "ttX" ]
Hists = []

channels=[""]
regions = [""]

for numyear, nameyear in enumerate(year):
    l0=[]
    Files = []
    for f in range(len(Samples)):
        l1=[]
        Files.append(ROOT.TFile.Open(HistAddress  + Samples[f]))
        print HistAddress + Samples[f]
        for numch, namech in enumerate(channels):
            l2=[]
            for numreg, namereg in enumerate(regions):
                l3=[]
                for numvar, namevar in enumerate(variables):
                    h= Files[f].Get(namevar)
                    h.SetFillColor(colors[f])
                    h.SetLineColor(colors[f])
                    l3.append(h)
                l2.append(l3)
            l1.append(l2)
        l0.append(l1)
    Hists.append(l0)

for numyear, nameyear in enumerate(year):
    for numch, namech in enumerate(channels):
        for numreg, namereg in enumerate(regions):
            for numvar, namevar in enumerate(variables):
                HH=[]
                HHsignal=[]
                for f in range(len(Samples)):
                    HH.append(Hists[numyear][f][numch][numreg][numvar])

                stackPlots(HH, HHsignal, SamplesName, namech, namereg, nameyear,namevar,variablesName[numvar])

