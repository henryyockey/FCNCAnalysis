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



def add_systematic_uncertainties(upHists, downHists, nomHists):
    h = nomHists['UL17_DY']
    hup = h.Clone()
    hup.Reset()
    hdown = h.Clone()
    hdown.Reset()
    nbins = hup.GetNbinsX()
    for up, down, (name, nom) in zip(upHists.values(), downHists.values(),  nomHists.items()):
        # swap values for up and down if they are flipped
        if name == "Fakes": continue
        for i in range(1, nbins+1):
            cv = nom.GetBinContent(i, ROOT.WCPoint("NONE"))
            uv = up.GetBinContent(i, ROOT.WCPoint("NONE"))
            dv = down.GetBinContent(i, ROOT.WCPoint("NONE"))
            if(uv < cv and dv > cv):
                up.SetBinContent(i, dv)
                down.SetBinContent(i, uv)
            elif (uv < cv and dv < cv):
                up.SetBinContent(i, 0)
            elif (uv > cv and dv > cv):
                down.SetBinContent(i, 0)
        hup.Add(up)
        hdown.Add(down)

    return (hup, hdown)

def add_square_errors(systematics, nom, variation):
    nbins = nom.GetNbinsX()
    err = nom.Clone('error')
    for i in range(1, nbins+1):
        b = nom.GetBinError(i)**2
#        print("stat unc", nom.GetBinError(i))
        cv = nom.GetBinContent(i, ROOT.WCPoint("NONE"))
        for sys, name in zip(systematics,["pu", "muIdIsoSf", "eleIDSf", "eleRecoSf", "prefiring", "bcTagSf", "bcTagSfUnCorr", "LTagSf", "LTagSfUnCorr"]):
            b = b + (sys.GetBinContent(i, ROOT.WCPoint("NONE"))-cv)**2
#            if cv>0: print(name, variation, str((sys.GetBinContent(i, ROOT.WCPoint("NONE")))/cv))
        if variation == "up":
            err.SetBinContent(i, cv + np.sqrt(np.abs(b)))
        elif variation == "down":
            err.SetBinContent(i, cv - np.sqrt(np.abs(b)))

    return err

def NomTotal(nomHists, skip=[]):
    nomTotal = nomHists[next(iter(nomHists))].Clone()
    nomTotal.Reset()
    for key,nom in nomHists.items():
        if key in skip: continue
        nomTotal.Add(nom)
    return nomTotal

def plot_stack_with_systematics(nomHists, upper_hist, lower_hist):
    """
        nomHists (list of TH1D): List of nominal histograms to be stacked.
        upper_hist (TH1D): Upper limit of systematic uncertainties.
        lower_hist (TH1D): Lower limit of systematic uncertainties.
    """

    nominal_stack = ROOT.THStack("nominal_stack", "Stacked Nominal Histograms with Systematics")
    legend = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
    legend.SetBorderSize(0)
    legend.SetFillStyle(0)

    colors = [ROOT.kRed, ROOT.kBlue, ROOT.kGreen, ROOT.kOrange, ROOT.kMagenta, 
              ROOT.kCyan, ROOT.kYellow, ROOT.kPink, ROOT.kViolet, ROOT.kTeal]

    for i, (hist_name, hist) in enumerate(nomHists.items()):
        hist.SetFillColor(colors[i % len(colors)])
        hist.SetLineColor(ROOT.kBlack)
        hist.SetLineWidth(1)
        hist.SetFillStyle(1001)
        nominal_stack.Add(hist)
        legend.AddEntry(hist, hist_name, "f")

    nbins = upper_hist.GetNbinsX()
    error_band = ROOT.TGraphAsymmErrors(nbins)

    for i in range(1, nbins + 1):
        x_value = upper_hist.GetBinCenter(i)
        y_nominal = sum([h.GetBinContent(i, ROOT.WCPoint("NONE")) for h in nomHists.values()])
        y_upper = upper_hist.GetBinContent(i, ROOT.WCPoint("NONE"))
        y_lower = lower_hist.GetBinContent(i, ROOT.WCPoint("NONE"))
        error_up = y_upper - y_nominal
        error_down = y_nominal - y_lower
        error_band.SetPoint(i - 1, x_value, y_nominal)
        error_band.SetPointError(i - 1, 0, 0, error_down, error_up)

    error_band.SetFillColor(ROOT.kGray + 2)
    error_band.SetFillStyle(3001)

