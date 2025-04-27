/*
Author: Giulia Cossutti

Macro to fill histograms about large jets for signal and two backgrounds

From inside the /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes directory run with
root -l -q EJ_reproduction/tasks/20250319_Task13/BuildHistos.C'("EJ_reproduction/tasks/20250221_Task10/50K_Reclustering.root", "signal", sel)' &
root -l -q EJ_reproduction/tasks/20250319_Task13/BuildHistos.C'("EJ_reproduction/tasks/20250310_Task12/50K_bkg_Top.root", "bkg_Top", sel)' &
root -l -q EJ_reproduction/tasks/20250319_Task13/BuildHistos.C'("EJ_reproduction/tasks/20250310_Task12/50K_bkg_HardQCD.root", "bkg_HardQCD", sel)' &
root -l -q EJ_reproduction/tasks/20250331_Task15/BuildHistos.C'("EJ_reproduction/tasks/20250331_Task15/50K_Reclustering_ddbar.root", "signal_ddbar", sel)' &

sel = 0, 1, 2 stands for event selection:
if 0: consider only events with >=2 small jets with |eta| < 2.4 and pT > 125 GeV  (EJ analysis selection)
if 1: consider only events with >=2 small jets with |eta| < 2.4
if 2: no analysis selection at all (consider all events)

Jet definition is equal for all event selections and datasets and is due to the detector's acceptance:
small jets have |eta| < 2.5 + large jets have |eta| < 1.8
*/

#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/classes/DelphesClasses.h"
#include "/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#endif

//------------------------------------------------------------------------------
//----------- FUNCTIONS -----------------------
//===============================
// Function to compute DeltaR between 2 jets
Double_t DeltaR(Jet* jet1, Jet* jet2){
 return sqrt( pow(jet1->Eta - jet2->Eta,2) + pow(jet1->Phi - jet2->Phi,2) );
}
//===============================
// Function to compute DeltaEta between 2 jets
Double_t DeltaEta(Jet* jet1, Jet* jet2){
 return jet1->Eta - jet2->Eta;
}
//===============================
// Function to compute DeltaPhi between 2 jets
Double_t DeltaPhi(Jet* jet1, Jet* jet2){
 return jet1->Phi - jet2->Phi;
}
//===============================
//----------- END FUNCTIONS ---------------------


