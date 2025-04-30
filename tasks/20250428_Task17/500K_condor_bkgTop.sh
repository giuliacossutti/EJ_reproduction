#!/bin/bash

#echo "setup cern"
#source /gfsvol01/atlas/giuliac/setup_cern.sh

cd /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/

DelphesPythia8 ./EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl /gfsvol01/atlas/giuliac/condor/20250428_Task17/pythia8_cards/bkgTop_${1}.cmnd /gfsvol01/atlas/giuliac/condor/20250428_Task17/root_outputs/50K_bkgTop_${1}.root > /gfsvol01/atlas/giuliac/condor/20250428_Task17/log_outputs/50K_bkgTop_${1}.log 
