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
from ROOT import TFile
from ROOT import TGaxis
from ROOT import THStack
import gc
from operator import truediv
import copy
TGaxis.SetMaxDigits(2)
import argparse

def EFTtoNormal(H, wc):
    hpx    = ROOT.TH1F( H.GetName(), H.GetName(), H.GetXaxis().GetNbins(), H.GetXaxis().GetXmin(),H.GetXaxis().GetXmax() )
    r=1
    for b in range(hpx.GetNbinsX()):
#        if H.GetBinContent(b+1,ROOT.WCPoint("NONE"))>0:
#            r = H.GetBinError(b+1)/H.GetBinContent(b+1,ROOT.WCPoint("NONE"))
        hpx.SetBinContent(b+1, H.GetBinContent(b+1,wc))
        hpx.SetBinError(b+1, H.GetBinError(b+1))
    hpx.SetL(H.GetLineColor())
    hpx.SetLineStyle(H.GetLineStyle())
    if hpx.Integral()>0:
        hpx.Scale(1/hpx.Integral())
    return hpx

def AnalyticAnomalousCoupling(H,Wcs,name, suffix=""):
    AACList=[]
    hpx    = ROOT.TH1F( H.GetName(), H.GetName(), H.GetXaxis().GetNbins(), H.GetXaxis().GetXmin(),H.GetXaxis().GetXmax() )
    for i in range(len(Wcs)):
        quad=hpx.Clone()    
        sm_lin_quad=hpx.Clone()
        for b in range(hpx.GetNbinsX()):
            binFit=H.GetBinFit(b+1)
            coeff=binFit.getCoefficient(Wcs[i],Wcs[i])
            quad.SetBinContent(b+1, coeff)
            quad.SetBinError(b+1, 0)     
            sm_lin_quad.SetBinContent(b+1, coeff)
            sm_lin_quad.SetBinError(b+1, 0)
        if quad.Integral()<0.01:
            print( "***WARNING--> WC "+Wcs[i]+' is not relevant for the '+name+' process, so it is ignored')
            continue    
        quad.SetName(name+'_quad_'+Wcs[i]+suffix)
        AACList.append(quad)
        sm_lin_quad.SetName(name+'_sm_lin_quad_'+Wcs[i]+suffix)
        AACList.append(sm_lin_quad)
        for j in range(i+1,len(Wcs)):
            coeffSum=H.GetSumFit()
            if coeffSum.getCoefficient(Wcs[j],Wcs[j])<0.01:
                continue
            sm_lin_quad_mixed=hpx.Clone()
            for b in range(hpx.GetNbinsX()):
                binFit=H.GetBinFit(b+1)
                coeff1=binFit.getCoefficient(Wcs[i],Wcs[i])
                coeff2=binFit.getCoefficient(Wcs[j],Wcs[j])
                coeffMix=binFit.getCoefficient(Wcs[i],Wcs[j])
                sm_lin_quad_mixed.SetBinContent(b+1, coeff1+coeff2+(2*coeffMix))
                sm_lin_quad_mixed.SetBinError(b+1, 0)
            sm_lin_quad_mixed.SetName(name+'_sm_lin_quad_mixed_'+Wcs[i]+'_'+Wcs[j]+suffix)
            AACList.append(sm_lin_quad_mixed)
    return AACList

def TH1EFTtoTH1(H, wc):
    hpx    = ROOT.TH1F( H.GetName(), H.GetName(), H.GetXaxis().GetNbins(), H.GetXaxis().GetXmin(),H.GetXaxis().GetXmax() )
    r=1
    for b in range(hpx.GetNbinsX()):
 #       if H.GetBinContent(b+1,ROOT.WCPoint(wc))>0:
#            r = H.GetBinError(b+1)/H.GetBinContent(b+1,ROOT.WCPoint("NONE"))
        hpx.SetBinContent(b+1, H.GetBinContent(b+1,wc))
        hpx.SetBinError(b+1, H.GetBinError(b+1))
    hpx.SetLineColor(H.GetLineColor())
    hpx.SetLineStyle(H.GetLineStyle())
    return hpx

