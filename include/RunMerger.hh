#ifndef MPIRUNMERGER_HH
#define MPIRUNMERGER_HH
#include <G4VUserMPIrunMerger.hh>
#include "MyRun.hh"

//Simple class showing how to reduce via MPI user defined run
class RunMerger : public G4VUserMPIrunMerger {
public:
  RunMerger(const MyRun* arun,G4int destination=G4MPImanager::kRANK_MASTER,
      G4int verb=0)
  : G4VUserMPIrunMerger(arun,destination,verb ) , fMyRun(arun) {}
protected:
  void Pack();
  G4Run* UnPack();
private:
  const MyRun* fMyRun;
};
#endif
