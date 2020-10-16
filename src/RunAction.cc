#include <G4VUserMPIrunMerger.hh>
#include "G4MPImanager.hh"
#include <stdio.h>
#include "G4Threading.hh"
#include "Analysis.hh"
#include "RunAction.hh"
#include "MyRun.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
RunAction::RunAction() : runM(NULL), thefilename("test")
{
  runM = new RunMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
RunAction::~RunAction()
{
  delete runM;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4Run* RunAction::GenerateRun()
{
  return new MyRun;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::BeginOfRunAction(const G4Run*)
{
  std::cout << "RunAction::BeginOfRunAction" << std::endl;
  Analysis* myana = Analysis::GetAnalysis();
  myana-> Clear();
  myana->Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::EndOfRunAction(const G4Run*)
{
  std::cout << "RunAction::EndOfRunAction" << std::endl;
   G4int rank = G4MPImanager::GetManager()-> GetRank();
  //NOTE: if only histograms are active actually we do not create the per-thread
  //ntuple file
  std::ostringstream fname;
  fname<<thefilename<<rank<<"-thread"<<G4Threading::G4GetThreadId();
  Analysis* myana = Analysis::GetAnalysis();
  myana-> Save(fname.str());
}
