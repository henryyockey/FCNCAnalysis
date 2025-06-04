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

    A[0].GetXaxis().SetRange(2, A[0].GetNbinsX())
    A[1].GetXaxis().SetRange(2, A[1].GetNbinsX())
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
        A[i].Draw("e HIST SAME")
        leg.AddEntry(A[i], AN[i]                           , "L");

    leg.SetBorderSize(0)
    leg.SetTextFont(42)
    leg.SetTextSize(0.04)
    leg.Draw("same")

    canvas.Print(R+"_"+I +".png")
    del canvas
    gc.collect()

variables = [
	"global_cuts",
	"tight_electron_cuts",
	"2los_cuts",
	"2lss_cuts"
]

g_cuts = [
	'trigger1',
	'trigger2',
	'nElectron>1'
]

te_cuts = [
	'pt>10',
	'abs(eta)<2.5',
	'abs(dxy)<0.05',
	'abs(dz)<0.01',
	'sip3d<0.4pt',
	'lostHits=0',
	'mvaFall17V1noIso',
	'conVeto',
	'sieie',
	'hoe<0.10',
	'eInvMinusPInv>-0.04',
	'nearbyJetDeepCSV<0.4941',
	'mvaTTH>0.90',
]

ll_cuts = [
	'nElectron=2',
	'sign_condition',
	'pt_condition'
] 

cutNames = [g_cuts, te_cuts, ll_cuts, ll_cuts]

HistAddress = '/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/hists/'


colors =  [ROOT.kBlack,ROOT.kYellow,ROOT.kGreen,ROOT.kBlue-3,ROOT.kRed-4,ROOT.kOrange-3, ROOT.kOrange-6, ROOT.kCyan-6,ROOT.kViolet, ROOT.kBlue-9, ROOT.kYellow-2]

DY = ROOT.TFile.Open(HistAddress + 'UL17_DY.root')
data = ROOT.TFile.Open(HistAddress + 'DoubleEG.root')

for numvar, namevar in enumerate(variables):
    HH=[]
    HHName=[]
    h1 = DY.Get(namevar).Clone()
    h2 = data.Get(namevar).Clone()
#    h1.Scale(1/h1.GetBinContent(1))
#    h2.Scale(1/h2.GetBinContent(1))
    HH.extend([h1,h2])
    HHName.extend(['1) DY10','2) DoubleEG'])  
    name = ''
    drawHistallW(HH,HHName, namevar, name)
    print('%s \t \t DY \t Data' %(namevar)) 
    for i, namecut in enumerate(cutNames[numvar]):
	print('%s \t \t %.3g \t %.3g' %(namecut, h1.GetBinContent(i+2), h2.GetBinContent(i+2)))
    print('\n')
	