void BuildHistos(const char *inFile, const char *outFile, Int_t sel=0)
{

  if(sel < 0 || sel > 2){
    cout << " sel must be 0, 1 or 2" << endl;
    exit(EXIT_FAILURE);
  }

  gSystem->Load("libDelphes");

  // Create chain of root trees
  TChain chain("Delphes");
  chain.Add(inFile);

  // Create object of class ExRootTreeReader
  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  Long64_t numberOfEntries = treeReader->GetEntries();

 // Get pointers to branches used in this analysis
  TClonesArray *branchEvent = treeReader->UseBranch("Event");

  TClonesArray *branchJet = treeReader->UseBranch("Jet");
  TClonesArray *branchGenJet = treeReader->UseBranch("GenJet");
  TClonesArray *branchReclusteredJet = treeReader->UseBranch("ReclusteredJet");
  TClonesArray *branchReclusteredGenJet = treeReader->UseBranch("ReclusteredGenJet");
  
  TClonesArray *branchParticle = treeReader->UseBranch("Particle");
  TClonesArray *branchTrack = treeReader->UseBranch("Track");
  TClonesArray *branchTower = treeReader->UseBranch("Tower");
    
  TClonesArray *branchEFlowTrack = treeReader->UseBranch("EFlowTrack");
  TClonesArray *branchEFlowPhoton = treeReader->UseBranch("EFlowPhoton");
  TClonesArray *branchEFlowNeutralHadron = treeReader->UseBranch("EFlowNeutralHadron");
  TClonesArray *branchGenMissingET = treeReader->UseBranch("GenMissingET");
  TClonesArray *branchMissingET = treeReader->UseBranch("MissingET");
  TClonesArray *branchElectron = treeReader->UseBranch("Electron");
  TClonesArray *branchMuon = treeReader->UseBranch("Muon");
  TClonesArray *branchPhoton = treeReader->UseBranch("Photon");
  TClonesArray *branchScalarHT = treeReader->UseBranch("ScalarHT");
  
  TClonesArray *branchJetEnergyScaledJet = treeReader->UseBranch("JetEnergyScaledJet");
  TClonesArray *branchECalTower = treeReader->UseBranch("ECalTower");
  TClonesArray *branchHCalTower = treeReader->UseBranch("HCalTower");
  TClonesArray *branchMuonMomentumSmearing = treeReader->UseBranch("MuonMomentumSmearing");


  // Book histograms
  TH1F *Invmass_2large = new TH1F("Invariant_mass_of_leading_and_subleading_large_jets", "", 150, 0.0, 6000.0);
  TH1F *Eta1_large = new TH1F("Eta_of_leading_large_jet", "", 160, -4., 12.);
  TH1F *Eta2_large = new TH1F("Eta_of_subleading_large_jet", "", 160, -4., 12.);
  TH1F *DeltaR_large = new TH1F("DeltaR_between_leading_and_subleading_large_jets", "", 100, 0., 10.);
  TH1F *DeltaEta_large = new TH1F("DeltaEta_between_leading_and_subleading_large_jets", "", 220, -5., 17.);
  TH1F *DeltaPhi_large = new TH1F("DeltaPhi_between_leading_and_subleading_large_jets", "", 270, -7., 20.);
  TH1F *JetPT1_large = new TH1F("leading_large_jet_pt", "", 150, 0.0, 3000.0);
  TH1F *JetPT2_large = new TH1F("subleading_large_jet_pt", "", 150, 0.0, 3000.0);
  TH1F *Njets_in_large = new TH1F("number_of_constituents_of_leading_and_subleading_large_jets", "", 30, -0.5, 29.5);

  TH2F *DeltaPhi_DeltaEta = new TH2F("DeltaPhi_vs_DeltaEta", "", 140, -7., 7., 150, -5., 10.);
  TH2F *DeltaR_DeltaEta = new TH2F("DeltaR_vs_DeltaEta", "", 80, 0., 8., 150, -5., 10.);
  TH2F *DeltaR_DeltaPhi = new TH2F("DeltaR_vs_DeltaPhi", "", 80, 0., 8., 260, -7., 14.);
  TH2F *JetPT1_JetPT2 = new TH2F("JetPT1_vs_JetPT2", "", 100, 0.0, 2000.0, 125, 0.0, 2500.0);
  TH2F *JetPT1_Invmass = new TH2F("JetPT1_vs_Invmass", "", 100, 0.0, 2000.0, 150, 0.0, 6000.0);
  TH2F *JetPT1_NConst = new TH2F("JetPT1_vs_NConst", "", 100, 0.0, 2000.0, 20, -0.5, 19.5);
  TH2F *Invmass_NConst = new TH2F("Invmass_vs_NConst", "", 100, 0.0, 4000.0, 20, -0.5, 19.5);

  // Counter of used events
  Int_t NofEv = 0;

  // Loop over all events
  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);

    //------------- Event selection ------------------
    // Event selection on jets: >=2 jets with |eta| < 2.4 (and pT > 125 GeV)

    if( sel == 0 || sel == 1 ){

      Int_t nOkJets = 0;

      // Loop over all jets to count jets with eta < 2.4 (and pT > 125 GeV)
       for(Int_t jetentry = 0; jetentry < branchJet->GetEntries(); ++jetentry)
         {
         Jet *jet = (Jet*) branchJet->At(jetentry);
         if(sel == 0 & abs(jet->Eta) < 2.4 & jet->PT > 125.0) nOkJets += 1;
         if(sel == 1 & abs(jet->Eta) < 2.4) nOkJets += 1;
         }

      // If there are less than 2 jets with eta < 2.4 and pT > 125 GeV go to next event
      if( nOkJets < 2 ) continue;

      //if(branchJet->GetEntries() ==1) cout << "Filter did not work: there is an event with " <<  branchJet->GetEntries() << " entries" << endl;
    }    
    //------------- End Event selection ------------------

     // Increment event counter
     NofEv += 1;

     // jet counter
     Int_t index_large = 0;

     Int_t index_large1 = 0;
     Int_t index_large2 = 0;

     // Loop over all large jets
     for(Int_t jetentry = 0; jetentry < branchReclusteredJet->GetEntries(); ++jetentry)
     {
       Jet *jet = (Jet*) branchReclusteredJet->At(jetentry);
       if( abs(jet->Eta) >= 1.8 ) continue;
       index_large += 1;

       // Indexes of leading and subleading large jets
       if(index_large == 1){
         index_large1 = jetentry;
       }else if(index_large == 2){
         index_large2 = jetentry;
         break;
       }
     }//end loop over all large jets

     // Fill histograms
     if(index_large >= 2){
       Jet *largejet1 = (Jet*) branchReclusteredJet->At(index_large1);
       Jet *largejet2 = (Jet*) branchReclusteredJet->At(index_large2);

       TLorentzVector *largejet1P4 = new TLorentzVector(1.0,1.0,1.0,1.0);
       largejet1P4->SetPtEtaPhiM(largejet1->PT,largejet1->Eta,largejet1->Phi,largejet1->Mass);
       TLorentzVector *largejet2P4 = new TLorentzVector(1.0,1.0,1.0,1.0);
       largejet2P4->SetPtEtaPhiM(largejet2->PT,largejet2->Eta,largejet2->Phi,largejet2->Mass);

       //Fill 1D histograms
       Invmass_2large->Fill( (*largejet1P4 + *largejet2P4).M()  );

       Eta1_large->Fill( largejet1->Eta );
       Eta2_large->Fill( largejet2->Eta );

       DeltaR_large->Fill( DeltaR(largejet1, largejet2) );
       DeltaEta_large->Fill( DeltaEta(largejet1, largejet2) );
       DeltaPhi_large->Fill( DeltaPhi(largejet1, largejet2) );

       JetPT1_large->Fill( largejet1->PT );
       JetPT2_large->Fill( largejet2->PT );

       Njets_in_large->Fill(largejet1->Constituents.GetEntriesFast());       
       Njets_in_large->Fill(largejet2->Constituents.GetEntriesFast());       

       //Fill 2D histograms
       DeltaPhi_DeltaEta->Fill( DeltaPhi(largejet1, largejet2) , DeltaEta(largejet1, largejet2) );
       DeltaR_DeltaEta->Fill( DeltaR(largejet1, largejet2) , DeltaEta(largejet1, largejet2) );
       DeltaR_DeltaPhi->Fill( DeltaR(largejet1, largejet2) , DeltaPhi(largejet1, largejet2) );

       JetPT1_JetPT2->Fill( largejet1->PT , largejet2->PT );
       JetPT1_Invmass->Fill( largejet1->PT , (*largejet1P4 + *largejet2P4).M() );
       JetPT1_NConst->Fill( largejet1->PT , largejet1->Constituents.GetEntriesFast() );
       JetPT1_NConst->Fill( largejet1->PT , largejet2->Constituents.GetEntriesFast() );
       Invmass_NConst->Fill( (*largejet1P4 + *largejet2P4).M() , largejet1->Constituents.GetEntriesFast() );
       Invmass_NConst->Fill( (*largejet1P4 + *largejet2P4).M() , largejet2->Constituents.GetEntriesFast() );
     }

  }// end loop over all events

