import datetime
import os
from os import path
import sys
import subprocess
import readline
import string
import glob
from joblib import Parallel, delayed

def f(name='ABC'):
    print name 
#    os.chdir('impacts')
    os.system(name)
if __name__ == '__main__':
#    for root, dirs, files in os.walk('/hadoop/store/user/rgoldouz/FullProduction/LimitsTTXEFTdoInitialFit'):
#        if len(files) > 0:
#            for f in files:
#                os.system('cp ' + root +'/'+ f + ' impacts/' + ('_'.join(f.split('_')[:-1]))+'.root')
#    for root, dirs, files in os.walk('/hadoop/store/user/rgoldouz/FullProduction/LimitsTTXEFTdoFitForNuisance'):
#        if len(files) > 0:
#            for f in files:
#                os.system('cp ' + root +'/'+ f + ' impacts/' + ('_'.join(f.split('_')[:-1]))+'.root')
    
    
    CouplingsDict = {
    'ctW':'-4,4',     'ctZ':'-5,5',
    'cpt':'-40,30',   'ctp':'-35,65',
    'ctli':'-10,10',  'ctlSi':'-10,10',
    'cQl3i':'-10,10', 'cptb':'-20,20',
    'ctG':'-2,2',     'cpQM':'-10,30',
    'ctlTi':'-2,2',   'ctei':'-10,10',
    'cQei':'-10,10',  'cQlMi':'-10,10',
    'cpQ3':'-15,10',  'cbW':'-5,5',
    'cQq13': '-1,1',  'cQq83': '-2,2',
    'cQq11': '-2,2','ctq1': '-2,2',
    'cQq81': '-5,5','ctq8': '-5,5',
    'ctt1': '-5,5', 'cQQ1': '-10,10',
    'cQt8': '-20,20', 'cQt1': '-10,10'
    }
    FrozenSys=[]
    Couplings = ['ctlTi', 'ctq1', 'ctq8', 'cQq83', 'cQQ1', 'cQt1', 'cQt8', 'ctt1', 'cQq81', 'cQlMi', 'cbW', 'cpQ3', 'ctei', 'ctlSi', 'cpQM', 'cQei', 'ctZ', 'cQl3i', 'ctG', 'cQq13', 'cQq11', 'cptb', 'ctli', 'ctp', 'cpt', 'ctW']
    Exclude =''
    if len(FrozenSys)>0:
        Exclude =' --exclude ' + (','.join(FrozenSys))
    Nuisance = ['lepSF_muon', 'lepSF_elec', 'btagSFbc_2016', 'btagSFbc_corr', 'btagSFlight_2016', 'PU', 'PreFiring', 'triggerSF', 'FSR','ISR', 'renormfact','JER','JES','btagSFbc_2016APV', 'btagSFlight_2016APV', 'btagSFbc_2018', 'btagSFlight_2018', 'btagSFbc_2017', 'btagSFlight_2017', 'lumi_flat', 'pdf_scale_qq_flat','qcd_scale_VV_flat','qcd_scale_VVV_flat', 'FF', 'FFpt', 'FFeta', 'FFcloseEl_2016', 'FFcloseMu_2016', 'FFcloseEl_2016APV', 'FFcloseMu_2016APV', 'FFcloseEl_2017', 'FFcloseMu_2017', 'FFcloseEl_2018', 'FFcloseMu_2018', 'pdf_scale_qg_flat', 'qcd_scale_tHq_flat', 'qcd_scale_V_flat', 'pdf_scale_gg_flat', 'qcd_scale_ttH_flat', 'qcd_scale_ttll_flat', 'qcd_scale_ttlnu_flat', 'btagSFlight_corr','charge_flips_flat']
#    Nuisance = ['btagSFlight_corr','charge_flips_flat']
    WorkSpace = 'njets_fullR2_rmNegativeUncty_withSys_anatest01.root'

    setParam = ''
    setParamRange = ''
    for key, value in CouplingsDict.items():
        setParam += key + '=0,'
        setParamRange += key + '=' + value+':'
    
    os.chdir('impacts')
    Jobs=[]
    Jobs1=[]
    Jobs2=[]
    for key, value in CouplingsDict.items():
        Couplings.remove(key)
        RunCommand1 = 'combineTool.py -M Impacts -m 125 -o ' + key + '_impacts.json -n ' + key + ' -d ' + WorkSpace + ' --redefineSignalPOIs ' + key + '  -t -1 ' + Exclude + ',' + (','.join(Couplings))
        Couplings.append(key)  
        RunCommand2 = 'plotImpacts.py -i ' + key + '_impacts.json -o OthersFixed_' + key + '_impacts --label-size 0.03 --cms-label ,other_WCs_fixed_to_SM'
        Jobs1.append(RunCommand1)
        Jobs2.append(RunCommand2)
        RunCommand1 = 'combineTool.py -M Impacts -m 125 -o ' + key + 'OF_impacts.json -n Float -d ' + WorkSpace + ' --redefineSignalPOIs ' + key + ' --floatOtherPOIs 1 -t -1 ' + Exclude 
        RunCommand2 = 'plotImpacts.py -i ' + key + 'OF_impacts.json -o OthersFloat_' + key + '_impacts --label-size 0.03 --cms-label ,other_WCs_float'
        RunCommand3 = 'plotImpacts.py -i OF_impacts.json -o OthersFloat_' + key + '_onlySys_impacts --label-size 0.03 --cms-label ,other_WCs_float' + ' --POI ' + key
        print RunCommand2
#        Jobs1.append(RunCommand1)
#        Jobs2.append(RunCommand2)
#        Jobs2.append(RunCommand3)
#    Jobs1.append('combineTool.py -M Impacts -m 125 -o OF_impacts.json -n Float -d ' + WorkSpace + ' --redefineSignalPOIs ' + (','.join(Couplings)) + ' --floatOtherPOIs 1 -t -1 ' + Exclude)
    print 'combineTool.py -M Impacts -m 125 -o OF_impacts.json -n Float -d ' + WorkSpace + ' --redefineSignalPOIs ' + (','.join(Couplings)) + ' --floatOtherPOIs 1 -t -1 ' + Exclude
    Parallel(n_jobs=30)(delayed(f)(i) for i in Jobs1)
    Parallel(n_jobs=30)(delayed(f)(i) for i in Jobs2)
