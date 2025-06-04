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


def ReweightString(WC, value):
   wclist = ['cpQM', 'cpt', 'ctA', 'ctZ', 'ctG', 'cQlM', 'cQe', 'ctl', 'cte', 'ctlS', 'ctlT', 'ctp']
   text = 'EFTrwgt4'
   for wc in wclist:
      if wc == WC: text += '_' + wc + '_' + str(value) 
      else: text += '_' + wc + '_0.0'

   return text

def Normalize(hEFT, wc):
   I = 0
   for i in range(1, hEFT.GetNbinsX()+1):
      I+=hEFT.GetBinContent(i, wc)
   if I==0: I=1
   for i in range(1, hEFT.GetNbinsX()+1):
      BC = hEFT.GetBinContent(i, wc)
      hEFT.SetBinContent(i, BC/I)
      err = hEFT.GetBinError(i)
      hEFT.SetBinError(i, err/I)
   hEFT.SetBinContent(hEFT.GetNbinsX()+1,0)
   hEFT.SetBinError(hEFT.GetNbinsX()+1, 0)
   hEFT.SetBinContent(0,0)
   hEFT.SetBinError(0,0)

   return hEFT

def drawHist_ch_reg_var_group(hBG, hEFT, signal, ch, reg, var):
   wcgroups = [['ctZ', 'ctp', 'cpQM', 'cpt'], ['cQlM', 'cQe','ctlS', 'ctlT', 'cte', 'ctl'], ['ctA', 'ctG']]
   wcRef    = [[   1 ,    2 ,     2 ,    2 ], [    2 ,    2 ,    2 ,     1 ,    2 ,    2 ], [   2 ,  0.8 ]] 
   norm=1/hBG.Integral()
   hBG = Normalize(hBG, ROOT.WCPoint("smpt")) 
   for i, group in enumerate(wcgroups):
      hNames = ['background']
      hlist = [hBG]
      for j, wcName in enumerate(group):
         text = ReweightString(wcName, wcRef[i][j])
         wc1 = ROOT.WCPoint(text)
         h = hEFT.Clone()
         h.Scale(wc1)
         h = Normalize(h,wc1)
         hlist.append(h)
         hNames.append(wcName)
      path = signal + '/' + ch + '/' + reg + '/' + 'group'+str(i) + '/'
      name = var
      drawHistallW(hlist, hNames,path,name )


def drawHist_ch_reg_var(hBG, hEFT, signal, ch, reg, var):
   wclist = ['cpQM', 'cpt', 'ctA', 'ctZ', 'ctG', 'cQlM', 'cQe', 'ctl', 'cte', 'ctlS', 'ctlT', 'ctp']
   wcRef  = [     2,     2,     1,   0.5,   0.2,    1.5,   1.5,   1.5,     2,    2.5,    0.7,     1]
   hNames = ['background', 'cpQM', 'cpt', 'ctA', 'ctZ', 'ctG', 'cQlM', 'cQe', 'ctl', 'cte', 'ctlS', 'ctlT', 'ctp']
   hlist = [hBG]
   for wcName, x in zip(wclist, wcRef):
      text = ReweightString(wcName, x)
      wc1 = ROOT.WCPoint(text)
      h = hEFT.Clone()
      h.Scale(wc1)
      hlist.append(h)
   path = 'output/test/'
   
   name = signal+'_'+ch+'_'+reg+'_'+var
   drawHistallW(hlist, hNames,path,name )

def drawHistallW(A,AN,path, name):
    if not os.path.exists(path):
       os.makedirs(path)
    

    canvas = ROOT.TCanvas(name, name,10,10,1100,628)
    canvas.cd()

    pad_name = "pad"
    pad1=ROOT.TPad(pad_name, pad_name, 0, 0, 1, 1 , 0)
    pad1.Draw()
    pad1.cd()
#    pad1.SetLogy()
    A[0].SetTitle(path)
    A[0].GetXaxis().SetTitle(name)
    A[0].GetYaxis().SetTitle('Fraction')

    maxi=A[0].GetMaximum()
    mini=A[0].GetMinimum()
    for i in range(len(A)):
        A[i].SetLineColor(i+1)
        if A[i].GetMaximum()>maxi:
           maxi=A[i].GetMaximum()
#        if A[i].GetMinimum()<mini:
#            mini=A[i].GetMinimum()
#        if i==9:
#            A[i].SetMarkerColor(12);

    A[0].SetMinimum(mini);
    A[0].SetMaximum(maxi);

    leg = ROOT.TLegend(0.65,0.5,0.88,0.88)

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

    canvas.Print(path+name +".png")
    del canvas
    gc.collect()




