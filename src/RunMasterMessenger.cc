#include "RunMasterMessenger.hh"


RunMasterMessenger::RunMasterMessenger(RunActionMaster* run)
:runA(run)
{
  myDir = new G4UIdirectory("/myrun/");
  myDir->SetGuidance("Run Commands");
  Cmd = new G4UIcmdWithAString("/myrun/filename",this);
  Cmd->SetGuidance("Choose Desired Run Output filename");
  Cmd->SetParameterName("filename",false);
  Cmd->SetDefaultValue("Test");

}

RunMasterMessenger::~RunMasterMessenger()
{
  delete Cmd;
}


void RunMasterMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if(command == Cmd)
  {
    G4String theCommand = newValue;
    runA->SetFilename(theCommand);
  }
  else
  {
    G4cerr << "ERROR RunMasterMessenger:: SetStepInputValue command != Cmd" << G4endl;
  }
}
