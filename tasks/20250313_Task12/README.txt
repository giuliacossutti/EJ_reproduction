Author: Giulia Cossutti
Date: 13th March 2025
Task: Task12

Run obtained with 
DelphesPythia8 ./EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl ./EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50.cmnd ./EJ_reproduction/tasks/20250313_Task12/50K_reclustering_test.root > ./EJ_reproduction/tasks/20250313_Task12/50K_reclustering_test.log &

Description:
- Simulated 50K events with standard ATLAS card plus Reclustering: FastJetFinder with anti-kT R=0.4 and pTmin=20 GeV,
  ReclusteredJetFinder with anti-kT R=1.0 and pTmin=200 GeV with input from JetEnergyScale/jets.
  Also GenJetFinder and ReclusteredGenJetFinder with same parameter values, input(ReclusteredGenJetFinder)=output(GenJetFinder)
- Test Pavel Demin's modifications to Delphes/modules/TreeWriter.cc to have jets as constituents of large jets

Pythia8 card file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50.cmnd

Delphes card file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl
with 
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250313_Task12/TreeWriter.cc
as Delphes/modules/TreeWriter.cc 

Data file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250313_Task12/50K_reclustering_test.root

Log file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250313_Task12/50K_reclustering_test.log

Analysis file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250313_Task12/reclustering_test.C

SPECIAL NOTE: EVENT SELECTION AND OBJECT DEFINITION
All plots and plot file end with:
*nothing: consider only events with >=2 small jets with |eta|<2.4 and pT<125 GeV + only small jets with |eta|<2.5 + only large jets with |eta|<1.8
*_nopT: consider only events with >=2 small jets with |eta|<2.4 + only small jets with |eta|<2.5 + only large jets with |eta|<1.8
*_nopTeta: consider all events + only small jets with |eta|<2.5 + only large jets with |eta|<1.8
Plots ending with _Norm are normalized by entries and bin width

Plot file:
- plots.root

Plots:
- Njets.pdf : Number of reco- and gen- small and large jets and number of their constituents
