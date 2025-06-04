import datetime
import os
from os import path
import sys
from lobster import cmssw
from lobster.core import AdvancedOptions, Category, Config, MultiProductionDataset, StorageConfiguration, Workflow, Dataset,ParentDataset
sys.path.append(os.path.abspath("."))
import Files_2017_nano
import Files_ULall_nano1
import Files_ULall_nano_FCNC
SAMPLES = {}
SAMPLES.update(Files_ULall_nano1.UL17)
#SAMPLES.update(Files_ULall_nano_FCNC.UL17)
#cmsswbase = os.environ['CMSSW_BASE']
timestamp_tag = datetime.datetime.now().strftime('%Y%m%d_%H%M')
#timestamp_tag = '000001'
username = "hyockey"

production_tag = "FCNC_nano_analysis61"            # For 'full_production' setup

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
    memory=2000,
    disk=10000
)

gsLL_resources = Category(
    name='gsLL',
    cores=1,
    memory=3900,
    disk=3900,
    mode='fixed'
)


#################################################################
wf = []
for key, value in SAMPLES.items():
#    if "UL17_WWTo2L2Nu" not in key: continue
    if "2017" not in value[3]: continue
#    if "data" not in key: continue
    print key
    if (value[3] == "UL17_mc"): value[3] = "2017"
    Analysis = Workflow(
        label='Analysis_%s' % (key),
        sandbox=cmssw.Sandbox(release='/afs/crc.nd.edu/user/h/hyockey/CMSSW_10_4_0'),
        globaltag=False,
        command='python Lobster_check.py ' + key + ' ' + value[1] +' ' + value[2] +' ' +value[3] +' ' +value[4] +' ' +value[5] +' ' +value[6] +' ' +value[7] +' ' +value[8] +' ' +value[9]+' @inputfiles',
        extra_inputs=[
            'Lobster_check.py',
            '../lib/libmain.so',
            '../lib/libcorrectionlib.so',
	    '../lib/libEFTGenReaderEFTHelperUtilities.so',
            '../include/MyAnalysis.h',
            '../lib/libRestFrames.so.1',
        ],
        outputs=['ANoutput.root'],
        dataset=Dataset(
           files=value[0],
           patterns=["*.root"],
           files_per_task =1
        ),
#        merge_command='hadd @outputfiles @inputfiles',
#        merge_size='2G',
        category=gsLL_resources
    )
    wf.append(Analysis)

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
#        bad_exit_codes=[127, 160],
#        log_level=1,
#        payload=10,
#        dashboard = False,
#        xrootd_servers=['ndcms.crc.nd.edu',
#                       'cmsxrootd.fnal.gov',
#                       'deepthought.crc.nd.edu'],
    )
)

