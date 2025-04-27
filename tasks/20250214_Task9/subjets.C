/*
Author: Giulia Cossutti

Macro to plot number of subjets with R=0.4 inside R=1.0 jets
Plot pT of jet and sum of leading and subleading subjets pT

From inside the /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes directory run with
root -l EJ_reproduction/tasks/20250214_Task9/subjets.C'("EJ_reproduction/tasks/20250214_Task9/50K_subjets.root")'
or
root -l EJ_reproduction/tasks/20250214_Task9/subjets.C'("EJ_reproduction/tasks/20250214_Task9/50K_subjets_Rprun.root")'
*/

#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/classes/DelphesClasses.h"
#include "/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#endif

//------------------------------------------------------------------------------

// Function to customize histograms:
// - Scale by number of entries and bin width
// - Customize markers and lines
void CustomizeHist(TH1F* hist, Style_t style, Color_t color, Bool_t scale = kFALSE){
  hist->Sumw2();
  if(scale == kTRUE) hist->Scale(1.0/(hist->GetEntries() * hist->GetXaxis()->GetBinWidth(2)));
  hist->SetMarkerStyle(style);
  hist-> SetMarkerColor(color);
  hist-> SetLineColor(color);
}

// Function to customize legends:
// - Customize text size of header and body
// - No border
void CustomizeLeg(TLegend* legend){
  TLegendEntry *header = (TLegendEntry*)legend->GetListOfPrimitives()->First();
  header->SetTextSize(0.03);
  gStyle->SetLegendTextSize(0.03);
  legend->SetBorderSize(0);
}


