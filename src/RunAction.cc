
#include "G4MPImanager.hh"
#include "Analysis.hh"
#include "RunAction.hh"
#include "g4root.hh"
#include "G4MPIhistoMerger.hh"
#include "MyRun.hh"
#include <stdio.h>

RunAction::RunAction()
        : G4UserRunAction(), thefilename("test"), runM(NULL)
{
  runM = new RunMessenger(this);
}

RunAction::~RunAction()
{
  delete runM;
}

G4Run* RunAction::GenerateRun()
{
  return new MyRun;
}

void RunAction::BeginOfRunAction(const G4Run*)
{
  Analysis* myana = Analysis::GetAnalysis();
  myana->Clear();
  myana->Book();
  std::cout << "Beginning Run..." << std::endl;
}

void RunAction::EndOfRunAction(const G4Run*)
{
  const G4int rank = G4MPImanager::GetManager()->GetRank();
  if(rank == 0)
  {
    G4String fname("data-rank0");
    Analysis* myana = Analysis::GetAnalysis();
    myana->Save(fname);
    myana->Close(false);
  }
  G4MPIhistoMerger hm(G4AnalysisManager::Instance());
  hm.SetVerbosity(0);
  hm.Merge();
  if(rank == 0)
  {
    std::ostringstream fname;
    G4String merged = "-merged";
    thefilename = thefilename + merged;
    fname.str(thefilename);
    Analysis* myana = Analysis::GetAnalysis();
    myana->Save(fname.str());
  }
  Analysis* myana = Analysis::GetAnalysis();
  myana->Close();
        
  if(remove("data-rank0.root") !=0)
     std::cerr << "ERROR: Potential Error with Data Transfer!" << std::endl;
  else
      std::cout << "Data Transferred Successfully" << std::endl;

}
