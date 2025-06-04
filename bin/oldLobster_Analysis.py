import datetime
import os
import os.path
import sys
from lobster import cmssw
from lobster.core import AdvancedOptions, Category, Config, MultiProductionDataset, StorageConfiguration, Workflow, Dataset,ParentDataset
#sys.path.append(os.path.join(os.path.dirname(__file__), '..'))
sys.path.append(os.path.abspath("."))
#import fcnc
import Files_2017_nano 
#from Files import mcFCNC_samples

SAMPLES = {}
#SAMPLES.update(mcFCNC_samples)
#SAMPLES.update(fcnc.mcFCNC_samples)
#SAMPLES.update(Files_2017_nano.UL17)
SAMPLES.update({
data_UL17_B_DoubleEG:[['rgoldouz/NanoAodPostProcessingUL/UL17/v1/data_UL17_B_DoubleEG/DoubleEG/crab_data_UL17_B_DoubleEG/211109_185412/0000'], 'data', 'DoubleEG', '2017', 'B', '1', '41.53', '1'],
data_UL17_E_DoubleEG:[['rgoldouz/NanoAodPostProcessingUL/UL17/v1/data_UL17_E_DoubleEG/DoubleEG/crab_data_UL17_E_DoubleEG/211109_185056/0000'], 'data', 'DoubleEG', '2017', 'E', '1', '41.53', '1'],
data_UL17_F_DoubleEG:[['rgoldouz/NanoAodPostProcessingUL/UL17/v1/data_UL17_F_DoubleEG/DoubleEG/crab_data_UL17_F_DoubleEG/211109_184307/0000'], 'data', 'DoubleEG', '2017', 'F', '1', '41.53', '1'],
data_UL17_C_DoubleEG:[['rgoldouz/NanoAodPostProcessingUL/UL17/v1/data_UL17_C_DoubleEG/DoubleEG/crab_data_UL17_C_DoubleEG/211109_183239/0000'], 'data', 'DoubleEG', '2017', 'C', '1', '41.53', '1'],
data_UL17_D_DoubleEG:[['rgoldouz/NanoAodPostProcessingUL/UL17/v1/data_UL17_D_DoubleEG/DoubleEG/crab_data_UL17_D_DoubleEG/211109_183003/0000'], 'data', 'DoubleEG', '2017', 'D', '1', '41.53', '1'],
}
)
cmsswbase = os.environ['CMSSW_BASE']
timestamp_tag = datetime.datetime.now().strftime('%Y%m%d_%H%M')

username = "hyockey"

production_tag = "FCNC_PostLHEAnalysis_data"            # For 'full_production' setup

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
        "file:///hadoop" + input_path,
        "root://deepthought.crc.nd.edu/" + input_path,  # Note the extra slash after the hostname!
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
    cores=2,
    memory=3000,
    disk=4000
)
#################################################################
wf = []
for key, value in SAMPLES.items():
    print key

    Analysis = Workflow(
        label='Analysis_%s' % (key),
        sandbox=cmssw.Sandbox(release='/afs/crc.nd.edu/user/h/hyockey/Analysis/CMSSW_10_4_0'),
        globaltag=False,
        command='python Lobster_check.py ' + key + ' ' + value[1] +' ' + value[2] +' ' +value[3] +' ' +value[4] +' ' +value[5] +' ' +value[6] +' ' +value[7] + ' ' +value[8] +' ' +value[9]+' @inputfiles',
        extra_inputs=[
            'Lobster_check.py',
            '../lib/main.so',
            '../lib/libEFTGenReaderEFTHelperUtilities.so',
            '../include/MyAnalysis.h',
        ],
        outputs=['ANoutput.root'],
        dataset=Dataset(
           files=value[0],
           patterns=["*.root"],
           files_per_task =10
        ),
#        merge_command='hadd @outputfiles @inputfiles',
#        merge_size='2G',
        category=gs_resources
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
        payload=10,
        dashboard = False,
        xrootd_servers=['ndcms.crc.nd.edu',
                       'cmsxrootd.fnal.gov',
                       'deepthought.crc.nd.edu'],
    )
)

