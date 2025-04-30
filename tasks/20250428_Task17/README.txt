Author: Giulia Cossutti
Date: 28th April 2025
Task: Task17

Run obtained with ht-condor
condor_submit file.sub
condor_q
condor_transfer_data ProcessID

File to make pythia8 cards:
makepythia8cards.sh

Ht-condor files:
500K_condor_signal.sh
500K_condor_signal.sub
500K_condor_bkgTop.sh
500K_condor_bkgTop.sub
500K_condor_bkgHardQCD.sh
500K_condor_bkgHardQCD.sub

Description:
- Simulation of 500K events of signal and 2 bkgs with ht-condor.
- Reproduction of the High mjj selection on signal and bkg samples. Used 2 types of selection. 

Pythia8 card file: obtained from
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/condor_signal.cmnd
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/condor_bkgTop.cmnd
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/condor_bkgHardQCD.cmnd
through
source makepythia8cards.sh

Delphes card file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl

Data file:
/gfsvol01/atlas/giuliac/condor/20250428_Task17/root_outputs/50K_sample_number.root

Log file:
/gfsvol01/atlas/giuliac/condor/20250428_Task17/log_outputs/50K_sample_number.log

Condor output files:
/gfsvol01/atlas/giuliac/condor/20250428_Task17/condor_out/sample_job_number.out

Analysis file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250428_Task17/HighMjj.C
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250428_Task17/MergeHighMjj.C

SPECIAL NOTE:
If file names contain:
_sample0 : signal
_sample1 : bkg_Top
_sample2 : bkg_HardQCD

Plot files:
Source of histograms, from HighMjj.C:
- signal.root : all histos from signal
- bkgTop.root : all histos from Top background
- bkgHardQCD.root : all histos from HardQCD background
Produced through them:
- histos_highmjj.root
Inside this file, names of signal histos end with _0, bkg histos 
end with _1 (bkgTop) and _2 (bkgHardQCD).

Plots:
- PTF1_PTF2.pdf : ABCD plane with PTF of leading and subleading large jet on axes
- PTF1noz_PTF2noz.pdf : ABCD plane with PTF_noDeltazcut of leading and subleading large jet on axes
