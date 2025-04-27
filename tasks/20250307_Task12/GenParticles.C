/*
Author: Giulia Cossutti

Studies with MC Truth GenParticles on ppbar events
Macro to plot Invariant Mass of Zv's daughters
Macro to plot DeltaR, DeltaEta, DeltaPhi between Zv's daughters
Macro to plot Eta of Zv's daughters
Macro to plot pT of all, leading and subleading Zv's daughters

From inside the /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes directory run with
root -l EJ_reproduction/tasks/20250307_Task12/GenParticles.C'("EJ_reproduction/tasks/20250307_Task12/50K_Reclustering_ppbar.root",selection)'
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
void CustomizeHist(TH1F* hist, Style_t style, Color_t color, Bool_t scale = kFALSE, Double_t mrksize = 0.8){
  hist->Sumw2();
  if(scale == kTRUE) hist->Scale(1.0/(hist->GetEntries() * hist->GetXaxis()->GetBinWidth(2)));
  hist->SetMarkerStyle(style);
  hist-> SetMarkerColor(color);
  hist-> SetLineColor(color);
  hist->SetMarkerSize(mrksize);
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
//=================================
// Function to draw histograms in a canva and save it in pdf
void DrawHist(Int_t sel, std::vector<TH1F*> hist, Bool_t Norm, TString XTitle, TString YTitle, std::array<Double_t,4> leglimits, TString legtitle, std::vector<TString> legentry, TString filename){
  if( hist.size() != legentry.size() ){
    cout << "Error in DrawHist: number of histograms not defined in " << legtitle << endl;
    cout << "hist size: " << hist.size() << "   legend entry size: " << legentry.size() << endl;
    exit(EXIT_FAILURE);
  }

  TString canva = "c" + filename;

  // Canva to draw histograms
  TCanvas *c = new TCanvas(canva, canva, 0, 0, 800, 600);
  gStyle->SetOptStat(0);
  gPad->SetLeftMargin(0.15);
  gPad->SetLogy();
  //gPad->SetLogx();

  hist.at(0)->GetXaxis()->SetTitle(XTitle);
  if(Norm == kTRUE){hist.at(0)->GetYaxis()->SetTitle(YTitle);}
  else{hist.at(0)->GetYaxis()->SetTitle("a.u.");}
  if(filename=="qDPT") hist.at(0)->GetYaxis()->SetRangeUser(0.000001,0.005);

  hist.at(0)->Draw("E,HIST");
  for(Int_t i = 1; i < hist.size(); ++i){
   hist.at(i)->Draw("E,HIST same");
  }

  // Legend
  TLegend* legend = new TLegend(leglimits[0],leglimits[1],leglimits[2],leglimits[3]);

  TString head;
  if(sel==0) head = "#splitline{#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV}}{ppbar - EJ selection - " + legtitle + "}";
  if(sel==1) head = "#splitline{#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV}}{ppbar - no sel. on pT - " + legtitle + "}";
  if(sel==2) head = "#splitline{#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV}}{ppbar - no selection - " + legtitle + "}";

  legend->SetHeader(head,"C");

  for(Int_t i = 0; i < hist.size(); ++i){
    legend->AddEntry(hist.at(i),legentry.at(i));
  }

  CustomizeLeg(legend);
  legend->Draw();

  TString printname;
  TString dir = "/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250307_Task12/";
  if(Norm==kFALSE){
    if(sel==0) printname = dir + filename + "_ppbar.pdf";
    if(sel==1) printname = dir + filename + "_ppbar_nopT.pdf";
    if(sel==2) printname = dir + filename + "_ppbar_nosel.pdf";
  }else if(Norm==kTRUE){
    if(sel==0) printname = dir + filename + "_ppbar_Norm.pdf";
    if(sel==1) printname = dir + filename + "_ppbar_nopT_Norm.pdf";
    if(sel==2) printname = dir + filename + "_ppbar_nosel_Norm.pdf";
  }

  c->SaveAs(printname);

  //c->Close();
}
//===============================
// Function to compute DeltaR between 2 particles
Double_t DeltaR(GenParticle* part1, GenParticle* part2){
 return sqrt( pow(part1->Eta - part2->Eta,2) + pow(part1->Phi - part2->Phi,2) );
}
//===============================
// Function to compute DeltaEta between 2 particles
Double_t DeltaEta(GenParticle* part1, GenParticle* part2){
 return part1->Eta - part2->Eta;
}
//===============================
// Function to compute DeltaPhi between 2 particles
Double_t DeltaPhi(GenParticle* part1, GenParticle* part2){
 return part1->Phi - part2->Phi;
}
//===============================
//---------- END FUNCTIONS -------------------

void GenParticles(const char *inputFile,Int_t selection=0)
{
  // Event selection & Small and Large Jet definition: 
  // if 0: consider only events with >=2 small jets with |eta| < 2.4 and pT > 125 GeV + small jets have |eta| < 2.5 + large jets have |eta| < 1.8 (EJ analysis)
  // if 1: consider only events with >=2 small jets with |eta| < 2.4 + small jets have |eta| < 2.5 + large jets have |eta| < 1.8 
  // if 2: all events are used (+ no jets are used)
  //Int_t selection = 0;

  // Normalisation of histograms
  Bool_t Norm = kTRUE;

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
  /*  
  TClonesArray *branchEFlowTrack = treeReader->UseBranch("EFlowTrack");
  TClonesArray *branchEFlowPhoton = treeReader->UseBranch("EFlowPhoton");
  TClonesArray *branchEFlowNeutralHadron = treeReader->UseBranch("EFlowNeutralHadron");
  TClonesArray *branchGenMissingET = treeReader->UseBranch("GenMissingET");
  TClonesArray *branchMissingET = treeReader->UseBranch("MissingET");
  TClonesArray *branchElectron = treeReader->UseBranch("Electron");
  TClonesArray *branchMuon = treeReader->UseBranch("Muon");
  TClonesArray *branchPhoton = treeReader->UseBranch("Photon");
  TClonesArray *branchScalarHT = treeReader->UseBranch("ScalarHT");
  */
  TClonesArray *branchJetEnergyScaledJet = treeReader->UseBranch("JetEnergyScaledJet");
  TClonesArray *branchECalTower = treeReader->UseBranch("ECalTower");
  TClonesArray *branchHCalTower = treeReader->UseBranch("HCalTower");
  TClonesArray *branchMuonMomentumSmearing = treeReader->UseBranch("MuonMomentumSmearing");

  // Book histograms
  TH1F *Invmass = new TH1F("Invariant_mass_of_Zv_daughters", "", 162, 0.0, 6480.0);
  TH1F *qDDeltaR = new TH1F("DeltaR_between_Zv_daughters", "", 140, 0., 14.);
  TH1F *qDDeltaEta = new TH1F("DeltaEta_between_Zv_daughters", "", 280, -12., 16.);
  TH1F *qDDeltaPhi = new TH1F("DeltaPhi_between_Zv_daughters", "", 240, -8., 16.);
  TH1F *qDEta = new TH1F("Eta_of_Zv_daughters", "", 220, -8., 14.);

  Double_t PTmax = 3000.;
  TH1F *qDPT = new TH1F("all_Zv_daughters_pt", "", PTmax/20., 0.0, PTmax);
  TH1F *qDPT1 = new TH1F("Leading_Zv_daughter_pt", "", PTmax/20., 0.0, PTmax);
  TH1F *qDPT2 = new TH1F("Subleading_Zv_daughter_pt", "", PTmax/20., 0.0, PTmax);


  // Counter of used Zv
  Int_t NofZv = 0;

  // Loop over all events
  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);

  //------------- Event selection ------------------
  // Event selection on jets: >=2 jets with |eta| < 2.4 and pT > 125 GeV

    Int_t nOkJets = 0;

    // Loop over all jets to count jets with eta < 2.4 and pT > 125 GeV
     for(Int_t jetentry = 0; jetentry < branchJet->GetEntries(); ++jetentry)
       {
       Jet *jet = (Jet*) branchJet->At(jetentry);
       if(selection == 0 & abs(jet->Eta) < 2.4 & jet->PT > 125.0) nOkJets += 1;
       if(selection == 1 & abs(jet->Eta) < 2.4) nOkJets += 1;
       }

    // If there are less than 2 jets with eta < 2.4 and pT > 125 GeV go to next event
    if( (selection == 0 || selection == 1 ) & (nOkJets < 2) ) continue;

    //if(branchJet->GetEntries() ==1) cout << "Filter did not work: there is an event with " <<  branchJet->GetEntries() << " entries" << endl;
    
  //------------- End Event selection ------------------

    // Loop over all particles to find Zv and its daughters
    for(Int_t partentry = 0; partentry < branchParticle->GetEntries(); ++partentry)
     {
      GenParticle *part = (GenParticle*) branchParticle->At(partentry);

      // Select the Zv
      if( (part->PID != 4900023) || (part->D1 == part->D2) ) continue;
      NofZv += 1;

      // Zv's daughters  
      GenParticle *qD1 = (GenParticle*) branchParticle->At(part->D1);
      GenParticle *qD2 = (GenParticle*) branchParticle->At(part->D2);

      //cout << "Event " << entry << " Zv: " << part->PID << " decayed into " << part->D1 << " that is " << qD1->PID << " and " << part->D2 << " that is " << qD2->PID << endl;
      //if( (abs(qD1->PID) != 4900101) || (abs(qD2->PID) != 4900101) ) cout << "Event " << entry << " Zv: " << part->PID << " decayed into " << part->D1 << " that is " << qD1->PID << " and " << part->D2 << " that is " << qD2->PID << endl;
      
      // Fill pT histograms
      qDPT->Fill(qD1->PT);
      qDPT->Fill(qD2->PT);
      if( qD1->PT > qD2->PT ){
        qDPT1->Fill(qD1->PT);
        qDPT2->Fill(qD2->PT);        
      }else{
        qDPT1->Fill(qD2->PT);
        qDPT2->Fill(qD1->PT);        
      }

      // Build P4 of Zv's daughters
      TLorentzVector *qD1P4 = new TLorentzVector(1.0,1.0,1.0,1.0);
      qD1P4->SetPtEtaPhiM(qD1->PT,qD1->Eta,qD1->Phi,qD1->Mass);
      TLorentzVector *qD2P4 = new TLorentzVector(1.0,1.0,1.0,1.0);
      qD2P4->SetPtEtaPhiM(qD2->PT,qD2->Eta,qD2->Phi,qD2->Mass);

      // Fill Invariant Mass, DeltaR, DeltaEta, DeltaPhi, Eta histograms
      Invmass->Fill( (*qD1P4 + *qD2P4).M()  );
      qDDeltaR->Fill( DeltaR(qD1, qD2) );
      qDDeltaEta->Fill( DeltaEta(qD1, qD2) );
      qDDeltaPhi->Fill( DeltaPhi(qD1, qD2) );
      qDEta->Fill( qD1->Eta );
      qDEta->Fill( qD2->Eta );

      break;
     }// end loop over all particles

  }// end loop over all events

