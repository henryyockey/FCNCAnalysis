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
import sys
import os
import subprocess
import readline
import string

#import Files_2017
MCSAMPLES = {}
#MCSAMPLES.update(Files_2017.mc2017_samples)

crossSection = {
'ST_antitop_tchannel': '26.38',
'ST_top_tchannel': '44.33',
'ST_top_schannel': '3.36',
'tW': '35.85',
'tbarW': '35.85',
'TTTo2L2Nu': '87.31',
'TTJets': '831.76',
'DY10to50': '18610',
'DY50': '6077.22',
'WZTo2L2Q':'5.595',
'ZZTo2L2Nu':'0.564',
'TTZToLLNuNu_M_10':'0.2529',
'WJetsToLNu':'61526.7',
'TTW':'0.2043',
'WZTo3LNu':'4.43',
'WWZ_4F':'0.1651',
'ZZTo4L':'1.256 ',
'WWW_4F':'0.2086',
'ZZZ':'0.01398',
'WWTo2L2Nu': '12.178'
}

blackList = ['ST_antitop_tchannel','ST_top_tchannel', 'ST_top_schannel', 'TTJets','fcnc']

text = ''
text += 'import sys \n'
text += 'import os \n'
text += 'import subprocess \n'
text += 'import readline \n'
text += 'import string \n'
text += '\n'

dirSamples = '/hadoop/store/user/rgoldouz/NanoAodPostProcessingUL/UL17/v2'
#dire = '/hadoop/store/user/rgoldouz/ExitedTopSamplesMCJan2021/'
dir_list = os.listdir(dirSamples)

neventsweight = 0
for key in dir_list:
    accept = True
    for S in blackList:
        if S in key:
            accept = False
    if accept:
        if 'data' in key:
            a,b,c,d = key.split("_")   
            MCSAMPLES[key] = [    [],    "data",    d,    "2017",    c,    "1",    "41.53",    "1"]
        else:
            MCSAMPLES[key] = [    [],    "mc",    "none",    "2017",    "none",    "1",    "41.53",    "0"]
#    if 'tp' in key:
#        continue
#    value[0] =[]
#    value[7] = str(0)
for root, dirs, files in os.walk(dirSamples):
    if len(files) > 0:
#        print root
        if 'data' in root:
            for key, value in MCSAMPLES.items():
                if key in root:
                    value[0].append(root[19:])
        else:
            neventsweight = 0
            neventsweightSumw = 0
            for fname in files:
                filename = root + '/' + fname
                if 'fail' in fname:
                    continue
                f = ROOT.TFile.Open(filename)
    #            tree_in = f.Get('Runs')
                tree_meta = f.Get('Runs')
                genEventCount = 0
                genEventSumw = 0
                evtTree = f.Get('Events')
                evtTree.SetBranchStatus("*", 0)
                evtTree.SetBranchStatus("genWeight", 1)
                evtTree.GetEntry(0)
                for i in range( tree_meta.GetEntries() ):
                    tree_meta.GetEntry(i)
                    genEventCount += tree_meta.genEventCount
                    genEventSumw += tree_meta.genEventSumw
                neventsweight += genEventCount
                neventsweightSumw += genEventSumw/abs(evtTree.genWeight)
                if tree_meta.GetEntries()>1:
                    print 'Warning number of MC Runs is more than 1, be careful about sum of the weights' 
                tree_meta.Reset()
                tree_meta.Delete()
#                print str(evtTree.genWeight)
#                print filename 
#                print '-nevent:' + str(genEventCount) +' -neventsweightSumw:'+str(genEventSumw) +' -neventsweightSumw/w:'+str(genEventSumw/abs(evtTree.genWeight))
                evtTree.Reset()
                evtTree.Delete()
                f.Close()
            for key, value in MCSAMPLES.items():
                if key in root: 
                    value[0].append(root[19:])
                    value[7] = str( float(value[7]) + neventsweightSumw)

for key, value in MCSAMPLES.items():
    for S, xs in crossSection.items():
        if S in key:
            value[5]=xs

for key, value in MCSAMPLES.items():
    if 'BNV' not in key or 'FCNC' not in key:
        continue
    neventsweight = 0
    neventsweightSumw = 0
    files = os.listdir('/hadoop/store/user/'+value[0][0])
    for fname in files:
        filename = '/hadoop/store/user/'+value[0][0] + '/' + fname
        if 'fail' in fname:
            continue
        f = ROOT.TFile.Open(filename)
        tree_meta = f.Get('Events')
        genEventCount = 0
        genEventSumw = 0
#        for i in range( tree_meta.GetEntries() ):
#            tree_meta.GetEntry(i)
#            genEventCount += tree_meta.genEventCount
#            genEventSumw += tree_meta.genEventSumw
        neventsweight +=  tree_meta.GetEntries()
        neventsweightSumw +=  tree_meta.GetEntries()
#        print filename + ':' + str(neventsweight) +'-'+str(neventsweightSumw) +' - ' + str(tree_meta.GetEntries())
        tree_meta.Reset()
        tree_meta.Delete()
        f.Close()    
        value[7] = str(neventsweight)


text += 'UL17={'                
#text += str(MCSAMPLES)
text += '\n'

for key, value in MCSAMPLES.items():
    if 'data' in key:
        continue
    text += '"'
    text += key
    text += '":'
    text += str(value)
    text += ','
    text += '\n'

text += '\n \n'
for key, value in MCSAMPLES.items():
    if 'data' not in key:
        continue
    text += '"'
    text += key
    text += '":'
    text += str(value)
    text += ','
    text += '\n'
    print(key, ' : ', value)
text += '}'
#
#diredata = '/hadoop/store/user/rgoldouz/Etop_nanoAODv9_DATA/'
#DATASAMPLES = {}
#DATASAMPLES.update(Files_2017.data2017_samples)
#
#for key, value in DATASAMPLES.items():
#    value[0] =[]
#for root, dirs, files in os.walk(diredata):
#    if len(files) > 0:
#        for key, value in DATASAMPLES.items():
#            if key in root:
#                value[0].append(root[19:])
#text += '\n'
#text += 'data2017_samples='
#text += str(DATASAMPLES)
#
open('Files_2017_nano.py', 'wt').write(text)
