Author: Giulia Cossutti
Date: 31st March 2025
Task: Task15

Run obtained with 
DelphesPythia8 ./EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl ./EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50_ddbar.cmnd ./EJ_reproduction/tasks/20250331_Task15/50K_Reclustering_ddbar.root > ./EJ_reproduction/tasks/20250331_Task15/50K_Reclustering_ddbar.log &

Description:
- Simulated 50K events with Zv decaying only to ddbar.
- Comparison between Number of Constituents of Large Jets with signal or this sample.

Pythia8 card file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50_ddbar.cmnd

Delphes card file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl

Data file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250331_Task15/50K_Reclustering_ddbar.root

Log file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250331_Task15/50K_Reclustering_ddbar.log

Analysis file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250331_Task15/BuildHistos.C
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250331_Task15/MergeHistos.C

Files ending with _ refer to:
*_0 or _sel0: consider only events with >=2 small jets with |eta|<2.4 and pT<125 GeV + only small jets with |eta|<2.5 + only large jets with |eta|<1.8
*_1 or _sel1: consider only events with >=2 small jets with |eta|<2.4 + only small jets with |eta|<2.5 + only large jets with |eta|<1.8
*_2 or _sel2: consider all events + only small jets with |eta|<2.5 + only large jets with |eta|<1.8

Plot file:
signal_ddbar.root

Plots:
- NConst.pdf : number of constituents of leading and subleading large jets. Comparison between signal, Zv -> d dbar and two backgrounds.
