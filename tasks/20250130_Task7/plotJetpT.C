/*
Author: Giulia Cossutti

Macro to plot the leading and subleading jet distribution

From inside the /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes directory run with
root -l EJ_reproduction/tasks/20250130_Task7/plotJetpT.C'("EJ_reproduction/tasks/20250130_Task7/20K.root")'
*/

#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/classes/DelphesClasses.h"
#include "/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/external/ExRootAnalysis/ExRootTreeReader.h"
#endif

//------------------------------------------------------------------------------

void plotJetpT(const char *inputFile)
{
  gSystem->Load("libDelphes");

  // Create chain of root trees
  TChain chain("Delphes");
  chain.Add(inputFile);

  // Create object of class ExRootTreeReader
  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  Long64_t numberOfEntries = treeReader->GetEntries();

  // Get pointers to branches used in this analysis
  TClonesArray *branchJet = treeReader->UseBranch("Jet");
  TClonesArray *branchEvent = treeReader->UseBranch("Event");

  // Book histograms
  TH1 *histJetPT0 = new TH1F("leading_jet_pt", "", 150, 15.0, 400.0);
  TH1 *histJetPT1 = new TH1F("subleading_jet_pt", "", 150, 15.0, 400.0);

  // Loop over all events
  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);

    
    //HepMCEvent *event = (HepMCEvent*) branchEvent -> At(0);
    //LHEFEvent *event = (LHEFEvent*) branchEvent -> At(0);
    //Float_t weight = event->Weight;

    // If event contains at least 2 jets
    if(branchJet->GetEntries() > 0)
    {
      // Take first jet
      Jet *jet0 = (Jet*) branchJet->At(0);

      // Plot jet transverse momentum
      histJetPT0->Fill(jet0->PT);

      // Print jet transverse momentum 
      //cout << "Leading Jet pt: "<<jet0->PT << endl;

       if(branchJet->GetEntries() > 1){
          // Take second jet
          Jet *jet1 = (Jet*) branchJet->At(1);

          // Plot jet transverse momentum
          histJetPT1->Fill(jet1->PT);

          // Print jet transverse momentum 
          //cout << "Subleading Jet pt: "<<jet1->PT << endl;
       }
    }

  }

  // Canva to draw histograms
  TCanvas *c1 = new TCanvas("c1", "c1", 0, 0, 800, 600);
  gPad->SetLogy();
  gStyle->SetOptStat(0);
  gPad->SetLeftMargin(0.15);

  // Customize histograms
  histJetPT0->Sumw2();
  histJetPT1->Sumw2();

  histJetPT0->Scale(1.0/(histJetPT0->GetEntries() * histJetPT0->GetXaxis()->GetBinWidth(2)));
  histJetPT1->Scale(1.0/(histJetPT1->GetEntries() * histJetPT1->GetXaxis()->GetBinWidth(2)));

  histJetPT0->SetMarkerStyle(kFullCircle);
  histJetPT1->SetMarkerStyle(kFullCircle);

  histJetPT0->SetLineColor(kRed);
  histJetPT0->SetMarkerColor(kRed);
  histJetPT1->SetLineColor(kBlue);
  histJetPT1->SetMarkerColor(kBlue);

  histJetPT0->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  histJetPT0->GetYaxis()->SetTitle("#frac{1}{N} #frac{dN}{dp_{T}}");

  // Show resulting histograms
  histJetPT0->Draw("E,HIST");
  histJetPT1->Draw("E,HIST same");

  // Legend
  TLegend* legend = new TLegend(0.327068,0.65625,0.870927,0.855903);
  legend->SetHeader("#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV}","C"); // option "C" allows to center the header
  legend->AddEntry(histJetPT0,"Leading Jet p_{T}");
  legend->AddEntry(histJetPT1,"Subleading Jet p_{T}");
  TLegendEntry *header = (TLegendEntry*)legend->GetListOfPrimitives()->First();
  header->SetTextSize(0.03);
  gStyle->SetLegendTextSize(0.03);
  legend->SetBorderSize(0);
  legend->Draw();

  c1->SaveAs("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250130_Task7/JetpT.pdf");

  TFile *f = new TFile("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250130_Task7/plots.root","RECREATE");
  histJetPT0->Write("",TObject::kOverwrite);
  histJetPT1->Write("",TObject::kOverwrite);


}