def SumofWeight(add):
    genEventSumw = 0
    genEventSumwScale = [0]*9
    genEventSumwPdf = [0]*100
    for root, dirs, files in os.walk(add):
        if len(files) == 0:
            continue
        for f in files:
            filename = root + '/' + f
            if 'fail' in f:
                continue
            fi = TFile.Open(filename)
            tree_meta = fi.Get('Runs')
            for i in range( tree_meta.GetEntries() ):
                tree_meta.GetEntry(i)
                genEventSumw += tree_meta.genEventSumw
                for pdf in range(100):
                    genEventSumwPdf[pdf] += tree_meta.LHEPdfSumw[pdf]*tree_meta.genEventSumw
                for Q in range(len(tree_meta.LHEScaleSumw)):
                    genEventSumwScale[Q] += tree_meta.LHEScaleSumw[Q]*tree_meta.genEventSumw
            tree_meta.Reset()
            tree_meta.Delete()
            fi.Close()
    if genEventSumwScale[8]==0:
        del genEventSumwScale[8]
    return [genEventSumw/x for x in genEventSumwScale] , [genEventSumw/x for x in genEventSumwPdf]

HistAddress = '/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/hists/'
year=['2017']
ULyear=['UL17']
LumiErr = [0.018, 0.018, 0.018, 0.018]
regions=["1Bjet"]
channels=[
#"LLpp","LLmm","3LonZ", 
"3LoffZp", "3LoffZm"];
variables=[
#   "lep1Pt",
   "BDTOutput"
]
sys = ["eleRecoSf", "eleIDSf", "muIdIsoSf", "bcTagSf", "LTagSf","pu", "prefiring", #"trigSF", "jer","muonScale","electronScale","muonRes", "unclusMET", 
      "bcTagSfUnCorr", "LTagSfUnCorr"] #,"JetPuID","topPt"]
#"eleRecoSf", "eleIDSf", "muIdSf", "muIsoSf", "bcTagSf", "LTagSf", "pu", "prefiring", "trigSF",  "muonScale","electronScale","muonRes","unclusMET", "bcTagSfUnCorr", "LTagSfUnCorr"]
sysJecNames = []#"AbsoluteMPFBias","AbsoluteScale","AbsoluteStat","Fragmentation","PileUpPtBB","PileUpPtEC1","PileUpPtEC2","PileUpPtHF","PileUpPtRef","RelativeFSR","RelativePtBB","RelativePtEC1","RelativePtEC2","RelativePtHF","RelativeBal","RelativeSample","RelativeStatEC","RelativeStatFSR","RelativeStatHF","SinglePionECAL","SinglePionHCAL","TimePtEta"]
sysJecNamesUnCorr = []#"AbsoluteStat","RelativePtEC1","RelativePtEC2","RelativeSample","RelativeStatEC","RelativeStatFSR","RelativeStatHF","TimePtEta"]
sysJecNamesCorr =[]#"AbsoluteMPFBias","AbsoluteScale","Fragmentation","PileUpPtBB","PileUpPtEC1","PileUpPtEC2","PileUpPtHF","PileUpPtRef","RelativeFSR","RelativePtBB","RelativePtHF","RelativeBal","RelativeStatFSR","RelativeStatHF"]
#HistAddress = '/afs/crc.nd.edu/user/r/rgoldouz/FCNC/NanoAnalysis/hists/'
SFDY={'2018ee': 1.1365510987619387, '2017emu': 1.3261806003138998, '2016preVFPee': 1.1633451628199767, '2017mumu': 1.3465242028877409, '2016postVFPemu': 1.462616384646022, 'Allmumu': 1.2578307798620003, 'Allemu': 1.2367413449370244, '2016preVFPmumu': 1.1158537954764174, 'Allee': 1.216005506276805, '2016postVFPmumu': 1.528904852150822, '2016postVFPee': 1.3992019749466855, '2016preVFPemu': 1.1393520594538817, '2018mumu': 1.183191225338832, '2017ee': 1.3061443536455783, '2018emu': 1.15963670483662}
#Samples = ['Data.root','Fakes.root', 'UL17_Conversion.root', 'UL17_Diboson.root', 'UL17_TTX.root', 'UL17_Triboson.root', 'UL17_wJets.root', 'UL17_DY.root', 'UL17_SingleTop.root', 'UL17_ttbar.root', 'FCNC-Production.root', 'FCNC-Decay.root']
#SamplesName = ['Data', 'Fakes','Conversion', 'Diboson', 'TTX', 'Triboson','W+jets', 'DY', 'Single_top',  't#bar{t}','FCNC-Prod','FCNC-Dec']
#SamplesNameCombined = ['data_obs','fakes', 'conv', 'dib', 'ttx', 'trib', 'wjets', 'DY', 'singletop', 'ttbar', 'FCNCProd', 'FCNCDec']
Samples = ['Data.root','Fakes.root','UL17_Diboson.root', 'UL17_TTX.root', 'Other.root', 'FCNC-Production.root', 'FCNC-Decay.root']
SamplesName = ['Data','Fakes','UL17_Diboson', 'UL17_TTX', 'Other', 'FCNC-Prod', 'FCNC-Deca']
SamplesNameCombined = ['data_obs','Fakes','UL17_Diboson', 'UL17_TTX', 'Other', 'FCNC-Prod', 'FCNC-Deca']


