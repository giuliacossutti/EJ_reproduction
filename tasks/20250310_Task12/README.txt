Author: Giulia Cossutti
Date: 10th March 2025
Task: Task12

Run obtained with 
DelphesPythia8 ./EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl ./EJ_reproduction/pythia8_cards/background_Top.cmnd ./EJ_reproduction/tasks/20250310_Task12/50K_bkg_Top.root > ./EJ_reproduction/tasks/20250310_Task12/50K_bkg_Top.log &
DelphesPythia8 ./EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl ./EJ_reproduction/pythia8_cards/background_HardQCD.cmnd ./EJ_reproduction/tasks/20250310_Task12/50K_bkg_HardQCD.root > ./EJ_reproduction/tasks/20250310_Task12/50K_bkg_HardQCD.log &

Description:
- Background simulation for Top:all and HardQCD:all (2->2 QCD jets) processes. pTHatMin=200. Gev/c for HardQCD, as pTmin of large jets.
- Simulated 50K events for each background with standard ATLAS card plus Reclustering: FastJetFinder with anti-kT R=0.4 and pTmin=20 GeV,
  ReclusteredJetFinder with anti-kT R=1.0 and pTmin=200 GeV with input from JetEnergyScale/jets.
  Also GenJetFinder and ReclusteredGenJetFinder with same parameter values, input(ReclusteredGenJetFinder)=output(GenJetFinder)
- Studies on large jets by filling histograms of signal and 2 bkg and then stacking and merging

Pythia8 card file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/background_Top.cmnd
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/background_HardQCD.cmnd

Delphes card file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl

Data file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250310_Task12/50K_bkg_Top.root
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250310_Task12/50K_bkg_HardQCD.root

Log file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250310_Task12/50K_bkg_Top.log
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250310_Task12/50K_bkg_HardQCD.log

Analysis file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250310_Task12/BuildHistos.C
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250310_Task12/MergeHistos.C

SPECIAL NOTE: EVENT SELECTION AND OBJECT DEFINITION
All plots and plot files end with:
*_0: consider only events with >=2 small jets with |eta|<2.4 and pT<125 GeV + only small jets with |eta|<2.5 + only large jets with |eta|<1.8
*_1: consider only events with >=2 small jets with |eta|<2.4 + only small jets with |eta|<2.5 + only large jets with |eta|<1.8
*_2: consider all events + only small jets with |eta|<2.5 + only large jets with |eta|<1.8

Two series of plots were made:
*.pdf : stack of 2 backgrounds + overlapped signal
*_stack.pdf : stack of 2 backgrounds and signal

Plot file:
- signal.root : all histos from signal
- bkg_Top.root : all histos from Top background
- bkg_HardQCD.root : all histos from HardQCD background

Plots:
- InvMass.pdf : Invariant mass of first 2 large jets
- Eta1.pdf : Eta of leading large jet
- Eta2.pdf : Eta of subleading large jet