//------------------------------------------------

  cout << "Number of used Zv: " << NofZv << endl;

  // Draw Invariant Mass histogram
  CustomizeHist(Invmass, kFullSquare, kBlue, Norm, 0.8);
  std::vector<TH1F*> vecInvmass = {Invmass};
  std::array<Double_t,4> leglimitsInvmass = {0.556391,0.652778,0.867168,0.878472};
  std::vector<TString> legentryInvmass = {"Zv's daughters"};
  DrawHist(selection,vecInvmass,Norm,"Invariant Mass [GeV/c^2]","#frac{1}{N} #frac{dN}{dM_{inv}} [c^2/GeV]",leglimitsInvmass,"Invariant Mass",legentryInvmass,"qDInvMass");

  // Draw DeltaR histogram
  CustomizeHist(qDDeltaR, kFullSquare, kBlue, Norm, 0.8);
  std::vector<TH1F*> vecDeltaR = {qDDeltaR};
  std::array<Double_t,4> leglimitsDeltaR = {0.556391,0.652778,0.867168,0.878472};
  std::vector<TString> legentryDeltaR = {"Zv's daughters"};
  DrawHist(selection,vecDeltaR,Norm,"#Delta R","#frac{1}{N} #frac{dN}{d #Delta R}",leglimitsDeltaR,"#Delta R between",legentryDeltaR,"qDDeltaR");

  // Draw DeltaEta histogram
  CustomizeHist(qDDeltaEta, kFullSquare, kBlue, Norm, 0.8);
  std::vector<TH1F*> vecDeltaEta = {qDDeltaEta};
  std::array<Double_t,4> leglimitsDeltaEta = {0.572682,0.685764,0.867168,0.878472};
  std::vector<TString> legentryDeltaEta = {"Zv's daughters"};
  DrawHist(selection,vecDeltaEta,Norm,"#Delta #eta","#frac{1}{N} #frac{dN}{d #Delta #eta}",leglimitsDeltaEta,"#Delta #eta between",legentryDeltaEta,"qDDeltaEta");

  // Draw DeltaPhi histogram
  CustomizeHist(qDDeltaPhi, kFullSquare, kBlue, Norm, 0.8);
  std::vector<TH1F*> vecDeltaPhi = {qDDeltaPhi};
  std::array<Double_t,4> leglimitsDeltaPhi = {0.556391,0.652778,0.867168,0.878472};
  std::vector<TString> legentryDeltaPhi = {"Zv's daughters"};
  DrawHist(selection,vecDeltaPhi,Norm,"#Delta #varphi","#frac{1}{N} #frac{dN}{d #Delta #varphi}",leglimitsDeltaPhi,"#Delta #varphi between",legentryDeltaPhi,"qDDeltaPhi");

  // Draw Eta histogram
  CustomizeHist(qDEta, kFullSquare, kBlue, Norm, 0.8);
  std::vector<TH1F*> vecEta = {qDEta};
  std::array<Double_t,4> leglimitsEta = {0.556391,0.652778,0.867168,0.878472};
  std::vector<TString> legentryEta = {"Zv's daughters"};
  DrawHist(selection,vecEta,Norm,"#eta","#frac{1}{N} #frac{dN}{d #eta}",leglimitsEta,"#eta of",legentryEta,"qDEta");

  // Draw PT histograms
  CustomizeHist(qDPT, kFullSquare, kBlue, Norm, 0.5);
  CustomizeHist(qDPT1, kFullSquare, kRed, Norm, 0.5);
  CustomizeHist(qDPT2, kFullSquare, kBlack, Norm, 0.5);
  std::vector<TH1F*> vecPT = {qDPT,qDPT1,qDPT2};
  std::array<Double_t,4> leglimitsPT = {0.567669,0.5625,0.878446,0.873264};
  std::vector<TString> legentryPT = {"all Zv's daughters","Zv's leading daughter","Zv's subleading daughter"};
  DrawHist(selection,vecPT,Norm,"p_{T} [GeV/c]","#frac{1}{N} #frac{dN}{dp_{T}} [c/GeV]",leglimitsPT,"pT of",legentryPT,"qDPT");

