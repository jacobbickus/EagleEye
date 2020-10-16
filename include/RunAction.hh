#ifndef RUN_ACTION_H
#define RUN_ACTION_H

#include "G4UserRunAction.hh"
#include "RunMessenger.hh"

class RunMessenger;

class RunAction : public G4UserRunAction {
public:
  RunAction();
  ~RunAction();

  virtual G4Run* GenerateRun();
  virtual void BeginOfRunAction(const G4Run* arun);
  virtual void EndOfRunAction(const G4Run* arun);
  
  void SetFilename(G4String val)
    {
      thefilename = val;
      //std::cout << "Filename set to: " << thefilename << std::endl;
    }

private:
  RunMessenger* runM;
  G4String thefilename;
};

#endif
