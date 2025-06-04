import datetime
import os
from os import path
import sys
from lobster import cmssw
from lobster.core import AdvancedOptions, Category, Config, MultiProductionDataset, StorageConfiguration, Workflow, Dataset,ParentDataset, EmptyDataset
sys.path.append(os.path.abspath("."))

cmsswbase = os.environ['CMSSW_BASE']
timestamp_tag = datetime.datetime.now().strftime('%Y%m%d_%H%M')

username = "rgoldouz"

production_tag = "LimitsTTXEFTdoFitForNuisance"            # For 'full_production' setup

# Only run over lhe steps from specific processes/coeffs/runs
process_whitelist = []
coeff_whitelist   = []
runs_whitelist    = []  # (i.e. MG starting points)

master_label = '%s_%s' % (production_tag,timestamp_tag)

input_path   = "/store/user/"
output_path  = "/store/user/$USER/FullProduction/%s" % (production_tag)
workdir_path = "/tmpscratch/users/$USER/FullProduction/%s" % (production_tag)
plotdir_path = "~/www/lobster/FullProduction/%s" % (production_tag)


storage = StorageConfiguration(
    input=[
    "hdfs://eddie.crc.nd.edu:19000"  + input_path,
    "root://deepthought.crc.nd.edu/" + input_path
    ],
    output=[
        "hdfs://eddie.crc.nd.edu:19000"  + output_path,
        "root://deepthought.crc.nd.edu/" + output_path, # Note the extra slash after the hostname!
        "gsiftp://T3_US_NotreDame"       + output_path,
        "srm://T3_US_NotreDame"          + output_path,
        "file:///hadoop"                 + output_path,
    ],
    disable_input_streaming=True,
)

#################################################################
# Worker Res.:
#   Cores:  12    | 4
#   Memory: 16000 | 8000
#   Disk:   13000 | 6500
#################################################################
gs_resources = Category(
    name='gs',
    cores=1,
    memory=8000,
    disk=8000
)
#################################################################
wf = []


for root, dirs, files in os.walk('/hadoop/store/user/rgoldouz/FullProduction/LimitsTTXEFTdoInitialFit'):
    if len(files) > 0:
        for f in files:
            os.system('cp ' + root +'/'+ f + ' InitialFit/' + ('_'.join(f.split('_')[:-1]))+'.root')


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
    Exclude =' --freezeParameters ' + (','.join(FrozenSys))
Nuisance = ['lepSF_muon', 'lepSF_elec', 'btagSFbc_2016', 'btagSFbc_corr', 'btagSFlight_2016', 'PU', 'PreFiring', 'triggerSF', 'FSR','ISR', 'renormfact','JES','btagSFbc_2016APV', 'btagSFlight_2016APV', 'btagSFbc_2018', 'btagSFlight_2018', 'btagSFbc_2017', 'btagSFlight_2017', 'lumi_flat', 'pdf_scale_qq_flat','qcd_scale_VV_flat','qcd_scale_VVV_flat', 'FF', 'FFpt', 'FFeta', 'FFcloseEl_2016', 'FFcloseMu_2016', 'FFcloseEl_2016APV', 'FFcloseMu_2016APV', 'FFcloseEl_2017', 'FFcloseMu_2017', 'FFcloseEl_2018', 'FFcloseMu_2018', 'pdf_scale_qg_flat', 'qcd_scale_tHq_flat', 'qcd_scale_V_flat', 'pdf_scale_gg_flat', 'qcd_scale_ttH_flat', 'qcd_scale_ttll_flat', 'qcd_scale_ttlnu_flat','btagSFlight_corr','charge_flips', 'JER_2016', 'JER_2016APV', 'JER_2017', 'JER_2018', '']
WorkSpace = 'njets_fullR2_rmNegativeUncty_withSys_anatest01.root'

setParam = ''
setParamRange = ''
for key, value in CouplingsDict.items():
    setParam += key + '=0,'
    setParamRange += key + '=' + value+':'

