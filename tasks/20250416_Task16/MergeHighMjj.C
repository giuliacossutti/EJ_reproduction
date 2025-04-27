/*
Author: Giulia Cossutti

Macro to plot histograms about High mjj selection for signal and two backgrounds.
Bkgs are scaled with pythia's calculated cross-sections, signal is scaled with 
arbitrary cross-section = sum of all bkgs cross-sections.
Histograms are saved in files.

From inside the /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes directory run with
root -l EJ_reproduction/tasks/20250416_Task16/MergeHighMjj.C 
*/

//------------------------------------------------------------------------------

//------------ FUNCTIONS ---------------------
//===================================
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
// Function to draw 2D histograms
void DrawHistos2D(std::vector<TFile*> file,const char *histname,std::vector<Double_t> weights,Double_t lumi,TString XTitle,TString YTitle,TString ZTitle,std::vector<std::array<Double_t,4>> leglimits,TString legtitle,std::vector<TString> legentry,const char *outname){

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
    TH2F *h = (TH2F*)file.at(i)->Get(histname)->Clone( (histname + std::to_string(i)).c_str() );
    hist.push_back(h);
  }

  // Draw signal and backgrounds separately

  TString canva;
  TString head;
  TString outpdf;

  // Maximum and minimum values of histograms
  Double_t Max = 1e6;
  Double_t Min = 1e3;

  for(Int_t i = 0; i < hist.size(); ++i){

    // Scale histograms by Integrated Luminosity, Cross-section and number of generated events
    hist.at(i)->Sumw2();
    hist.at(i)->Scale(weights.at(i));

    cout << histname << " number " << i << " scaled by " << weights.at(i)  << endl;

    // Scale also by bin width
    //hist.at(i)->Scale(1./(hist.at(i)->GetXaxis()->GetBinWidth(2) * hist.at(i)->GetYaxis()->GetBinWidth(2)) );

    //cout << histname << " number " << i << " scaled by " << weights.at(i) / (hist.at(i)->GetXaxis()->GetBinWidth(2) * hist.at(i)->GetYaxis()->GetBinWidth(2)) << endl;
    //cout << "binwidth X : " << hist.at(i)->GetXaxis()->GetBinWidth(2) << "  binwidth Y : " << hist.at(i)->GetYaxis()->GetBinWidth(2) << endl;

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
    TLegend* legend = new TLegend(leglimits.at(i)[0],leglimits.at(i)[1],leglimits.at(i)[2],leglimits.at(i)[3]);
    head = "#splitline{#splitline{Model D, m_{Zv} = 3.0 TeV, c#tau_{#pi_{D}} = 50 mm}{PYTHIA8+DELPHES, #sqrt{s}=13.6 TeV, L= " + TString::Format("%.1f",lumi/1000.) + " fb^{-1}}}{High mjj selection - " + legtitle + "}";
    legend->SetHeader(head,"C");
    CustomizeLeg(legend);

    // Add the right entry to the legend
    legend->AddEntry(hist.at(i),legentry.at(i));
    legend->Draw();

    // ABCD plane: vertical and horizontal lines
    TLine* vert = new TLine(0.2,0.0, 0.2,1.0);
    TLine* hor = new TLine(0.0,0.2, 1.0,0.2);

    vert->SetLineColor(kRed);
    hor->SetLineColor(kRed);

    vert->SetLineWidth(2);
    hor->SetLineWidth(2);

    vert->Draw("same");
    hor->Draw("same");

    // ABCD plane: ABCD letters
    TLatex A;
    A.SetTextSize(0.04);
    A.SetTextColor(kRed);
    A.SetTextFont(62);
    A.DrawLatex(0.1,0.1,"A");
    A.DrawLatex(0.1,0.3,"B");
    A.DrawLatex(0.3,0.1,"C");
    A.DrawLatex(0.3,0.3,"D");

    outpdf.Form("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250416_Task16/%s_sample%d.pdf",outname,i);
    c->SaveAs(outpdf);

    // Write on file
    hist.at(i)->Write("",TObject::kOverwrite);

    //c->Close();
  }

}
//===============================
//---------- END FUNCTIONS -------------------

void MergeHighMjj()
{

  // Import histograms 
  TString signame;
  signame.Form("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250416_Task16/signal.root");

  TString bkgTname;
  bkgTname.Form("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250416_Task16/bkg_Top.root");

  TString bkgHname;
  bkgHname.Form("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250416_Task16/bkg_HardQCD.root");

  TFile *f_sig = new TFile(signame);
  TFile *f_bkgT = new TFile(bkgTname);
  TFile *f_bkgH = new TFile(bkgHname);

  // Integrated Luminosity [pb^-1] (arbitrary, no real data here)
  Double_t lumi = 51838.9;

  // Cross-section [pb] 
  Double_t xsec_bkgT = 818.8;
  Double_t xsec_bkgH = 75570;
  Double_t xsec_sig = xsec_bkgH + xsec_bkgT;

  // Number of MC events
  Double_t ntot_sig = 50000;
  Double_t ntot_bkgT = 50000;
  Double_t ntot_bkgH = 50000;

  // Prepare info to draw histos and draw them
  std::vector<TFile*> infiles = {f_sig,f_bkgT,f_bkgH};
  std::vector<TString> legentry = {TString::Format("f #bar{f} -> Zv, #sigma = %.1f pb",xsec_sig),TString::Format("top production, #sigma = %.1f pb",xsec_bkgT),TString::Format("2->2 hard QCD jets, #sigma = %.0f pb",xsec_bkgH)};

  // Calculate weights
  std::vector<Double_t> xsec = {xsec_sig,xsec_bkgT,xsec_bkgH};
  std::vector<Double_t> ntot = {ntot_sig,ntot_bkgT,ntot_bkgH};
  std::vector<Double_t> weights;

  for(Int_t i = 0; i < infiles.size(); ++i){
    weights.push_back( lumi*xsec.at(i) / ntot.at(i) );
  }

  // Legend limits
  std::array<Double_t,4> leglimits_sig = {0.472431,0.661458,0.783208,0.885417};
  std::array<Double_t,4> leglimits_bkg = {0.392231,0.15625,0.83584,0.435764};

  std::vector< std::array<Double_t,4> > leglimits = {leglimits_sig,leglimits_bkg,leglimits_bkg};

   // write all scaled histograms on file
  TString outname;
  outname.Form("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250416_Task16/histos_highmjj.root");

  TFile *f = new TFile(outname,"RECREATE");

  // 2D histograms

  DrawHistos2D(infiles,"PTF1_vs_PTF2",weights,lumi,"Leading Jet PTF","Sub-leading Jet PTF","Events",leglimits,"ABCD plane",legentry,"PTF1_PTF2");

  DrawHistos2D(infiles,"PTF1noz_vs_PTF2noz",weights,lumi,"Leading Jet PTF_{no #Delta z cut}","Sub-leading Jet PTF_{no #Delta z cut}","Events",leglimits,"ABCD plane",legentry,"PTF1noz_PTF2noz");

}//end MergeHighMjj

