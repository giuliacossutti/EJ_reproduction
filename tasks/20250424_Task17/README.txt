Author: Giulia Cossutti
Date: 24th April 2025
Task: Task17

Run obtained with:
From inside directory
/gfsvol01/atlas/giuliac/UpdatedDelphes/delphes
run with
DelphesPythia8 /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50.cmnd /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250424_Task17/100_test.root > /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250424_Task17/100_test.log & 
DelphesPythia8 /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50.cmnd /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250424_Task17/100_test_demin.root > /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250424_Task17/100_test_demin.log & 

From inside directory
/gfsvol01/atlas/giuliac/prova/delphes
run with
DelphesPythia8 /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/ee_zh.cmnd /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250424_Task17/100_test_VertexSmearing.root > /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250424_Task17/100_test_VertexSmearing.log & 
DelphesPythia8 /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering_TrkSmearing.tcl /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/ee_zh.cmnd /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250424_Task17/100_test_TrkSmearing.root > /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250424_Task17/100_test_TrkSmearing.log & 

Description:
- Simulated 100 events with updated master version of delphes installed in /gfsvol01/atlas/giuliac/UpdatedDelphes/delphes.
- Simulated 100 events with updated master version of delphes installed in /gfsvol01/atlas/giuliac/UpdatedDelphes/delphes and Pavel Demin's TreeWriter.cc . 
- Simulated 100 events with updated master version of delphes installed in /gfsvol01/atlas/giuliac/prova/delphes and Pavel Demin's TreeWriter.cc with example card with vertex smearing. 
- Simulated 100 events with updated master version of delphes installed in /gfsvol01/atlas/giuliac/prova/delphes and Pavel Demin's TreeWriter.cc with example card with vertex smearing
  and Track Smearing module in delphes card. 
- Performed tests on class of constituents of large jets and on finding primary vertex through fit on tracks.

Pythia8 card file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50.cmnd
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/pythia8_cards/ee_zh.cmnd

Delphes card file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering_TrkSmearing.tcl

Data file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250424_Task17/100_test.root
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250424_Task17/100_test_demin.root
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250424_Task17/100_test_VertexSmearing.root
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250424_Task17/100_test_TrkSmearing.root

Log file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250424_Task17/100_test.log
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250424_Task17/100_test_demin.log
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250424_Task17/100_test_VertexSmearing.log
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250424_Task17/100_test_TrkSmearing.log

Analysis file:
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250424_Task17/reclustering_test.C

Plot file:

Plots:

