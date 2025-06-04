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

def stackPlots(hists, SignalHists, Fnames, ch = "channel", reg = "region", year='2017', var="sample", varname="v", density=False):
    if not os.path.exists(year):
       os.makedirs(year)
    if not os.path.exists(year + '/' + ch):
       os.makedirs(year + '/' + ch)
    if not os.path.exists(year + '/' + ch +'/'+reg):
       os.makedirs(year + '/' + ch +'/'+reg)

    hs = ROOT.THStack("hs","")
#    print(len(hists))
    sumbg = 0
    for h in hists[1:]:
        sumbg+=h.Integral()

    if density==False : sumbg=1
    if sumbg == 0: sumbg=1
    for num in range(1,len(hists)):
#        print(type(hists[num]))
        h = hists[num]
        for i in range(1, h.GetNbinsX()+1):
            BC = super(ROOT.TH1EFT, h).GetBinContent(i)
            h.SetBinContent(i, BC/sumbg)
        h.SetBinContent(h.GetNbinsX()+1,0)
        h.SetBinContent(0,0)
    
	hs.Add(h)

    dummy = hists[0]


    canvas = ROOT.TCanvas(year+ch+reg+var,year+ch+reg+var,50,50,865,780)
    canvas.SetGrid();
    canvas.SetBottomMargin(0.17)
    canvas.cd()

    legend = ROOT.TLegend(0.7,0.55,0.9,0.88)
    legend.SetBorderSize(0)
    legend.SetTextFont(42)
    legend.SetTextSize(0.04)

    pad1=ROOT.TPad("pad1", "pad1", 0, 0.315, 1, 0.99 , 0)#used for the hist plot
    pad2=ROOT.TPad("pad2", "pad2", 0, 0.0, 1, 0.305 , 0)#used for the ratio plot
    pad1.Draw()
    pad2.Draw()
    pad2.SetGridy()
    pad2.SetTickx()
    pad1.SetBottomMargin(0.02)
    pad1.SetLeftMargin(0.14)
    pad1.SetRightMargin(0.05)
    pad2.SetTopMargin(0.1)
    pad2.SetBottomMargin(0.4)
    pad2.SetLeftMargin(0.14)
    pad2.SetRightMargin(0.05)
    pad2.SetFillStyle(0)
    pad1.SetFillStyle(0)
    pad1.cd()
    pad1.SetLogx(ROOT.kFALSE)
    pad2.SetLogx(ROOT.kFALSE)
    pad1.SetLogy(ROOT.kFALSE)

    y_min=0
    if density==True: 
        integral = dummy.Integral()
        if integral == 0: integral = 1
        super(ROOT.TH1EFT, dummy).Scale(1/integral)
   
    y_max=1.6*dummy.GetMaximum()
    dummy.SetMarkerStyle(20)
    dummy.SetMarkerSize(1.2)
    dummy.SetTitle("")
    dummy.GetYaxis().SetTitle('Events')
    dummy.GetXaxis().SetLabelSize(0)
    dummy.GetYaxis().SetTitleOffset(0.8)
    dummy.GetYaxis().SetTitleSize(0.07)
    dummy.GetYaxis().SetLabelSize(0.04)
    dummy.GetYaxis().SetRangeUser(y_min,y_max)
    
    dummy.Draw("e")
    hs.Draw("histSAME")
    for H in SignalHists:
        H.SetLineWidth(2)
        H.SetFillColor(0)
        H.SetLineStyle(9)
        H.Draw("histSAME")
    dummy.Draw("eSAME")
    dummy.Draw("AXISSAMEY+")
    dummy.Draw("AXISSAMEX+")

    Lumi = '138'
    if (year == '2016preVFP'):
        Lumi = '19.52'
    if (year == '2016postVFP'):
        Lumi = '16.81'
    if (year == '2017'):
        Lumi = '41.48'
    if (year == '2018'):
        Lumi = '59.83'
    label_cms="CMS Preliminary"
    Label_cms = ROOT.TLatex(0.2,0.92,label_cms)
    Label_cms.SetNDC()
    Label_cms.SetTextFont(61)
    Label_cms.Draw()
    Label_lumi = ROOT.TLatex(0.71,0.92,Lumi+" fb^{-1} (13 TeV)")
    Label_lumi.SetNDC()
    Label_lumi.SetTextFont(42)
    Label_lumi.Draw("same")
    Label_channel = ROOT.TLatex(0.2,0.8,year +" / "+ch+" ("+reg+")")
    Label_channel.SetNDC()
    Label_channel.SetTextFont(42)
    Label_channel.Draw("same")


    legend.AddEntry(dummy,Fnames[0],'ep')
    for num in range(1,len(hists)):
        legend.AddEntry(hists[num],Fnames[num],'F')
    for H in range(len(SignalHists)):
        legend.AddEntry(SignalHists[H], Fnames[len(hists)+H],'L')
    legend.Draw("same")

    if (hs.GetStack().Last().Integral()>0):
        Label_DM = ROOT.TLatex(0.2,0.75,"Data/MC = " + str(round(hists[0].Integral()/hs.GetStack().Last().Integral(),2)))
        Label_DM.SetNDC()
        Label_DM.SetTextFont(42)
        Label_DM.Draw("same")

    pad1.Update()

    pad2.cd()
    SumofMC = hs.GetStack().Last()
    dummy_ratio = hists[0].Clone()
    dummy_ratio.SetTitle("")
    dummy_ratio.SetMarkerStyle(20)
    dummy_ratio.SetMarkerSize(1.2)
    dummy_ratio.GetXaxis().SetTitle(varname)
#    dummy_ratio.GetXaxis().CenterTitle()
    dummy_ratio.GetYaxis().CenterTitle()
    dummy_ratio.GetXaxis().SetMoreLogLabels()
    dummy_ratio.GetXaxis().SetNoExponent()
    dummy_ratio.GetXaxis().SetTitleSize(0.04/0.3)
    dummy_ratio.GetYaxis().SetTitleSize(0.04/0.3)
    dummy_ratio.GetXaxis().SetTitleFont(42)
    dummy_ratio.GetYaxis().SetTitleFont(42)
    dummy_ratio.GetXaxis().SetTickLength(0.05)
    dummy_ratio.GetYaxis().SetTickLength(0.05)
    dummy_ratio.GetXaxis().SetLabelSize(0.115)
    dummy_ratio.GetYaxis().SetLabelSize(0.089)
    dummy_ratio.GetXaxis().SetLabelOffset(0.02)
    dummy_ratio.GetYaxis().SetLabelOffset(0.01)
    dummy_ratio.GetYaxis().SetTitleOffset(0.42)
    dummy_ratio.GetXaxis().SetTitleOffset(1.1)
    dummy_ratio.GetYaxis().SetNdivisions(504)
    dummy_ratio.GetYaxis().SetRangeUser(0.5,1.5)
    dummy_ratio.Divide(SumofMC)
    dummy_ratio.SetStats(ROOT.kFALSE)
    dummy_ratio.GetYaxis().SetTitle('Data/Pred.')
    dummy_ratio.Draw()
    canvas.Print(year + '/' + ch +'/'+reg+'/'+var + ".png")
    del canvas
    gc.collect()
                                                                                            
