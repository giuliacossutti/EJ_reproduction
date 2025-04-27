/*
Author: Giulia Cossutti

Macro to merge histograms about large jets for signal and two backgrounds

From inside the /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes directory run with
root -l EJ_reproduction/tasks/20250319_Task13/MergeHistos.C'(sel)' 

sel = 0, 1, 2 stands for event selection:
if 0: consider only events with >=2 small jets with |eta| < 2.4 and pT > 125 GeV  (EJ analysis selection)
if 1: consider only events with >=2 small jets with |eta| < 2.4
if 2: no analysis selection at all (consider all events)

Jet definition is equal for all event selections and datasets and is due to the detector's acceptance:
small jets have |eta| < 2.5 + large jets have |eta| < 1.8
*/

//------------------------------------------------------------------------------

//------------ FUNCTIONS ---------------------
//===================================
// Function to customize signal histogram:
void CustomizeHist(TH1F* hist, Style_t style, Color_t color){
  hist->SetMarkerStyle(style);
  hist-> SetMarkerColor(color);
  hist-> SetLineColor(color);
  hist->SetMarkerSize(1.0);
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
//===================================
// Function to draw histograms in a canva and save it in pdf
// File vector must have signal file first and then all bkg files
// Signal is overlapped to the stack of backgrounds
void DrawHistos(Int_t sel,std::vector<TFile*> file,const char *histname,std::vector<Double_t> weights,std::vector<Color_t> color,TString XTitle,TString YTitle,std::array<Double_t,4> leglimits,TString legtitle,std::vector<TString> legentry,const char *outname){

   // Control size of inputs
   if( !( (file.size() == weights.size()) && (color.size() == legentry.size()) && (file.size() == color.size()) )  ){
    cout << "Error in DrawHistos: number of histograms not defined in " << histname << endl;
    cout << "file size: " << file.size() << "   weights size: " << weights.size() << endl;
    cout << "color size: " << color.size() << "   legend entry size: " << legentry.size() << endl;
    exit(EXIT_FAILURE);
  }

  // Take histograms from files
  std::vector<TH1F*> hist;

  for(Int_t i = 0; i < file.size(); ++i){
    TH1F *h = (TH1F*)file.at(i)->Get(histname)->Clone( ("h_" + std::to_string(i)).c_str() );
    hist.push_back(h);
  }

  // Scale histograms by Integrated Luminosity, Cross-section and number of generated events
  for(Int_t i = 0; i < hist.size(); ++i){
    hist.at(i)->Sumw2();
    hist.at(i)->Scale(weights.at(i));
    cout << histname << " number " << i << " scaled by " << weights.at(i) << endl;
  }

  // Canva to draw histograms
  TString canva;
  canva.Form("c%s",histname);

  TCanvas *c = new TCanvas(canva, canva, 0, 0, 800, 600);
  gStyle->SetOptStat(0);
  gPad->SetLeftMargin(0.15);
  gPad->SetLogy();
  //gPad->SetLogx();

  // Customize signal histo
  CustomizeHist(hist.at(0), kFullCircle, color.at(0));

  // Customize and add bkg histos to stack
  THStack *h_stack = new THStack();
  for(Int_t i = 1; i < hist.size(); ++i){
    hist.at(i)->SetFillColor(color.at(i));
    hist.at(i)-> SetLineColor(kBlue);
    h_stack->Add(hist.at(i));
  }
  
  // Draw histos
  h_stack->Draw("E, HIST");
  hist.at(0)->Draw("E,HIST same");

  // Titles of axes
  h_stack->GetXaxis()->SetTitleSize(.045);
  h_stack->GetYaxis()->SetTitleSize(.045);
  h_stack->GetXaxis()->SetTitle(XTitle);
  h_stack->GetYaxis()->SetTitle(YTitle);

  // Legend
  TLegend* legend = new TLegend(leglimits[0],leglimits[1],leglimits[2],leglimits[3]);

  TString head;
  if(sel==0) head = "#splitline{#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV, L=100fb^{-1}}}{EJ selection - " + legtitle + "}";
  if(sel==1) head = "#splitline{#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV, L=100fb^{-1}}}{no sel. on pT - " + legtitle + "}";
  if(sel==2) head = "#splitline{#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV, L=100fb^{-1}}}{no selection - " + legtitle + "}";

  legend->SetHeader(head,"C");

  for(Int_t i = 0; i < hist.size(); ++i){
    legend->AddEntry(hist.at(i),legentry.at(i));
  }

  CustomizeLeg(legend);
  legend->Draw();

  TString outpdf;
  outpdf.Form("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250319_Task13/%s_%d.pdf",outname,sel);
  c->SaveAs(outpdf);

  //c->Close();
}
//===================================
// Function to draw histograms in a canva and save it in pdf
// File vector must have signal file first and then all bkg files
// Signal is stacked with backgrounds
void DrawHistosStack(Int_t sel,std::vector<TFile*> file,const char *histname,std::vector<Double_t> weights,std::vector<Color_t> color,TString XTitle,TString YTitle,std::array<Double_t,4> leglimits,TString legtitle,std::vector<TString> legentry,const char *outname){

   // Control size of inputs
   if( !( (file.size() == weights.size()) && (color.size() == legentry.size()) && (file.size() == color.size()) )  ){
    cout << "Error in DrawHistos: number of histograms not defined in " << histname << endl;
    cout << "file size: " << file.size() << "   weights size: " << weights.size() << endl;
    cout << "color size: " << color.size() << "   legend entry size: " << legentry.size() << endl;
    exit(EXIT_FAILURE);
  }

  // Take histograms from files
  std::vector<TH1F*> hist;

  for(Int_t i = 0; i < file.size(); ++i){
    TH1F *h = (TH1F*)file.at(i)->Get(histname)->Clone( ("h_" + std::to_string(i)).c_str() );
    hist.push_back(h);
  }

  // Scale histograms by Integrated Luminosity, Cross-section and number of generated events
  for(Int_t i = 0; i < hist.size(); ++i){
    hist.at(i)->Sumw2();
    hist.at(i)->Scale(weights.at(i));
    cout << histname << " number " << i << " scaled by " << weights.at(i) << endl;
  }

  // Canva to draw histograms
  TString canva;
  canva.Form("cstack%s",histname);

  TCanvas *c = new TCanvas(canva, canva, 0, 0, 800, 600);
  gStyle->SetOptStat(0);
  gPad->SetLeftMargin(0.15);
  gPad->SetLogy();
  //gPad->SetLogx();

  // Customize and add bkg histos to stack
  THStack *h_stack = new THStack();
  for(Int_t i = 1; i < hist.size(); ++i){
    hist.at(i)->SetFillColor(color.at(i));
    hist.at(i)-> SetLineColor(kBlue);
    h_stack->Add(hist.at(i));
  }

  // Add signal on top of stack
  hist.at(0)->SetFillColor(color.at(0));
  hist.at(0)-> SetLineColor(kBlue);
  h_stack->Add(hist.at(0));
  
  // Draw histos
  h_stack->Draw("E, HIST");

  // Titles of axes
  h_stack->GetXaxis()->SetTitleSize(.045);
  h_stack->GetYaxis()->SetTitleSize(.045);
  h_stack->GetXaxis()->SetTitle(XTitle);
  h_stack->GetYaxis()->SetTitle(YTitle);

  // Legend
  TLegend* legend = new TLegend(leglimits[0],leglimits[1],leglimits[2],leglimits[3]);

  TString head;
  if(sel==0) head = "#splitline{#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV, L=100fb^{-1}}}{EJ selection - " + legtitle + "}";
  if(sel==1) head = "#splitline{#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV, L=100fb^{-1}}}{no sel. on pT - " + legtitle + "}";
  if(sel==2) head = "#splitline{#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV, L=100fb^{-1}}}{no selection - " + legtitle + "}";

  legend->SetHeader(head,"C");

  for(Int_t i = 0; i < hist.size(); ++i){
    legend->AddEntry(hist.at(i),legentry.at(i));
  }

  CustomizeLeg(legend);
  legend->Draw();

  TString outpdf;
  outpdf.Form("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250319_Task13/%s_%d_stack.pdf",outname,sel);
  c->SaveAs(outpdf);

  //c->Close();
}
//===============================
// Function to draw 2D histograms
void DrawHistos2D(Int_t sel,std::vector<TFile*> file,const char *histname,std::vector<Double_t> weights,TString XTitle,TString YTitle,TString ZTitle,std::array<Double_t,4> leglimits,TString legtitle,std::vector<TString> legentry,const char *outname){

   // Control size of inputs
   if( !( (file.size() == weights.size()) && (file.size() == legentry.size()) )  ){
    cout << "Error in DrawHistos: number of histograms not defined in " << histname << endl;
    cout << "file size: " << file.size() << "   weights size: " << weights.size() << endl;
    cout << "legend entry size: " << legentry.size() << endl;
    exit(EXIT_FAILURE);
  }

  // Take histograms from files
  std::vector<TH2F*> hist;

  for(Int_t i = 0; i < file.size(); ++i){
    TH2F *h = (TH2F*)file.at(i)->Get(histname)->Clone( ("h_" + std::to_string(i)).c_str() );
    hist.push_back(h);
  }

  // Draw signal and backgrounds separately

  TString canva;
  TString head;
  TString outpdf;

  // Maximum and minimum values of histograms
  Double_t Max = 1e8;
  Double_t Min = 1e3;

  for(Int_t i = 0; i < hist.size(); ++i){

    // Scale histograms by Integrated Luminosity, Cross-section and number of generated events
    hist.at(i)->Sumw2();
    hist.at(i)->Scale(weights.at(i));
    cout << histname << " number " << i << " scaled by " << weights.at(i) << endl;

    // Canva to draw histograms
    canva.Form("c%s_%d",histname,i);

    TCanvas *c = new TCanvas(canva, canva, 0, 0, 800, 600);
    gStyle->SetOptStat(0);
    gPad->SetRightMargin(0.15);
    gPad->SetLeftMargin(0.15);
    //gPad->SetLogy();
    //gPad->SetLogx();
    gPad->SetLogz();

    // Draw histos
    hist.at(i)->Draw("COLZ0");
    hist.at(i)->SetMaximum(Max);
    hist.at(i)->SetMinimum(Min);

    // Titles of axes
    hist.at(i)->GetXaxis()->SetTitleSize(.045);
    hist.at(i)->GetYaxis()->SetTitleSize(.045);
    hist.at(i)->GetZaxis()->SetTitleSize(.045);
    hist.at(i)->GetXaxis()->SetTitle(XTitle);
    hist.at(i)->GetYaxis()->SetTitle(YTitle);
    hist.at(i)->GetZaxis()->SetTitle(ZTitle);

    // Legend
    TLegend* legend = new TLegend(leglimits[0],leglimits[1],leglimits[2],leglimits[3]);

    if(sel==0) head = "#splitline{#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV, L=100fb^{-1}}}{EJ selection - " + legtitle + "}";
    if(sel==1) head = "#splitline{#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV, L=100fb^{-1}}}{no sel. on pT - " + legtitle + "}";
    if(sel==2) head = "#splitline{#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV, L=100fb^{-1}}}{no selection - " + legtitle + "}";

    legend->SetHeader(head,"C");
    CustomizeLeg(legend);

    // Add the right entry to the legend
    legend->AddEntry(hist.at(i),legentry.at(i));
    legend->Draw();

    outpdf.Form("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250319_Task13/%s_sample%d_sel%d.pdf",outname,i,sel);
    c->SaveAs(outpdf);

    //c->Close();
  }

}
//===============================
//---------- END FUNCTIONS -------------------


void MergeHistos(Int_t sel=0)
{

  if(sel < 0 || sel > 2){
    cout << " sel must be 0, 1 or 2" << endl;
    exit(EXIT_FAILURE);
  }

  // Import histograms 
  TString signame;
  signame.Form("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250319_Task13/signal_%d.root",sel);

  TString bkgTname;
  bkgTname.Form("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250319_Task13/bkg_Top_%d.root",sel);

  TString bkgHname;
  bkgHname.Form("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250319_Task13/bkg_HardQCD_%d.root",sel);

  TFile *f_sig = new TFile(signame);
  TFile *f_bkgT = new TFile(bkgTname);
  TFile *f_bkgH = new TFile(bkgHname);

  // Integrated Luminosity [pb^-1] (arbitrary, no real data here)
  Double_t lumi = 100000;

  // Cross-section [pb] 
  Double_t xsec_sig = 50000;
  Double_t xsec_bkgT = 818.8;
  Double_t xsec_bkgH = 75570;

  // Number of MC events
  Double_t ntot_sig = 50000;
  Double_t ntot_bkgT = 50000;
  Double_t ntot_bkgH = 50000;

  // Prepare info to draw histos and draw them
  std::vector<TFile*> infiles = {f_sig,f_bkgT,f_bkgH};
  std::vector<Color_t> colors = {kBlack,kGreen,kRed};
  std::vector<TString> legentry = {TString::Format("f #bar{f} -> Zv, #sigma = %.0f pb",xsec_sig),TString::Format("top production, #sigma = %.1f pb",xsec_bkgT),TString::Format("2->2 hard QCD jets, #sigma = %.0f pb",xsec_bkgH)};
  //std::vector<TString> legentry = {TString::Format("signal: f #bar{f} -> Zv, #sigma = %.0f pb",xsec_sig),"bkg: top production","bkg: 2->2 hard QCD jets"};

  // Calculate weights
  std::vector<Double_t> xsec = {xsec_sig,xsec_bkgT,xsec_bkgH};
  std::vector<Double_t> ntot = {ntot_sig,ntot_bkgT,ntot_bkgH};
  std::vector<Double_t> weights;

  for(Int_t i = 0; i < infiles.size(); ++i){
    weights.push_back( lumi*xsec.at(i) / ntot.at(i) );
  }

  // 1D histograms

  std::array<Double_t,4> leglimitsInvmass = {0.516291,0.548611,0.827068,0.869792};
  DrawHistos(sel,infiles,"Invariant_mass_of_leading_and_subleading_large_jets",weights,colors,"Invariant Mass [GeV/c^{2}]","#events",leglimitsInvmass,"Invariant Mass of first 2 Large Jets",legentry,"InvMass");
  DrawHistosStack(sel,infiles,"Invariant_mass_of_leading_and_subleading_large_jets",weights,colors,"Invariant Mass [GeV/c^{2}]","#events",leglimitsInvmass,"Invariant Mass of first 2 Large Jets",legentry,"InvMass");

  std::array<Double_t,4> leglimitsEta1 = {0.5,0.611111,0.810777,0.833333};
  DrawHistos(sel,infiles,"Eta_of_leading_large_jet",weights,colors,"#eta","# events",leglimitsEta1,"#eta of Leading Large Jet",legentry,"Eta1");
  DrawHistosStack(sel,infiles,"Eta_of_leading_large_jet",weights,colors,"#eta","# events",leglimitsEta1,"#eta of Leading Large Jet",legentry,"Eta1");

  std::array<Double_t,4> leglimitsEta2 = {0.5,0.611111,0.810777,0.833333};
  DrawHistos(sel,infiles,"Eta_of_subleading_large_jet",weights,colors,"#eta","# events",leglimitsEta2,"#eta of Subleading Large Jet",legentry,"Eta2");
  DrawHistosStack(sel,infiles,"Eta_of_subleading_large_jet",weights,colors,"#eta","# events",leglimitsEta2,"#eta of Subleading Large Jet",legentry,"Eta2");

  std::array<Double_t,4> leglimitsDeltaR = {0.5,0.611111,0.810777,0.833333};
  DrawHistos(sel,infiles,"DeltaR_between_leading_and_subleading_large_jets",weights,colors,"#Delta R","# events",leglimitsDeltaR,"#Delta R between first 2 Large Jets",legentry,"DeltaR");
  DrawHistosStack(sel,infiles,"DeltaR_between_leading_and_subleading_large_jets",weights,colors,"#Delta R","# events",leglimitsDeltaR,"#Delta R between first 2 Large Jets",legentry,"DeltaR");

  std::array<Double_t,4> leglimitsDeltaEta = {0.5,0.611111,0.810777,0.833333};
  DrawHistos(sel,infiles,"DeltaEta_between_leading_and_subleading_large_jets",weights,colors,"#Delta #eta","# events",leglimitsDeltaEta,"#Delta #eta between first 2 Large Jets",legentry,"DeltaEta");
  DrawHistosStack(sel,infiles,"DeltaEta_between_leading_and_subleading_large_jets",weights,colors,"#Delta #eta","# events",leglimitsDeltaEta,"#Delta #eta between first 2 Large Jets",legentry,"DeltaEta");

  std::array<Double_t,4> leglimitsDeltaPhi = {0.5,0.611111,0.810777,0.833333};
  DrawHistos(sel,infiles,"DeltaPhi_between_leading_and_subleading_large_jets",weights,colors,"#Delta #varphi","# events",leglimitsDeltaPhi,"#Delta #varphi between first 2 Large Jets",legentry,"DeltaPhi");
  DrawHistosStack(sel,infiles,"DeltaPhi_between_leading_and_subleading_large_jets",weights,colors,"#Delta #varphi","# events",leglimitsDeltaPhi,"#Delta #varphi between first 2 Large Jets",legentry,"DeltaPhi");

  std::array<Double_t,4> leglimitsPT1 = {0.5,0.611111,0.810777,0.833333};
  DrawHistos(sel,infiles,"leading_large_jet_pt",weights,colors,"p_{T} [GeV/c]","# events",leglimitsPT1,"pT of Leading Large Jet",legentry,"JetPT1");
  DrawHistosStack(sel,infiles,"leading_large_jet_pt",weights,colors,"p_{T} [GeV/c]","# events",leglimitsPT1,"pT of Leading Large Jet",legentry,"JetPT1");

  std::array<Double_t,4> leglimitsPT2 = {0.5,0.611111,0.810777,0.833333};
  DrawHistos(sel,infiles,"subleading_large_jet_pt",weights,colors,"p_{T} [GeV/c]","# events",leglimitsPT2,"pT of Subleading Large Jet",legentry,"JetPT2");
  DrawHistosStack(sel,infiles,"subleading_large_jet_pt",weights,colors,"p_{T} [GeV/c]","# events",leglimitsPT2,"pT of Subleading Large Jet",legentry,"JetPT2");

  std::array<Double_t,4> leglimitsNC = {0.444862,0.609375,0.755639,0.831597};
  DrawHistos(sel,infiles,"number_of_constituents_of_leading_and_subleading_large_jets",weights,colors,"Number of Constituents","2 #times # events",leglimitsNC,"Number of Constituents of L and Subl Large Jets",legentry,"NConst");
  DrawHistosStack(sel,infiles,"number_of_constituents_of_leading_and_subleading_large_jets",weights,colors,"Number of Constituents","2 #times # events",leglimitsNC,"Number of Constituents of L and Subl Large Jets",legentry,"NConst");


  // 2D histograms

  std::array<Double_t,4> leglimitsDPE = {0.303258,0.657986,0.614035,0.880208};
  DrawHistos2D(sel,infiles,"DeltaPhi_vs_DeltaEta",weights,"#Delta #varphi","#Delta #eta","# events",leglimitsDPE,"#Delta #varphi vs #Delta #eta of first 2 Large Jets",legentry,"DeltaPhi_DeltaEta");

  std::array<Double_t,4> leglimitsDRE = {0.303258,0.657986,0.614035,0.880208};
  DrawHistos2D(sel,infiles,"DeltaR_vs_DeltaEta",weights,"#Delta R","#Delta #eta","# events",leglimitsDRE,"#Delta R vs #Delta #eta of first 2 Large Jets",legentry,"DeltaR_DeltaEta");

  std::array<Double_t,4> leglimitsDRP = {0.303258,0.657986,0.614035,0.880208};
  DrawHistos2D(sel,infiles,"DeltaR_vs_DeltaPhi",weights,"#Delta R","#Delta #varphi","# events",leglimitsDRP,"#Delta R vs #Delta #varphi of first 2 Large Jets",legentry,"DeltaR_DeltaPhi");

  std::array<Double_t,4> leglimitsPT1_PT2 = {0.367168,0.661458,0.677945,0.883681};
  DrawHistos2D(sel,infiles,"JetPT1_vs_JetPT2",weights,"p_{T} of Leading Large Jet [GeV/c]","p_{T} of Subleading Large Jet [GeV/c]","# events",leglimitsPT1_PT2,"L Large Jet pT vs SubL Large Jet pT",legentry,"JetPT1_JetPT2");

  std::array<Double_t,4> leglimitsPT1_IM = {0.367168,0.661458,0.677945,0.883681};
  DrawHistos2D(sel,infiles,"JetPT1_vs_Invmass",weights,"p_{T} of Leading Large Jet [GeV/c]","Invariant Mass [GeV/c^2]","# events",leglimitsPT1_IM,"L Large Jet pT vs Invariant Mass of first two Large Jets",legentry,"JetPT1_Invmass");

  std::array<Double_t,4> leglimitsPT1_NC = {0.367168,0.661458,0.677945,0.883681};
  DrawHistos2D(sel,infiles,"JetPT1_vs_NConst",weights,"p_{T} of Leading Large Jet [GeV/c]","Number of Constituents","2 #times # events",leglimitsPT1_NC,"L Large Jet pT vs # of constituents of first 2 Large Jets",legentry,"JetPT1_NConst");

  std::array<Double_t,4> leglimitsIM_NC = {0.367168,0.661458,0.677945,0.883681};
  DrawHistos2D(sel,infiles,"Invmass_vs_NConst",weights,"Invariant Mass [GeV/c^2]","Number of Constituents","2 #times # events",leglimitsIM_NC,"Invariant Mass vs # of constituents of first 2 Large Jets",legentry,"Invmass_NConst");

}//end MergeHistos