NormalizationErr = [0, 0.5, 0.5, 0.3, 0.05, 0.1, 0,0,0,0,0,0, 0,0,0,0,0,0]
SignalSamples=[
'FCNC-Prod','FCNC-Deca',
]
colors =  [ROOT.kBlack,ROOT.kGreen,ROOT.kBlue+8,ROOT.kRed-4,ROOT.kOrange-3, ROOT.kCyan+1, ROOT.kYellow, ROOT.kMagenta-4,ROOT.kBlue, ROOT.kGray+1,ROOT.kGray+3,ROOT.kYellow-2]
#WCs=["ctp","ctlS","cte","ctl","ctlT","ctZ","cpt","cpQM","ctA","cQe","ctG","cQlM"]
#WCs=["ctp","ctG"]
WCs=["cQlM", "cQe", "ctl", "cte", "ctlS", "ctlT"]

#parser = argparse.ArgumentParser()
#parser.add_argument('-f', '--first', type=str)
#parser.add_argument('-s', '--second', type=str)
#args = parser.parse_args()
#if args.first and args.second:
#    WCs = [args.first, args.second]

wc1 = ROOT.WCPoint("EFTrwgt1_cT_1_cS_0")
wcName = "cT"
print 'making combine files'

Hists = []
HistsSysUp = []
HistsSysDown = []
HistsFake = []
for numyear, nameyear in enumerate(year):
    l0=[]
    l0Up = []
    l0Down = []
    Files = []
    for f in range(len(Samples)):
        l1=[]
        l1Up = []
        l1Down = []
        Files.append(ROOT.TFile.Open(HistAddress + Samples[f]))
        print HistAddress + Samples[f]
        for numch, namech in enumerate(channels):
            l2=[]
            l2Up = []
            l2Down = []
            for numreg, namereg in enumerate(regions):
                l3=[]
                l3Up = []
                l3Down = []
                for numvar, namevar in enumerate(variables):
                    print namech + '_' + namereg + '_' + namevar
                    h= Files[f].Get(namech + '_' + namereg + '_' + namevar)
                    if not ("Data" in Samples[f] or "Fake" in Samples[f]):
                        l4Up = []
                        l4Down = []
                        for s in sys:
                            hup = Files[f].Get(namech + '_' + namereg + '_' + namevar + '_' + s + 'Up')
                            hdown = Files[f].Get(namech + '_' + namereg + '_' + namevar + '_' + s + 'Down')
                            hup.SetFillColor(colors[f])
                            hup.SetLineColor(colors[f])
                            hdown.SetFillColor(colors[f])
                            hdown.SetLineColor(colors[f])
                            l4Up.append(hup)
                            l4Down.append(hdown)
                        l3Up.append(l4Up)
                        l3Down.append(l4Down)
#                    print namevar + ":" + str(h.Integral())
#                    if 'njet' in namevar:
#                        print namech + '_' + namereg + '_' + namevar + str(h.GetBinFit(3).getDim())
#                        print namech + '_' + namereg + '_' + namevar + str(h.GetBinContent(3,wc1))
                    h.SetFillColor(colors[f])
                    h.SetLineColor(colors[f])
                    
                    l3.append(h)
                l2.append(l3)
                l2Up.append(l3Up)
                l2Down.append(l3Down)
            l1.append(l2)
            l1Up.append(l2Up)
            l1Down.append(l2Down)
        l0.append(l1)
        l0Up.append(l1Up)
        l0Down.append(l1Down)
    Hists.append(l0)
    HistsSysUp.append(l0Up)
    HistsSysDown.append(l0Down)

