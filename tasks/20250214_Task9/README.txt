Author: Giulia Cossutti
Date: 14th February 2025
Task: Task9

Run obtained with 
- Subjets, anti-kT R=1.0, SoftDrop R=0.4
DelphesPythia8 ./EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_subjets.tcl ./EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50.cmnd ./EJ_reproduction/tasks/20250214_Task9/50K_subjets.root > ./EJ_reproduction/tasks/20250214_Task9/50K_subjets.log & 
- Subjets, anti-kT R=1.0, SoftDrop R=0.4, Pruning Rprun=0.4
DelphesPythia8 ./EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_subjets_Rprun.tcl ./EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50.cmnd ./EJ_reproduction/tasks/20250214_Task9/50K_subjets_Rprun.root > ./EJ_reproduction/tasks/20250214_Task9/50K_subjets_Rprun.log &

Description:
- Simulated 50K events to reproduce the EJ signal given by model D, mZv = 3 TeV, c*tau(piD) = 50 mm 
  using the delphes_card_ATLAS_EJ_reproduction_subjets.tcl : it's the standard ATLAS delphes card except from
  FastJetFinder: parameters R = 1.0, MinJetpT = 125 GeV in the anti-kt algorithm match the selection 
  criteria of the EJ ATLAS analysis. set InputArray EFlowMerger/eflow, N-subjettiness, jet grooming are added.
  SoftDrop searches for subjets with R=0.4.
- Simulated 50K events to reproduce the EJ signal given by model D, mZv = 3 TeV, c*tau(piD) = 50 mm 
  using the delphes_card_ATLAS_EJ_reproduction_subjets.tcl : it's the standard ATLAS delphes card except from
  FastJetFinder: parameters R = 1.0, MinJetpT = 125 GeV in the anti-kt algorithm match the selection 
  criteria of the EJ ATLAS analysis. set InputArray EFlowMerger/eflow, N-subjettiness, jet grooming are added.
  SoftDrop searches for subjets with R=0.4. Pruning with Rprun = 0.4 instead of default Rprun = 0.8.

Pythia8 card file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50.cmnd

Delphes card file:
- Subjets, anti-kT R=1.0, SoftDrop R=0.4
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_subjets.tcl
- Subjets, anti-kT R=1.0, SoftDrop R=0.4, Pruning Rprun=0.4
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_subjets_Rprun.tcl

Data file:
- Subjets, anti-kT R=1.0, SoftDrop R=0.4
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250210_Task8/50K_subjets.root
- Subjets, anti-kT R=1.0, SoftDrop R=0.4, Pruning Rprun=0.4
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250210_Task8/50K_subjets_Rprun.root

Log file:
- Subjets, anti-kT R=1.0, SoftDrop R=0.4
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250210_Task8/50K_subjets.log
- Subjets, anti-kT R=1.0, SoftDrop R=0.4, Pruning Rprun=0.4
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250210_Task8/50K_subjets_Rprun.log

Analysis file:
subjets.C

Plot file:
- Subjets, anti-kT R=1.0, SoftDrop R=0.4
plots.root
- Subjets, anti-kT R=1.0, SoftDrop R=0.4, Pruning Rprun=0.4
plots_Rprun.root

Plots:
- Nsubjets.pdf: Number of subjets in all and first two jets
- JetpT.pdf: all jets, leading jet, subleading jet pT, pT of sum of (leading+subleading) jets, subjets of LJet and SublJet
- Nsubjets_Rprun.pdf: Number of subjets in all and first two jets with Rprun=0.4
- JetpT_Rprun.pdf: all jets, leading jet, subleading jet pT, pT of sum of (leading+subleading) jets, subjets of LJet and SublJet with Rprun=0.4