#    nominal_stack.Draw("HIST")
#    error_band.Draw("E3 SAME")
#    legend.Draw()

    drawables = []
#    drawables.append((nominal_stack, "HIST"))
    drawables.append((error_band, "E SAME"))
    drawables.append((legend, ""))

    return drawables

def plot_with_systematics(nominal_hist, upper_hist, lower_hist, path, name):
    nbins = nominal_hist.GetNbinsX()

    error_band = ROOT.TGraphAsymmErrors(nbins)

    # Loop over bins and calculate asymmetric errors
    for i in range(1, nbins+1):
        x_value = nominal_hist.GetBinCenter(i)
        y_nominal = nominal_hist.GetBinContent(i,ROOT.WCPoint("NONE"))
        y_upper = upper_hist.GetBinContent(i,ROOT.WCPoint("NONE"))
        y_lower = lower_hist.GetBinContent(i,ROOT.WCPoint("NONE"))
        error_up = y_upper - y_nominal
        error_down = y_nominal - y_lower
        error_band.SetPoint(i-1, x_value, y_nominal)
        error_band.SetPointError(i-1, 0, 0, error_down, error_up)

    error_band.SetFillColor(ROOT.kGray + 2)
    error_band.SetFillStyle(3001)

    canvas = ROOT.TCanvas("canvas", "Nominal with Systematics", 800, 600)

    nominal_hist.SetLineColor(ROOT.kBlue)
    nominal_hist.SetLineWidth(2)
    nominal_hist.Draw("HIST")

    error_band.Draw("E4 SAME")

    canvas.Update()

    canvas.SaveAs("{}.png".format(path+name))

def plot_ratio_with_systematics(nominal_hist, upper_hist, lower_hist):
    nbins = nominal_hist.GetNbinsX()

    # Create histograms for ratio plot: upper and lower
    ratio_upper = ROOT.TH1D("ratio_upper", "Upper/Lower Ratio", nbins, nominal_hist.GetXaxis().GetXmin(), nominal_hist.GetXaxis().GetXmax())
    ratio_lower = ROOT.TH1D("ratio_lower", "Upper/Lower Ratio", nbins, nominal_hist.GetXaxis().GetXmin(), nominal_hist.GetXaxis().GetXmax())

    for i in range(1, nbins + 1):
        y_nominal = nominal_hist.GetBinContent(i, ROOT.WCPoint("NONE"))
        y_upper = upper_hist.GetBinContent(i, ROOT.WCPoint("NONE"))
        y_lower = lower_hist.GetBinContent(i, ROOT.WCPoint("NONE"))

        if y_nominal > 0:
            # Upper ratio (above 1)
            ratio_up = (y_upper / y_nominal) - 1
            # Lower ratio (below 1)
            ratio_down = 1 - (y_lower / y_nominal)

            # Set bin content for the ratios
            ratio_upper.SetBinContent(i, ratio_up)
            ratio_lower.SetBinContent(i, -ratio_down)  # Invert the lower ratio to be below 0

    # Style the ratio plots
    ratio_upper.SetLineColor(ROOT.kRed)
    ratio_upper.SetMarkerColor(ROOT.kRed)
    ratio_upper.SetMarkerStyle(20)
    ratio_upper.SetMarkerSize(0.8)
    ratio_upper.GetYaxis().SetRangeUser(-.5, .5)

    ratio_lower.SetLineColor(ROOT.kBlue)
    ratio_lower.SetMarkerColor(ROOT.kBlue)
    ratio_lower.SetMarkerStyle(20)
    ratio_lower.SetMarkerSize(0.8)

    # Draw a line at y = 0 for reference
    zero_line = ROOT.TLine(nominal_hist.GetXaxis().GetXmin(), 0, nominal_hist.GetXaxis().GetXmax(), 0)
    zero_line.SetLineColor(ROOT.kBlack)
    zero_line.SetLineStyle(2)  # Dashed line

    drawables = []
    drawables.append((ratio_upper, 'E'))
    drawables.append((ratio_lower, 'E SAME'))
    drawables.append((zero_line, 'SAME'))    

    return drawables