//------------------------------------------------

  //cout << "Number of used Events: " << NofEv << endl;

//-------------------------------------------------------
  // write all plots on file
  TString outname;
  outname.Form("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250331_Task15/%s_%d.root",outFile,sel);

  TFile *f = new TFile(outname,"RECREATE");
  
  Invmass_2large->Write("",TObject::kOverwrite);
  Eta1_large->Write("",TObject::kOverwrite);
  Eta2_large->Write("",TObject::kOverwrite);
  DeltaR_large->Write("",TObject::kOverwrite);
  DeltaEta_large->Write("",TObject::kOverwrite);
  DeltaPhi_large->Write("",TObject::kOverwrite);
  JetPT1_large->Write("",TObject::kOverwrite);
  JetPT2_large->Write("",TObject::kOverwrite);
  Njets_in_large->Write("",TObject::kOverwrite);
  
  DeltaPhi_DeltaEta->Write("",TObject::kOverwrite);
  DeltaR_DeltaEta->Write("",TObject::kOverwrite);
  DeltaR_DeltaPhi->Write("",TObject::kOverwrite);
  JetPT1_JetPT2->Write("",TObject::kOverwrite);
  JetPT1_Invmass->Write("",TObject::kOverwrite);
  JetPT1_NConst->Write("",TObject::kOverwrite);
  Invmass_NConst->Write("",TObject::kOverwrite);
  
}// end BuildHistos
