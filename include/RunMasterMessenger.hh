#ifndef RunMasterMessenger_h
#define RunMasterMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"
#include "RunActionMaster.hh"
#include "G4UIdirectory.hh"

class RunActionMaster;
class G4UIcmdWithAString;
class G4UIdirectory;

class RunMasterMessenger: public G4UImessenger
{
public:
  RunMasterMessenger(RunActionMaster*);
  ~RunMasterMessenger();

  void SetNewValue(G4UIcommand*, G4String);
private:
  RunActionMaster* runA;
  G4UIcmdWithAString* Cmd;
  G4UIdirectory *myDir;
};

#endif
