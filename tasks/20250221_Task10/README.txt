Author: Giulia Cossutti
Date: 21st February 2025
Task: Task10

Run obtained with 
DelphesPythia8 ./EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl ./EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50.cmnd ./EJ_reproduction/tasks/20250221_Task10/50K_Reclustering.root > ./EJ_reproduction/tasks/20250221_Task10/50K_Reclustering.log &

Description:
- Simulated 50K events with standard ATLAS card plus Reclustering: FastJetFinder with anti-kT R=0.4 and pTmin=20 GeV,
  ReclusteredJetFinder with anti-kT R=1.0 and pTmin=200 GeV with input from JetEnergyScale/jets.
  Also GenJetFinder and ReclusteredGenJetFinder with same parameter values, input(ReclusteredGenJetFinder)=output(GenJetFinder)
- 3 types of plots, with 3 different event selections and small and large jet definitions

Pythia8 card file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50.cmnd

Delphes card file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl

Data file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250210_Task8/50K_Reclustering.root

Log file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250210_Task8/50K_Reclustering.log

Analysis file:
Reclustering.C

SPECIAL NOTE: EVENT SELECTION AND OBJECT DEFINITION
All plots and plot file end with:
*nothing: consider only events with >=2 small jets with |eta|<2.4 and pT<125 GeV + only small jets with |eta|<2.5 + only large jets with |eta|<1.8
*_nopT: consider only events with >=2 small jets with |eta|<2.4 + only small jets with |eta|<2.5 + only large jets with |eta|<1.8
*_nopTeta: consider all events
Plots ending with _Norm are normalized by entries and bin width

Plot file:
plots.root
plots_nopT.root
plots_nopTeta.root

Plots:
- Njets.pdf : Number of small and large jets passing selection criteria and constituents of small and large jets
- Invmass.pdf : Invariant mass of first 2 and 4 small jets, first 2 large jets
- DeltaR.pdf : DeltaR between leading and subleading small and large jets, and for events with exactly 2 large jets
- DeltaEta.pdf : DeltaEta between leading and subleading small and large jets, and for events with exactly 2 large jets
- DeltaPhi.pdf : DeltaPhi between leading and subleading small and large jets, and for events with exactly 2 large jets
- JetPT.pdf : jet pT of all, leading, subleading, sum of first 2 small and large, sum of first 4 small
