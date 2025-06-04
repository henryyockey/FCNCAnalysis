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
from StackPlotsBDT import stackPlotsBDT
TGaxis.SetMaxDigits(2)

f = ROOT.TFile.Open("~/FCNC/postLHE_analysis/ANoutput.root")

#particles = ["Top1", "Top2", "W", "Z"]
#variables = ["Mass", "Pt", "Eta"]

particles = ["T","H", "WT", "W1", "W2"]
variables = ["Mass"]#, "Pt", "Phi", "Eta", "CosDecayAngle"]

for part in particles:
   for var in variables:
      print("Gen"+part+var)
#      genPart = f.Get("genPart_"+part+"_"+var)    
      genJigsaw = f.Get("genJigsaw_"+part+"_"+var)
      recoJigsaw = f.Get("recoJigsaw_"+part+"_"+var)

      canvas = ROOT.TCanvas("reco"+part+var, "RJR for gen and reco level", 800,600)
  
      myBlue = ROOT.TColor.GetColor("#004488")
      myYellow =  ROOT.TColor.GetColor("#DDAA33")
      myBlack = ROOT.TColor.GetColor("#000000")

#      genPart.SetLineStyle(1)
#      genPart.SetLineWidth(3)
#      genPart.SetLineColor(myBlack)
#      genPart.Draw("same hist")

      ymax=0
      for h in [genJigsaw, recoJigsaw]:
         if h.GetMaximum()>ymax:
            ymax=h.GetMaximum()

      genJigsaw.GetYaxis().SetRangeUser(0,ymax)


      genJigsaw.SetFillStyle(1001)
      genJigsaw.SetFillColorAlpha(myYellow, 0.5)
#      genJigsaw.Scale(1.0/genJigsaw.Integral())
      genJigsaw.Draw("same HIST")

      recoJigsaw.SetFillStyle(1001)
      recoJigsaw.SetFillColorAlpha(myBlue, 0.5)
#      recoJigsaw.Scale(1.0/recoJigsaw.Integral())
      recoJigsaw.Draw("same HIST")
      
      
      legend = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
      legend.AddEntry(genJigsaw, "Gen Level Jigsaw", "f")
      legend.AddEntry(recoJigsaw, "Reco Level Jigsaw", "f")
#      legend.AddEntry(genPart, "Gen level objet", "l")
      legend.Draw()
      
#      canvas.SetLogx()

      canvas.Update()
      canvas.SaveAs("~/FCNC/postLHE_analysis/plot/jigsaw/reco"+part+var+".png")

#f.close()
