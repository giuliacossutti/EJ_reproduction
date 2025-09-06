/*
Author: Giulia Cossutti

Macro to calculate significance and figure of merit of cuts to separate signal from background.
Bkgs are scaled with pythia's calculated cross-sections, signal is scaled with 
arbitrary cross-section = sum of all bkgs cross-sections.
Histograms have been saved in files in 20250324_Task14.

From inside the /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes directory run with
root -l EJ_reproduction/tasks/20250906_ThesisPlots/FigureOfMerit.C'(sel)' 

sel = 0, 1, 2 stands for event selection:
if 0: consider only events with >=2 small jets with |eta| < 2.4 and pT > 125 GeV  (EJ analysis selection)
if 1: consider only events with >=2 small jets with |eta| < 2.4
if 2: no analysis selection at all (consider all events)

Jet definition is equal for all event selections and datasets and is due to the detector's acceptance:
small jets have |eta| < 2.5 + large jets have |eta| < 1.8
*/

//------------------------------------------------------------------------------

#include <numeric>
#include <iostream>

//------------ FUNCTIONS ---------------------
//===================================
// Function to calculate figure of merit with 1D cut
void FOM1D(Int_t sel,TFile* infile,const char *histname,Int_t samplenumber,Double_t firstcut,Double_t lastcut,Int_t ncuts,Double_t endcuts){

  if(ncuts <= 1){
    cout << "Use 2 or more cuts." << endl;
    exit(EXIT_FAILURE);
  }

  // Take histograms from files
  std::vector<TH1F*> hist;

  TString histname_i;
  for(Int_t i = 0; i < samplenumber; ++i){
    histname_i.Form("%d",i);
    histname_i = histname + histname_i;
    TH1F *h = (TH1F*)infile->Get(histname_i)->Clone( (histname_i) );
    hist.push_back(h);
  }

  // Calculate figure of merit
  Double_t cut;
  Double_t signal;
  Double_t bkg;
  Double_t bincenter;
  Double_t significance;

  // Canva to draw histograms
  TString canva;
  canva.Form("c%s",histname);

  TCanvas *c = new TCanvas(canva, canva, 0, 0, 800, 600);
  gStyle->SetOptStat(0);
  gStyle->SetTitleSize(0.045);
  gPad->SetBottomMargin(0.11);
  gPad->SetLeftMargin(0.15);
  gPad->SetLogy();

  // Graph of figure of merit
  TGraph *fom = new TGraph();
  fom->SetMarkerStyle(kFullCircle);
  fom->SetMarkerColor(kBlack);
  fom->SetLineColor(kRed);
  fom->SetLineWidth(3);
  fom->GetXaxis()->SetTitleSize(.050);
  fom->GetYaxis()->SetTitleSize(.050);
  fom->GetXaxis()->SetLabelSize(.050);
  fom->GetYaxis()->SetLabelSize(.050);
  fom->GetXaxis()->SetTitle("cut on m_{jj}");
  fom->GetYaxis()->SetTitle("#frac{S}{#sqrt{Bkg}}");
  TString fomname;
  fomname.Form("FOM_of_%s_sel_%d",histname, sel);
  TString fomtitle;
  fomtitle.Form("FOM of %s, events between cut and %.2f, sel %d",histname, endcuts, sel);
  fom->SetNameTitle(fomname,fomtitle);

  TH1F *histsignal = new TH1F(fomname + "_sig",fomtitle,ncuts,firstcut-(lastcut-firstcut)/(2.*(ncuts-1)),lastcut+(lastcut-firstcut)/(2.*(ncuts-1)) );
  TH1F *histbkg = new TH1F(fomname + "_bkg",fomtitle,ncuts,firstcut-(lastcut-firstcut)/(2.*(ncuts-1)),lastcut+(lastcut-firstcut)/(2.*(ncuts-1)) );
  histsignal->SetLineColor(kBlue);
  histbkg->SetLineColor(kRed);
  histsignal->SetLineWidth(3);
  histbkg->SetLineWidth(3);
  histsignal->GetXaxis()->SetTitleSize(.050);
  histsignal->GetYaxis()->SetTitleSize(.050);
  histsignal->GetXaxis()->SetLabelSize(.050);
  histsignal->GetYaxis()->SetLabelSize(.050);
  histsignal->GetXaxis()->SetTitle("cut on m_{jj}");
  histsignal->GetYaxis()->SetTitle("Signal or Bkg left (%)");
/*
  // Output file
  TString outfilename;
  outfilename.Form("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250325_Task14/FOM_%s_sel%d.txt",histname, sel);
  ofstream outfile;
  outfile.open(outfilename);
  outfile << "This file contains significance S/sqrt(Bkg) for the histogram " << histname << endl;
  outfile << "Cuts are between cut and " << endcuts << " with " << ncuts << " cuts between " << firstcut << " and " << lastcut << " included." << endl;
  outfile << "" << endl;
*/
  // Total number of events
  std::vector<Double_t> integrals;
  for(Int_t j = 0; j < hist.size(); ++j){
    integrals.push_back(hist.at(j)->Integral());
  }

  // Loop over cuts
  for(Int_t i = 0; i < ncuts; ++i){
  
    signal = 0.;
    bkg = 0.;

    // Calculate integral cut
    cut = firstcut + i*(lastcut-firstcut)/(ncuts-1);

    // Loop over histograms
    for(Int_t j = 0; j < hist.size(); ++j){
      // Loop over bins of a histogram
      for(Int_t k = 1; k <= hist.at(j)->GetXaxis()->GetNbins(); ++k){
        bincenter = hist.at(j)->GetXaxis()->GetBinCenter(k);
        if( (bincenter < cut && bincenter > endcuts) || (bincenter > cut && bincenter < endcuts) ){
          if(j==0){
            signal += hist.at(j)->GetBinContent(k);
          }else{
            bkg += hist.at(j)->GetBinContent(k);
          }            
        }
      }// end loop over bins of a histogram
    }// end loop over histograms

    // Calculate significance and put it in a graph and in a file with the percentage of considered signal and bkgs
    if(bkg != 0. ){
      significance = signal/sqrt(bkg);
    }else{
      significance = 0.;
    }

    fom->AddPoint(cut,significance);
    //outfile <<"Cut:\t"<<cut<<"\tsignificance: "<<significance<<" ,\tLeft "<<100.*signal/integrals.at(0)<<" % signal, \t"<<100.*bkg/accumulate(integrals.begin()+1,integrals.end(),0.)<<" % bkg."<<endl;    

    histsignal->Fill(cut,100.*signal/integrals.at(0) );
    histbkg->Fill(cut,100.*bkg/accumulate(integrals.begin()+1,integrals.end(),0.) );

  }// end loop over cuts

  fom->Draw("APL");
  //outfile.close();

  // Save Figure Of Merit in a pdf
  TString outpdf;
  outpdf.Form("/gfsvol01/atlas/giuliac/plots_and_outputs/Zprimethesisplots/fom_%s_%d",histname,sel);
  c->SaveAs(outpdf + ".pdf");
  //c->Close();

  // Save histos of signal and bkg left
  TCanvas *csig = new TCanvas(canva + "_sig", canva + "_sig", 0, 0, 800, 600);
  gStyle->SetOptStat(0);
  gStyle->SetTitleSize(0.045);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.11);
  //gPad->SetRightMargin(0.20);
  gPad->SetLogy();

  histsignal->Draw("HIST");
  histbkg->Draw("HIST, same");

  //TLegend* legend = new TLegend(0.735589,0.706597,0.83584,0.807292);
  TLegend* legend = new TLegend(0.670426,0.722222,0.868421,0.850694);
  legend->AddEntry(histsignal,"signal");
  legend->AddEntry(histbkg,"background");
  gStyle->SetLegendTextSize(0.04);
  legend->Draw();

  csig->SaveAs(outpdf + "_signal_bkg_left" + ".pdf");
}
//===================================
// Function to calculate figure of merit with 2D rectangular cut
void FOM2D_rectangle(Int_t sel,TFile* infile,const char *histname,Int_t samplenumber,Double_t firstcutx,Double_t lastcutx,Int_t ncutsx,Double_t firstcuty,Double_t lastcuty,Int_t ncutsy){

  if(ncutsx <= 1 || ncutsy <= 1){
    cout << "Use 2 or more cuts for each dimension" << endl;
    exit(EXIT_FAILURE);
  }

  // Take histograms from files
  std::vector<TH2F*> hist;

  TString histname_i;
  for(Int_t i = 0; i < samplenumber; ++i){
    histname_i.Form("%d",i);
    histname_i = histname + histname_i;
    TH2F *h = (TH2F*)infile->Get(histname_i)->Clone( (histname_i) );
    hist.push_back(h);
  }

  // Calculate figure of merit
  Double_t cutx;
  Double_t cuty;
  Double_t signal;
  Double_t bkg;
  Double_t bincenterx;
  Double_t bincentery;
  Double_t significance;

  // Canva to draw histograms
  TString canva;
  canva.Form("crectangle_%s",histname);

  TCanvas *c = new TCanvas(canva, canva, 0, 0, 800, 600);
  gStyle->SetOptStat(0);
  gStyle->SetTitleSize(0.050);
  gStyle->SetLabelSize(0.050);
  gPad->SetLeftMargin(0.15);
  gPad->SetRightMargin(0.20);
  gPad->SetLogz();

  // 2D histogram of figure of merit
  TString fomname;
  fomname.Form("Rectangular_FOM_of_%s_sel_%d",histname, sel);
  TString fomtitle;
  fomtitle.Form("Rectangular FOM of %s, sel %d",histname, sel);

  char* axes = strdup(histname);
  char* axesnames1 = strtok(axes,"_");
  char* axesnames2 = strtok(NULL,"_");
  char* axesnames3 = strtok(NULL,"_");
  char axestitle1[100];
  char axestitle2[100];
  const char* cut = "cut on ";
  strcpy(axestitle1,cut);
  strcat(axestitle1,axesnames1);
  strcpy(axestitle2,cut);
  strcat(axestitle2,axesnames3);
  const char* XTitle = axestitle1;
  const char* YTitle = axestitle2;

  TH2F *fom = new TH2F(fomname,fomtitle,ncutsx,firstcutx-(lastcutx-firstcutx)/(2.*(ncutsx-1)),lastcutx+(lastcutx-firstcutx)/(2.*(ncutsx-1)),ncutsy,firstcuty-(lastcuty-firstcuty)/(2.*(ncutsy-1)),lastcuty+(lastcuty-firstcuty)/(2.*(ncutsy-1)));
  fom->GetXaxis()->SetTitleSize(.050);
  fom->GetYaxis()->SetTitleSize(.050);
  fom->GetZaxis()->SetTitleSize(.050);
  fom->GetXaxis()->SetLabelSize(.040);
  fom->GetYaxis()->SetLabelSize(.040);
  fom->GetZaxis()->SetLabelSize(.040);
  fom->GetXaxis()->SetTitle(XTitle);
  fom->GetYaxis()->SetTitle(YTitle);

  // 2D histograms of signal and bkg left
  TH2F* histsignal = (TH2F*)fom->Clone("Signal_Left_" + fomname);
  TH2F* histbkg = (TH2F*)fom->Clone("Bkg_Left_" + fomname);

  fom->GetZaxis()->SetTitle("#frac{S}{#sqrt{Bkg}}");
  histsignal->GetZaxis()->SetTitle("Signal left (%)");
  histbkg->GetZaxis()->SetTitle("Bkg left (%)");
/*
  // Output file
  TString outfilename;
  outfilename.Form("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250325_Task14/Rectangular_FOM_%s_sel%d.txt",histname, sel);
  ofstream outfile;
  outfile.open(outfilename);
  outfile << "This file contains significance S/sqrt(Bkg) for the histogram " << histname << endl;
  outfile << "Cuts are between cut and +infinity, with " << ncutsx << " xcuts between " << firstcutx << " and " << lastcutx << " included, " << ncutsy << " ycuts between " << firstcuty << " and " << lastcuty << " included. " <<endl;
  outfile << "XCut is " << XTitle << " , YCut is " << YTitle << endl;
  outfile << "" << endl;
*/
  // Total number of events
  std::vector<Double_t> integrals;
  for(Int_t j = 0; j < hist.size(); ++j){
    integrals.push_back(hist.at(j)->Integral());
  }

  // Loop over cuts
  for(Int_t ix = 0; ix < ncutsx; ++ix){
  for(Int_t iy = 0; iy < ncutsy; ++iy){
  
    signal = 0.;
    bkg = 0.;

    // Calculate cuts
    cutx = firstcutx + ix*(lastcutx-firstcutx)/(ncutsx-1);
    cuty = firstcuty + iy*(lastcuty-firstcuty)/(ncutsy-1);

    // Loop over histograms
    for(Int_t j = 0; j < hist.size(); ++j){
      // Loop over bins of a histogram
      for(Int_t kx = 1; kx <= hist.at(j)->GetXaxis()->GetNbins(); ++kx){
      for(Int_t ky = 1; ky <= hist.at(j)->GetYaxis()->GetNbins(); ++ky){
        bincenterx = hist.at(j)->GetXaxis()->GetBinCenter(kx);
        bincentery = hist.at(j)->GetYaxis()->GetBinCenter(ky);
        if( (bincenterx > cutx) || (bincentery > cuty) ){
          if(j==0){
            signal += hist.at(j)->GetBinContent(kx,ky);
          }else{
            bkg += hist.at(j)->GetBinContent(kx,ky);
          }            
        }
      }// end loop over xbins of a histogram
      }// end loop over ybins of a histogram
    }// end loop over histograms

    // Calculate significance and put it in a histogram and in a file with the percentage of considered signal and bkgs
    if(bkg != 0. ){
      significance = signal/sqrt(bkg);
    }else{
      significance = 0.;
    }

    fom->Fill(cutx,cuty,significance);
    //outfile <<"XCut:\t"<<cutx<<" YCut:\t"<<cuty<<"\tsignificance: "<<significance<<" ,\tLeft "<<100.*signal/integrals.at(0)<<" % signal, \t"<<100.*bkg/accumulate(integrals.begin()+1,integrals.end(),0.)<<" % bkg."<<endl;    

    histsignal->Fill(cutx,cuty,100.*signal/integrals.at(0) );
    histbkg->Fill(cutx,cuty,100.*bkg/accumulate(integrals.begin()+1,integrals.end(),0.) );

  }// end loop over ycuts
  }// end loop over xcuts

  fom->Draw("COLZ");
  //outfile.close();

  // Save Figure Of Merit in a pdf
  TString outpdf;
  outpdf.Form("/gfsvol01/atlas/giuliac/plots_and_outputs/Zprimethesisplots/Rectangular_fom_%s_%d",histname,sel);
  c->SaveAs(outpdf + ".pdf");
  //c->Close();

  // Save histos of signal and bkg left
  TCanvas *csig = new TCanvas(canva + "_sig", canva + "_sig", 0, 0, 800, 600);
  gStyle->SetOptStat(0);
  gStyle->SetTitleSize(0.050);
  gStyle->SetLabelSize(0.050);
  gPad->SetLeftMargin(0.15);
  gPad->SetRightMargin(0.20);
  gPad->SetLogz();

  histsignal->GetXaxis()->SetTitleSize(.050);
  histsignal->GetYaxis()->SetTitleSize(.050);
  histsignal->GetZaxis()->SetTitleSize(.050);
  histsignal->GetXaxis()->SetLabelSize(.040);
  histsignal->GetYaxis()->SetLabelSize(.040);
  histsignal->GetZaxis()->SetLabelSize(.040);
  histsignal->Draw("COLZ");
  histsignal->SetMaximum(100.);
  histsignal->SetMinimum(0.);
  csig->SaveAs(outpdf + "_signal_left" + ".pdf");

  TCanvas *cbkg = new TCanvas(canva + "_bkg", canva + "_bkg", 0, 0, 800, 600);
  gStyle->SetOptStat(0);
  gStyle->SetTitleSize(0.050);
  gStyle->SetLabelSize(0.050);
  gPad->SetLeftMargin(0.15);
  gPad->SetRightMargin(0.20);
  gPad->SetLogz();

  histbkg->GetXaxis()->SetTitleSize(.050);
  histbkg->GetYaxis()->SetTitleSize(.050);
  histbkg->GetZaxis()->SetTitleSize(.050);
  histbkg->GetXaxis()->SetLabelSize(.040);
  histbkg->GetYaxis()->SetLabelSize(.040);
  histbkg->GetZaxis()->SetLabelSize(.040);
  histbkg->Draw("COLZ");
  histbkg->SetMaximum(100.);
  histbkg->SetMinimum(0.);
  cbkg->SaveAs(outpdf + "_bkg_left" + ".pdf");
}
//===================================
// Function to calculate figure of merit with 2D line cut
void FOM2D_line(Int_t sel,TFile* infile,const char *histname,Int_t samplenumber,Double_t firstcutm,Double_t lastcutm,Int_t ncutsm,Double_t firstcutq,Double_t lastcutq,Int_t ncutsq){

  if(ncutsm <= 1 || ncutsq <= 1){
    cout << "Use 2 or more cuts for each dimension" << endl;
    exit(EXIT_FAILURE);
  }

  // Take histograms from files
  std::vector<TH2F*> hist;

  TString histname_i;
  for(Int_t i = 0; i < samplenumber; ++i){
    histname_i.Form("%d",i);
    histname_i = histname + histname_i;
    TH2F *h = (TH2F*)infile->Get(histname_i)->Clone( (histname_i) );
    hist.push_back(h);
  }

  // Calculate figure of merit
  Double_t cutm;
  Double_t cutq;
  Double_t signal;
  Double_t bkg;
  Double_t bincenterx;
  Double_t bincentery;
  Double_t significance;

  // Canva to draw histograms
  TString canva;
  canva.Form("cline_%s",histname);

  TCanvas *c = new TCanvas(canva, canva, 0, 0, 800, 600);
  gStyle->SetOptStat(0);
  gStyle->SetTitleSize(0.050);
  gStyle->SetLabelSize(0.050);
  gPad->SetLeftMargin(0.15);
  gPad->SetRightMargin(0.20);
  gPad->SetLogz();

  // 2D histogram of figure of merit
  char* axes = strdup(histname);
  char* axesnames1 = strtok(axes,"_");
  char* axesnames2 = strtok(NULL,"_");
  char* axesnames3 = strtok(NULL,"_");

  TString fomname;
  fomname.Form("2D_Linear_FOM_of_%s_sel_%d",histname, sel);
  TString fomtitle;
  fomtitle.Form("2D Linear FOM, sel %d: %s >= m %s + q",sel,axesnames3,axesnames1);

  const char* XTitle = "m";
  const char* YTitle = "q";

  TH2F *fom = new TH2F(fomname,fomtitle,ncutsm,firstcutm-(lastcutm-firstcutm)/(2.*(ncutsm-1)),lastcutm+(lastcutm-firstcutm)/(2.*(ncutsm-1)),ncutsq,firstcutq-(lastcutq-firstcutq)/(2.*(ncutsq-1)),lastcutq+(lastcutq-firstcutq)/(2.*(ncutsq-1)));
  fom->GetXaxis()->SetTitleSize(.050);
  fom->GetYaxis()->SetTitleSize(.050);
  fom->GetZaxis()->SetTitleSize(.050);
  fom->GetXaxis()->SetLabelSize(.040);
  fom->GetYaxis()->SetLabelSize(.040);
  fom->GetZaxis()->SetLabelSize(.040);
  fom->GetXaxis()->SetTitle(XTitle);
  fom->GetYaxis()->SetTitle(YTitle);

  // 2D histograms of signal and bkg left
  TH2F* histsignal = (TH2F*)fom->Clone("Signal_Left_" + fomname);
  TH2F* histbkg = (TH2F*)fom->Clone("Bkg_Left_" + fomname);

  fom->GetZaxis()->SetTitle("#frac{S}{#sqrt{Bkg}}");
  histsignal->GetZaxis()->SetTitle("Signal left (%)");
  histbkg->GetZaxis()->SetTitle("Bkg left (%)");
/*
  // Output file
  TString outfilename;
  outfilename.Form("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250325_Task14/2D_Linear_FOM_%s_sel%d.txt",histname, sel);
  ofstream outfile;
  outfile.open(outfilename);
  outfile << "This file contains significance S/sqrt(Bkg) for the histogram " << histname << endl;
  outfile << "Cuts are 2D linear cuts: y >= m x + q  with "<< ncutsm <<" m values between "<< firstcutm <<" and "<< lastcutm <<" included, "<< ncutsq <<" q values between "<< firstcutq <<" and "<< lastcutq <<" included. "<<endl;
  outfile << "x is " << axesnames1 << " , y is " << axesnames3 << endl;
  outfile << "" << endl;
*/
  // Total number of events
  std::vector<Double_t> integrals;
  for(Int_t j = 0; j < hist.size(); ++j){
    integrals.push_back(hist.at(j)->Integral());
  }

  // Loop over cuts
  for(Int_t im = 0; im < ncutsm; ++im){
  for(Int_t iq = 0; iq < ncutsq; ++iq){
  
    signal = 0.;
    bkg = 0.;

    // Calculate m and q values for cuts
    cutm = firstcutm + im*(lastcutm-firstcutm)/(ncutsm-1);
    cutq = firstcutq + iq*(lastcutq-firstcutq)/(ncutsq-1);

    // Loop over histograms
    for(Int_t j = 0; j < hist.size(); ++j){
      // Loop over bins of a histogram
      for(Int_t kx = 1; kx <= hist.at(j)->GetXaxis()->GetNbins(); ++kx){
      for(Int_t ky = 1; ky <= hist.at(j)->GetYaxis()->GetNbins(); ++ky){
        bincenterx = hist.at(j)->GetXaxis()->GetBinCenter(kx);
        bincentery = hist.at(j)->GetYaxis()->GetBinCenter(ky);
        if( bincentery >= bincenterx * cutm + cutq ){
          if(j==0){
            signal += hist.at(j)->GetBinContent(kx,ky);
          }else{
            bkg += hist.at(j)->GetBinContent(kx,ky);
          }            
        }
      }// end loop over xbins of a histogram
      }// end loop over ybins of a histogram
    }// end loop over histograms

    // Calculate significance and put it in a histogram and in a file with the percentage of considered signal and bkgs
    if(bkg != 0. ){
      significance = signal/sqrt(bkg);
    }else{
      significance = 1e8;
    }

    fom->Fill(cutm,cutq,significance);
    //outfile <<"mcut:\t"<<cutm<<" qcut:\t"<<cutq<<"\tsignificance: "<<significance<<" ,\tLeft "<<100.*signal/integrals.at(0)<<" % signal, \t"<<100.*bkg/accumulate(integrals.begin()+1,integrals.end(),0.)<<" % bkg."<<endl;    

    histsignal->Fill(cutm,cutq,100.*signal/integrals.at(0) );
    histbkg->Fill(cutm,cutq,100.*bkg/accumulate(integrals.begin()+1,integrals.end(),0.) );

  }// end loop over ycuts
  }// end loop over xcuts

  fom->Draw("COLZ");
  //outfile.close();

  // Save Figure Of Merit in a pdf
  TString outpdf;
  outpdf.Form("/gfsvol01/atlas/giuliac/plots_and_outputs/Zprimethesisplots/2D_Linear_fom_%s_%d",histname,sel);
  c->SaveAs(outpdf + ".pdf");
  //c->Close();

  // Save histos of signal and bkg left
  TCanvas *csig = new TCanvas(canva + "_sig", canva + "_sig", 0, 0, 800, 600);
  gStyle->SetOptStat(0);
  gStyle->SetTitleSize(0.050);
  gStyle->SetLabelSize(0.050);
  gPad->SetLeftMargin(0.15);
  gPad->SetRightMargin(0.20);
  gPad->SetLogz();

  histsignal->GetXaxis()->SetTitleSize(.050);
  histsignal->GetYaxis()->SetTitleSize(.050);
  histsignal->GetZaxis()->SetTitleSize(.050);
  histsignal->GetXaxis()->SetLabelSize(.040);
  histsignal->GetYaxis()->SetLabelSize(.040);
  histsignal->GetZaxis()->SetLabelSize(.040);
  histsignal->Draw("COLZ");
  histsignal->SetMaximum(100.);
  histsignal->SetMinimum(0.);
  csig->SaveAs(outpdf + "_signal_left" + ".pdf");

  TCanvas *cbkg = new TCanvas(canva + "_bkg", canva + "_bkg", 0, 0, 800, 600);
  gStyle->SetOptStat(0);
  gStyle->SetTitleSize(0.050);
  gStyle->SetLabelSize(0.050);
  gPad->SetLeftMargin(0.15);
  gPad->SetRightMargin(0.20);
  gPad->SetLogz();

  histbkg->GetXaxis()->SetTitleSize(.050);
  histbkg->GetYaxis()->SetTitleSize(.050);
  histbkg->GetZaxis()->SetTitleSize(.050);
  histbkg->GetXaxis()->SetLabelSize(.040);
  histbkg->GetYaxis()->SetLabelSize(.040);
  histbkg->GetZaxis()->SetLabelSize(.040);
  histbkg->Draw("COLZ");
  histbkg->SetMaximum(100.);
  histbkg->SetMinimum(0.);
  cbkg->SaveAs(outpdf + "_bkg_left" + ".pdf");

}
//===============================
//---------- END FUNCTIONS -------------------