def create_canvas_with_subpads():
    canvas = ROOT.TCanvas("canvas", "Systematics with Error", 800, 800)
    canvas.Divide(1, 2)  # 1 column, 2 rows

    # Get the pads
    pad1 = canvas.cd(1)
    pad1.SetPad(0, 0.315, 1, .99)  # Upper pad, larger (35% of canvas height for lower pad)

    pad2 = canvas.cd(2)
    pad2.SetPad(0, 0, 1, 0.305)  # Lower pad, smaller

    #pad2.SetTickx()
    #pad1.SetBottomMargin(0.1)
    #pad1.SetLeftMargin(0.01)
    #pad1.SetRightMargin(0.01)
    #pad2.SetBottomMargin(0.1)
    #pad2.SetLeftMargin(0.11)
    #pad2.SetRightMargin(0.1)
    #pad2.SetFillStyle(0)
    #pad1.SetFillStyle(0)
    #pad1.SetLogx(ROOT.kFALSE)
    #pad2.SetLogx(ROOT.kFALSE)
    #pad1.SetLogy(ROOT.kFALSE)

    return canvas, pad1, pad2

def save_canvas(canvas, channel, region, variable):
    path = "systematics/{}/{}".format(channel, region)
    if not os.path.exists(path):
        os.makedirs(path)
    canvas.SaveAs("{}/{}_{}_{}.png".format(path,channel,region,variable))
    canvas.Close()
    del canvas

def StackPlot_by_ch_reg_var(ch, reg, var, systematics, samples):
    upHists = defaultdict(dict)         #2d nested dict
    downHists = defaultdict(dict)
    nomHists = {}
    fs = lambda samp: "../hists/{}.root".format(samp)

    for sam in samples:
        f = ROOT.TFile.Open(fs(sam))
        nomHists[sam] = f.Get('{}_{}_{}'.format(ch, reg, var))
        if sam == 'Fakes': continue
        for sys in systematics:
            downHists[sys][sam] = f.Get('SysDown_{}_{}_{}_{}'.format(sys, ch, reg, var))
            upHists[sys][sam] = f.Get('SysUp_{}_{}_{}_{}'.format(sys, ch, reg, var))
    hup = []
    hdown = []
    nomHistsWOFakes = {key:value for key, value in nomHists.items() if not key == "Fakes"}
    for sys in systematics:
        up = NomTotal(upHists[sys])
        down = NomTotal(downHists[sys])
        hup.append(up)
        hdown.append(down)
#        added = add_systematic_uncertainties(upHists[sys],downHists[sys], nomHistsWOFakes)
#        hup.append(added[0])
#        hdown.append(added[1])

    nomTotal = NomTotal(nomHists, skip=["Fakes"])
    errUp = add_square_errors(hup, nomTotal, "up")
    errDown = add_square_errors(hdown, nomTotal, "down")

    canvas, pad1, pad2 = create_canvas_with_subpads()

    pad1.cd()
    stack_draw = plot_stack_with_systematics(nomHists, errUp, errDown)
    for drawable, option in stack_draw:
        drawable.Draw(option)
    canvas.Update()
   
    pad2.cd()
    ratio_draw = plot_ratio_with_systematics(nomTotal, errUp, errDown)
    for drawable, option in ratio_draw:
        drawable.Draw(option)

    save_canvas(canvas, ch, reg, var)

