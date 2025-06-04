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
from StackPlots import stackPlots
TGaxis.SetMaxDigits(2)

from array import array
from ROOT import TColor

def set_custom_color(hist,i):
    # Define a list of distinct colors (you can add more if needed)
    custom_colors = [
        ROOT.kRed, ROOT.kBlue, ROOT.kGreen + 3, ROOT.kOrange, ROOT.kViolet, ROOT.kMagenta,
        ROOT.kCyan,  ROOT.kYellow, ROOT.kAzure + 7
    ]

    # Loop through the histograms in the stack and assign colors
    
    color = custom_colors[i % len(custom_colors)]  # Cycle through the list of custom colors
    hist.SetFillColor(color)
    hist.SetLineColor(ROOT.kBlack)  # Optionally, set a black border
    hist.SetLineWidth(1)  # Set a thin border line width

def set_custom_colors(stack):
    # Define a list of distinct colors (you can add more if needed)
    custom_colors = [
        ROOT.kRed, ROOT.kBlue, ROOT.kGreen + 3, ROOT.kOrange, ROOT.kViolet, ROOT.kMagenta,
        ROOT.kCyan,  ROOT.kYellow, ROOT.kAzure + 7
    ]

    # Loop through the histograms in the stack and assign colors
    for idx, hist in enumerate(stack.GetHists()):
        color = custom_colors[idx % len(custom_colors)]  # Cycle through the list of custom colors
        hist.SetFillColor(color)
        hist.SetLineColor(ROOT.kBlack)  # Optionally, set a black border
        hist.SetLineWidth(1)  # Set a thin border line width


def EFTtoTH1F(H, wc):
    hpx = ROOT.TH1F( "TH1F_{}".format(H.GetName()), "TH1F_{}".format(H.GetName()), H.GetXaxis().GetNbins(), H.GetXaxis().GetXmin(),H.GetXaxis().GetXmax() )
    r = 1
    for b in range(hpx.GetNbinsX()):
        if H.GetBinContent(b+1,ROOT.WCPoint("NONE"))>0:
            r = H.GetBinError(b+1)/H.GetBinContent(b+1,ROOT.WCPoint("NONE"))
        hpx.SetBinContent(b+1, H.GetBinContent(b+1,wc))
        hpx.SetBinError(b+1, r*H.GetBinContent(b+1,wc))
    return hpx

def normalize_signal(hist, wc):
    h = EFTtoTH1F(hist, wc)
    i = h.Integral()
    if i>0: h.Scale(1/i)
    return h

def normalize_stack(stack):
    
    total_integral = sum(hist.Integral() for hist in stack.GetHists())
    new_stack = ROOT.THStack(stack.GetName() + "_TH1F", stack.GetTitle() + " (TH1F)")

    for hist in stack.GetHists():
        converted_hist = EFTtoTH1F(hist, ROOT.WCPoint("NONE"))
        new_stack.Add(converted_hist)


    if total_integral > 0:
        for hist in new_stack.GetHists():
            hist.Scale(1.0 / total_integral)
    set_custom_colors(new_stack)
    return new_stack

WC = ROOT.WCPoint("EFTRwgt4_cQe_1_cQlM_1_cte_1_ctl_1_ctlT_1_ctlS_1")

# Lists for channels, regions, and variables
channels = ["3LoffZp", "3LoffZm"]  # Add your actual channel names
regions = ["1Bjet"]  # Add your actual region names
variables = ["BDTOutput"]  # Add your actual variable names

# Lists for samples
background_samples = ["UL17_Diboson.root","UL17_TTX.root", "Fakes.root", "Other.root"]
#"UL17_Conversion.root", "UL17_Diboson.root", "UL17_TTX.root", "UL17_Triboson.root", 
#"UL17_wJets.root", "UL17_DY.root", "UL17_SingleTop.root", "UL17_ttbar.root",
#"Fakes.root"
#]  # Add actual background files
signal_samples = ["FCNC-Production.root"]#, "FCNC-Decay.root"]  # Add actual signal files
data_samples = ["Data.root"]