if not os.path.exists('CombinedFilesFCNC'):
    os.makedirs('CombinedFilesFCNC')
else:
    os.system('rm -rf CombinedFilesFCNC/'+ wcName +'_*')
bins = array( 'd',[0.0,40.0,120.0,200.0,400.0,600.0,800.0, 1000.0] )

processes=[]
Integrals=[]
nuisances={}
for numyear, nameyear in enumerate(year):
    HL1=[]
    HI1=[]
    for numch, namech in enumerate(channels):
        HL2=[]
        HI2=[]
        for numreg, namereg in enumerate(regions):
            HL3=[]
            HI3=[]
            #first write the observed data hist
            hfile = ROOT.TFile( 'CombinedFilesFCNC/' +  nameyear+'_'+namech+'_'+namereg+'.root', 'RECREATE', 'combine input histograms' )
            hNormal = TH1EFTtoTH1(Hists[numyear][0][numch][numreg][0],wc1)
            hNormal.SetName(SamplesNameCombined[0])
            hNormal.Write()
            for f in range(2, len(Samples)): #skip fakes + data
                for s in range(len(sys)):
                    hNormalUp = TH1EFTtoTH1(HistsSysUp[numyear][f][numch][numreg][0][s],wc1)
                    hNormalDown = TH1EFTtoTH1(HistsSysDown[numyear][f][numch][numreg][0][s],wc1)
                    for n in range(hNormal.GetNbinsX()):
                        if hNormalUp.GetBinContent(n+1) < 0 : hNormalUp.SetBinContent(n+1,0.0000001)
                    for n in range(hNormal.GetNbinsX()):
                        if hNormalDown.GetBinContent(n+1) < 0 : hNormalDown.SetBinContent(n+1,0.0000001)
                    hNormalUp.SetName(SamplesNameCombined[f]+'_{}Up'.format(sys[s]))
                    hNormalUp.Write()
                    hNormalDown.SetName(SamplesNameCombined[f]+'_{}Down'.format(sys[s]))
                    hNormalDown.Write()
 
            for f in range(1,len(Samples)):
                HL4=[]
                HI4=[]
                if 'FCNC' not in SamplesNameCombined[f]:
                    hNormal = TH1EFTtoTH1(Hists[numyear][f][numch][numreg][0],wc1)
                    for n in range(hNormal.GetNbinsX()):
                       if hNormal.GetBinContent(n+1) < 0 : hNormal.SetBinContent(n+1,0.0000001)
                    hNormal.SetName(SamplesNameCombined[f])
                    hNormal.Write()
                    HL4.append(SamplesNameCombined[f])
                    HI4.append(hNormal.Integral())
                else:
                    SignalH=AnalyticAnomalousCoupling(Hists[numyear][f][numch][numreg][0],WCs,SamplesNameCombined[f])       
                    for s in range(len(sys)):
                        SignalHUp=AnalyticAnomalousCoupling(HistsSysUp[numyear][f][numch][numreg][0][s],WCs,SamplesNameCombined[f], suffix="_{}Up".format(sys[s]))
                        SignalHDown=AnalyticAnomalousCoupling(HistsSysDown[numyear][f][numch][numreg][0][s],WCs,SamplesNameCombined[f], suffix="_{}Down".format(sys[s]))
                        for HUp, HDown in zip(SignalHUp, SignalHDown):
                            HUp.Write()
                            HDown.Write()    
                    for H in SignalH:
                        H.Write()             
                        HL4.append(H.GetName())
                        HI4.append(H.Integral())
                HL3.append(HL4)
                HI3.append(HI4)
            HL2.append(HL3)
            HI2.append(HI3)
            hfile.Write()
            hfile.Close()
            os.system('mv CombinedFilesFCNC/'+nameyear+'_'+namech+'_'+namereg+'.root CombinedFilesFCNC/org.root')
            f1 = ROOT.TFile.Open('CombinedFilesFCNC/org.root',"READ")
            hfile = ROOT.TFile( 'CombinedFilesFCNC/' + nameyear+'_'+namech+'_'+namereg+'.root', 'RECREATE', 'combine input histograms' )
            my_list = f1.GetListOfKeys()
            for obj in my_list: # obj is TKey
                if obj.GetClassName() == "TH1F":
                    RF = f1.Get(obj.GetName())
                    RF.SetName(obj.GetName())
                    RF.Rebin(len(bins)-1,"",bins)
                    hfile.WriteObject(RF, RF.GetName())
            f1.Close()
