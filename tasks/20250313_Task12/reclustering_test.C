
/*
Author: Giulia Cossutti

Test delphes' modification in TreeWriter to have Jets as constituents of reclustered jets

From inside the /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes directory run with
root -l EJ_reproduction/tasks/20250313_Task12/reclustering_test.C'("EJ_reproduction/tasks/20250313_Task12/50K_reclustering_test.root",filter,Norm)'
*/

#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/classes/DelphesClasses.h"
#include "/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#endif

//------------------------------------------------------------------------------

//---------- FUNCTIONS -------------------
//===================================
// Function to customize histograms:
// - Scale by number of entries and bin width
// - Customize markers and lines
void CustomizeHist(TH1F* hist, Style_t style, Color_t color, Bool_t scale = kFALSE){
  hist->Sumw2();
  if(scale == kTRUE) hist->Scale(1.0/(hist->GetEntries() * hist->GetXaxis()->GetBinWidth(2)));
  hist->SetMarkerStyle(style);
  hist-> SetMarkerColor(color);
  hist-> SetLineColor(color);
  hist->SetMarkerSize(0.8);
  hist->GetXaxis()->SetTitleSize(.045);
  hist->GetYaxis()->SetTitleSize(.045);
}
//=================================
// Function to customize legends:
// - Customize text size of header and body
// - No border
void CustomizeLeg(TLegend* legend){
  TLegendEntry *header = (TLegendEntry*)legend->GetListOfPrimitives()->First();
  header->SetTextSize(0.03);
  gStyle->SetLegendTextSize(0.03);
  legend->SetBorderSize(0);
  legend->SetEntrySeparation(0.01);
}
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


