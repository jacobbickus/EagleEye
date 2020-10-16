#include "RunMerger.hh"
#include "MyRun.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunMerger::Pack() {
  //Very imporant, here fMyRun is const!
  //Register a user-data in the user Run class with MPI merger
  InputUserData(const_cast<int*>(&(fMyRun->fDummyCounter)),MPI::INT,1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4Run* RunMerger::UnPack() {
  //Create a dummy user-Run, used to contain data received via MPI
  MyRun* aDummyRun = new MyRun;
  OutputUserData(&(aDummyRun->fDummyCounter),MPI::INT,1);
  return aDummyRun;
}

