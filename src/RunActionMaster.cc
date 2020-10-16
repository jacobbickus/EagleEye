
#include "G4MPImanager.hh"
#include "Analysis.hh"
#include "RunActionMaster.hh"
#include "g4root.hh"
#include "G4MPIhistoMerger.hh"
#include "MyRun.hh"

RunActionMaster::RunActionMaster()
        : G4UserRunAction(), thefilename("test"), runM(NULL)
{
  runM = new RunMasterMessenger(this);
}

RunActionMaster::~RunActionMaster()
{
  delete runM;
}

G4Run* RunActionMaster::GenerateRun()
{
  return new MyRun;
}

void RunActionMaster::BeginOfRunAction(const G4Run*)
{
  Analysis* myana = Analysis::GetAnalysis();
  myana->Clear();
  myana->Book();
  //std::cout << "Beginning Run..." << std::endl;
}

void RunActionMaster::EndOfRunAction(const G4Run*)
{
  const G4int rank = G4MPImanager::GetManager()->GetRank();
  if(rank == 0)
  {
    G4String rankFilename = thefilename + "-temp";
    G4String fname(rankFilename);
    Analysis* myana = Analysis::GetAnalysis();
    myana->Save(fname);
    myana->Close(false);
  }
  std::cout << "Merging " << rank << std::endl;
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
    std::cout << "Merged File saved" << std::endl;
  }
  Analysis* myana = Analysis::GetAnalysis();
  myana->Close();
  std::cout << "Run Complete " << rank << std::endl;

}