void FigureOfMerit(Int_t sel=0)
{

  if(sel < 0 || sel > 2){
    cout << " sel must be 0, 1 or 2" << endl;
    exit(EXIT_FAILURE);
  }

  // Open histograms' file
  TString filename;
  filename.Form("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250324_Task14/histos_sel%d.root",sel);

  TFile *infile = new TFile(filename);

  // Calculate 1D FOMs
/*
  // DeltaR
  FOM1D(sel,infile,"DeltaR_between_leading_and_subleading_large_jets",3,1.0,6.0,21,0.);

  // Number of Constituents
  FOM1D(sel,infile,"number_of_constituents_of_leading_and_subleading_large_jets",3,-0.5,9.5,11,10.5);

  // Leading Jet PT
  FOM1D(sel,infile,"leading_large_jet_pt",3,250,1600,28,2500);

  // Subleading Jet PT
  FOM1D(sel,infile,"subleading_large_jet_pt",3,250,1600,28,2500);

  // Invariant Mass
  FOM1D(sel,infile,"Invariant_mass_of_leading_and_subleading_large_jets",3,250,3000,56,6000);
*/
  // Calculate 2D Rectangular FOMs
  FOM2D_rectangle(sel,infile,"JetPT1_vs_Invmass",3,300.,1200.,19,400.,2500.,22);
/*
  FOM2D_rectangle(sel,infile,"JetPT1_vs_NConst",3,300.,1200.,19,0.5,7.5,8);
  FOM2D_rectangle(sel,infile,"Invmass_vs_NConst",3,400.,2500.,22,0.5,7.5,8);

  // Calculate 2D Linear FOMs
  FOM2D_line(sel,infile,"Invmass_vs_NConst",3,-0.007,0.,71,3.,10.,15);
*/
  FOM2D_line(sel,infile,"JetPT1_vs_NConst",3,-0.01,0.,101,3.,10.,15);

}
