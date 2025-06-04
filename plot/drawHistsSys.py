import ROOT
import string
import gc
import numpy as np
ROOT.gStyle.SetOptStat(0)
def plot_histogram_ratios(hist_nominal, hist_up, hist_down, name):
    """
    Parameters:
    hist_nominal: ROOT.TH1F
        The nominal histogram (denominator).
    hist_up: ROOT.TH1F
        The histogram for the 'up' variation (numerator for the first ratio).
    hist_down: ROOT.TH1F
        The histogram for the 'down' variation (numerator for the second ratio).
    """

    ROOT.gROOT.SetBatch(True)

    output_file = "{}.png".format(name)
    canvas_name = "canvas"

    # Clone histograms to avoid modifying the originals
    ratio_up_over_nominal = hist_up.Clone('ratio_up_over_nominal')
    ratio_down_over_nominal = hist_down.Clone('ratio_down_over_nominal')

    # Divide histograms
    ratio_up_over_nominal.Divide(hist_nominal)
    ratio_down_over_nominal.Divide(hist_nominal)

    # Subtract 1 to center the ratio around zero
#    ratio_up_over_nominal.Add(ratio_up_over_nominal, -1)
#    ratio_down_over_nominal.Add(ratio_down_over_nominal, -1)

    # Create a canvas
    canvas = ROOT.TCanvas(canvas_name, canvas_name, 800, 600)

    # Draw the first histogram (up variation) as bars with customization
    ratio_up_over_nominal.SetLineColor(ROOT.kRed)
    ratio_up_over_nominal.SetFillColor(ROOT.kRed)  # Set the fill color
#    ratio_up_over_nominal.SetFillStyle(3004)  # Set fill style to transparent red bars
    ratio_up_over_nominal.SetTitle("Systematics")
    ratio_up_over_nominal.Draw("")

    # Draw the second histogram (down variation) as bars on the same canvas
    ratio_down_over_nominal.SetLineColor(ROOT.kBlue)
    ratio_down_over_nominal.SetFillColor(ROOT.kBlue)  # Set the fill color
#    ratio_down_over_nominal.SetFillStyle(3005)  # Set fill style to transparent blue bars
    ratio_down_over_nominal.Draw("SAME")

    x_min = ratio_up_over_nominal.GetXaxis().GetXmin()
    x_max = ratio_up_over_nominal.GetXaxis().GetXmax()
    line = ROOT.TLine(x_min, 1, x_max, 1)
    line.SetLineColor(ROOT.kBlack)  # Color of the line
    line.SetLineStyle(ROOT.kDashed) # Style of the line (dashed)
    line.SetLineWidth(2)           # Width of the line
    line.Draw("EY0 SAME")

    # Add a legend
    legend = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
    legend.AddEntry(ratio_up_over_nominal, "SysUp", "f")  # Use "f" for filled
    legend.AddEntry(ratio_down_over_nominal, "SysDown", "f")  # Use "f" for filled
    legend.Draw()

    # Save the canvas to a PNG file
    canvas.Print("systematics/" + output_file)

    # Update and show the canvas
    del canvas
    gc.collect()


channels = [
   "LLpp", 
#   "LLmm", 
   "3LonZ", 
   "3LoffZp", 
#   "3LoffZm"
]
regions = [
#   "0Bjet",
#   "1Bjet", 
   "G1Bjet"
]
vars = ["lep1Pt", 
#        "lep1Eta",
#        "lep1Phi",
#        "lep2Pt",
#        "lep2Eta",
#        "lep2Phi",
#        "llM", 
#        "llPt",
#        "llDr",
#        "llDphi",
        "jet1Pt",
#        "jet1Eta",
#        "jet1Phi",
        "njet",  
        "nbjet",
#        "Met", 
#        "MetPhi",
        "nVtx", 
#        "llMZw"
]
sys = ["pu", "muIdIsoSf", "eleIDSf", "eleRecoSf", "prefiring", "bcTagSf", "bcTagSfUnCorr", "LTagSf", "LTagSfUnCorr" ]

fName = "../ANoutput.root"

f = ROOT.TFile(fName, "READ")


for s in sys:
    for ch in channels:
        for reg in regions:
            for v in vars:
         
                Nnom = "{}_{}_{}".format(ch,reg,v)
                Nup  = "SysUp_{}_{}_{}_{}".format(s,ch,reg,v)
                Ndown= "SysDown_{}_{}_{}_{}".format(s,ch,reg,v)
                print(Nnom)
                hnom = f.Get(Nnom)
                hup  = f.Get(Nup)
                hdown= f.Get(Ndown)

                
                plot_histogram_ratios(hnom, hup, hdown,"{}_{}_{}_{}".format(s,ch,reg,v) )


