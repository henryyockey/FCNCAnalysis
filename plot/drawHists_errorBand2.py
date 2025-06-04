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
from ROOT import WCPoint
import gc
from operator import truediv
import copy
TGaxis.SetMaxDigits(2)

def EFTtoTH1F(H, wc):
    hpx = ROOT.TH1F( "TH1F_{}".format(H.GetName()), "TH1F_{}".format(H.GetName()), H.GetXaxis().GetNbins(), H.GetXaxis().GetXmin(),H.GetXaxis().GetXmax() )
    r = 1
    for b in range(hpx.GetNbinsX()):
        if H.GetBinContent(b+1,ROOT.WCPoint("NONE"))>0:
            r = H.GetBinError(b+1)/H.GetBinContent(b+1,ROOT.WCPoint("NONE"))
        hpx.SetBinContent(b+1, H.GetBinContent(b+1,wc))
        hpx.SetBinError(b+1, r*H.GetBinContent(b+1,wc))
    return hpx


f = ROOT.TFile.Open("../hists/FCNC-Production.root")
h_nominal = f.Get("3LoffZp_1Bjet_BDTOutput")
h_error_up = f.Get("3LoffZp_1Bjet_BDTOutput_puUp")
h_error_down = f.Get("3LoffZp_1Bjet_BDTOutput_puDown")

wc = ROOT.WCPoint("eft_cQe_0.1_cQlM_0.1_ctl_0.1_ctlT_0.1_ctlS_0.1_cte_0.1")#_ctG_0.1_ctp_0.1")

h_nominal = EFTtoTH1F(h_nominal, wc)
h_error_up = EFTtoTH1F(h_error_up, wc)
h_error_down = EFTtoTH1F(h_error_down, wc)

c = ROOT.TCanvas("c", "Nominal with Asymmetric Errors", 800, 600)

# Draw the nominal histogram as a line
h_nominal.SetLineColor(ROOT.kBlack)
h_nominal.SetLineWidth(2)
h_nominal.Draw("HIST")

# Create a TGraphAsymmErrors for the error bands
error_boxes=[]
# Loop through each bin to draw the error boxes
for i in range(1, h_nominal.GetNbinsX() + 1):
    x_center = h_nominal.GetBinCenter(i)
    x_width = h_nominal.GetBinWidth(i) / 2
    y_nominal = h_nominal.GetBinContent(i)
    y_error_up = h_error_up.GetBinContent(i)
    y_error_down = h_error_down.GetBinContent(i)
#    y_error_down = 2*y_nominal-y_error_up
#    print(y_nominal, y_error_up, y_error_down)

    # Create a box for the upward error
    box_up = ROOT.TBox(x_center - x_width, y_nominal, x_center + x_width, y_error_up)
    box_up.SetFillColorAlpha(ROOT.kRed, 0.7)  # Set color and transparency
    box_up.Draw("SAME")
    error_boxes.append(box_up)

    # Create a box for the downward error
    box_down = ROOT.TBox(x_center - x_width, y_error_down, x_center + x_width, y_nominal)
    box_down.SetFillColorAlpha(ROOT.kBlue, 0.7)  # Set color and transparency
    box_down.Draw("SAME")
    error_boxes.append(box_down)

# Optionally draw the nominal histogram again on top to emphasize the line
h_nominal.Draw("HIST SAME")

# Save the canvas directly to PDF
c.SaveAs("plot_with_asymmetric_error_boxes.pdf")
