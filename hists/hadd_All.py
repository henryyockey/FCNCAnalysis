import sys
import os
import subprocess
import readline
import string
import glob
from joblib import Parallel, delayed
sys.path.append('/afs/crc.nd.edu/user/r/rgoldouz/FCNC/NanoAnalysis/bin/')
import Files_ULall_nano

def f(name):
    if 'ANoutput' not in name:
        print name
    os.system(name)

if __name__ == '__main__':
    Fhadd=[]
    SAMPLES = {}
    SAMPLES.update(Files_ULall_nano.UL17)
    
    
    addedFilesData = {"2016preVFP": [],"2016postVFP": [], "2017": [], "2018": []} 
    addedFilesMcDY = {"2016preVFP": [],"2016postVFP": [], "2017": [], "2018": []}
    addedFilesMcWJets = {"2016preVFP": [],"2016postVFP": [], "2017": [], "2018": []}
    addedFilesMcttbar = {"2016preVFP": [],"2016postVFP": [], "2017": [], "2018": []}
    addedFilesMcDiboson = {"2016preVFP": [],"2016postVFP": [], "2017": [], "2018": []}
    addedFilesMcTriboson = {"2016preVFP": [],"2016postVFP": [], "2017": [], "2018": []}
    addedFilesMcTTX = {"2016preVFP": [],"2016postVFP": [], "2017": [], "2018": []}
    addedFilesMcST = {"2016preVFP": [],"2016postVFP": [], "2017": [], "2018": []}
    addedFilesMcConv = {"2016preVFP": [],"2016postVFP": [], "2017": [], "2018": []}
    addedFilesMcFCNCprod = {"2016preVFP": [],"2016postVFP": [], "2017": [], "2018": []}
    addedFilesMcFCNCdecay = {"2016preVFP": [],"2016postVFP": [], "2017": [], "2018": []}
    
    os.system('rm *.root')
    dist = "/hadoop/store/user/rgoldouz/FullProduction/AnalysisTOPFCNC/Analysis_" 
    
    for keyUL, value in SAMPLES.items():
        if "UL17" not in keyUL:
            continue
        key = keyUL.replace("UL", "20")
        hadd='hadd ' + key + '.root '
        for filename in os.listdir(dist + keyUL):
            hadd += dist + keyUL + '/' + filename + ' '
        Fhadd.append(hadd)
    Parallel(n_jobs=40)(delayed(f)(i) for i in Fhadd)
    #    os.system(hadd)
    for keyUL, value in SAMPLES.items():
        key = keyUL.replace("UL", "20")
        year = value[3]
        if value[1]=='data':
            addedFilesData[year].append(key + '.root')
#        elif 'FCNC' in key:
#            continue
        elif 'DY' in key:
            addedFilesMcDY[year].append( key + '.root')
        elif '_WJetsToLNu' in key:
            addedFilesMcWJets[year].append(key + '.root')
        elif 'TTTo' in key and 'sys' not in key:
            addedFilesMcttbar[year].append( key + '.root')
#        elif 'WWTo' in key or 'WZTo' in key or 'ZZTo' in key or 'ZGTo' in key:
        elif 'WWTo' in key or 'WZTo' in key or 'ZZTo' in key:
            addedFilesMcDiboson[year].append( key + '.root')
        elif 'WWW' in key or 'WWZ' in key or 'WZZ' in key or 'ZZZ' in key:
            addedFilesMcTriboson[year].append( key + '.root')
        elif 'ST' in key or 'tW' in key or 'tbarW' in key:
            addedFilesMcST[year].append( key + '.root')
        elif 'TTG' in key:
            addedFilesMcConv[year].append( key + '.root')
        elif 'Production' in key:
            addedFilesMcFCNCprod[year].append( key + '.root')
        elif 'Decay' in key:
            addedFilesMcFCNCdecay[year].append( key + '.root')
        else:
            addedFilesMcTTX[year].append(key + '.root')
    for key, value in addedFilesData.items():
#        if key != '2016preVFP':
#            continue
        Fmerged=[]
        print('hadd ' +key+'_data.root ' + ' '.join(addedFilesData[key]))
        print('hadd ' +key+'_DY.root ' + ' '.join(addedFilesMcDY[key]))
        print('hadd ' +key+'_WJets.root ' + ' '.join(addedFilesMcWJets[key]))
        print('hadd ' +key+'_ttbar.root ' + ' '.join(addedFilesMcttbar[key]))
        print('hadd ' +key+'_Diboson.root ' + ' '.join(addedFilesMcDiboson[key]))
        print('hadd ' +key+'_Triboson.root ' + ' '.join(addedFilesMcTriboson[key]))
        print('hadd ' +key+'_ST.root ' + ' '.join(addedFilesMcST[key]))
        print('hadd ' +key+'_TTX.root ' + ' '.join(addedFilesMcTTX[key]))
        print('hadd ' +key+'_Conv.root ' + ' '.join(addedFilesMcConv[key]))
        print('hadd ' +key+'_FCNCDecay.root ' + ' '.join(addedFilesMcFCNCdecay[key]))
        print('hadd ' +key+'_FCNCProduction.root ' + ' '.join(addedFilesMcFCNCprod[key]))

#    BtagFiles=[]
#    for key, value in addedFilesData.items():
#        text = 'hadd ' + 'mc_' + key + '.root ' + key+'_DY.root ' +key+'_ttbar.root ' +key+'_tW.root '    
#        BtagFiles.append(text)
#    Parallel(n_jobs=4)(delayed(f)(i) for i in BtagFiles)
#
#    AllFiles=[]
#    proc = ['data', 'DY','WJets','ttbar','tW','other',
#'STBNV_TBCE',
#'STBNV_TBUE',
#'STBNV_TDCE',
#'STBNV_TDUE',
#'STBNV_TSCE',
#'STBNV_TSUE',
#'TTBNV_TBCE',
#'TTBNV_TBUE',
#'TTBNV_TDCE',
#'TTBNV_TDUE',
#'TTBNV_TSCE',
#'TTBNV_TSUE',
#'STBNV_TBCMu',
#'STBNV_TBUMu',
#'STBNV_TDCMu',
#'STBNV_TDUMu',
#'STBNV_TSCMu',
#'STBNV_TSUMu',
#'TTBNV_TBCMu',
#'TTBNV_TBUMu',
#'TTBNV_TDCMu',
#'TTBNV_TDUMu',
#'TTBNV_TSCMu',
#'TTBNV_TSUMu']
#    for p in proc:
#        text = 'hadd ' + 'All_' + p + '.root '
#        for key, value in addedFilesData.items():
#            text = text + key+'_'+p+'.root '   
#        AllFiles.append(text)
#
#    Parallel(n_jobs=len(proc))(delayed(f)(i) for i in AllFiles)   
#    addedFilesMcttbarSys = {"hdampUP": [],"hdampDOWN": [], "CR1": [], "CR2": [], "TuneCP5up": [], "TuneCP5down": [], "erdON": [], 'STBNV_TDUE':[], 'STBNV_TDUMu':[]}
#    for keyUL, value in SAMPLES.items():
#        key = keyUL.replace("UL", "20")
#        for keysys, valuesys in addedFilesMcttbarSys.items():
#            if keysys in key:
#                addedFilesMcttbarSys[keysys].append( key + '.root')
#    for keysys, valuesys in addedFilesMcttbarSys.items():
#        os.system( 'hadd All_' + valuesys[0].split('_',1)[1] + ' '+ ' '.join(addedFilesMcttbarSys[keysys]) )



