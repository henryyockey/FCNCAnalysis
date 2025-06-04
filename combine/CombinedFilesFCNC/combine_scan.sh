#!/bin/bash

# Define the list of all possible parameters
ALL_PARAMETERS=("k_ctlT" "k_cQlM" "k_ctlS" "k_cQe" "k_cte" "k_ctl")

# Define dictionaries for default values and parameter ranges
declare -A SET_PARAMETERS_DICT=(
  ["k_ctlT"]=0
  ["k_cQlM"]=0
  ["k_ctlS"]=0
  ["k_cQe"]=0
  ["k_cte"]=0
  ["k_ctl"]=0
)

declare -A PARAMETER_RANGES_DICT=(
  ["k_ctlT"]="-0.15,0.15"
  ["k_cQlM"]="-0.15,0.15"
  ["k_ctlS"]="-0.15,0.15"
  ["k_cQe"]="-0.15,0.15"
  ["k_cte"]="-0.15,0.15"
  ["k_ctl"]="-0.15,0.15"
)

# User-provided parameter (can be passed as an argument)
PARAMETER=$1

# Validate the input parameter
if [[ ! " ${ALL_PARAMETERS[@]} " =~ " ${PARAMETER} " ]]; then
  echo "Error: Invalid parameter '${PARAMETER}'. Choose one from: ${ALL_PARAMETERS[*]}"
  exit 1
fi

# Construct the setParameters string dynamically
SET_PARAMETERS="r=1"
for param in "${ALL_PARAMETERS[@]}"; do
  SET_PARAMETERS+=",${param}=${SET_PARAMETERS_DICT[$param]}"
done

# Construct the parameterRanges string dynamically
PARAMETER_RANGES=""
for param in "${ALL_PARAMETERS[@]}"; do
  PARAMETER_RANGES+="${param}=${PARAMETER_RANGES_DICT[$param]}:"
done
# Remove the trailing colon
PARAMETER_RANGES=${PARAMETER_RANGES%:}

# Freeze all parameters except the chosen one
FREEZE_PARAMETERS=$(echo "${ALL_PARAMETERS[@]}" | sed "s/${PARAMETER}//" | tr ' ' ',')

# Define parameters for the combine command
MODEL_FILE="model_test.root"
OUTPUT_NAME="${PARAMETER}"
MASS=125
TOYS=-1

# Combine command
combine -M MultiDimFit \
  --algo=grid \
  --points 500 \
  --verbose 1 \
  -m $MASS \
  -n $OUTPUT_NAME \
  -d $MODEL_FILE \
  -P $PARAMETER \
  -t $TOYS \
  --setParameters $SET_PARAMETERS \
  --setParameterRanges $PARAMETER_RANGES \
  --freezeParameters r,$FREEZE_PARAMETERS

# Define parameters for mkEFTScan.py
SCAN_SCRIPT="$HOME/AAC/CMSSW_10_2_13/src/HiggsAnalysis/AnalyticAnomalousCoupling/scripts/mkEFTScan.py"
INPUT_FILE="higgsCombine${OUTPUT_NAME}.MultiDimFit.mH${MASS}.root"
OUTPUT_DIR="finalOutput/1DScan_${PARAMETER}_asimov"
MAXNLL=500
LUMI=138
XLABEL="${PARAMETER}_scan"

# mkEFTScan command
python $SCAN_SCRIPT \
  $INPUT_FILE \
  -p $PARAMETER \
  -o $OUTPUT_DIR \
  -maxNLL $MAXNLL \
  -lumi $LUMI \
  -cms \
  -preliminary \
  -xlabel "$XLABEL"

echo "combine -M MultiDimFit \
  --algo=grid \
  --points 500 \
  --verbose 1 \
  -m ${MASS} \
  -n ${OUTPUT_NAME} \
  -d ${MODEL_FILE} \
  -P ${PARAMETER} \
  -t ${TOYS} \
  --setParameters ${SET_PARAMETERS} \
  --setParameterRanges ${PARAMETER_RANGES} \
  --freezeParameters r,${FREEZE_PARAMETERS}"

echo "python ${SCAN_SCRIPT} \
  ${INPUT_FILE} \
  -p ${PARAMETER} \
  -o ${OUTPUT_DIR} \
  -maxNLL ${MAXNLL} \
  -lumi ${LUMI} \
  -cms \
  -preliminary \
  -xlabel "${XLABEL}""