#            os.system('rm CombinedFilesFCNC/org.root')
            hfile.Write()
            hfile.Close()
        HL1.append(HL2)
        HI1.append(HI2)
    processes.append(HL1)
    Integrals.append(HI1)

#nProcesses = len(processes[numyear][numch][numreg]) * sum(len(item) for item in processes[numyear][numch][numreg])
nProcesses = 0
for i in range(len(processes[numyear][numch][numreg])):
    for j in range(len(processes[numyear][numch][numreg][i])):
        nProcesses+=1


for numyear, nameyear in enumerate(year):
    for numch, namech in enumerate(channels):
        for numreg, namereg in enumerate(regions):
            cardName = 'C'+namech+'_'+nameyear+'_' + namereg
            T1 = 'max 1 number of categories \n' +\
                 'jmax * number of samples minus one\n' +\
                 'kmax * number of nuisance parameters\n' +\
                 '------------\n'+\
                 'shapes * * '  + nameyear+'_'+namech+'_'+namereg+'.root' + ' $PROCESS $PROCESS_$SYSTEMATIC\n' +\
                 '------------\n'+\
                 'bin'.ljust(45) + cardName + '\n'+\
                 'observation'.ljust(45) + str(Hists[numyear][0][numch][numreg][0].Integral()) +'\n'+\
                 '------------\n'
            print(Hists[numyear][0][numch][numreg][0].Integral())
            T1 = T1 +'bin'.ljust(45)
            for i in range(len(processes[numyear][numch][numreg])):
                for j in range(len(processes[numyear][numch][numreg][i])):
                    T1 = T1 + cardName.ljust(40)
            T1 = T1 + '\n'
            T1 = T1 +'process'.ljust(45)
            c=1
            for i in range(len(processes[numyear][numch][numreg])):
                for j in range(len(processes[numyear][numch][numreg][i])):
                    T1 = T1 + str(c).ljust(40)
                    c=c+1
            T1 = T1 + '\n'
            T1 = T1 +'process'.ljust(45)
            for i in range(len(processes[numyear][numch][numreg])):
                for j in range(len(processes[numyear][numch][numreg][i])):
                    T1 = T1 + processes[numyear][numch][numreg][i][j].ljust(40)
            T1 = T1 + '\n'
            T1 = T1 +'rate'.ljust(45)
            for i in range(len(processes[numyear][numch][numreg])):
                for j in range(len(processes[numyear][numch][numreg][i])):
                    I = Integrals[numyear][numch][numreg][i][j]
                    if (I<0): I=0.0001
                    T1 = T1 + str(I).ljust(40)
            T1 = T1 + '\n'
            T1 = T1 +'------------\n' 