regions = [
    "0Bjet","1Bjet", "G1Bjet", "All"
]


channels = [
#      "EpEm", "EpmMUmp", "MUpMUm",
      "LLpp", "LLmm", #"LFpp", "LFmm", "FFpp", "FFmm",
      "3LonZ", "3LoffZp", "3LoffZm","4L",
#      "LLFonZ", "LFFonZ","FFFonZ",
#      "LLFoffZp", "LFFoffZp","FFFoffZp",
#      "LLFoffZm", "LFFoffZm","FFFoffZm"

]

variables = ["lep1Pt", "lep1Eta","lep1Phi","lep2Pt","lep2Eta","lep2Phi","llM","llPt","llDr","llDphi","jet1Pt","jet1Eta","jet1Phi", "njet",
"nbjet","Met","MetPhi","nVtx", "llMZw","lep1Pt200" ]
variablesName = ["lep1Pt", "lep1Eta","lep1Phi","lep2Pt","lep2Eta","lep2Phi","llM","llPt","llDr","llDphi","jet1Pt","jet1Eta","jet1Phi", "njet",      "nbjet","Met","MetPhi","nVtx", "llMZw","lep1Pt200" ]


HistAddress = '/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/hists/'


colors =  [
ROOT.kBlack,
ROOT.kGreen,
ROOT.kBlue+8,
ROOT.kRed-4,
ROOT.kOrange-3, 
ROOT.kCyan+1, 
ROOT.kYellow, 
ROOT.kMagenta-4,
ROOT.kYellow+2,
ROOT.kViolet, 
ROOT.kGray+1,
ROOT.kGray+3
]

year=['output']

background = ROOT.TFile.Open(HistAddress + "background.root")
production = ROOT.TFile.Open(HistAddress + "FCNC-Production.root")
decay      = ROOT.TFile.Open(HistAddress + "FCNC-Decay.root")

Hists = []

for i, ch in enumerate(channels):
   for j, reg in enumerate(regions):
      for k, var in enumerate(variables):
         if i!=0 or j!=0 or k!=0: continue
         hBG    = background.Get(channels[i] + '_' + regions[j] + '_' + variables[k])
         hProd  = production.Get(channels[i] + '_' + regions[j] + '_' + variables[k])
         hDecay =      decay.Get(channels[i] + '_' + regions[j] + '_' + variables[k])
         drawHist_ch_reg_var_group(hBG, hProd, 'production', channels[i], regions[j], variables[k])
         drawHist_ch_reg_var_group(hBG, hDecay, 'decay', channels[i], regions[j], variables[k])
#
#for numyear, nameyear in enumerate(year):
#    l0=[]
#    Files = []
#    for f in range(len(Samples)):
#        l1=[]
#        Files.append(ROOT.TFile.Open(HistAddress  + Samples[f]))
#        print HistAddress + Samples[f]
#        for numch, namech in enumerate(channels):
#            l2=[]
#            for numreg, namereg in enumerate(regions):
#                l3=[]
#                for numvar, namevar in enumerate(variables):
#                    h= Files[f].Get(namech + '_' + namereg + '_' + namevar)
#                    h.SetFillColor(colors[f])
#                    h.SetLineColor(colors[f])
#                    l3.append(h)
#                l2.append(l3)
#            l1.append(l2)
#        l0.append(l1)
#    Hists.append(l0)


#for numyear, nameyear in enumerate(year):
#    for numch, namech in enumerate(channels):
#        for numreg, namereg in enumerate(regions):
#            for numvar, namevar in enumerate(variables):
#                HH=[]  
#		HHsignal=[]              
#                for f in range(len(Samples)):
##                    HH.append(Hists[numyear][f][numch][numreg][numvar])
#                    if 'FCNC' in Samples[f]:
#                        text = 'EFTrwgt4_cpQM_1.0_cpt_1.0_ctA_1.0_ctZ_0.5_ctG_0.1_cQlM_1.0_cQe_1.0_ctl_1.0_cte_1.0_ctlS_1.0_ctlT_0.05_ctp_1.0'
#                        wc1 = ROOT.WCPoint(text)
#                        Hists[numyear][f][numch][numreg][numvar].Scale(wc1)
#                        HHsignal.append(Hists[numyear][f][numch][numreg][numvar])
#                    else:
#                        HH.append(Hists[numyear][f][numch][numreg][numvar])
#
#
#                stackPlots(HH, HHsignal, SamplesName, namech, namereg, nameyear,namevar,variablesName[numvar])
	#	#drawHistallW(HH, SamplesName, namech + '_' + namereg + '_' + namevar, '') 