# Open all background and signal ROOT files and store them in a dictionary
dist = "/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/hists/"
background_files = {sample: ROOT.TFile.Open(dist+sample) for sample in background_samples}
signal_files = {sample: ROOT.TFile.Open(dist+sample) for sample in signal_samples}
data_files = {sample: ROOT.TFile.Open(dist+sample) for sample in data_samples}

for ch in channels:
    for reg in regions:
        for var in variables:
            # Initialize a fresh canvas for each iteration
            canvas = ROOT.TCanvas("canvas", "Stacked Histograms", 800, 600)
            canvas.Clear()
            
            # Create a stack for the background histograms
            stack = ROOT.THStack("stack", "2017")
            legend = ROOT.TLegend(0.35, 0.65, 0.65, 0.85) 
            
            # Loop over background files to get, scale, and stack histograms
            for idx, (bg_sample, bg_file) in enumerate(background_files.items()):
                hist_name = "{}_{}_{}".format(ch, reg, var)
                
                hist = bg_file.Get(hist_name)
                
                if not hist:
                    print("Warning: Histogram '{}' not found in '{}'".format(hist_name, bg_sample))
                    continue  # Skip if histogram is missing
                
                hist_clone = hist.Clone()
                hist_clone.SetDirectory(0)
                set_custom_color(hist_clone, idx)                
                stack.Add(hist_clone)
                
                # Add entry to legend
                legend.AddEntry(hist_clone, bg_sample, "f")

            # Draw the stack
            total_background_integral = sum(hist.Integral() for hist in stack.GetHists())
            print("Integral: {}".format(total_background_integral))
#            stack = normalize_stack(stack)
            stack.Draw("HIST")
            stack.GetXaxis().SetTitle(var)
            stack.GetYaxis().SetTitle("Events")

            stack.SetMaximum(total_background_integral)  # A little above 1 for the top of the stack (10% more space)
            stack.SetMinimum(0)    # Set minimum to 0
            # Loop over signal files to get, scale, and draw histograms
            for sig_sample, sig_file in signal_files.items():
                hist_name = "{}_{}_{}".format(ch, reg, var)
                
                sig_hist = sig_file.Get(hist_name)
                
                if not sig_hist:
                    print("Warning: Signal histogram '{}' not found in '{}'".format(hist_name, sig_sample))
                    continue
                
                sig_hist_clone = sig_hist.Clone()
                sig_hist_clone.SetDirectory(0)
                
                sig_hist_clone = normalize_signal(sig_hist_clone, WC)                
                sig_hist_clone.Scale(total_background_integral)
                sig_hist_clone.SetLineColor(ROOT.kGray)
                sig_hist_clone.SetLineWidth(2)
                sig_hist_clone.SetLineStyle(2)
                sig_hist_clone.Draw("HIST SAME")
                legend.AddEntry(sig_hist_clone, sig_sample, "l")
            
            for data_sample, data_file in data_files.items():
                hist_name = "{}_{}_{}".format(ch, reg, var)

                data_hist = data_file.Get(hist_name)
                print("Data: {}".format(data_hist.Integral()))
                if not data_hist:
                    print("Warning: Data histogram '{}' not found in '{}'".format(hist_name, data_sample))
                    continue

                data_hist_clone = data_hist.Clone()
                data_hist_clone.SetDirectory(0)

                # Normalize data by the total integral of the background stack
#                if total_background_integral > 0:
#                    data_hist_clone = EFTtoTH1F(data_hist_clone, ROOT.WCPoint("NONE"))
#                    data_hist_clone.Scale(1/total_background_integral)
                # Set markers for data
                data_hist_clone.SetMarkerStyle(20)  # Solid circle markers
                data_hist_clone.SetMarkerSize(1.0)  # Marker size
                data_hist_clone.SetMarkerColor(ROOT.kBlack)  # Black color for data
                data_hist_clone.Draw("PE SAME")  # 'PE' means markers with error bars
                legend.AddEntry(data_hist_clone, data_sample, "p")  # "p" for point in legend

            legend.Draw()
            canvas.Update()
            canvas.SaveAs("{}_{}_{}_stacked.png".format(ch, reg, var))



# Close all opened ROOT files
for file in background_files.values():
    file.Close()
for file in signal_files.values():
    file.Close()
for file in data_files.values():
    file.Close()
