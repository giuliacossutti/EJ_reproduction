/*
Author: Giulia Cossutti

Test delphes' last version and modification in TreeWriter to have Jets as constituents of reclustered jets

From inside the /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes directory run with
root -l EJ_reproduction/tasks/20250424_Task17/reclustering_test.C'("EJ_reproduction/tasks/20250424_Task17/100_test.root")'
root -l EJ_reproduction/tasks/20250424_Task17/reclustering_test.C'("EJ_reproduction/tasks/20250424_Task17/100_test_demin.root")'
*/

#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/classes/DelphesClasses.h"
#include "/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#endif

//------------------------------------------------------------------------------

//---------- FUNCTIONS -------------------
//===============================
// Function to loop over constituents of a jet
void LoopOverConstituents(Jet* jet, TString string){
         for(Int_t j = 0; j < jet->Constituents.GetEntriesFast(); ++j)
          {
            // Define objects for loop on constituents
            TObject *object;
            GenParticle *particle;
            //Electron *electron;
            //Photon *photon;
            Muon *muon;
            Track *track;
            Tower *tower;
            Jet *jet_in;

            object = jet->Constituents.At(j);

            // Check if the constituent is accessible
            if(object == 0) continue;
  
            cout << string << " JET  " <<jet->Constituents.GetEntriesFast() << " constituents. Constituent is a " << object->IsA() << endl;

            if(object->IsA() == GenParticle::Class())
            {
              particle = (GenParticle*) object;
               cout << "    GenPart pt: " << particle->PT << ", eta: " << particle->Eta << ", phi: " << particle->Phi << endl;
            }
            else if(object->IsA() == Track::Class())
            {
              track = (Track*) object;
               cout << "    Track pt: " << track->PT << ", eta: " << track->Eta << ", phi: " << track->Phi << endl;
            }
            else if(object->IsA() == Tower::Class())
            {
              tower = (Tower*) object;
               cout << "    Tower pt: " << tower->ET << ", eta: " << tower->Eta << ", phi: " << tower->Phi << endl;
            }
            else if(object->IsA() == Jet::Class())
            {
              jet_in = (Jet*) object;
               cout << "    Jet pt: " << jet_in->PT << ", eta: " << jet_in->Eta << ", phi: " << jet_in->Phi << endl;
            }
            else if(object->IsA() == Muon::Class())
            {
              muon = (Muon*) object;
               cout << "    Muon pt: " << muon->PT  << endl;
            }
          }// end loop over constituents of large jets
}// end function to loop over constituents of a jet
//===============================
//---------- END FUNCTIONS -------------------

void reclustering_test(const char *inputFile)
{

  gSystem->Load("libDelphes");

  // Create chain of root trees
  TChain chain("Delphes");
  chain.Add(inputFile);

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

  // Loop over all events
  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);

     // Loop over all small jets
     for(Int_t jetentry = 0; jetentry < branchJet->GetEntries(); ++jetentry)
     {
       Jet *jet = (Jet*) branchJet->At(jetentry);
       if( abs(jet->Eta) >= 2.5 ) continue;

       // Loop over leading and subleading small jet constituents
       if(entry <= 5) LoopOverConstituents(jet, "SMALL");   

       if(jetentry >= 2) break;
     }//end loop over all small jets

     // Loop over all small genjets
     for(Int_t jetentry = 0; jetentry < branchGenJet->GetEntries(); ++jetentry)
     {
       Jet *jet = (Jet*) branchGenJet->At(jetentry);
       if( abs(jet->Eta) >= 2.5 ) continue;

       // Loop over leading and subleading small genjet constituents
       if(entry <= 5) LoopOverConstituents(jet, "GEN SMALL");   

       if(jetentry >= 2) break;
     }//end loop over all small genjets

     // Loop over all large jets
     for(Int_t jetentry = 0; jetentry < branchReclusteredJet->GetEntries(); ++jetentry)
     {
       Jet *jet = (Jet*) branchReclusteredJet->At(jetentry);
       if( abs(jet->Eta) >= 1.8 ) continue;

       // Loop over leading and subleading large jet constituents
       if(entry <= 20) LoopOverConstituents(jet, "LARGE");   

       if(jetentry >= 2) break;
     }//end loop over all large jets

     // Loop over all large genjets
     for(Int_t jetentry = 0; jetentry < branchReclusteredGenJet->GetEntries(); ++jetentry)
     {
       Jet *jet = (Jet*) branchReclusteredGenJet->At(jetentry);
       if( abs(jet->Eta) >= 1.8 ) continue;

       // Loop over leading and subleading large genjet constituents
       if(entry <= 20) LoopOverConstituents(jet, " GEN LARGE");   

       if(jetentry >= 2) break;
     }//end loop over all large genjets

  }//end loop over all events

}
