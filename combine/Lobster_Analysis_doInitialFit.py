import datetime
import os
from os import path
import sys
from lobster import cmssw
from lobster.core import AdvancedOptions, Category, Config, MultiProductionDataset, StorageConfiguration, Workflow, Dataset,ParentDataset, EmptyDataset
sys.path.append(os.path.abspath("."))

timestamp_tag = datetime.datetime.now().strftime('%Y%m%d_%H%M')
username = "rgoldouz"
production_tag = "LimitsFCNC"            # For 'full_production' setup

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
        "file:///cms/cephfs/data/store/user/",
        "root://hactar01.crc.nd.edu//store/user/",
    ],
    output=[
        # Until a separate bug is fixed file://cms/cephfs needs to be the first output so the initial lobster validation passes.
        "file:///cms/cephfs/data"+output_path,
        "root://hactar01.crc.nd.edu/"+output_path,
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

CouplingsDict = {
"k_ctp":'-1,1',"k_ctlS":'-1,1',"k_cte":'-1,1',"k_ctl":'-1,1',"k_ctlT":'-1,1',"k_ctZ":'-1,1',"k_cpt":'-1,1',"k_cpQM":'-1,1',"k_ctA":'-1,1',"k_cQe":'-1,1',"k_ctG":'-1,1',"k_cQlM":'-1,1'
}
FrozenSys=[]
Couplings = ["k_ctp","k_ctlS","k_cte","k_ctl","k_ctlT","k_ctZ","k_cpt","k_cpQM","k_ctA","k_cQe","k_ctG","k_cQlM"]
Exclude =''
if len(FrozenSys)>0:
    Exclude =' --freezeParameters ' + (','.join(FrozenSys))  
WorkSpace = 'FCNC_lep1Pt_statOnly.root'
setParam = ''
setParamRange = ''
for key, value in CouplingsDict.items():
    setParam += key + '=0,'
    setParamRange += key + '=' + value+':'

setParamRange = setParamRange[:-1]
for key, value in CouplingsDict.items():
    JobName = 'doFitFrozen_' + key
    Couplings.remove(key)
    RunCommand = 'combine -M MultiDimFit --algo=grid --points 200 --verbose -1 -m 125   -t -1 -n ' + key + ' -d ' + WorkSpace +  ' -P ' + key  + ' --floatOtherPOIs 0 --setParameters r=1,' + setParam + ' --setParameterRanges '+ setParamRange + ' --freezeParameters r,' + (','.join(Couplings))
    Couplings.append(key)
    print RunCommand
    Analysis = Workflow(
        label=JobName,
        sandbox=cmssw.Sandbox(release='/afs/crc.nd.edu/user/r/rgoldouz/Limit_combined/forLobster/CMSSW_10_2_13'),
        globaltag=False,
        command=RunCommand,
        extra_inputs=[
            WorkSpace,
        ],
        outputs=['higgsCombine_initialFit_' + key+'.MultiDimFit.mH125.root'],
        dataset=EmptyDataset(),
        category=gs_resources
    )
    wf.append(Analysis)
JobName = 'doInitialFitFloat'
RunCommand = 'combineTool.py -M Impacts -m 125 --doInitialFit --robustFit 1 --redefineSignalPOIs ctZ,cQl3i,ctG,cQq13,cQq11,cptb,ctli,ctp,cpt,ctlTi,ctq1,ctq8,cQlMi,cQQ1,cQt1,cQt8,ctt1,cQq81,cQq83,cbW,cpQ3,ctei,cQei,ctW,cpQM,ctlSi -n Float -d ' + WorkSpace   + ' --verbose 3 -t -1 --setParameters ' + setParam + ' --setParameterRanges '+ setParamRange  + Exclude

#print RunCommand

Analysis = Workflow(
    label=JobName,
    sandbox=cmssw.Sandbox(release='/afs/crc.nd.edu/user/r/rgoldouz/Limit_combined/forLobster/CMSSW_10_2_13'),
    globaltag=False,
    command=RunCommand,
    extra_inputs=[
       WorkSpace,
    ],
    outputs=['higgsCombine_initialFit_Float.MultiDimFit.mH125.root'],
    dataset=EmptyDataset(),
    category=gs_resources
)
#wf.append(Analysis)

config = Config(
    label=master_label,
    workdir=workdir_path,
    plotdir=plotdir_path,
    storage=storage,
    workflows=wf,
    advanced=AdvancedOptions(
        bad_exit_codes=[127, 160],
        log_level=1,
        osg_version='3.6',
        abort_threshold=0,
        abort_multiplier=100,
    )
)