#            'bin'.ljust(45) + cardName.ljust(40) + cardName.ljust(40) + cardName.ljust(40) + cardName.ljust(40) + cardName.ljust(40) + cardName.ljust(40) +'\n'+\
#            'process'.ljust(45) +'-1'.ljust(40) +'0'.ljust(40) + '1'.ljust(40) + '2'.ljust(40) + '3'.ljust(40) + '4'.ljust(40) +'\n'+\
#            'rate'.ljust(45) + str(Hists[numyear][0][numch][numreg][0].Integral()).ljust(40) + str(Hists[numyear][1][numch][numreg][0].Integral()).ljust(40) + str(Hists[numyear][1][numch][numreg][0].Integral()).ljust(40) + str(Hists[numyear][2][numch][numreg][0].Integral()).ljust(40)+\
#            str(Hists[numyear][3][numch][numreg][0].Integral()).ljust(40) + str(Hists[numyear][4][numch][numreg][0].Integral()).ljust(40) + '\n'+\
#            '------------\n'+\
#            'Other_norm'.ljust(35)+'lnN'.ljust(10) + '-'.ljust(40) + '-'.ljust(40)  + '1.5'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) +'\n'+\
#            T1 += 'DY_norm'.ljust(35) + 'lnN'.ljust(10) + ''.join(('1.3'.ljust(40) if i == 6 else '-'.ljust(40)) for i in range(nProcesses)) + '\n'
#            T1 += 'tt_norm'.ljust(35) + 'lnN'.ljust(10) + ''.join(('1.05'.ljust(40) if i == 8 else '-'.ljust(40)) for i in range(nProcesses)) + '\n'
#            'tt_norm'.ljust(35)+'lnN'.ljust(10) + '-'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '1.05'.ljust(40) + '-'.ljust(40) +'\n'
#            'tW_norm'.ljust(35)+'lnN'.ljust(10) + '-'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '1.1'.ljust(40) +'\n'
#            'MuIDSF'.ljust(35)+'lnN'.ljust(10) + '1.01'.ljust(40) + '1.01'.ljust(40) + '1.01'.ljust(40) + '1.01'.ljust(40) + '1.01'.ljust(40) + '1.01'.ljust(40) +'\n'
#            if '2016' in nameyear:
#                T1 = T1 + 'lumi2016'.ljust(35)+'lnN'.ljust(10) + '1.01'.ljust(40) + '1.01'.ljust(40) + '1.01'.ljust(40) + '1.01'.ljust(40) + '1.01'.ljust(40) + '1.01'.ljust(40) +'\n'    
#                T1 = T1 + 'lumiCorr16-17-18'.ljust(35)+'lnN'.ljust(10) + '1.006'.ljust(40) + '1.006'.ljust(40) + '1.006'.ljust(40) + '1.006'.ljust(40) + '1.006'.ljust(40) + '1.006'.ljust(40) +'\n'
#                T1 = T1 + 'DY_METmodel2016'.ljust(35)+'lnN'.ljust(10) + '-'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '1.2'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) +'\n'
            if '2017' in nameyear:
                T1 = T1 + 'lumi2017'.ljust(35)+'lnN'.ljust(10) + ('1.02'.ljust(40)*nProcesses) +'\n'
                T1 = T1 + 'lumiCorr16-17-18'.ljust(35)+'lnN'.ljust(10) + ('1.009'.ljust(40)*nProcesses) +'\n'
                T1 = T1 + 'lumiCorr17-18'.ljust(35)+'lnN'.ljust(10) + ('1.006'.ljust(40)*nProcesses) +'\n'
#                T1 = T1 + 'DY_METmodel2017'.ljust(35)+'lnN'.ljust(10) + '-'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '1.2'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) +'\n'
#            if '2018' in nameyear:
#                T1 = T1 + 'lumi2018'.ljust(35)+'lnN'.ljust(10) + '1.015'.ljust(40) + '1.015'.ljust(40) + '1.015'.ljust(40) + '1.015'.ljust(40) + '1.015'.ljust(40) + '1.015'.ljust(40) +'\n'
#                T1 = T1 + 'lumiCorr16-17-18'.ljust(35)+'lnN'.ljust(10) + '1.02'.ljust(40) + '1.02'.ljust(40) + '1.02'.ljust(40) + '1.02'.ljust(40) + '1.02'.ljust(40) + '1.02'.ljust(40) +'\n'
#                T1 = T1 + 'lumiCorr17-18'.ljust(35)+'lnN'.ljust(10) + '1.002'.ljust(40) + '1.002'.ljust(40) + '1.002'.ljust(40) + '1.002'.ljust(40) + '1.002'.ljust(40) + '1.002'.ljust(40) +'\n'
#                T1 = T1 + 'DY_METmodel2018'.ljust(35)+'lnN'.ljust(10) + '-'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '2.0'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) +'\n'
#            if 'Mu' in namesig:
#                T1 = T1 + 'MuTtDyDiff'.ljust(35)+'lnN'.ljust(10) + '1.005'.ljust(40) + '1.005'.ljust(40) + '1.005'.ljust(40) + '1.005'.ljust(40) + '1.005'.ljust(40) + '1.005'.ljust(40) +'\n'
#            else:
#                T1 = T1 + 'EleTtDyDiff'.ljust(35)+'lnN'.ljust(10) + '1.01'.ljust(40) + '1.01'.ljust(40) + '1.01'.ljust(40) + '1.01'.ljust(40) + '1.01'.ljust(40) + '1.01'.ljust(40) +'\n'
#            for numsys, namesys in enumerate(sysJecNamesCorr):
#                T1 = T1 +  'jes' + namesys.ljust(32)  +'shape'.ljust(10)  + '1'.ljust(40) +  '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) +'\n'
#            for numsys, namesys in enumerate(sysJecNamesUnCorr):
#                T1 = T1 + 'Y'+  nameyear + 'jes' + namesys.ljust(27)  +'shape'.ljust(10)  + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) +'\n'
            for b in sys:
                if 'jer' in b or 'unclusMET' in b or 'UnCorr' in b or 'topPt' in b or "muonScale" in b or "muonRes" in b:
                    continue 
