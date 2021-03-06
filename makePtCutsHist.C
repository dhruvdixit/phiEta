void makePtCutsHist()
{
  // load analysis framework
  gSystem->Load("libANALYSIS");

  //gROOT->LoadMacro("$ALICE_ROOT/PWG/EMCAL/macros/CreateAODChain.C"); //Where do I find this?
  //TChain* chain = CreateAODChain("/home/Dhruv/spring16/files_pb.txt", 8); //How do I get this files?
  //Manual Chaining
  TChain *chain = new TChain("aodTree");
  chain->Add("$HOME/phiEta/AliAOD_168464_pass2_AOD145_0145.root");

  // for includes use either global setting in $HOME/.rootrc
  // ACLiC.IncludePaths: -I$(ALICE_ROOT)/include
  // or in each macro
  gSystem->AddIncludePath("-I$ALICE_ROOT/include");
  gSystem->AddIncludePath("-I$ALICE_PHYSICS/include");

  // Create the analysis manager
  AliAnalysisManager *mgr = new AliAnalysisManager("testAnalysis");

  AliVEventHandler* aodH = new AliAODInputHandler();
  mgr->SetInputEventHandler(aodH);

  // Create task

  gROOT->LoadMacro("$HOME/phiEta/AliAnalysisTaskPtCuts.cxx+g");
  AliAnalysisTaskPtCuts *task = new AliAnalysisTaskPtCuts("task");

  // Add task
  mgr->AddTask(task);

  // Create containers for input/output
  AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer();
  AliAnalysisDataContainer *coutput = mgr->CreateContainer("histos", TList::Class(), AliAnalysisManager::kOutputContainer, "ubuntu_test.root");

  // Connect input/output
  mgr->ConnectInput(task, 0, cinput);
  mgr->ConnectOutput(task, 0, coutput);

  // Enable debug printouts
  mgr->SetDebugLevel(2);

  if (!mgr->InitAnalysis())
    return;

  mgr->PrintStatus();

  mgr->StartAnalysis("local",chain);
}
