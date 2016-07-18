#ifndef AliAnalysisTaskPtCuts_cxx
#define AliAnalysisTaskPtCuts_cxx

// example of an analysis task creating a p_t spectrum
// Authors: Panos Cristakoglou, Jan Fiete Grosse-Oetringhaus, Christian Klein-Boesing

class TH1F;
class AliAODEvent;

#include "AliAnalysisTask.h"

class AliAnalysisTaskPtCuts : public AliAnalysisTask 
{
 public:
  AliAnalysisTaskPtCuts() : AliAnalysisTask(), fAOD(0), fHistList(0) {}
  AliAnalysisTaskPtCuts(const char *name);
  virtual ~AliAnalysisTaskPtCuts() {}
  
  virtual void   ConnectInputData(Option_t *);
  virtual void   CreateOutputObjects();
  virtual void   Exec(Option_t *option);
  virtual void   Terminate(Option_t *);
  
  AliAODEvent *fAOD;    //AOD object
  TList       *fHistList; // Data storing list

  TH1F        *fHistCent; //Centrality spectrum
  TH1F        *fHistPt; //Pt spectrum
  TH1F        *fHistPt2; //Pt spectrum
  TH1F        *fHistE; //E spectrum
  TH1F        *fHistE2; //E spectrum
  TH1F        *fHistPhi; //E spectrum
  TH1F        *fHistPhi2; //E spectrum
  TH1F        *fHistEta; //E spectrum
  TH1F        *fHistEta2; //E spectrum

  TH2F        *fHistDelPt;//delta pt and eta
  TH2F        *fHistDelE;//delta E and eta
  TH2F        *fHistDelPhi;//delta phi and eta
  TH2F        *fHistDelE_10;//delta E and eta at 0-10 centrality 
  TH2F        *fHistDelPhi_10;//delta phi and eta at 0-10 centrality
  TH2F        *fHistDelE_40;//delta E and eta at 20-40 centrality
  TH2F        *fHistDelPhi_40;//delta phi and eta at 20-40 centrality

 private:   
  AliAnalysisTaskPtCuts(const AliAnalysisTaskPtCuts&); // not implemented
  AliAnalysisTaskPtCuts& operator=(const AliAnalysisTaskPtCuts&); // not implemented
  
  ClassDef(AliAnalysisTaskPtCuts, 1); // example of analysis
};

#endif
