Author: Giulia Cossutti
Date: 24th March 2025
Task: Task14

Run obtained with
DelphesPythia8 ./EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl ./EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50.cmnd ./EJ_reproduction/tasks/20250221_Task10/50K_Reclustering.root > ./EJ_reproduction/tasks/20250221_Task10/50K_Reclustering.log & 
DelphesPythia8 ./EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl ./EJ_reproduction/pythia8_cards/background_Top.cmnd ./EJ_reproduction/tasks/20250310_Task12/50K_bkg_Top.root > ./EJ_reproduction/tasks/20250310_Task12/50K_bkg_Top.log &
DelphesPythia8 ./EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl ./EJ_reproduction/pythia8_cards/background_HardQCD.cmnd ./EJ_reproduction/tasks/20250310_Task12/50K_bkg_HardQCD.root > ./EJ_reproduction/tasks/20250310_Task12/50K_bkg_HardQCD.log &

Description:
- Studies on large jets by filling histograms of signal and 2 bkg and then stacking and merging.
  Same of 20250319_Task13, but with signal xsection = sum of all bkgs xsections.
  All plots in root files to be used for figure of merit and best cuts' determination.

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
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250319_Task13/BuildHistos.C
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250324_Task14/MergeHistos.C

SPECIAL NOTE: EVENT SELECTION AND OBJECT DEFINITION
All plots and plot files end with:
*_0: consider only events with >=2 small jets with |eta|<2.4 and pT<125 GeV + only small jets with |eta|<2.5 + only large jets with |eta|<1.8
*_1: consider only events with >=2 small jets with |eta|<2.4 + only small jets with |eta|<2.5 + only large jets with |eta|<1.8
*_2: consider all events + only small jets with |eta|<2.5 + only large jets with |eta|<1.8
If they contain:
_sample0 : signal
_sample1 : bkg_Top
_sample2 : bkg_HardQCD

Two series of 1D plots were made:
*.pdf : stack of 2 backgrounds + overlapped signal
*_stack.pdf : stack of 2 backgrounds and signal

Plot file:
Source of histograms, from 20250319_Task13:
- signal.root : all histos from signal
- bkg_Top.root : all histos from Top background
- bkg_HardQCD.root : all histos from HardQCD background
Produced, to be used for figure of merit and best cuts' determination:
- histos_sel0.root
- histos_sel1.root
- histos_sel2.root
Inside these files, names of signal histos end with _0, bkg histos 
end with _1 (bkg_Top) and _2 (bkg_HardQCD).

Plots:
====== 1D plots =========
- InvMass.pdf : Invariant mass of first 2 large jets
- Eta1.pdf : Eta of leading large jet
- Eta2.pdf : Eta of subleading large jet
- DeltaR.pdf : DeltaR between leading and subleading large jets
- DeltaEta.pdf : DeltaEta between leading and subleading large jets
- DeltaPhi.pdf : DeltaPhi between leading and subleading large jets
- JetPT1.pdf : pT of leading large jet
- JetPT2.pdf : pT of subleading large jet
- NConst.pdf : Number of constituents of leading and subleading large jets
====== 2D plots =========
- DeltaPhi_DeltaEta.pdf : DeltaPhi vs DeltaEta of first 2 large jets
- DeltaR_DeltaEta.pdf : DeltaR vs DeltaEta of first 2 large jets
- DeltaR_DeltaPhi.pdf : DeltaR vs DeltaEta of first 2 large jets
- JetPT1_JetPT2.pdf : leading vs subleading large jet pT
- JetPT1_Invmass.pdf : leading large jet pT vs Invariant mass of first two large jets 
- JetPT1_NConst.pdf : leading large jet pT vs number of constituents of first two large jets 
- Invmass_NConst.pdf : Invariant mass vs number of constituents of first two large jets 
