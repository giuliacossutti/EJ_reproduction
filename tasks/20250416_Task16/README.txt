Author: Giulia Cossutti
Date: 16th April 2025
Task: Task16

Run obtained with
DelphesPythia8 ./EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl ./EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50.cmnd ./EJ_reproduction/tasks/20250221_Task10/50K_Reclustering.root > ./EJ_reproduction/tasks/20250221_Task10/50K_Reclustering.log & 
DelphesPythia8 ./EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl ./EJ_reproduction/pythia8_cards/background_Top.cmnd ./EJ_reproduction/tasks/20250310_Task12/50K_bkg_Top.root > ./EJ_reproduction/tasks/20250310_Task12/50K_bkg_Top.log &
DelphesPythia8 ./EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl ./EJ_reproduction/pythia8_cards/background_HardQCD.cmnd ./EJ_reproduction/tasks/20250310_Task12/50K_bkg_HardQCD.root > ./EJ_reproduction/tasks/20250310_Task12/50K_bkg_HardQCD.log &

Description:
- Reproduction of the High mjj selection on signal and bkg samples. Used 2 types of selection. 

Pythia8 card file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50.cmnd
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/background_Top.cmnd
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/background_HardQCD.cmnd

Delphes card file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl

Data file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250221_Task10/50K_Reclustering.root
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250310_Task12/50K_bkg_Top.root
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250310_Task12/50K_bkg_HardQCD.root

Log file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250221_Task10/50K_Reclustering.log
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250310_Task12/50K_bkg_Top.log
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250310_Task12/50K_bkg_HardQCD.log

Analysis file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250416_Task16/HighMjj.C
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250416_Task16/MergeHighMjj.C

SPECIAL NOTE:
If file names contain:
_sample0 : signal
_sample1 : bkg_Top
_sample2 : bkg_HardQCD

Plot files:
Source of histograms, from HighMjj.C:
- signal.root : all histos from signal
- bkg_Top.root : all histos from Top background
- bkg_HardQCD.root : all histos from HardQCD background
Produced through them:
- histos_highmjj.root
Inside this file, names of signal histos end with _0, bkg histos 
end with _1 (bkg_Top) and _2 (bkg_HardQCD).

Plots:
- PTF1_PTF2.pdf : ABCD plane with PTF of leading and subleading large jet on axes
- PTF1noz_PTF2noz.pdf : ABCD plane with PTF_noDeltazcut of leading and subleading large jet on axes
