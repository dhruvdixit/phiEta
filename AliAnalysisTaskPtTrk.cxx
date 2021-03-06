#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TMath.h"

#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"

#include "AliAODEvent.h"
#include "AliAODTrack.h"
#include "AliAODInputHandler.h"

#include "AliAODCluster.h"
#include "AliAODCaloCluster.h"
#include "AliAnalysisTaskPtTrk.h"

// example of an analysis task creating a p_t spectrum
// Authors: Panos Cristakoglou, Jan Fiete Grosse-Oetringhaus, Christian Klein-Boesing

ClassImp(AliAnalysisTaskPtTrk)

//________________________________________________________________________
AliAnalysisTaskPtTrk::AliAnalysisTaskPtTrk(const char *name) 
  : AliAnalysisTask(name, ""), fAOD(0), fHistList(0)
{
  // Constructor

  // Define input and output slots here
  // Input slot #0 works with a TChain
  DefineInput(0, TChain::Class());
  // Output slot #0 writes into a TList container
  DefineOutput(0, TList::Class());
}

//________________________________________________________________________
void AliAnalysisTaskPtTrk::ConnectInputData(Option_t *) 
{
  // Connect AOD or AOD here
  // Called once

  TTree* tree = dynamic_cast<TTree*> (GetInputData(0));
  if (!tree) {
    Printf("ERROR: Could not read chain from input slot 0");
  } else {
    // Disable all branches and enable only the needed ones
    // The next two lines are different when data produced as AliAODEvent is read
    /*
    tree->SetBranchStatus("*", kFALSE);
    tree->SetBranchStatus("fTracks.*", kTRUE);
    */

    AliAODInputHandler *esdH = dynamic_cast<AliAODInputHandler*> (AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler());

    if (!esdH) {
      Printf("ERROR: Could not get AODInputHandler");
    } else
      fAOD = esdH->GetEvent();
  }
}

//________________________________________________________________________
void AliAnalysisTaskPtTrk::CreateOutputObjects()
{
  // Create histograms
  // Called once

  fHistPt = new TH1F("fHistPt", "P_{T} distribution", 120, -0.5, 119.5);
  fHistPt->GetXaxis()->SetTitle("P_{T} (GeV/c)");
  fHistPt->GetYaxis()->SetTitle("dN/dP_{T} (c/GeV)");
  fHistPt->SetMarkerStyle(kFullCircle);

  fHistE = new TH1F("fHistE", "Energy distribution", 100, -0.5, 99.5);
  fHistE->GetXaxis()->SetTitle("E (GeV/)");
  fHistE->GetYaxis()->SetTitle("dN/dE (GeV)");
  fHistE->SetMarkerStyle(kFullSquare);
  
  fHistDelAngle = new TH2F("fHistDelAngle", "Delta Phi Eta distribution", 64,-0.05, 6.35, 20, -0.05,1.95 );
  fHistDelAngle->GetXaxis()->SetTitle("Del Phi");
  fHistDelAngle->GetYaxis()->SetTitle("Del Eta");
  
  fHistList = new TList();
  
  //Adding to the histogram list
  fHistList->Add(fHistPt);
  fHistList->Add(fHistE);
  fHistList->Add(fHistDelAngle);
}

//________________________________________________________________________
void AliAnalysisTaskPtTrk::Exec(Option_t *) 
{
  // Main loop
  // Called for each event

  if (!fAOD) {
    Printf("ERROR: fAOD not available");
    return;
  }

  Printf("There are %d tracks in this event", fAOD->GetNumberOfTracks());
  Printf("There are %d clusters in this event", fAOD->GetNumberOfCaloClusters());

  /*// Track loop to fill a pT spectrum
  for (Int_t iTracks = 0; iTracks < fAOD->GetNumberOfTracks(); iTracks++) 
    {
      AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(iTracks));
      
      } //track loop */
  
  //Delta Phi and Eta loop on the tracks
  //currently this does all permutations in a bubble sort mamner, but if order does not matter, then set j = i+1
  for(int i = 0; i < fAOD->GetNumberOfTracks(); i++)
    {
      AliAODTrack* iTrack = static_cast<AliAODTrack*>(fAOD->GetTrack(i));
      
      if (!iTrack) 
	{
	  Printf("ERROR: Could not receive iTrack %d", i);
	  continue;
	}
      fHistPt->Fill(iTrack->Pt());
      
      for(int j = i+1;j < fAOD->GetNumberOfTracks(); j++)	
	{
	  if(i != j)
	    {
	      AliAODTrack* jTrack = static_cast<AliAODTrack*>(fAOD->GetTrack(j));
	      if ((!jTrack)) 
		{
		  Printf("ERROR: Could not receive jTrack %d", j);
		  continue;
		}
	      //Phi stuff
	      double iPhi = iTrack->Phi();
	      double jPhi = jTrack->Phi();
	      double del_Phi = TMath::Abs(iPhi-jPhi);
	      //Theta stuff
	      double iEta = iTrack->Eta();
	      double jEta = jTrack->Eta();
	      double del_Eta = TMath::Abs(iEta-jEta);

	      fHistDelAngle->Fill(del_Phi, del_Eta);

	    }
	}//j loop
    }//i loop

  //Calo Clusters loop to fill the E spectrum
  for(Int_t iClusters = 0; iClusters < fAOD->GetNumberOfCaloClusters(); iClusters++)
    {
      AliAODCaloCluster* cluster = static_cast<AliAODCaloCluster*>(fAOD->GetCaloCluster(iClusters));
      if (!cluster) 
	{
	  Printf("ERROR: Could not receive cluster %d", iClusters);
	  continue;
	}
      if(cluster->IsEMCAL())
	{
	  fHistE->Fill(cluster->E());
	}
    }
  
  // Post output data.
  PostData(0, fHistList);
}      

//________________________________________________________________________
void AliAnalysisTaskPtTrk::Terminate(Option_t *) 
{
  // Draw result to the screen
  // Called once at the end of the query
  
  //Hist List
  fHistList = static_cast<TList*> (GetOutputData(0));
  if (!fHistList) 
    {
      Printf("ERROR: fHistList not available");
      return;
    }
  
  fHistPt = static_cast<TH1F*>(fHistList->FindObject("fHistPt"));
  if (!fHistPt) 
    {
      Printf("ERROR: fHistPt not available");
      return;
    }
  TCanvas *c1 = new TCanvas("AliAnalysisTaskPtTrk","Pt",10,10,510,510);
  c1->cd(1)->SetLogy();
  fHistPt->Draw("E");
  
  //E hist
  fHistE = static_cast<TH1F*>(fHistList->FindObject("fHistE"));
  if (!fHistE) 
    {
      Printf("ERROR: fHistE not available");
      return;
    }
  
  TCanvas *c2 = new TCanvas("c1","E",10,10,510,510);
  c2->cd(1)->SetLogy();
  fHistE->Draw("E");
  
  //Angle Hist
  fHistDelAngle = static_cast<TH2F*>(fHistList->FindObject("fHistDelAngle"));
  if (!fHistDelAngle) 
    {
      Printf("ERROR: fHistDelAngle not available");
      return;
    }
  
  TCanvas *c3 = new TCanvas("c3","Del Angle",10,10,510,510);
  c3->cd(1)->SetLogz();
  fHistDelAngle->Draw("colz");

}










