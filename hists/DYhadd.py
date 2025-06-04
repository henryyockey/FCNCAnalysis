import sys
import os
import subprocess
import readline
import string
import glob
sys.path.append('/afs/crc.nd.edu/user/h/hyockey/FCNC/postLHE_analysis/bin')
import Files_ULall_nano1 
SAMPLES = {}
SAMPLES.update(Files_ULall_nano1.UL17)

def hadd_list(fname, flist):
   hadd = 'hadd ' + fname
   for file in flist:
      hadd += ' rootfiles/' + file
   os.system(hadd)

os.system('rm *.root')
os.system('rm rootfiles/*.root')
dist = "/cms/cephfs/data/store/user/hyockey/FullProduction/FCNC_nano_analysis61/Analysis_" 

for key, value in SAMPLES.items():
#    if "Production" not in key: continue
    year = value[3]
    if "2017" not in year: continue
    hadd='hadd -k rootfiles/' + key + '.root '
    if os.path.isdir(dist+key):
        for filename in os.listdir(dist + key):
            hadd += dist + key + '/' + filename + ' '
#        print dist + key + '/' + filename + ' '
    else:
        print("{} is not a not a file".format(dist+key))
    os.system(hadd)

hadd_data = 'hadd Data.root'
datas = ['DoubleEG', 'DoubleMuon', 'MuonEG', 'SingleElectron', 'SingleMuon']
letters = ['B', 'C', 'D', 'E', 'F']

for data in datas:
    for letter in letters:
	hadd_data += ' rootfiles/data_UL17_' + letter + '_'  + data +'.root'

os.system(hadd_data)

DY = [#
'UL17_DY10to50.root',
'UL17_DY50.root',
] 

WJets = [#
'UL17_WJetsToLNu.root'
]

ttbar = [#
'UL17_TTTo2L2Nu.root',
'UL17_TTToSemiLeptonic.root',
]

Diboson = [
'UL17_WZTo3LNu.root',
'UL17_ZZTo4L.root',
'UL17_WWTo2L2Nu.root',
]

Triboson = [
'UL17_WWW_4F.root',
'UL17_WWZ_4F.root',
'UL17_WZZ.root',
'UL17_ZZZ.root',
]

TTX = [
'UL17_ttHnobb.root',
'UL17_TTWJetsToLNu.root',
'UL17_TTZToLLNuNu_M_10.root',
'UL17_ZGToLLG.root',
'UL17_tZq.root',
'UL17_tttt.root',
]

TX = [
'UL17_TWZToLL_tlept_Whad.root',
'UL17_TWZToLL_tlept_Wlept.root',
'UL17_TWZToLL_thad_Wlept.root',
]
ST = [#
'UL17_ST_antitop_t_channel.root',
'UL17_ST_top_t_channel.root',
'UL17_ST_top_s_channel.root',
'UL17_tbarW.root',
'UL17_tW.root',
]

Conv = [
'UL17_TTGamma_Dilept.root',
'UL17_TTGamma_SingleLept.root',
]

FCNCprod = [
'UL17_tuFCNC_tHProduction_skimmed.root',
'UL17_tuFCNC_tllProduction_noH_skimmed.root',
]

FCNCdecay = [
'UL17_tuFCNC_uHDecay_skimmed.root',
'UL17_tuFCNC_ullDecay_noH_skimmed.root',
]

hadd_list('UL17_DY.root', DY)
hadd_list('UL17_wJets.root', WJets)
hadd_list('UL17_ttbar.root', ttbar)
hadd_list('UL17_Diboson.root', Diboson)
hadd_list('UL17_Triboson.root', Triboson)
hadd_list('UL17_TTX.root', TTX)
hadd_list('UL17_TX.root', TX)
hadd_list('UL17_SingleTop.root', ST)
hadd_list('UL17_Conversion.root', Conv)
hadd_list('FCNC-Production.root', FCNCprod)
hadd_list('FCNC-Decay.root', FCNCdecay)
