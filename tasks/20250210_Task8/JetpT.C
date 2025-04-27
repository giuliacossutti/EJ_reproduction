/*
Author: Giulia Cossutti

Macro to plot the total, leading and subleading jet distribution
Plot invariant mass of first two jets
Plot track D0 of all tracks and first 2 jets constituent tracks

From inside the /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes directory run with
root -l EJ_reproduction/tasks/20250210_Task8/JetpT.C'("EJ_reproduction/tasks/20250210_Task8/50K.root")'
*/

#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/classes/DelphesClasses.h"
#include "/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootResult.h"
//#include "/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250210_Task8/JetFilter.h"
#endif

//------------------------------------------------------------------------------

void JetpT(const char *inputFile)
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
  TClonesArray *branchParticle = treeReader->UseBranch("Particle");
  TClonesArray *branchTrack = treeReader->UseBranch("Track");
  TClonesArray *branchEFlowTrack = treeReader->UseBranch("EFlowTrack");
  TClonesArray *branchEFlowPhoton = treeReader->UseBranch("EFlowPhoton");
  TClonesArray *branchEFlowNeutralHadron = treeReader->UseBranch("EFlowNeutralHadron");
  //TClonesArray *branchEFlowTower = treeReader->UseBranch("EFlowTower");
  //TClonesArray *branchEFlowMuon = treeReader->UseBranch("EFlowMuon");

  // Book histograms
  TH1 *histJetPT = new TH1F("all_jet_pt", "", 185, 200.0, 3900.0);
  TH1 *histJetPT0 = new TH1F("leading_jet_pt", "", 185, 200.0, 3900.0);
  TH1 *histJetPT1 = new TH1F("subleading_jet_pt", "", 185, 200.0, 3900.0);

  TH1 *Invmass = new TH1F("Inv_mass_first_2_jets_from_constituents", "", 200, 0.0, 4000.0);
  TH1 *InvmassSD = new TH1F("Inv_mass_first_2_jets_from_SoftDrop", "", 200, 0.0, 4000.0);

  TH1 *TrackD0 = new TH1F("all_track_D0", "", 48, 0.0, 300.0);
  TH1 *TrackD0Jet = new TH1F("first_2_jets_track_D0", "", 48, 0.0, 300.0);

  // Define objects for Invariant mass calculation
  TObject *object;
  TLorentzVector momentum;
  GenParticle *particle;
  Track *track;
  Tower *tower;

  // Loop over all events
  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);
    
    //HepMCEvent *event = (HepMCEvent*) branchEvent -> At(0);
    //LHEFEvent *event = (LHEFEvent*) branchEvent -> At(0);
    //Float_t weight = event->Weight;

//------------- Jet Filter ------------------
  // Jet Filter: >=2 jets with eta < 2.4

    Int_t nOkJets = 0;

    // Loop over all jets to count jets with eta < 2.4
     for(Int_t jetentry = 0; jetentry < branchJet->GetEntries(); ++jetentry)
       {
       Jet *jet = (Jet*) branchJet->At(jetentry);
       if(jet->Eta < 2.4) nOkJets += 1;
       }

    // If there are less than 2 jets with eta < 2.4 go to next event
    if(nOkJets < 2) continue;

    if(branchJet->GetEntries() ==1) cout << "Filter did not work: there is an event with " <<  branchJet->GetEntries() << " entries" << endl;
    

