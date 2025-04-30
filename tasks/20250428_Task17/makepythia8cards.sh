#!/bin/bash

for seed in $(seq 1 10)
do
	declare -i myseed=$((129 + $seed))
	echo $myseed
	sed s/MYSEED/$myseed/  /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/condor_signal.cmnd > /gfsvol01/atlas/giuliac/condor/20250428_Task17/pythia8_cards/signal_$seed.cmnd
	sed s/MYSEED/$myseed/  /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/condor_bkgTop.cmnd > /gfsvol01/atlas/giuliac/condor/20250428_Task17/pythia8_cards/bkgTop_$seed.cmnd
	sed s/MYSEED/$myseed/  /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/condor_bkgHardQCD.cmnd > /gfsvol01/atlas/giuliac/condor/20250428_Task17/pythia8_cards/bkgHardQCD_$seed.cmnd
done