void reclustering_test(const char *inputFile,Int_t filter=0,Bool_t Norm=kFALSE)
{
  // Event selection & Small and Large Jet definition: 
  // if 0: consider only events with >=2 small jets with |eta| < 2.4 and pT > 125 GeV + small jets have |eta| < 2.5 + large jets have |eta| < 1.8 (EJ analysis)
  // if 1: consider only events with >=2 small jets with |eta| < 2.4 + small jets have |eta| < 2.5 + large jets have |eta| < 1.8 
  // if 2: no event selection + small jets have |eta| < 2.5 + large jets have |eta| < 1.8
  //Int_t filter = 0;

  // Normalisation of histograms
  Bool_t NjetsNorm = Norm;

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

  // Book histograms
  TH1F *Njets_small = new TH1F("number_of_jets_with_R0.4", "", 30, -0.5, 29.5);
  TH1F *Njets_large = new TH1F("number_of_reclustered_jets_with_R1.0", "", 30, -0.5, 29.5);
  TH1F *Njets_in_large = new TH1F("number_of_constituents_of_leading_and_subleading_jets_with_R1.0", "", 30, -0.5, 29.5);
  TH1F *Njets_in_small = new TH1F("number_of_constituents_of_leading_and_subleading_jets_with_R0.4", "", 30, -0.5, 29.5);

  TH1F *Ngenjets_small = new TH1F("number_of_genjets_with_R0.4", "", 30, -0.5, 29.5);
  TH1F *Ngenjets_large = new TH1F("number_of_reclustered_genjets_with_R1.0", "", 30, -0.5, 29.5);
  TH1F *Ngenjets_in_large = new TH1F("number_of_constituents_of_leading_and_subleading_genjets_with_R1.0", "", 30, -0.5, 29.5);
  TH1F *Ngenjets_in_small = new TH1F("number_of_constituents_of_leading_and_subleading_genjets_with_R0.4", "", 30, -0.5, 29.5);

  // Loop over all events
  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);

  //------------- Event Selection ------------------
  // Event Selection: >=2 jets with |eta| < 2.4 and pT > 125 GeV

    Int_t nOkJets = 0;

    // Loop over all jets to count jets with eta < 2.4 and pT > 125 GeV
    if( filter == 0 || filter == 1 ){
     for(Int_t jetentry = 0; jetentry < branchJet->GetEntries(); ++jetentry)
       {
       Jet *jet = (Jet*) branchJet->At(jetentry);
       if(filter == 0 & abs(jet->Eta) < 2.4 & jet->PT > 125.0) nOkJets += 1;
       if(filter == 1 & abs(jet->Eta) < 2.4) nOkJets += 1;
       }

    // If there are less than 2 jets with eta < 2.4 and pT > 125 GeV go to next event
    if( nOkJets < 2 ) continue;

    //if(branchJet->GetEntries() ==1) cout << "Filter did not work: there is an event with " <<  branchJet->GetEntries() << " entries" << endl;
    }
  //------------- End Event Selection ------------------

     // jet counter
     Int_t index_small = 0;
     Int_t index_large = 0;
     Int_t index_gensmall = 0;
     Int_t index_genlarge = 0;

     // Loop over all small jets
     for(Int_t jetentry = 0; jetentry < branchJet->GetEntries(); ++jetentry)
     {
       Jet *jet = (Jet*) branchJet->At(jetentry);
       if( abs(jet->Eta) >= 2.5 ) continue;
       index_small += 1;

       // Loop over leading and subleading small jet constituents
       if (index_small == 1 || index_small == 2){
         //if(entry <= 5) LoopOverConstituents(jet, "SMALL");   
         Njets_in_small->Fill(jet->Constituents.GetEntriesFast());     
       }//end if

     }//end loop over all small jets

     // Loop over all small genjets
     for(Int_t jetentry = 0; jetentry < branchGenJet->GetEntries(); ++jetentry)
     {
       Jet *jet = (Jet*) branchGenJet->At(jetentry);
       if( abs(jet->Eta) >= 2.5 ) continue;
       index_gensmall += 1;

       // Loop over leading and subleading small genjet constituents
       if (index_gensmall == 1 || index_gensmall == 2){
         //if(entry <= 5) LoopOverConstituents(jet, "GEN SMALL");   
         Ngenjets_in_small->Fill(jet->Constituents.GetEntriesFast());     
       }//end if

     }//end loop over all small genjets

     // Loop over all large jets
     for(Int_t jetentry = 0; jetentry < branchReclusteredJet->GetEntries(); ++jetentry)
     {
       Jet *jet = (Jet*) branchReclusteredJet->At(jetentry);
       if( abs(jet->Eta) >= 1.8 ) continue;
       index_large += 1;

       // Loop over leading and subleading large jet constituents
       if (index_large == 1 || index_large == 2){   
         if(entry <= 20) LoopOverConstituents(jet, "LARGE");   
         Njets_in_large->Fill(jet->Constituents.GetEntriesFast());
       }//end if

     }//end loop over all large jets

     // Loop over all large genjets
     for(Int_t jetentry = 0; jetentry < branchReclusteredGenJet->GetEntries(); ++jetentry)
     {
       Jet *jet = (Jet*) branchReclusteredGenJet->At(jetentry);
       if( abs(jet->Eta) >= 1.8 ) continue;
       index_genlarge += 1;

       // Loop over leading and subleading large genjet constituents
       if (index_genlarge == 1 || index_genlarge == 2){   
         if(entry <= 20) LoopOverConstituents(jet, " GEN LARGE");   
         Ngenjets_in_large->Fill(jet->Constituents.GetEntriesFast());
       }//end if

     }//end loop over all large genjets

     // Fill number of jets histograms
     Njets_small->Fill(index_small);
     Njets_large->Fill(index_large);
     Ngenjets_small->Fill(index_gensmall);
     Ngenjets_large->Fill(index_genlarge);

  }//end loop over all events

//-----------------------------------------------------------------

  // Canva to draw Number of jets histograms
  TCanvas *c1 = new TCanvas("c1", "c1", 0, 0, 800, 600);
  gStyle->SetOptStat(0);
  gPad->SetLeftMargin(0.15);

  //Bool_t NjetsNorm = kTRUE;
  //Bool_t NjetsNorm = kFALSE;
  CustomizeHist(Njets_small, kFullSquare, kBlue, NjetsNorm);
  CustomizeHist(Njets_large, kFullCross, kRed, NjetsNorm);
  CustomizeHist(Njets_in_large, kFullCross, kGreen+1, NjetsNorm);
  CustomizeHist(Njets_in_small, kFullSquare, kViolet+1, NjetsNorm);

  CustomizeHist(Ngenjets_small, kFullSquare, kAzure+6, NjetsNorm);
  CustomizeHist(Ngenjets_large, kFullCross, kOrange-3, NjetsNorm);
  CustomizeHist(Ngenjets_in_large, kFullCross, kSpring-7, NjetsNorm);
  CustomizeHist(Ngenjets_in_small, kFullSquare, kPink+1, NjetsNorm);

  Njets_large->GetXaxis()->SetTitle("Number of jets or constituents");
  if(NjetsNorm == kTRUE){Njets_large->GetYaxis()->SetTitle("#frac{1}{N} #frac{dN}{dN_{jets or constituents}}");}
  else{Njets_large->GetYaxis()->SetTitle("a.u.");}

  // Show resulting histograms
  Njets_large->Draw("E,HIST");
  Njets_small->Draw("E,HIST same");
  Njets_in_large->Draw("E,HIST same");
  Njets_in_small->Draw("E,HIST same");

  Ngenjets_large->Draw("E,HIST same");
  Ngenjets_small->Draw("E,HIST same");
  Ngenjets_in_large->Draw("E,HIST same");
  Ngenjets_in_small->Draw("E,HIST same");

  // Legend
  TLegend* legend = new TLegend(0.489975,0.232639,0.837093,0.875);

  if(filter==0) legend->SetHeader("#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{#splitline{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV}{Number of Jets with Reclustering - EJ}}","C");
  if(filter==1) legend->SetHeader("#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{#splitline{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV}{Number of Jets with Reclustering - no pT}}","C");
  if(filter==2) legend->SetHeader("#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{#splitline{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV}{Number of Jets with Reclustering - no sel}}","C");

  legend->AddEntry(Njets_small,"Small (R=0.4)");
  legend->AddEntry(Njets_large,"Large (R=1.0)");
  legend->AddEntry(Njets_in_small,"Constituents of L or Subl Small");
  legend->AddEntry(Njets_in_large,"Constituents of L or Subl Large");

  legend->AddEntry(Ngenjets_small,"GenSmall (R=0.4)");
  legend->AddEntry(Ngenjets_large,"GenLarge (R=1.0)");
  legend->AddEntry(Ngenjets_in_small,"Constituents of L or Subl GenSmall");
  legend->AddEntry(Ngenjets_in_large,"Constituents of L or Subl GenLarge");

  CustomizeLeg(legend);
  legend->Draw();

  if(NjetsNorm==kFALSE){
    if(filter==0) c1->SaveAs("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250313_Task12/Njets.pdf"); 
    if(filter==1) c1->SaveAs("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250313_Task12/Njets_nopT.pdf"); 
    if(filter==2) c1->SaveAs("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250313_Task12/Njets_nopTeta.pdf"); 
  }else if(NjetsNorm==kTRUE){
    if(filter==0) c1->SaveAs("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250313_Task12/Njets_Norm.pdf"); 
    if(filter==1) c1->SaveAs("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250313_Task12/Njets_nopT_Norm.pdf"); 
    if(filter==2) c1->SaveAs("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250313_Task12/Njets_nopTeta_Norm.pdf"); 
  }
  
  //c1->Close();

//-------------------------------------------------------
  // write all plots on file
  if(filter==0 & Norm==kFALSE) TFile *f = new TFile("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250313_Task12/plots.root","RECREATE");
  if(filter==1 & Norm==kFALSE) TFile *f = new TFile("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250313_Task12/plots_nopT.root","RECREATE");
  if(filter==2 & Norm==kFALSE) TFile *f = new TFile("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250313_Task12/plots_nopTeta.root","RECREATE");
  if(filter==0 & Norm==kTRUE) TFile *f = new TFile("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250313_Task12/plots_Norm.root","RECREATE");
  if(filter==1 & Norm==kTRUE) TFile *f = new TFile("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250313_Task12/plots_nopT_Norm.root","RECREATE");
  if(filter==2 & Norm==kTRUE) TFile *f = new TFile("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250313_Task12/plots_nopTeta_Norm.root","RECREATE");

  Njets_small->Write("",TObject::kOverwrite);
  Njets_large->Write("",TObject::kOverwrite);
  Njets_in_large->Write("",TObject::kOverwrite);
  Njets_in_small->Write("",TObject::kOverwrite);

  Ngenjets_small->Write("",TObject::kOverwrite);
  Ngenjets_large->Write("",TObject::kOverwrite);
  Ngenjets_in_large->Write("",TObject::kOverwrite);
  Ngenjets_in_small->Write("",TObject::kOverwrite);


}

