#ifndef RUN_ACTION_H
#define RUN_ACTION_H

#include "G4UserRunAction.hh"

class RunAction : public G4UserRunAction {
public:
  RunAction();
  ~RunAction();

  virtual G4Run* GenerateRun();
  virtual void BeginOfRunAction(const G4Run* arun);
  virtual void EndOfRunAction(const G4Run* arun);
};

#endif