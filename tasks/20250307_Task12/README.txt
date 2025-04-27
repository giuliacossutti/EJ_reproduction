Author: Giulia Cossutti
Date: 7th March 2025
Task: Task12

Run obtained with 
DelphesPythia8 ./EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl ./EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50_ppbar.cmnd ./EJ_reproduction/tasks/20250307_Task12/50K_Reclustering_ppbar.root > ./EJ_reproduction/tasks/20250307_Task12/50K_Reclustering_ppbar.log &

Description:
- Analogous to Reclustering but with ppbar collision instead of pp (pythia8card differs only in this)
- Simulated 50K events with standard ATLAS card plus Reclustering: FastJetFinder with anti-kT R=0.4 and pTmin=20 GeV,
  ReclusteredJetFinder with anti-kT R=1.0 and pTmin=200 GeV with input from JetEnergyScale/jets.
  Also GenJetFinder and ReclusteredGenJetFinder with same parameter values, input(ReclusteredGenJetFinder)=output(GenJetFinder)
- Studies on Zv's daughters: 3 types of plots, with 3 different event selections and small and large jet definitions

Pythia8 card file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50_ppbar.cmnd

Delphes card file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl

Data file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250307_Task12/50K_Reclustering_ppbar.root

Log file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250307_Task12/50K_Reclustering_ppbar.log

Analysis file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250307_Task12/GenParticles.C

SPECIAL NOTE: EVENT SELECTION AND OBJECT DEFINITION
All plots and plot file from Reclustering_Truth.C end with:
*nothing: consider only events with >=2 small jets with |eta|<2.4 and pT<125 GeV + only small jets with |eta|<2.5 + only large jets with |eta|<1.8
*_nopT: consider only events with >=2 small jets with |eta|<2.4 + only small jets with |eta|<2.5 + only large jets with |eta|<1.8
*_nopTeta: consider all events and all jets
All plots and plot file from GenParticles.C end with:
*nothing: consider only events with >=2 small jets with |eta|<2.4 and pT<125 GeV + only small jets with |eta|<2.5 + only large jets with |eta|<1.8
*_nopT: consider only events with >=2 small jets with |eta|<2.4 + only small jets with |eta|<2.5 + only large jets with |eta|<1.8
*_nopTeta: consider all events (no jets are used)
Plots ending with _Norm are normalized by entries and bin width

Plot file:
===from GenParticles.C===
- qD.root

Plots:
===from GenParticles.C===
- Invmass.pdf : Invariant mass of Zv's daughters
- DeltaR.pdf : DeltaR between Zv's daughters
- DeltaEta.pdf : DeltaEta between Zv's daughters
- DeltaPhi.pdf : DeltaPhi between Zv's daughters
- Eta.pdf : Eta of Zv's daughters
- PT.pdf : PT of all, leading and subleading Zv's daughters