//-------------------------------------------------------
  // write all plots on file
  if(selection==0 & Norm==kFALSE) TFile *f = new TFile("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250307_Task12/qD_ppbar.root","RECREATE");
  if(selection==1 & Norm==kFALSE) TFile *f = new TFile("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250307_Task12/qD_ppbar_nopT.root","RECREATE");
  if(selection==2 & Norm==kFALSE) TFile *f = new TFile("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250307_Task12/qD_ppbar_nopTeta.root","RECREATE");
  if(selection==0 & Norm==kTRUE) TFile *f = new TFile("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250307_Task12/qD_ppbar_Norm.root","RECREATE");
  if(selection==1 & Norm==kTRUE) TFile *f = new TFile("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250307_Task12/qD_ppbar_nopT_Norm.root","RECREATE");
  if(selection==2 & Norm==kTRUE) TFile *f = new TFile("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250307_Task12/qD_ppbar_nopTeta_Norm.root","RECREATE");

  Invmass->Write("",TObject::kOverwrite);
  qDDeltaR->Write("",TObject::kOverwrite);
  qDDeltaEta->Write("",TObject::kOverwrite);
  qDDeltaPhi->Write("",TObject::kOverwrite);
  qDEta->Write("",TObject::kOverwrite);

  qDPT->Write("",TObject::kOverwrite);
  qDPT1->Write("",TObject::kOverwrite);
  qDPT2->Write("",TObject::kOverwrite);

}
