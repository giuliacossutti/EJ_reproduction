Author: Giulia Cossutti
Date: 10th February 2025
Task: Task8

Run obtained with
DelphesPythia8 ./EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_NoSoftDrop.tcl ./EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50.cmnd ./EJ_reproduction/tasks/20250210_Task8/50K_NoSoftDrop.root > ./EJ_reproduction/tasks/20250210_Task8/50K_NoSoftDrop.log &
DelphesPythia8 ./EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction.tcl ./EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50.cmnd ./EJ_reproduction/tasks/20250210_Task8/50K.root > ./EJ_reproduction/tasks/20250210_Task8/50K.log &

Description:
- Simulated 50K events to reproduce the EJ signal given by model D, mZv = 3 TeV, c*tau(piD) = 50 mm 
  using the delphes_card_ATLAS_EJ_reproduction_NoSoftDrop.tcl : it's the standard ATLAS delphes card except from
  two FastJetFinder parameters: R = 0.4, MinJetpT = 125 GeV in the anti-kt algorithm. These settings match
  the selection criteria of the EJ ATLAS analysis.
- Simulated 50K events to reproduce the EJ signal given by model D, mZv = 3 TeV, c*tau(piD) = 50 mm 
  using the delphes_card_ATLAS_EJ_reproduction.tcl : it's the standard ATLAS delphes card except from
  FastJetFinder: parameters R = 0.4, MinJetpT = 125 GeV in the anti-kt algorithm match the selection 
  criteria of the EJ ATLAS analysis. set InputArray EFlowMerger/eflow, N-subjettiness, jet grooming are added. 

Pythia8 card file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50.cmnd

Delphes card file:
- First card used, with set InputArray Calorimeter/towers, no N-subjettiness, no jet grooming
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_NoSoftDrop.tcl
- Second card used, with set InputArray EFlowMerger/eflow, N-subjettiness, jet grooming
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction.tcl

Data file:
- First card used, with set InputArray Calorimeter/towers, no N-subjettiness, no jet grooming
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250210_Task8/50K_NoSoftDrop.root
- Second card used, with set InputArray EFlowMerger/eflow, N-subjettiness, jet grooming
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250210_Task8/50K.root

Log file:
- First card used, with set InputArray Calorimeter/towers, no N-subjettiness, no jet grooming
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250210_Task8/50K_NoSoftDrop.log
- Second card used, with set InputArray EFlowMerger/eflow, N-subjettiness, jet grooming
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250210_Task8/50K.log

Analysis file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250210_Task8/JetFilter.h  (not used)
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250210_Task8/JetpT.C

Plot file:
plots.root

Plots:
- JetpT.pdf: All, leading and subleading jet pT distribution 
- InvariantMass.pdf: Invariant mass of first two jets calculated by P4 of constituents or by SoftDroppedP4[0]
- TrackD0.pdf: Track D0 (impact parameter of track) of all tracks and of first 2 jets constituent tracks