for key, value in CouplingsDict.items():
    for sys in Nuisance:
        if sys in FrozenSys:
            continue
        JobName = 'doFitForNuisance' + key + sys
        if path.exists('/hadoop/store/user/rgoldouz/FullProduction/LimitsTTXEFTdoFitForNuisance/' + JobName):
            if len(os.listdir('/hadoop/store/user/rgoldouz/FullProduction/LimitsTTXEFTdoFitForNuisance/' + JobName))>0:
                continue
            else:
                os.system('rm -rf /hadoop/store/user/rgoldouz/FullProduction/LimitsTTXEFTdoFitForNuisance/' + JobName)
        Couplings.remove(key)
        RunCommand = 'combine -M MultiDimFit --algo impact --robustFit 1 -m 125 -n _paramFit_' + key + '_'+ sys + ' -d ' + WorkSpace + ' --redefineSignalPOIs ' + key + ' -P ' + sys + ' -P ' + key + ' --floatOtherPOIs 0 -t -1 --setParameters ' + setParam + ' --setParameterRanges '+ setParamRange + ' --freezeParameters ' + (','.join(Couplings))
        Couplings.append(key)
#        print RunCommand
        Analysis = Workflow(
            label=JobName,
            sandbox=cmssw.Sandbox(release='/afs/crc.nd.edu/user/r/rgoldouz/Limit_combined/forLobster/CMSSW_10_2_13'),
            globaltag=False,
            command=RunCommand,
            extra_inputs=[
                WorkSpace,
                'InitialFit/higgsCombine_initialFit_'+key +'.MultiDimFit.mH125.root'
            ],
            outputs=['higgsCombine_paramFit_'+ key + '_' + sys + '.MultiDimFit.mH125.root'],
            dataset=EmptyDataset(),
            category=gs_resources
        )
        wf.append(Analysis)

for sys in Nuisance:
    if sys in FrozenSys:
        continue
    JobName = 'doFitForNuisanceFloat' + sys
    RunCommand = 'combine -M MultiDimFit --algo impact --robustFit 1 -m 125 -n _paramFit_Float' + '_'+ sys + ' -d ' + WorkSpace + ' -P ' + sys + ' --floatOtherPOIs 1 -t -1 --setParameters ' + setParam + ' --setParameterRanges '+ setParamRange +  Exclude 
    Analysis = Workflow(
        label=JobName,
        sandbox=cmssw.Sandbox(release='/afs/crc.nd.edu/user/r/rgoldouz/Limit_combined/forLobster/CMSSW_10_2_13'),
        globaltag=False,
        command=RunCommand,
        extra_inputs=[
            WorkSpace,
            'InitialFit/higgsCombine_initialFit_Float.MultiDimFit.mH125.root'
        ],
        outputs=['higgsCombine_paramFit_Float'+ '_' + sys + '.MultiDimFit.mH125.root'],
        dataset=EmptyDataset(),
        category=gs_resources
    )
#    wf.append(Analysis)
for key, value in CouplingsDict.items():
    JobName = 'doFitForNuisanceFloat' + key
    RunCommand = 'combine -M MultiDimFit --algo impact --robustFit 1 -m 125 -n _paramFit_Float' + '_'+ key + ' -d ' + WorkSpace + ' -P ' + key + ' --floatOtherPOIs 1 -t -1 --setParameters ' + setParam + ' --setParameterRanges '+ setParamRange + Exclude
    Analysis = Workflow(
        label=JobName,
        sandbox=cmssw.Sandbox(release='/afs/crc.nd.edu/user/r/rgoldouz/Limit_combined/forLobster/CMSSW_10_2_13'),
        globaltag=False,
        command=RunCommand,
        extra_inputs=[
            WorkSpace,
            'InitialFit/higgsCombine_initialFit_Float.MultiDimFit.mH125.root'
        ],
        outputs=['higgsCombine_paramFit_Float'+ '_' + key + '.MultiDimFit.mH125.root'],
        dataset=EmptyDataset(),
        category=gs_resources
    )
#    wf.append(Analysis)
#    print RunCommand
config = Config(
    label=master_label,
    workdir=workdir_path,
    plotdir=plotdir_path,
    storage=storage,
    workflows=wf,
    advanced=AdvancedOptions(
        bad_exit_codes=[127, 160],
        log_level=1,
        payload=10,
        dashboard = False,
        xrootd_servers=['ndcms.crc.nd.edu',
                       'cmsxrootd.fnal.gov',
                       'deepthought.crc.nd.edu'],
    )
)

