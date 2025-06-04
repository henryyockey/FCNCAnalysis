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


# Lists for channels, regions, and variables
channels = ["3LoffZp", "3LoffZm"]  # Add your actual channel names
regions = ["1Bjet"]  # Add your actual region names
variables = ["BDTOutput"]  # Add your actual variable names

# Lists for samples
background_samples = [
"UL17_Conversion.root", "UL17_Diboson.root", "UL17_TTX.root", "UL17_Triboson.root",
"UL17_wJets.root", "UL17_DY.root", "UL17_SingleTop.root", "UL17_ttbar.root"

]  # Add actual background files
signal_samples = ["FCNC-Production.root", "FCNC-Decay.root"]  # Add actual signal files

# Open all background and signal ROOT files and store them in a dictionary
dist = "/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/hists/"
background_files = {sample: ROOT.TFile.Open(dist+sample) for sample in background_samples}
signal_files = {sample: ROOT.TFile.Open(dist+sample) for sample in signal_samples}

for ch in channels:
    for reg in regions:
        for var in variables:
            # Initialize a fresh canvas for each iteration
            canvas = ROOT.TCanvas("canvas", "Stacked Histograms", 800, 600)
            canvas.Clear()

            # Create a stack for the background histograms
            stack = ROOT.THStack("stack", "{}_{}_{} Background Stack".format(ch, reg, var))
            legend = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)

            # Loop over background files to get, scale, and stack histograms
            for idx, (bg_sample, bg_file) in enumerate(background_files.items()):
                hist_name = "{}_{}_{}".format(ch, reg, var)
                hist = bg_file.Get(hist_name)

                if not hist:
                    print("Warning: Histogram '{}' not found in '{}'".format(hist_name, bg_sample))
                    continue  # Skip if histogram is missing

                hist_clone = hist.Clone()
                hist_clone.SetDirectory(0)

                hist_clone.SetFillColor(ROOT.kAzure - idx)
                stack.Add(hist_clone)

                # Add entry to legend
                legend.AddEntry(hist_clone, bg_sample, "f")

            # Draw the stack on the primary Y-axis
            stack.Draw("HIST")
            stack.GetXaxis().SetTitle(var)
            stack.GetYaxis().SetTitle("Events (normalized)")

            # Create a secondary Y-axis for the signal histograms
            max_y = stack.GetMaximum()  # Get the maximum Y-value from the stack
            max_signal_y = 0  # Variable to track maximum Y for the signal histograms

            # Loop over signal files to get and draw the signal histograms on the second Y-axis
            for sig_sample, sig_file in signal_files.items():
                hist_name = "{}_{}_{}".format(ch, reg, var)
                sig_hist = sig_file.Get(hist_name)

                if not sig_hist:
                    print("Warning: Signal histogram '{}' not found in '{}'".format(hist_name, sig_sample))
                    continue

                sig_hist_clone = sig_hist.Clone()
                sig_hist_clone.SetDirectory(0)

                # Find the maximum Y value for signal histograms
                max_signal_y = max(max_signal_y, sig_hist_clone.GetMaximum())

                sig_hist_clone.SetLineColor(ROOT.kRed)
                sig_hist_clone.SetLineWidth(2)
                sig_hist_clone.SetLineStyle(2)
                sig_hist_clone.Draw("HIST SAME")

                # Add entry to legend
                legend.AddEntry(sig_hist_clone, sig_sample, "l")

            # Adjust the secondary Y-axis (right axis) based on the maximum of the signal histograms
            max_y = max(max_y, max_signal_y)

            # Create a secondary axis using TGaxis
            axis2 = ROOT.TGaxis(stack.GetXaxis().GetXmin(), 0, stack.GetXaxis().GetXmax(), 0, 0, max_signal_y, 510, "")
            axis2.SetLineColor(ROOT.kRed)
            axis2.SetLabelColor(ROOT.kRed)
            axis2.SetTitle("Signal Events")
            axis2.SetTitleColor(ROOT.kRed)
            axis2.Draw()

            # Draw the legend
            legend.Draw()

            # Save canvas
            canvas.Update()
            canvas.SaveAs("{}_{}_{}_stacked.png".format(ch, reg, var))


