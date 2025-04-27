Author: Giulia Cossutti
Date: 25th March 2025
Task: Task14

Run obtained with
DelphesPythia8 ./EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl ./EJ_reproduction/pythia8_cards/EJ_D_Z3.0_ctau50.cmnd ./EJ_reproduction/tasks/20250221_Task10/50K_Reclustering.root > ./EJ_reproduction/tasks/20250221_Task10/50K_Reclustering.log & 
DelphesPythia8 ./EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl ./EJ_reproduction/pythia8_cards/background_Top.cmnd ./EJ_reproduction/tasks/20250310_Task12/50K_bkg_Top.root > ./EJ_reproduction/tasks/20250310_Task12/50K_bkg_Top.log &
DelphesPythia8 ./EJ_reproduction/delphes_cards/delphes_card_ATLAS_EJ_reproduction_Reclustering.tcl ./EJ_reproduction/pythia8_cards/background_HardQCD.cmnd ./EJ_reproduction/tasks/20250310_Task12/50K_bkg_HardQCD.root > ./EJ_reproduction/tasks/20250310_Task12/50K_bkg_HardQCD.log &

Description:
- Calculation of figures of merit (significance S/sqrt(Bkg) for different cuts) to determine best cuts for cut-based analysis

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
/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250325_Task14/FigureOfMerit.C

SPECIAL NOTE: EVENT SELECTION AND OBJECT DEFINITION
All plots and plot files end with:
*_0 or _sel0: consider only events with >=2 small jets with |eta|<2.4 and pT<125 GeV + only small jets with |eta|<2.5 + only large jets with |eta|<1.8
*_1 or _sel1: consider only events with >=2 small jets with |eta|<2.4 + only small jets with |eta|<2.5 + only large jets with |eta|<1.8
*_2 or _sel2: consider all events + only small jets with |eta|<2.5 + only large jets with |eta|<1.8
If they contain:
_sample0 : signal
_sample1 : bkg_Top
_sample2 : bkg_HardQCD
If they end with:
_signal_bkg_left : plots of fraction of signal and bkg events remaining after the cut

Plot file:
Source of histograms, from 20250319_Task13:
- signal.root : all histos from signal
- bkg_Top.root : all histos from Top background
- bkg_HardQCD.root : all histos from HardQCD background
Produced through them, used for figure of merit and best cuts' determination:
- histos_sel0.root
- histos_sel1.root
- histos_sel2.root
Inside these files, names of signal histos end with _0, bkg histos 
end with _1 (bkg_Top) and _2 (bkg_HardQCD).

Plots:
====== 1D plots =========
- fom_DeltaR_between_leading_and_subleading_large_jets.pdf : significance with 1D cuts on DeltaR
- fom_number_of_constituents_of_leading_and_subleading_large_jets.pdf : significance with 1D cuts on NConst
- fom_leading_large_jet_pt.pdf : significance with 1D cuts on leading large jet pT (JetPT1)
- fom_subleading_large_jet_pt.pdf : significance with 1D cuts on subleading large jet pT (JetPT2)
- fom_Invariant_mass_of_leading_and_subleading_large_jets.pdf : significance with 1D cuts on Invmass
====== 2D plots =========
- Rectangular_fom_JetPT1_vs_Invmass.pdf : significance with 2D rectangular cuts (on x = JetPT1, y = Invmass)
- Rectangular_fom_JetPT1_vs_NConst.pdf : significance with 2D rectangular cuts (on x = JetPT1, y = NConst)
- Rectangular_fom_Invmass_vs_NConst.pdf : significance with 2D rectangular cuts (on x = Invmass, y = NConst)
- 2D_Linear_fom_JetPT1_vs_NConst.pdf : significance with 2D linear cuts (on x = JetPT1, y = NConst) varying m, q of the line
- 2D_Linear_fom_Invmass_vs_NConst.pdf : significance with 2D linear cuts (on x = Invmass, y = NConst) varying m, q of the line

Significance, signal, bkg values are in text files FOM.txt, with same names as the pdf plots.
