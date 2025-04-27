#ifndef JetFilter_h
#define JetFilter_h

R__LOAD_LIBRARY(libDelphes)
#include "/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/classes/DelphesClasses.h"
#include "/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/external/ExRootAnalysis/ExRootTreeReader.h"


// Function to select only events with >=2 jets with eta < 2.4
TClonesArray * JetFilter(ExRootTreeReader * treeReader){

Long64_t numberOfEntries = treeReader->GetEntries();

TClonesArray *branchJetFiltered = treeReader->UseBranch("Jet");
//TClonesArray *branchJetToFilter = treeReader->UseBranch("Jet");
//TClonesArray *branchJetFiltered = branchJetToFilter;

  // Loop over all events
  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {  
  // Load selected branches with data from specified event
  treeReader->ReadEntry(entry);

  // Select if event contains at least 2 jets with eta < 2.4
  if(branchJetFiltered->GetEntries() > 0)
    {

     // Loop over all jets to remove jets with eta >= 2.4
     for(Int_t jetentry = 0; jetentry < branchJetFiltered->GetEntries(); ++jetentry)
       {

       Jet *jet = (Jet*) branchJetFiltered->At(jetentry);
       if(jet->Eta >= 2.4) branchJetFiltered->RemoveAt(jetentry);
      
       }

    }

   // If 0 or >=2 jets remain: ok
   // If 1 jet remains: remove it
   if(branchJetFiltered->GetEntries() ==1) branchJetFiltered->RemoveAt(0);

  }

return branchJetFiltered;

}

#endif