def Plot_Sys_Ratios(ch, reg, var, systematics, samples):
    upHists = defaultdict(dict)         #2d nested dict
    downHists = defaultdict(dict)
    nomHists = {}

    fs = lambda samp: "../hists/{}.root".format(samp)

    for sam in samples:
        f = ROOT.TFile.Open(fs(sam))
        nomHists[sam] = f.Get('{}_{}_{}'.format(ch, reg, var))
        if sam == 'Fakes': continue
        for sys in systematics:
            downHists[sys][sam] = f.Get('SysDown_{}_{}_{}_{}'.format(sys, ch, reg, var))
            upHists[sys][sam] = f.Get('SysUp_{}_{}_{}_{}'.format(sys, ch, reg, var))
    
    nomTotal = nomHists["UL17_DY"]
    canvas = ROOT.TCanvas("canvas")
    colors = [ROOT.kRed, ROOT.kBlue, ROOT.kGreen, ROOT.kOrange, ROOT.kMagenta,
              ROOT.kCyan, ROOT.kYellow, ROOT.kPink, ROOT.kViolet, ROOT.kTeal]

    hists=[]
    for samp, color in zip(samples,colors):
        print(sys)
        #added = add_systematic_uncertainties(upHists[sys],downHists[sys], nomHists)
        hup = upHists[sys]["UL17_DY"]
        hdown = downHists[sys]["UL17_DY"]
        for i in range(1, hup.GetNbinsX()):
            nomCon = nomTotal.GetBinContent(i,ROOT.WCPoint("NONE"))
            if nomCon == 0:
               hup.SetBinContent(i, 0)
               hdown.SetBinContent(i, 0)
            else :
               hup.SetBinContent(i, (hup.GetBinContent(i,ROOT.WCPoint("NONE"))-nomCon)/nomCon) 
               hdown.SetBinContent(i, (hdown.GetBinContent(i,ROOT.WCPoint("NONE"))-nomCon)/nomCon)
        hup.SetMinimum(-1)
        hup.SetMaximum(1)
        hup.SetLineColor(color)
        hdown.SetLineColor(color)
        hup.SetDirectory(0)  
        hdown.SetDirectory(0)
        hists.append(hup)
        hists.append(hdown) 

    for h in hists:
        h.Draw("HIST SAME")       

    canvas.SaveAs("systematics/ratios/sysRatio_{}_{}.png".format(ch,reg))
    canvas.Close()
    del canvas

def Plot_Sys_Ratios_bySample(ch, reg, var, systematics, samples):
    upHists = defaultdict(dict)         #2d nested dict
    downHists = defaultdict(dict)
    nomHists = {}

    fs = lambda samp: "../hists/{}.root".format(samp)

    for sam in samples:
        f = ROOT.TFile.Open(fs(sam))
        nomHists[sam] = f.Get('{}_{}_{}'.format(ch, reg, var))
        if sam == 'Fakes': continue
        for sys in systematics:
            downHists[sys][sam] = f.Get('SysDown_{}_{}_{}_{}'.format(sys, ch, reg, var))
            upHists[sys][sam] = f.Get('SysUp_{}_{}_{}_{}'.format(sys, ch, reg, var))
    hists=[]
    for samp in samples:
        if samp == "Fakes": continue
        nom = nomHists[samp]
        up = upHists["pu"][samp]
        down = downHists["pu"][samp]
        for i in range(1, up.GetNbinsX()):
            nomCon = nom.GetBinContent(i,ROOT.WCPoint("NONE"))
            if nomCon == 0:
               up.SetBinContent(i, 0)
               down.SetBinContent(i, 0)
            else :
               up.SetBinContent(i, (up.GetBinContent(i,ROOT.WCPoint("NONE"))-nomCon)/nomCon)
               down.SetBinContent(i, (down.GetBinContent(i,ROOT.WCPoint("NONE"))-nomCon)/nomCon)
        up.SetMinimum(-1)
        up.SetMaximum(1)
        up.SetDirectory(0)
        down.SetDirectory(0)
        canvas = ROOT.TCanvas("canvas")
        up.Draw("HIST")
        down.Draw("HIST SAME")
        canvas.SaveAs("systematics/ratios/sysRatio_{}_{}_{}.png".format(ch,reg, samp))
        canvas.Close()
        del canvas

