/*
Author: Giulia Cossutti

Macro to reproduce High mjj selection and ABCD plane for signal and two backgrounds

From inside the /gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes directory run with
root -l -q EJ_reproduction/tasks/20250416_Task16/HighMjj.C'("EJ_reproduction/tasks/20250221_Task10/50K_Reclustering.root", "signal")' &
root -l -q EJ_reproduction/tasks/20250416_Task16/HighMjj.C'("EJ_reproduction/tasks/20250310_Task12/50K_bkg_Top.root", "bkg_Top")' &
root -l -q EJ_reproduction/tasks/20250416_Task16/HighMjj.C'("EJ_reproduction/tasks/20250310_Task12/50K_bkg_HardQCD.root", "bkg_HardQCD")' &

*/

#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/classes/DelphesClasses.h"
#include "/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#include "modules/TrackCovariance.h"
#include "external/TrackCovariance/TrkUtil.h"
#include "external/TrackCovariance/VertexFit.h"
#endif

//------------------------------------------------------------------------------
//----------- FUNCTIONS -----------------------
//===============================
// Function to compute DeltaR between 2 jets
Double_t DeltaR(Jet* jet1, Jet* jet2){
 return sqrt( pow(jet1->Eta - jet2->Eta,2) + pow(jet1->Phi - jet2->Phi,2) );
}
//===============================
// Function to compute DeltaR between a jet and a track
Double_t DeltaR(Jet* jet, Track* trk){
 return sqrt( pow(jet->Eta - trk->Eta,2) + pow(jet->Phi - trk->Phi,2) );
}
//===============================
// Function to compute DeltaR between 2 tracks
Double_t DeltaR(Track* trk1, Track* trk2){
 return sqrt( pow(trk1->Eta - trk2->Eta,2) + pow(trk1->Phi - trk2->Phi,2) );
}
//===============================
// Function to find primary vertex from fit tracks
Double_t FindPVZ(ExRootTreeReader* treeReader,Int_t entry,TClonesArray *branchTrackPV,TClonesArray *branchGenPartPV){
  TClonesArray *branchTrack = branchTrackPV;
  TClonesArray *branchGenPart = branchGenPartPV;

  Double_t zPV = 1e5;

		// Load selected branches with data from specified event
		treeReader->ReadEntry(entry);
		Int_t NtrG = branchTrack->GetEntries();
		TVectorD** pr = new TVectorD * [NtrG];		// Track Parameters
		TMatrixDSym** cv = new TMatrixDSym * [NtrG];	// Track covariances
		Bool_t *fprvx = new Bool_t[NtrG];		// Primary vertex flag
		//
		// test Particle branch
		Int_t Ngen = branchGenPart->GetEntries();
		//std::cout << "Nr. of generated particles: " << Ngen << std::endl;
		// If event contains at least 1 track
		//
		Double_t Nprim = 0.0;
		Double_t xpv = 0.0;		// Init true primary vertex position
		Double_t ypv = 0.0;
		Double_t zpv = 0.0;
		if (branchTrack->GetEntries() > 0)
		{
			// Loop on tracks
			for (Int_t it = 0; it < branchTrack->GetEntries(); it++)
			{
				Track* trk = (Track*)branchTrack->At(it);
				//
				// Start fitting all available tracks
				//
				// Reconstructed track parameters
				Double_t obsD0 = trk->D0;
				Double_t obsPhi = trk->Phi;
				Double_t obsC = trk->C;
				Double_t obsZ0 = trk->DZ;
				Double_t obsCtg = trk->CtgTheta;
				//std::cout << "Got track parameters for track " << it << std::endl;
				//
				// Load all tracks for vertex fit 
				Double_t oPar[5] = { obsD0, obsPhi, obsC, obsZ0, obsCtg };
				TVectorD obsPar(5, oPar);	// Fill observed parameters
				pr[it] = new TVectorD(obsPar);
				cv[it] = new TMatrixDSym(trk->CovarianceMatrix());
			//
			// Find true primary vertex
				GenParticle* gp = (GenParticle*)trk->Particle.GetObject();
				//std::cout << "GenParticle pointer "<<gp << std::endl;
				//
				// Position of origin in mm
				Double_t x = gp->X;
				Double_t y = gp->Y;
				Double_t z = gp->Z;
				Bool_t prim = kTRUE;	// Is primary?
				fprvx[it] = kFALSE;
				Int_t mp = gp->M1;	// Mother
				while (mp > 0) {
					GenParticle* gm =
						(GenParticle*)branchGenPart->At(mp);
					Double_t xm = gm->X;
					Double_t ym = gm->Y;
					Double_t zm = gm->Z;
					if (x != xm || y != ym || z != zm) {
						prim = kFALSE;
						break;
					}
					else mp = gm->M1;
				}
				if (prim) {		// It's a primary track
					Nprim++;
					fprvx[it] = kTRUE;
					xpv = x;	// Store true primary
					ypv = y;
					zpv = z;
				}

			}		// End loop on tracks
		}
		if(entry%100 ==0){
		  std::cout << "Event "<<entry<<" opened containing " << NtrG << " / "<< Nprim 
		  << "   Total / primary tracks"<< std::endl;
		}
		//std::cout<<"PVtxFind true vertex: Nprim= "<<Nprim<<", x,y,z= "<<xpv<<", "<<ypv<<", "<<zpv<<std::endl;
		//
		// Find primary vertex
		//
		//Beam constraint
		TVectorD xpvc(3);
		xpvc(0) = 1.0;
		xpvc(1) = -2.0;
		xpvc(2) = 10.0;
		TMatrixDSym covpvc(3); covpvc.Zero();
		covpvc(0, 0) = 0.0097 * 0.0097;
		covpvc(1, 1) = 2.55e-05 * 2.55e-05;
		covpvc(2, 2) = 0.64 * 0.64;
		if(Nprim == 0){
			xpv = xpvc(0);
			ypv = xpvc(1);
			zpv = xpvc(2);
		}
		//
		//
		// Skim tracks
		Int_t nSkim = 0;
		Int_t* nSkimmed = new Int_t[NtrG];
		TVectorD** PrSk = new TVectorD * [1];
		TMatrixDSym** CvSk = new TMatrixDSym * [1];
		Double_t MaxChi2 = 9.;
		for (Int_t n = 0; n < NtrG; n++) {
			PrSk[0] = new TVectorD(*pr[n]);
			CvSk[0] = new TMatrixDSym(*cv[n]);
			VertexFit* Vskim = new VertexFit(1,PrSk, CvSk);
			Vskim->AddVtxConstraint(xpvc, covpvc);
			Double_t Chi2One = Vskim->GetVtxChi2();
			//std::cout<<"Track "<<n<<", Chi2 = "<<Chi2One<<std::endl;
			//
			if (Chi2One < MaxChi2) {
				nSkimmed[nSkim] = n;
				//std::cout << "nSkimmed[" << nSkim << "] = " << n << std::endl;
				nSkim++;
			}
			// Cleanup
			delete Vskim;
		}
		delete PrSk[0];
		delete CvSk[0];
		delete[] PrSk;
		delete[] CvSk;
		//
		// Load tracks for primary fit
		Int_t MinTrk = 1;	// Minumum # tracks for vertex fit
		std::vector<Int_t> trnum;
		if (nSkim >= MinTrk) {
			TVectorD** PrFit = new TVectorD * [nSkim];
			TMatrixDSym** CvFit = new TMatrixDSym * [nSkim];
			for (Int_t n = 0; n < nSkim; n++) {
				PrFit[n] = new TVectorD(*pr[nSkimmed[n]]);
				CvFit[n] = new TMatrixDSym(*cv[nSkimmed[n]]);
				trnum.push_back(nSkimmed[n]);
			}
			delete[] nSkimmed;
			Int_t Nfound = nSkim;
			if(entry%100 ==0)std::cout << "Found tracks "<<Nfound << std::endl;
			const Int_t MaxFound = 100; Double_t Chi2LL[MaxFound]; Double_t *Chi2L = Chi2LL;
			VertexFit* Vtx = new VertexFit(nSkim, PrFit, CvFit);
			//std::cout << "Vertex fit created " << std::endl;
			Vtx->AddVtxConstraint(xpvc, covpvc);
			//
			// Remove tracks with large chi2
			Double_t MaxChi2Fit = 8.0;
			Bool_t Done = kFALSE;
			while (!Done) {
				//std::cout << "After while " << std::endl;
				// Find largest Chi2 contribution
				TVectorD Chi2List = Vtx->GetVtxChi2List();	// Get contributions to Chi2
				//std::cout << "After Chi2List.  " << std::endl; Chi2List.Print();
				//Double_t* Chi2L = new Double_t[Nfound];
				Chi2L = Chi2List.GetMatrixArray();
				Int_t iMax = TMath::LocMax(Nfound, Chi2L);
				//std::cout << "iMax =  "<<iMax << std::endl;
				Double_t Chi2Mx = Chi2L[iMax];
				//std::cout << "Chi2Mx "<<Chi2Mx << std::endl;
				if (Chi2Mx > MaxChi2Fit && Nfound > 1) {
					//std::cout << "Before remove.  Nfound = "<<Nfound << std::endl;
					Vtx->RemoveTrk(iMax);
					trnum.erase(trnum.begin() + iMax);
					//std::cout << "After remove." << std::endl;
					Nfound--;
				}
				else {
					Done = kTRUE;
				}
			}
			//
			//std::cout << "Before getting vertex " << std::endl;
			//

                        // zPV in case Chi2One = nan
			zPV = zpv;

			// Require minimum number of tracks in vertex
			Int_t Nmin = 1;
			if (Nfound >= Nmin) {
				TVectorD xvtx = Vtx->GetVtx();
				//std::cout << "Found vertex " << xvtx(0)<<", "<<xvtx(1)<<", "<<xvtx(2) << std::endl;
				zPV = xvtx(2);
			}
			//
			// Clean
			delete Vtx;
			for (Int_t i = 0; i < nSkim; i++) delete PrFit[i];
			for (Int_t i = 0; i < nSkim; i++) delete CvFit[i];
			delete[] PrFit;
			delete[] CvFit;
			delete[] fprvx;
			trnum.clear();
		}

		//std::cout << "Vertex chi2/Ndof = " << Chi2 / Ndof << std::endl;
		//
		// Cleanup
		for (Int_t i = 0; i < NtrG; i++) delete pr[i];
		for (Int_t i = 0; i < NtrG; i++) delete cv[i];
		delete[] pr;
		delete[] cv;

  return zPV;
}
//===============================
// Function to calculate Prompt Track Fraction without z cut
Double_t PTFnoz(ExRootTreeReader* treeReader,Int_t entry,TClonesArray *branchTrackPTF, Jet* jet){
  TClonesArray *branchTrack = branchTrackPTF;

  // Load selected branches with data from specified event
  treeReader->ReadEntry(entry);

  // Sum of PT of selected tracks
  Double_t sumPT = 0.0;

  // Number of used tracks
  Int_t count = 0;

  // Loop on tracks
  for (Int_t it = 0; it < branchTrack->GetEntries(); it++){
    Track* trk = (Track*)branchTrack->At(it);

    // Track definition for Large Radius Tracking algorithm
    if( (trk->D0 >= 300) || (trk->DZ >= 500) || (trk->PT <= 1) || (abs(trk->Eta) >= 3.0) ) continue;

    // Track within DeltaR = 1.2 from the jet
    if( DeltaR(jet, trk) >= 1.2 ) continue;

    // Cut on d0
    if( trk->D0 >= 2.5 * (0.0463/(trk->PT) + 0.0195) ) continue;

    // Increment track counter
    count += 1;

    // Add track PT to sum
    sumPT += trk->PT;
  }

  //cout << "Tracks used in PTFnoz for event " << entry << " : " << count << endl; 

  return sumPT/(jet->PT);
}
//===============================
// Function to calculate Prompt Track Fraction with z cut
Double_t PTF(ExRootTreeReader* treeReader,Int_t entry,TClonesArray *branchTrackPTF, Jet* jet, Double_t zPV){
  TClonesArray *branchTrack = branchTrackPTF;

  // Load selected branches with data from specified event
  treeReader->ReadEntry(entry);

  // Sum of PT of selected tracks
  Double_t sumPT = 0.0;

  // Number of used tracks
  Int_t count = 0;

  // Loop on tracks
  for (Int_t it = 0; it < branchTrack->GetEntries(); it++){
    Track* trk = (Track*)branchTrack->At(it);

    // Track definition for Large Radius Tracking algorithm
    if( (trk->D0 >= 300) || (trk->DZ >= 500) || (trk->PT <= 1) || (abs(trk->Eta) >= 3.0) ) continue;

    // Track within DeltaR = 1.2 from the jet
    if( DeltaR(jet, trk) >= 1.2 ) continue;

    // Cut on d0
    if( trk->D0 >= 2.5 * (0.0463/(trk->PT) + 0.0195) ) continue;

    // Cut on z0
    if( abs(zPV - trk->DZ) >= 10 ) continue;

    // Increment track counter
    count += 1;

    // Add track PT to sum
    sumPT += trk->PT;
  }

  //cout << "Tracks used in PTF for event " << entry << " : " << count << endl; 

  return sumPT/(jet->PT);
}
//===============================
// Function to calculate 2-point Energy Correlation Function of a jet
Double_t ECF2(ExRootTreeReader* treeReader,Int_t entry,TClonesArray *branchTrackECF, Jet* jet){
  TClonesArray *branchTrack = branchTrackECF;

  // Load selected branches with data from specified event
  treeReader->ReadEntry(entry);

  // ECF2
  Double_t ECF2 = 0.0;

  // Loop1 on tracks
  for (Int_t i = 0; i < branchTrack->GetEntries(); i++){
    Track* trk1 = (Track*)branchTrack->At(i);

    // Track definition for Large Radius Tracking algorithm
    if( (trk1->D0 >= 300) || (trk1->DZ >= 500) || (trk1->PT <= 1) || (abs(trk1->Eta) >= 3.0) ) continue;

    // Track within DeltaR = 1.0 from the jet
    if( DeltaR(jet, trk1) >= 1.0 ) continue;

    // Loop2 on tracks
    for (Int_t j = 0; j < i; j++){
      Track* trk2 = (Track*)branchTrack->At(j);

      // Track definition for Large Radius Tracking algorithm
      if( (trk2->D0 >= 300) || (trk2->DZ >= 500) || (trk2->PT <= 1) || (abs(trk2->Eta) >= 3.0) ) continue;

      // Track within DeltaR = 1.0 from the jet
      if( DeltaR(jet, trk2) >= 1.0 ) continue;

      // Add to ECF2
      ECF2 += trk1->PT * trk2->PT * DeltaR(trk1, trk2);
    }
  }

  return ECF2;
}
//===============================
//----------- END FUNCTIONS ---------------------

