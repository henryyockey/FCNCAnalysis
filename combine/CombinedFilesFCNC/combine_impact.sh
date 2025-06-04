#!/bin/bash

# Function to print and execute a command
run_command() {
    echo "Running command: $1"
    eval "$1"
}

# Check if a parameter is passed as argument
if [ -z "$1" ]; then
  echo "Error: No parameter of interest (POI) specified!"
  echo "Usage: ./run_impacts.sh <parameter_of_interest>"
  exit 1
fi

# Parameter of interest (POI) passed as argument
PARAM1="$1"

# List of all possible parameters
ALL_PARAMS="k_cQe k_cQlM k_ctlT k_ctl k_ctlS k_cte"

# Generate list of frozen parameters by excluding the POI
FREEZE_PARAMS=$(echo $ALL_PARAMS | sed "s/\b$PARAM1\b//g")

# Command 1: Initial fit
combine_command_1="combineTool.py -M Impacts -m 125 --doInitialFit --robustFit 1 -t -1 -n ${PARAM1} -d model_test.root -P ${PARAM1} --floatOtherPOIs 1 --setParameters r=1,${PARAM1}=0 --setParameterRanges ${PARAM1}=-0.1,0.1:k_ctlT=-1,1:k_ctlS=-1,1:k_cte=-1,1:k_ctl=-1,1 --freezeParameters r,${FREEZE_PARAMS}"

# Print and run the first combine command
run_command "$combine_command_1"

# Command 2: Perform fits
combine_command_2="combineTool.py -M Impacts -m 125 --robustFit 1 --doFits -t -1 -n ${PARAM1} -d model_test.root --redefineSignalPOIs ${PARAM1} --setParameters r=1,${PARAM1}=0 --setParameterRanges ${PARAM1}=-0.1,0.1:k_ctlT=-1,1:k_ctlS=-1,1:k_cte=-1,1:k_ctl=-1,1 --freezeParameters r,${FREEZE_PARAMS}"

# Print and run the second combine command
run_command "$combine_command_2"

# Command 3: Output the impacts to a json file
combine_command_3="combineTool.py -M Impacts -m 125 -o ${PARAM1}_impacts.json -n ${PARAM1} -t -1 -d model_test.root --redefineSignalPOIs ${PARAM1} --exclude r,${FREEZE_PARAMS}"

# Print and run the third combine command
run_command "$combine_command_3"

# Command 4: Plot the impacts
plot_command="plotImpacts.py -i ${PARAM1}_impacts.json -o impacts_${PARAM1}"

# Print and run the plot command
run_command "$plot_command"