//------------- End Jet Filter ------------------

    // After the Jet Filter event contains at least 2 jets with eta < 2.4

     // Ok jet counter
     Int_t index = 0;

     // Momentum for invariant mass from constituents
     momentum.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);

     // jetentry of leading and subleading jet
     Int_t firstindex = 0;
     Int_t secondindex = 0;

     // Loop over all jets
     for(Int_t jetentry = 0; jetentry < branchJet->GetEntries(); ++jetentry)
     {
       Jet *jet = (Jet*) branchJet->At(jetentry);
       if(jet->Eta >= 2.4) continue;
       index += 1;

       // Fill jet pT histograms
       histJetPT->Fill(jet->PT);
       if(index == 1) histJetPT0->Fill(jet->PT);
       if(index == 2) histJetPT1->Fill(jet->PT);

      // Print jet transverse momentum 
      // cout << "Jet pt: "<<jet->PT << endl;
      //if(index == 1) cout << "Leading Jet pt: "<<jet->PT << endl;
      //if(index == 2) cout << "Subleading Jet pt: "<<jet->PT << endl;

//---------------- Calculate invariant mass ----------------------

      // Invariant mass from constituents
      if (index == 1 || index == 2){   
        // Loop over leading and subleading jet constituents
        for(Int_t j = 0; j < jet->Constituents.GetEntriesFast(); ++j)
        {
          object = jet->Constituents.At(j);

          // Check if the constituent is accessible
          if(object == 0) continue;
          // cout << jet->Constituents.GetEntriesFast() << endl;

          if(object->IsA() == GenParticle::Class())
          {
            particle = (GenParticle*) object;
           // cout << "    GenPart pt: " << particle->PT << ", eta: " << particle->Eta << ", phi: " << particle->Phi << endl;
            momentum += particle->P4();
          }
          else if(object->IsA() == Track::Class())
          {
            track = (Track*) object;
            // cout << "    Track pt: " << track->PT << ", eta: " << track->Eta << ", phi: " << track->Phi << endl;
            momentum += track->P4();

            // Track D0 of tracks in first two jets
            TrackD0Jet->Fill(track->D0);
            // cout << "    Track D0 in jet: " << track->D0 << endl;
          }
          else if(object->IsA() == Tower::Class())
          {
            tower = (Tower*) object;
            // cout << "    Tower pt: " << tower->ET << ", eta: " << tower->Eta << ", phi: " << tower->Phi << endl;
            momentum += tower->P4();
          }
        }//end for loop on jet constituents
      }//end if


      // Invariant mass from SoftDrop

      if(index == 1) firstindex = jetentry;
      if(index == 2) secondindex = jetentry;

     }// end for loop on jets

     // Invariant mass from constituents
     Invmass->Fill(momentum.M());
     // cout << "Invariant mass from constituents of event " << entry << " : " << momentum.M() << endl;

      // Invariant mass from SoftDrop
     Jet *jet1 = (Jet*) branchJet->At(firstindex);
     Jet *jet2 = (Jet*) branchJet->At(secondindex);

     // jet->SoftDroppedP4[5] : first entry (i = 0) is the total SoftDropped Jet 4-momenta and from i = 1 to 4 are the pruned subjets 4-momenta
     InvmassSD->Fill( (jet1->SoftDroppedP4[0] + jet2->SoftDroppedP4[0]).M()  );
     //  cout << "Invmass from SoftDrop: " << (jet1->SoftDroppedP4[0] + jet2->SoftDroppedP4[0]).M() << endl;


     // All tracks D0

     // Loop over all tracks
     for(Int_t trackentry = 0; trackentry < branchTrack->GetEntries(); ++trackentry)
     {
       Track *alltrack = (Track*) branchTrack->At(trackentry);
       TrackD0->Fill(alltrack->D0);
     }
    
  }

  // Canva to draw pT histograms
  TCanvas *c1 = new TCanvas("c1", "c1", 0, 0, 800, 600);
  gPad->SetLogy();
  gStyle->SetOptStat(0);
  gPad->SetLeftMargin(0.15);

  // Customize histograms
  histJetPT->Sumw2();
  histJetPT0->Sumw2();
  histJetPT1->Sumw2();

  histJetPT->Scale(1.0/(histJetPT->GetEntries() * histJetPT->GetXaxis()->GetBinWidth(2)));
  histJetPT0->Scale(1.0/(histJetPT0->GetEntries() * histJetPT0->GetXaxis()->GetBinWidth(2)));
  histJetPT1->Scale(1.0/(histJetPT1->GetEntries() * histJetPT1->GetXaxis()->GetBinWidth(2)));

  histJetPT->SetMarkerStyle(kFullCircle);
  histJetPT0->SetMarkerStyle(kFullCircle);
  histJetPT1->SetMarkerStyle(kFullCircle);

  histJetPT->SetLineColor(kBlack);
  histJetPT->SetMarkerColor(kBlack);
  histJetPT0->SetLineColor(kRed);
  histJetPT0->SetMarkerColor(kRed);
  histJetPT1->SetLineColor(kBlue);
  histJetPT1->SetMarkerColor(kBlue);

  histJetPT->GetXaxis()->SetRangeUser(200.0, 3900.0);
  histJetPT->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  histJetPT->GetYaxis()->SetTitle("#frac{1}{N} #frac{dN}{dp_{T}}");

  // Show resulting histograms
  histJetPT->Draw("E,HIST");
  histJetPT0->Draw("E,HIST same");
  histJetPT1->Draw("E,HIST same");

  // Legend
  TLegend* legend = new TLegend(0.401003,0.631944,0.878446,0.833333);
  legend->SetHeader("#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV}","C"); // option "C" allows to center the header
  legend->AddEntry(histJetPT,"All Jet p_{T}");
  legend->AddEntry(histJetPT0,"Leading Jet p_{T}");
  legend->AddEntry(histJetPT1,"Subleading Jet p_{T}");
  TLegendEntry *header = (TLegendEntry*)legend->GetListOfPrimitives()->First();
  header->SetTextSize(0.03);
  gStyle->SetLegendTextSize(0.03);
  legend->SetBorderSize(0);
  legend->Draw();

  //c1->SaveAs("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250210_Task8/JetpT.pdf");
  c1->Close();

  // Canva to draw Invariant mass histogram
  TCanvas *c2 = new TCanvas("c2", "c2", 0, 0, 800, 600);
  gStyle->SetOptStat(0);
  gPad->SetLeftMargin(0.15);

  // Customize histograms
  Invmass->Sumw2();
  InvmassSD->Sumw2();

  Invmass->Scale(1.0/(Invmass->GetEntries() * Invmass->GetXaxis()->GetBinWidth(2)));
  InvmassSD->Scale(1.0/(InvmassSD->GetEntries() * InvmassSD->GetXaxis()->GetBinWidth(2)));

  Invmass->SetMarkerStyle(kFullCircle);
  InvmassSD->SetMarkerStyle(kFullCircle);

  Invmass->SetLineColor(kBlue);
  Invmass->SetMarkerColor(kBlue);
  InvmassSD->SetLineColor(kRed);
  InvmassSD->SetMarkerColor(kRed);

  Invmass->GetXaxis()->SetTitle("Invariant Mass [GeV/c^2]");
  Invmass->GetYaxis()->SetTitle("#frac{1}{N} #frac{dN}{dM_{inv}}");

  // Show resulting histograms
  Invmass->Draw("E,HIST");
  InvmassSD->Draw("E,HIST same");

  // Legend
  TLegend* legendIM = new TLegend(0.526316,0.614583,0.89599,0.854167);
  legendIM->SetHeader("#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{#splitline{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV}{Invariant Mass of first two jets}}","C"); // option "C" allows to center the header
  legendIM->AddEntry(Invmass,"From constituents");
  legendIM->AddEntry(InvmassSD,"From SoftDrop");
  TLegendEntry *headerIM = (TLegendEntry*)legendIM->GetListOfPrimitives()->First();
  headerIM->SetTextSize(0.03);
  gStyle->SetLegendTextSize(0.03);
  legendIM->SetBorderSize(0);
  legendIM->Draw();

  //c2->SaveAs("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250210_Task8/InvariantMass.pdf");
  c2->Close();

  // Canva to draw Track D0 histograms
  TCanvas *c3 = new TCanvas("c3", "c3", 0, 0, 800, 600);
  gPad->SetLogy();
  gStyle->SetOptStat(0);
  gPad->SetLeftMargin(0.15);

  TrackD0->Sumw2();
  TrackD0Jet->Sumw2();

  TrackD0->Scale(1.0/(TrackD0->GetEntries() * TrackD0->GetXaxis()->GetBinWidth(2)));
  TrackD0Jet->Scale(1.0/(TrackD0Jet->GetEntries() * TrackD0Jet->GetXaxis()->GetBinWidth(2)));

  TrackD0->SetMarkerStyle(kFullCircle);
  TrackD0Jet->SetMarkerStyle(kFullCircle);

  TrackD0->SetLineColor(kBlue);
  TrackD0->SetMarkerColor(kBlue);
  TrackD0Jet->SetLineColor(kRed);
  TrackD0Jet->SetMarkerColor(kRed);

  TrackD0->GetXaxis()->SetTitle("Track D0 [mm]");
  TrackD0->GetYaxis()->SetTitle("#frac{1}{N} #frac{dN}{dD0}");

  // Show resulting histograms
  TrackD0->Draw("E,HIST");
  TrackD0Jet->Draw("E,HIST same");

  // Legend
  TLegend* legendD0 = new TLegend(0.526316,0.614583,0.89599,0.854167);
  legendD0->SetHeader("#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{#splitline{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV}{Track D0 of}}","C"); // option "C" allows to center the header
  legendD0->AddEntry(TrackD0,"all tracks");
  legendD0->AddEntry(TrackD0Jet,"first two jets tracks");
  TLegendEntry *headerD0 = (TLegendEntry*)legendD0->GetListOfPrimitives()->First();
  headerD0->SetTextSize(0.03);
  gStyle->SetLegendTextSize(0.03);
  legendD0->SetBorderSize(0);
  legendD0->Draw();

  c3->SaveAs("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250210_Task8/TrackD0.pdf");
  //c3->Close();

//----------------------------------------------
  // write all plots on file
  TFile *f = new TFile("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250210_Task8/plots.root","RECREATE");
  histJetPT->Write("",TObject::kOverwrite);
  histJetPT0->Write("",TObject::kOverwrite);
  histJetPT1->Write("",TObject::kOverwrite);

  Invmass->Write("",TObject::kOverwrite);
  InvmassSD->Write("",TObject::kOverwrite);

  TrackD0->Write("",TObject::kOverwrite);
  TrackD0Jet->Write("",TObject::kOverwrite);

}