void HighMjj(const char *inFile, const char *outFile)
{

  gSystem->Load("libDelphes");

  // Create chain of root trees
  TChain chain("Delphes");
  chain.Add(inFile);

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
  TH2F *PTF1_PTF2 = new TH2F("PTF1_vs_PTF2", "", 100, 0., 1., 100, 0., 1.);
  TH2F *PTF1noz_PTF2noz = new TH2F("PTF1noz_vs_PTF2noz", "", 100, 0., 1., 100, 0., 1.);

  // Counter of used events
  Int_t NofEv = 0;
  Int_t NofEv_noz = 0;

  // Invariant mass of leading and subleading large jets
  Double_t mjj = 0.0;

  // z of Hard Scattering Primary Vertex
  Double_t zPV = 0.0;

  // Prompt Track Fraction of leading and subleading large jets
  Double_t PTF1 = 0.0;
  Double_t PTF2 = 0.0;
  Double_t PTF1noz = 0.0;
  Double_t PTF2noz = 0.0;

  // 2-point Energy Correlation Function of leading and subleading large jets
  Double_t ECF21 = 0.0; 
  Double_t ECF22 = 0.0; 

  // Loop over all events
  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
//  for(Int_t entry = 0; entry < 10; ++entry)
  {
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);

    //------------- Event selection ------------------

    // Number of jets
    if( branchReclusteredJet->GetEntries() < 2 ) continue;

    // Large-R jet trigger
    Jet *ljet = (Jet*) branchReclusteredJet->At(0);
    if( ljet->PT <= 460. ) continue;

    // Find Leading and Subleading Large jets
    Int_t index_large = 0;
    Int_t index_large1 = 0;
    Int_t index_large2 = 0;

    // Loop over all large jets
    for(Int_t jetentry = 0; jetentry < branchReclusteredJet->GetEntries(); ++jetentry)
    {
       Jet *jet = (Jet*) branchReclusteredJet->At(jetentry);
       if( abs(jet->Eta) >= 1.5 ) continue;
       index_large += 1;

       // Indexes of leading and subleading large jets
       if(index_large == 1){
         index_large1 = jetentry;
       }else if(index_large == 2){
         index_large2 = jetentry;
         break;
       }
    }//end loop over all large jets

    if(index_large < 2) continue;

    Jet *largejet1 = (Jet*) branchReclusteredJet->At(index_large1);
    Jet *largejet2 = (Jet*) branchReclusteredJet->At(index_large2);

    // Leading and Subleading jet pT
    if( largejet1->PT <= 520.) continue;
    if( largejet2->PT <= 300.) continue;

    // Invariant mass mjj
    TLorentzVector *largejet1P4 = new TLorentzVector(1.0,1.0,1.0,1.0);
    largejet1P4->SetPtEtaPhiM(largejet1->PT,largejet1->Eta,largejet1->Phi,largejet1->Mass);
    TLorentzVector *largejet2P4 = new TLorentzVector(1.0,1.0,1.0,1.0);
    largejet2P4->SetPtEtaPhiM(largejet2->PT,largejet2->Eta,largejet2->Phi,largejet2->Mass);

    mjj = (*largejet1P4 + *largejet2P4).M() ;
    if( mjj <= 1000.) continue;

    // Cut on 2-point Energy Correlation Function
    ECF21 = ECF2(treeReader, entry, branchTrack, largejet1);
    ECF22 = ECF2(treeReader, entry, branchTrack, largejet2);

    if( (ECF21/largejet1->PT <= 40) || (ECF22/largejet2->PT <= 40) ) continue;

    // Impose zPV = 0.0
    zPV = 0.0;
/*
    // Find z of Hard Scattering Primary Vertex
    zPV = FindPVZ(treeReader,entry,branchTrack,branchParticle);
    if( zPV > 1e4 ) cout << "  Something went wrong in finding Primary Vertex" << endl;
*/    
    // PTF of jets
    PTF1 = PTF(treeReader, entry, branchTrack, largejet1, zPV); 
    PTF2 = PTF(treeReader, entry, branchTrack, largejet2, zPV); 
    PTF1noz = PTFnoz(treeReader, entry, branchTrack, largejet1); 
    PTF2noz = PTFnoz(treeReader, entry, branchTrack, largejet2); 

    // Counter of used events with PTFnoz
    NofEv_noz += 1;

    // Fill histograms
    PTF1noz_PTF2noz->Fill(PTF1noz,PTF2noz);

    // Selection on DeltaPTF
    if( ((PTF1 - PTF1noz) >= 0.4) || ((PTF2 - PTF2noz) >= 0.4) ) continue;

    // Counter of used events with cut on DeltaPTF
    NofEv += 1;

    // Fill histograms
    PTF1_PTF2->Fill(PTF1,PTF2);

    //------------- End Event selection ------------------
  }// end loop over all events

//------------------------------------------------

  // write number of used events in text file
  TString outfilename;
  outfilename.Form("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250416_Task16/%s.txt",outFile);
  ofstream outfile;
  outfile.open(outfilename);

  outfile << "This file contains number of events passing high mjj selection in the sample " << outFile << endl;
  outfile << "" << endl;
  outfile << "Number of used Events without DeltaPTF selection: " << NofEv_noz << " out of " << numberOfEntries << endl;
  outfile << "Number of used Events with DeltaPTF selection: " << NofEv << " out of " << numberOfEntries << endl;

  outfile.close();

  cout << "Number of used Events without DeltaPTF: " << NofEv_noz << endl;
  cout << "Number of used Events: " << NofEv << endl;

//-------------------------------------------------------
  // write all plots on file
  TString outname;
  outname.Form("/gfsvol01/atlas/giuliac/HiggsTutorial/MG5_aMC_v3_5_6/Delphes/EJ_reproduction/tasks/20250416_Task16/%s.root",outFile);

  TFile *f = new TFile(outname,"RECREATE");
  
  PTF1_PTF2->Write("",TObject::kOverwrite);
  PTF1noz_PTF2noz->Write("",TObject::kOverwrite);
  
}// end HighMjj
