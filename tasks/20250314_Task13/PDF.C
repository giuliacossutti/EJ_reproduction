/*
Author: Giulia Cossutti

Studies with MC Truth GenParticles 
Macro to plot Invariant Mass of Zv's daughters distinguishing between Zv's mothers

From inside the /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes directory run with
root -l EJ_reproduction/tasks/20250314_Task13/PDF.C'("EJ_reproduction/tasks/20250221_Task10/50K_Reclustering.root",selection)'
root -l EJ_reproduction/tasks/20250314_Task13/PDF.C'("EJ_reproduction/tasks/20250307_Task12/50K_Reclustering_ppbar.root",selection)'
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
  if((scale == kTRUE) && (hist->GetEntries() != 0) ) hist->Scale(1.0/(hist->GetEntries() * hist->GetXaxis()->GetBinWidth(2)));
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

  hist.at(0)->Draw("E,HIST");
  for(Int_t i = 1; i < hist.size(); ++i){
   hist.at(i)->Draw("E,HIST same");
  }

  // Legend
  TLegend* legend = new TLegend(leglimits[0],leglimits[1],leglimits[2],leglimits[3]);

  TString head;
  if(sel==0) head = "#splitline{#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV}}{EJ, " + legtitle + "}";
  if(sel==1) head = "#splitline{#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV}}{no pT, " + legtitle + "}";
  if(sel==2) head = "#splitline{#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV}}{no sel, " + legtitle + "}";

  legend->SetHeader(head,"C");

  for(Int_t i = 0; i < hist.size(); ++i){
    legend->AddEntry(hist.at(i),legentry.at(i));
  }

  CustomizeLeg(legend);
  legend->Draw();

  TString printname;
  TString dir = "/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250314_Task13/";
  if(Norm==kFALSE){
    if(sel==0) printname = dir + filename + ".pdf";
    if(sel==1) printname = dir + filename + "_nopT.pdf";
    if(sel==2) printname = dir + filename + "_nosel.pdf";
  }else if(Norm==kTRUE){
    if(sel==0) printname = dir + filename + "_Norm.pdf";
    if(sel==1) printname = dir + filename + "_nopT_Norm.pdf";
    if(sel==2) printname = dir + filename + "_nosel_Norm.pdf";
  }

  c->SaveAs(printname);

  //c->Close();
}
//===============================
//---------- END FUNCTIONS -------------------

void PDF(const char *inputFile,Int_t selection=0)
{
  // Event selection & Small and Large Jet definition: 
  // if 0: consider only events with >=2 small jets with |eta| < 2.4 and pT > 125 GeV + small jets have |eta| < 2.5 + large jets have |eta| < 1.8 (EJ analysis)
  // if 1: consider only events with >=2 small jets with |eta| < 2.4 + small jets have |eta| < 2.5 + large jets have |eta| < 1.8 
  // if 2: all events are used (+ no jets are used)
  //Int_t selection = 0;

  if(selection < 0 || selection > 2){
    cout << " sel must be 0, 1 or 2" << endl;
    exit(EXIT_FAILURE);
  }

  // Normalisation of histograms
  Bool_t Norm = kTRUE;

  // Distinguish between input files
  const char * ppfile = "EJ_reproduction/tasks/20250221_Task10/50K_Reclustering.root";
  const char * ppbarfile = "EJ_reproduction/tasks/20250307_Task12/50K_Reclustering_ppbar.root";

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
  Double_t maxIM = 6640.0;
  TH1F *Invmass_u = new TH1F("Invariant_mass_of_Zv_daughters_uubar_parents", "", maxIM/40. , 0.0, maxIM);
  TH1F *Invmass_d = new TH1F("Invariant_mass_of_Zv_daughters_ddbar_parents", "", maxIM/40. , 0.0, maxIM);
  TH1F *Invmass_c = new TH1F("Invariant_mass_of_Zv_daughters_ccbar_parents", "", maxIM/40. , 0.0, maxIM);
  TH1F *Invmass_s = new TH1F("Invariant_mass_of_Zv_daughters_ssbar_parents", "", maxIM/40. , 0.0, maxIM);
  TH1F *Invmass_b = new TH1F("Invariant_mass_of_Zv_daughters_bbbar_parents", "", maxIM/40. , 0.0, maxIM);

  // Counter of used Zv
  Int_t NofZv = 0;

  // Loop over all events
  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);

  //------------- Event selection ------------------
  // Event selection on jets: >=2 jets with |eta| < 2.4 and pT > 125 GeV

    if(selection == 0 || selection == 1 ){
      Int_t nOkJets = 0;

      // Loop over all jets to count jets with eta < 2.4 and pT > 125 GeV
       for(Int_t jetentry = 0; jetentry < branchJet->GetEntries(); ++jetentry)
         {
         Jet *jet = (Jet*) branchJet->At(jetentry);
         if(selection == 0 & abs(jet->Eta) < 2.4 & jet->PT > 125.0) nOkJets += 1;
         if(selection == 1 & abs(jet->Eta) < 2.4) nOkJets += 1;
         }

      // If there are less than 2 jets with eta < 2.4 and pT > 125 GeV go to next event
      if( nOkJets < 2 ) continue;

      //if(branchJet->GetEntries() ==1) cout << "Filter did not work: there is an event with " <<  branchJet->GetEntries() << " entries" << endl;
    }
  //------------- End Event selection ------------------

    // Zv's mothers  
    GenParticle *qM1;
    GenParticle *qM2;

    // Loop over all particles to find Zv and its daughters
    for(Int_t partentry = 0; partentry < branchParticle->GetEntries(); ++partentry)
     {
      GenParticle *part = (GenParticle*) branchParticle->At(partentry);

      // Select the Zv - first instance
      if( (part->PID == 4900023) && (part->M1 != part->M2) ){
        // Zv's mothers  
        qM1 = (GenParticle*) branchParticle->At(part->M1);
        qM2 = (GenParticle*) branchParticle->At(part->M2);
      }

      // Select the Zv - last instance
      if( (part->PID != 4900023) || (part->D1 == part->D2) ) continue;
      NofZv += 1;

      // Zv's daughters  
      GenParticle *qD1 = (GenParticle*) branchParticle->At(part->D1);
      GenParticle *qD2 = (GenParticle*) branchParticle->At(part->D2);

      //cout << "Event " << entry << " Zv: " << part->PID << " decayed into " << part->D1 << " that is " << qD1->PID << " and " << part->D2 << " that is " << qD2->PID << endl;
      //if( (abs(qD1->PID) != 4900101) || (abs(qD2->PID) != 4900101) ) cout << "Event " << entry << " Zv: " << part->PID << " decayed into " << part->D1 << " that is " << qD1->PID << " and " << part->D2 << " that is " << qD2->PID << endl;
      
      // Build P4 of Zv's daughters
      TLorentzVector *qD1P4 = new TLorentzVector(1.0,1.0,1.0,1.0);
      qD1P4->SetPtEtaPhiM(qD1->PT,qD1->Eta,qD1->Phi,qD1->Mass);
      TLorentzVector *qD2P4 = new TLorentzVector(1.0,1.0,1.0,1.0);
      qD2P4->SetPtEtaPhiM(qD2->PT,qD2->Eta,qD2->Phi,qD2->Mass);

      //cout << "Event " << entry << " Zv produced by " << qM1->PID << " and " << qM2->PID << endl;

      // Fill Invariant Mass histograms
      if(  abs(qM1->PID) == 1 ){
        Invmass_d->Fill( (*qD1P4 + *qD2P4).M()  );
      }else if(  abs(qM1->PID) == 2 ){
        Invmass_u->Fill( (*qD1P4 + *qD2P4).M()  );
      }else if(  abs(qM1->PID) == 3 ){
        Invmass_s->Fill( (*qD1P4 + *qD2P4).M()  );
      }else if(  abs(qM1->PID) == 4 ){
        Invmass_c->Fill( (*qD1P4 + *qD2P4).M()  );
      }else if(  abs(qM1->PID) == 5 ){
        Invmass_b->Fill( (*qD1P4 + *qD2P4).M()  );
      }

      break;
     }// end loop over all particles

  }// end loop over all events

//------------------------------------------------

  cout << "Number of used Zv: " << NofZv << endl;

  // Draw Invariant Mass histogram
  CustomizeHist(Invmass_u, kFullSquare, kRed, Norm, 0.8);
  CustomizeHist(Invmass_d, kFullSquare, kBlue, Norm, 0.8);
  CustomizeHist(Invmass_c, kFullSquare, kPink+1, Norm, 0.8);
  CustomizeHist(Invmass_s, kFullSquare, kSpring, Norm, 0.8);
  CustomizeHist(Invmass_b, kFullSquare, kBlack, Norm, 0.8);
  std::vector<TH1F*> vecInvmass = {Invmass_d,Invmass_u,Invmass_s,Invmass_c,Invmass_b};
  std::array<Double_t,4> leglimitsInvmass = {0.54386,0.493056,0.854637,0.833333};
  std::vector<TString> legentryInvmass = {"d#bar{d} -> Zv","u#bar{u} -> Zv","s#bar{s} -> Zv","c#bar{c} -> Zv","b#bar{b} -> Zv"};

  if(strcmp(inputFile, ppfile) == 0){
    DrawHist(selection,vecInvmass,Norm,"Invariant Mass [GeV/c^2]","#frac{1}{N} #frac{dN}{dM_{inv}} [c^2/GeV]",leglimitsInvmass,"Invariant Mass of Zv's daughters, pp",legentryInvmass,"qDInvMass_pp");
  }else if(strcmp(inputFile, ppbarfile) == 0){
    DrawHist(selection,vecInvmass,Norm,"Invariant Mass [GeV/c^2]","#frac{1}{N} #frac{dN}{dM_{inv}} [c^2/GeV]",leglimitsInvmass,"Invariant Mass of Zv's daughters, p#bar{p}",legentryInvmass,"qDInvMass_ppbar");
  }

//-------------------------------------------------------
  // write all plots on file
  TString outpp;

  if(strcmp(inputFile, ppfile) == 0){
    outpp = "_pp";
  }else if(strcmp(inputFile, ppbarfile) == 0){
    outpp = "_ppbar";
  }

  TString outname;
  outname.Form("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250314_Task13/%s_%d%s","PDF_plots",selection, Norm ? "_Norm" : "");
  outname = outname + outpp + ".root";

  TFile *f = new TFile(outname,"RECREATE");

  Invmass_u->Write("",TObject::kOverwrite);
  Invmass_d->Write("",TObject::kOverwrite);
  Invmass_c->Write("",TObject::kOverwrite);
  Invmass_s->Write("",TObject::kOverwrite);
  Invmass_b->Write("",TObject::kOverwrite);

}