void subjets(const char *inputFile)
{
  // decide on legend and file titles
  const char * Rprunfile = "EJ_reproduction/tasks/20250214_Task9/50K_subjets_Rprun.root";

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

  // Book histograms
  TH1F *Nsubj = new TH1F("number_of_subjets", "", 11, -0.5, 10.5);
  TH1F *Nsubj_first = new TH1F("number_of_subjets_in_first_two_jets", "", 11, -0.5, 10.5);

  TH1F *JetPT = new TH1F("all_jet_pt", "", 200, 0.0, 4000.0);
  TH1F *JetPT1 = new TH1F("leading_jet_pt", "", 200, 0.0, 4000.0);
  TH1F *JetPT2 = new TH1F("subleading_jet_pt", "", 200, 0.0, 4000.0);
  TH1F *JetPT12 = new TH1F("pt_of_first_two_jets", "", 200, 0.0, 4000.0);
  TH1F *JetPT1s = new TH1F("pt_of_first_two_subjets_inside_leading_jet", "", 200, 0.0, 4000.0);
  TH1F *JetPT2s = new TH1F("pt_of_first_two_subjets_inside_subleading_jet", "", 200, 0.0, 4000.0);

  // Loop over all events
  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);

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

     // Ok jet counter
     Int_t index = 0;

     // jetentry of leading and subleading jet
     Int_t firstindex = 0;
     Int_t secondindex = 0;

     // Loop over all jets
     for(Int_t jetentry = 0; jetentry < branchJet->GetEntries(); ++jetentry)
     {
       Jet *jet = (Jet*) branchJet->At(jetentry);
       if(jet->Eta >= 2.4) continue;
       index += 1;

       // Fill number of subjets histograms
       Nsubj->Fill(jet->NSubJetsSoftDropped);
       if (index == 1 || index == 2) Nsubj_first->Fill(jet->NSubJetsSoftDropped);

       // Fill all jet pT histogram from SoftDrop
       JetPT->Fill( jet->SoftDroppedP4[0].Pt() );

      // Indexes of leading and subleading jets
      if(index == 1) firstindex = jetentry;
      if(index == 2) secondindex = jetentry;

     }// end for loop on jets

     // Take leading and subleading jets
     Jet *jet1 = (Jet*) branchJet->At(firstindex);
     Jet *jet2 = (Jet*) branchJet->At(secondindex);

     // jet->SoftDroppedP4[5] : first entry (i = 0) is the total SoftDropped Jet 4-momenta and from i = 1 to 4 are the pruned subjets 4-momenta

     // Pt of leading or subleading jet from SoftDrop
     JetPT1->Fill( jet1->SoftDroppedP4[0].Pt() );
     JetPT2->Fill( jet2->SoftDroppedP4[0].Pt() );

     // Pt of sum of first two jets or subjets from SoftDrop
     JetPT12->Fill( (jet1->SoftDroppedP4[0] + jet2->SoftDroppedP4[0]).Pt()  );
     JetPT1s->Fill( (jet1->SoftDroppedP4[1] + jet1->SoftDroppedP4[2]).Pt()  );
     JetPT2s->Fill( (jet2->SoftDroppedP4[1] + jet2->SoftDroppedP4[2]).Pt()  );

  }// end for loop on events

  // Canva to draw Number of subjets histograms
  TCanvas *c1 = new TCanvas("c1", "c1", 0, 0, 800, 600);
  gStyle->SetOptStat(0);
  gPad->SetLeftMargin(0.15);

  CustomizeHist(Nsubj, kFullSquare, kBlue, kFALSE);
  CustomizeHist(Nsubj_first, kFullCircle, kRed, kFALSE);

  Nsubj->GetXaxis()->SetTitle("Number of subjets");
  //Nsubj->GetYaxis()->SetTitle("#frac{1}{N} #frac{dN}{dN_{subjets}}");
  Nsubj->GetYaxis()->SetTitle("# jets");

  // Show resulting histograms
  Nsubj->Draw("E,HIST");
  Nsubj_first->Draw("E,HIST same");

  // Legend
  TLegend* legend = new TLegend(0.401003,0.572917,0.877193,0.833333);

  if(strcmp(inputFile, Rprunfile) != 0){legend->SetHeader("#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{#splitline{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV}{Jets: antikT R=1.0; Subjets: SoftDrop R=0.4}}","C");}
  else{legend->SetHeader("#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{#splitline{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV}{Jets: antikT R=1.0; Subjets: SoftDrop&Rprun R=0.4}}","C"); };

  legend->AddEntry(Nsubj,"All Jets");
  legend->AddEntry(Nsubj_first,"Leading and Subleading Jets");
  CustomizeLeg(legend);
  legend->Draw();

  
  if(strcmp(inputFile, Rprunfile) != 0){ c1->SaveAs("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250214_Task9/Nsubjets.pdf"); }
  else{ c1->SaveAs("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250214_Task9/Nsubjets_Rprun.pdf"); };
  
  //c1->Close();

  // Canva to draw pT histograms
  TCanvas *c2 = new TCanvas("c2", "c2", 0, 0, 1000, 600);
  gStyle->SetOptStat(0);
  gPad->SetLogy();
  gPad->SetLeftMargin(0.15);

  CustomizeHist(JetPT, kFullSquare, kBlack, kTRUE);
  CustomizeHist(JetPT1, kFullSquare, kViolet, kTRUE);
  CustomizeHist(JetPT2, kFullSquare, kBlue, kTRUE);
  CustomizeHist(JetPT12, kFullCircle, kGreen, kTRUE);
  CustomizeHist(JetPT1s, kFullCircle, kOrange, kTRUE);
  CustomizeHist(JetPT2s, kFullCircle, kRed, kTRUE);

  JetPT->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  JetPT->GetYaxis()->SetTitle("#frac{1}{N} #frac{dN}{dp_{T}}");

  // Show resulting histograms
  JetPT->Draw("E,HIST");
  JetPT1->Draw("E,HIST same");
  JetPT2->Draw("E,HIST same");
  JetPT12->Draw("E,HIST same");
  JetPT1s->Draw("E,HIST same");
  JetPT2s->Draw("E,HIST same");

  // Legend
  TLegend* legend_pT = new TLegend(0.492481,0.314236,0.87594,0.833333);
  
  if(strcmp(inputFile, Rprunfile) != 0){legend_pT->SetHeader("#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{#splitline{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV}{Jets: antikT R=1.0; Subjets: SoftDrop R=0.4}}","C"); }
  else{legend_pT->SetHeader("#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{#splitline{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV}{Jets: antikT R=1.0; Subjets: SoftDrop&Rprun R=0.4}}","C"); };
  
  legend_pT->AddEntry(JetPT,"All Jets pT");
  legend_pT->AddEntry(JetPT1,"Leading Jet pT");
  legend_pT->AddEntry(JetPT2,"Subleading Jet pT");
  legend_pT->AddEntry(JetPT12,"L + Subl Jet pT");
  legend_pT->AddEntry(JetPT1s,"L + Subl Subjet pT of L");
  legend_pT->AddEntry(JetPT2s,"L + Subl Subjet pT of Subl");
  CustomizeLeg(legend_pT);
  legend_pT->Draw();

  
  if(strcmp(inputFile, Rprunfile) != 0){ c2->SaveAs("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250214_Task9/JetpT.pdf"); }
  else{ c2->SaveAs("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250214_Task9/JetpT_Rprun.pdf"); };
  
  //c2->Close();

//----------------------------------------------
  // write all plots on file
  if(strcmp(inputFile, Rprunfile) != 0){ TFile *f = new TFile("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250214_Task9/plots.root","RECREATE");}
  else{ TFile *f = new TFile("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250214_Task9/plots_Rprun.root","RECREATE");};

  Nsubj->Write("",TObject::kOverwrite);
  Nsubj_first->Write("",TObject::kOverwrite);

  JetPT->Write("",TObject::kOverwrite);
  JetPT1->Write("",TObject::kOverwrite);
  JetPT2->Write("",TObject::kOverwrite);
  JetPT12->Write("",TObject::kOverwrite);
  JetPT1s->Write("",TObject::kOverwrite);
  JetPT2s->Write("",TObject::kOverwrite);


}