#                if not b=="LTagSF": continue
                T1 = T1 +  b.ljust(35)  +'shape'.ljust(10)#  + '1'.ljust(40) + '1'.ljust(40)  + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) +'\n'
                count = 0
                for i in range(len(processes[numyear][numch][numreg])):
                    for j in range(len(processes[numyear][numch][numreg][i])):
                        count+=1
                        q = '-' if i == 0 else '1' 
                        T1 += q.ljust(40)
                T1 += '\n'
                print('nProcesses={}'.format(count))

#            T1 = T1 +  'muonScale'.ljust(35)  +'shape'.ljust(10)  + '-'.ljust(40) + '-'.ljust(40)  + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) +'\n'
#            T1 = T1 +  'muonRes'.ljust(35)  +'shape'.ljust(10)  + '-'.ljust(40) + '-'.ljust(40)  + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) +'\n'
#            T1 = T1 +  'topPt'.ljust(35)  +'shape'.ljust(10)  + '-'.ljust(40) + '-'.ljust(40)  + '-'.ljust(40) + '-'.ljust(40) + '1'.ljust(40) + '-'.ljust(40) +'\n'
#            T1 = T1 + 'Y'+ nameyear + 'unclusMET'.ljust(30)  +'shape'.ljust(10)  + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) +'\n'
#            T1 = T1 + 'Y'+ nameyear + 'bcTagSfUnCorr'.ljust(30)  +'shape'.ljust(10)  + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) +'\n'
#            T1 = T1 + 'Y'+ nameyear + 'LTagSfUnCorr'.ljust(30)  +'shape'.ljust(10)  + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) +'\n'
#            T1 = T1 + 'Y'+ nameyear + 'jer'.ljust(30)  +'shape'.ljust(10)  + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) + '1'.ljust(40) +'\n'
#            T1 = T1 +  'pdf'.ljust(35)  +'shape'.ljust(10)  + '1'.ljust(40) + '1'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '1'.ljust(40) + '-'.ljust(40) +'\n'
#            bpb= 'tt_QS'
#            T1 = T1 +  bpb.ljust(35)  +'shape'.ljust(10)  + '-'.ljust(40) + '1'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '1'.ljust(40) + '-'.ljust(40) +'\n'                    
#            bpb= 'tt_ISR'
#            T1 = T1 +  bpb.ljust(35)  +'shape'.ljust(10)  + '-'.ljust(40) + '1'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '1'.ljust(40) + '-'.ljust(40) +'\n'
#            for b in ttSysOther:
#                bpb= 'tt_' + b
#                T1 = T1 +  bpb.ljust(35)  +'shape'.ljust(10)  + '-'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '1'.ljust(40) + '-'.ljust(40) +'\n'
#            for b in ttsysCR:
#                bpb= 'tt_' + b
#                T1 = T1 +  bpb.ljust(35)  +'shape'.ljust(10)  + '-'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '1'.ljust(40) + '-'.ljust(40) +'\n'
#            T1 = T1 + 'FSR'.ljust(35)+'shape'.ljust(10) + '1'.ljust(40) + '1'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '1'.ljust(40) + '-'.ljust(40) +'\n'
#            T1 = T1 + 'Signal_ISR'.ljust(35)+'shape'.ljust(10) + '1'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) +'\n'
#            bpb= 'Signal_QS' 
#            T1 = T1 +  bpb.ljust(35)  +'shape'.ljust(10)  + '1'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) + '-'.ljust(40) +'\n'
            T1 = T1 + '* autoMCStats 10' + '\n'
            open('CombinedFilesFCNC/' + cardName +'.txt', 'wt').write(T1)
#    
#    
#    
    



