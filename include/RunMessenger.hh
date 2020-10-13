#ifndef RunMessenger_h
#define RunMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"
#include "RunAction.hh"
#include "G4UIdirectory.hh"

class RunAction;
class G4UIcmdWithAString;
class G4UIdirectory;

class RunMessenger: public G4UImessenger
{
public:
  RunMessenger(RunAction*);
  ~RunMessenger();

  void SetNewValue(G4UIcommand*, G4String);
private:
  RunAction* runA;
  G4UIcmdWithAString* Cmd;
  G4UIdirectory *myDir;
};

#endif
