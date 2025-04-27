Author: Giulia Cossutti
Date: 18th March 2025
Task: Task13

Run obtained with 
DelphesPythia8 ./EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl ./EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50.cmnd ./EJ_reproduction/tasks/20250221_Task10/50K_Reclustering.root > ./EJ_reproduction/tasks/20250221_Task10/50K_Reclustering.log &
DelphesPythia8 ./EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl ./EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50_ppbar.cmnd ./EJ_reproduction/tasks/20250307_Task12/50K_Reclustering_ppbar.root > ./EJ_reproduction/tasks/20250307_Task12/50K_Reclustering_ppbar.log &

Description:
- Studies on Zv's daughters: 3 types of plots, with 3 different event selections and small and large jet definitions.
  Plotted Zv's daughters' invariant mass distinguishing between the quark type of Zv's mothers. Plots done for pp
  and ppbar events to search for differences due to PDFs.

Pythia8 card file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50.cmnd
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50_ppbar.cmnd

Delphes card file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl

Data file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250221_Task10/50K_Reclustering.root
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250307_Task12/50K_Reclustering_ppbar.root

Log file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250221_Task10/50K_Reclustering.log
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250307_Task12/50K_Reclustering_ppbar.log

Analysis file:
PDF.C

SPECIAL NOTE: EVENT SELECTION AND OBJECT DEFINITION
All plots and plot file end with:
*nothing or _0: consider only events with >=2 small jets with |eta|<2.4 and pT<125 GeV + only small jets with |eta|<2.5 + only large jets with |eta|<1.8
*_nopT or _1: consider only events with >=2 small jets with |eta|<2.4 + only small jets with |eta|<2.5 + only large jets with |eta|<1.8
*_nopTeta or _2: consider all events + only small jets with |eta|<2.5 + only large jets with |eta|<1.8
Plots ending with _Norm are normalized by entries and bin width
Plots ending with _pp are from pp events, plots ending with _ppbar are from ppbar events.

Plot file:
- PDF_plots.root

Plots:
- qDInvmass.pdf : Invariant mass of Zv's daughters distinguishing between types of Zv's parents.
