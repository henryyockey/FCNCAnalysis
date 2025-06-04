combineTool.py -M Impacts \
    -m 125 --doInitialFit --robustFit 1  -t -1 -n k_cQe \
    -d model_test.root \
    -P k_cQe \
    --floatOtherPOIs 1\
    --setParameters r=1,k_cte=0,k_ctl=0,k_cQe=0,k_cQlM=0,k_ctlT=0,k_ctlS=0 \
    --setParameterRanges k_cte=-1,1:k_ctl=-1:1:k_cQe=-1,1:k_cQlM=-1,1:k_ctlT=-1,1:k_ctlS=-1,1 \
    --freezeParameters r,k_cte,k_ctl,k_cQe,k_cQlM,k_ctlT,k_ctlS \
    --exclude r,k_cte,k_ctl,k_cQe,k_cQlM,k_ctlT,k_ctlS

combineTool.py -M Impacts  -m 125 --robustFit 1 --doFits  -t -1 -n k_cQe \
    -d model_test.root  \
    --redefineSignalPOIs k_cQe  \
    --setParameters r=1,k_cte=0,k_ctl=0,k_cQe=0,k_cQlM=0,k_ctlT=0,k_ctlS=0 \
    --setParameterRanges k_cte=-1,1:k_ctl=-1:1:k_cQe=-1,1:k_cQlM=-1,1:k_ctlT=-1,1:k_ctlS=-1,1 \
    --freezeParameters r,k_cte,k_ctl,k_cQe,k_cQlM,k_ctlT,k_ctlS \
    --exclude r,k_cte,k_ctl,k_cQe,k_cQlM,k_ctlT,k_ctlS


combineTool.py -M Impacts  -m 125 \
   -o k_cQe_impacts.json -n k_cQe  -t -1 -d model_test.root \
  --redefineSignalPOIs k_cQe \
  --exclude r,k_cte,k_ctl,k_cQe,k_cQlM,k_ctlT,k_ctlS \

plotImpacts.py -i k_cQe_impacts.json -o impacts