def Plot_Sys_Ratios_test(ch, reg, var, systematics, samples):

    upHists = defaultdict(dict)         #2d nested dict
    downHists = defaultdict(dict)
    nomHists = {}

    fs = lambda samp: "../hists/{}.root".format(samp)

    for sam in samples:
        f = ROOT.TFile.Open(fs(sam))
        nomHists[sam] = f.Get('{}_{}_{}'.format(ch, reg, var))
        if sam == 'Fakes': continue
        for sys in systematics:
            downHists[sys][sam] = f.Get('SysDown_{}_{}_{}_{}'.format(sys, ch, reg, var))
            upHists[sys][sam] = f.Get('SysUp_{}_{}_{}_{}'.format(sys, ch, reg, var))

    colors = [ROOT.kRed, ROOT.kBlue, ROOT.kGreen, ROOT.kOrange, ROOT.kMagenta,
              ROOT.kCyan, ROOT.kYellow, ROOT.kPink, ROOT.kViolet, ROOT.kTeal]
    canvas = ROOT.TCanvas("test", "test")
    hists = []
    for j, sys in enumerate(systematics):
        upStack = upHists[sys]["UL17_DY"]
        downStack = downHists[sys]["UL17_DY"]
        nomStack = nomHists["UL17_DY"]
        upStack.Reset()
        downStack.Reset()
        nomStack.Reset()
        for sam in samples:
            if sam == "Fakes": continue
            up = upHists[sys][sam].Clone()
            down = downHists[sys][sam].Clone()
            nom = nomHists[sam].Clone()
            upStack.Add(up)
            downStack.Add(down)
            nomStack.Add(nom)

        upStack.Add(nomStack,-1)
        downStack.Add(nomStack,-1)
        for i in range(1, upStack.GetNbinsX()):
            if nomStack.GetBinContent(i, ROOT.WCPoint("None"))==0:
                upStack.SetBinContent(i,0)
                downStack.SetBinContent(i,0)
            else:
                upStack.SetBinContent(i, upStack.GetBinContent(i, ROOT.WCPoint("None"))/nomStack.GetBinContent(i, ROOT.WCPoint("None")))
                downStack.SetBinContent(i, downStack.GetBinContent(i, ROOT.WCPoint("None"))/nomStack.GetBinContent(i, ROOT.WCPoint("None")))
        upStack.SetLineColor(colors[i%len(colors)])
        upStack.SetMinimum(-0.5)
        upStack.SetMaximum(0.5)
        downStack.SetLineColor(colors[i%len(colors)])
        upStack.Draw("SAME")
        downStack.Draw("SAME")
    canvas.SaveAs("systematics/test/plot_{}_{}_{}.png".format(ch, reg, var))
    canvas.Close()
    del canvas
         


def main():
    year = "2017"
    systematics = ["pu", "muIdIsoSf", "eleIDSf", "eleRecoSf", "prefiring", "bcTagSf", "bcTagSfUnCorr", "LTagSf", "LTagSfUnCorr"]
    channels    = [#'LLpp', 'LLmm', '3LonZ', 
                  '3LoffZp', '3LoffZm']
    regions     = [#'0Bjet', 
                  '1Bjet', 
                  # 'G1Bjet'
                  ]
#    variables   = ["lep1Pt", "lep1Eta", "lep1Phi", "lep2Pt", "lep2Eta", "lep2Phi", "llM", "llPt", "llDr", "llDphi", "jet1Pt", "jet1Eta", "jet1Phi", "njet", "nbjet", "Met", "MetPhi", "nVtx"]
    variables = ["BDTOutput"]
    samples     = ['UL17_Conversion', 'UL17_Diboson', 'UL17_TTX', 'UL17_Triboson', 'UL17_wJets', 'UL17_DY', 'UL17_SingleTop', 'UL17_ttbar', 'Fakes']
   
    samples_dir = '../hists/'
    output_base_dir = 'plotsWithErrors'

    fs = lambda samp: "../hists/{}.root".format(samp)

#    StackPlot_by_ch_reg_var('LLpp', '1Bjet', 'njet',systematics, samples)
#    Plot_Sys_Ratios_test("LLpp", "1Bjet", 'njet', systematics, samples)

    for ch in channels:
        for reg in regions:
#            Plot_Sys_Ratios_test(ch, reg, 'njet', systematics, samples)
            for var in variables:
                StackPlot_by_ch_reg_var(ch, reg, var, systematics, samples)
                Plot_Sys_Ratios_test(ch, reg, var, systematics, samples)      

if __name__ == "__main__":
    main()

