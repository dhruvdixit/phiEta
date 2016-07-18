#ifndef AliAnalysisTaskPtTrk_cxx
#define AliAnalysisTaskPtTrk_cxx

// example of an analysis task creating a p_t spectrum
// Authors: Panos Cristakoglou, Jan Fiete Grosse-Oetringhaus, Christian Klein-Boesing

class TH1F;
class AliAODEvent;

#include "AliAnalysisTask.h"

class AliAnalysisTaskPtTrk : public AliAnalysisTask 
{
 public:
  AliAnalysisTaskPtTrk() : AliAnalysisTask(), fAOD(0), fHistList(0) {}
  AliAnalysisTaskPtTrk(const char *name);
  virtual ~AliAnalysisTaskPtTrk() {}
  
  virtual void   ConnectInputData(Option_t *);
  virtual void   CreateOutputObjects();
  virtual void   Exec(Option_t *option);
  virtual void   Terminate(Option_t *);
  
  AliAODEvent *fAOD;    //AOD object
  TList       *fHistList; // Data storing list
  TH1F        *fHistPt; //Pt spectrum
  TH1F        *fHistE; //E spectrum
  TH2F        *fHistDelAngle;//delta phi and theta

 private:   
  AliAnalysisTaskPtTrk(const AliAnalysisTaskPtTrk&); // not implemented
  AliAnalysisTaskPtTrk& operator=(const AliAnalysisTaskPtTrk&); // not implemented
  
  ClassDef(AliAnalysisTaskPtTrk, 1); // example of analysis
};

#endif
