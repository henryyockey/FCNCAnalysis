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



channels = [
#      "EpEm", "EpmMUmp", "MUpMUm",
      "LLpp", "LLmm", #"LFpp", "LFmm", "FFpp", "FFmm",
      "3LonZ", "3LoffZp", "3LoffZm","4L",
#      "LLFonZ", "LFFonZ","FFFonZ",
#      "LLFoffZp", "LFFoffZp","FFFoffZp",
#      "LLFoffZm", "LFFoffZm","FFFoffZm"

]


HistAddress = '/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/hists/'

background = ROOT.TFile.Open(HistAddress + "background.root")
production = ROOT.TFile.Open(HistAddress + "FCNC-Production.root")
decay      = ROOT.TFile.Open(HistAddress + "FCNC-Decay.root")

wcstr = ['cpQM', 'cpt', 'ctZ', 'cQlM', 'cQe', 'ctl', 'cte', 'ctlS', 'ctlT', 'ctA', 'ctG', 'ctp']
wcval = [     2,     2,   0.5,    1.5,   1.5,   1.5,     2,    2.5,    0.7,     1,   0.2,     1]


wclist = [ROOT.WCPoint(ReweightString(st,val)) for st, val in zip(wcstr, wcval)]

frac = np.zeros((len(channels), len(wclist)+1))
for i, ch in enumerate(channels):
   hBG    = background.Get(channels[i] + '_1Bjet_lep1Eta')
   IBG = hBG.Integral()
   frac[i][0]=IBG
   for j, wc in enumerate(wclist): 
      hProd  = decay.Get(channels[i] + '_1Bjet_lep1Eta')
      hProd.Scale(wc)
      IProd = hProd.Integral()
      frac[i][j+1] = IProd
      
for j in range(len(wclist)+1):
   N = sum(frac[:,j])
   for i in range(len(channels)):
      frac[i][j]*=(1/N)

canvas = ROOT.TCanvas("test", "test",50,50,865,780)
      
hs = THStack("", "")
legend = ROOT.TLegend(0.7,0.75,0.9,0.95)
legend.SetBorderSize(0)
legend.SetTextFont(42)
legend.SetTextSize(0.03)

for i in range(len(channels)):
   h = ROOT.TH1F(channels[i], channels[i], len(wclist)+1, 0, len(wclist)+1)
   for j in range(len(wclist)+1):
      h.SetBinContent(j+1, frac[i][j])
      h.GetXaxis().SetBinLabel(j+1, "bg" if j==0 else wcstr[j-1])
   h.SetFillColor(i+2)
   h.SetLineColor(i+2)
   legend.AddEntry(h,channels[i])
   hs.Add(h)
hs.SetMaximum(1.3)
hs.Draw()
#canvas.BuildLegend()
legend.Draw("SAME")
canvas.Print("decay.png") 























