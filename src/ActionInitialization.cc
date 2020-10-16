#include "ActionInitialization.hh"
#include "G4Threading.hh"
#include "RunAction.hh"
#include "RunActionMaster.hh"


ActionInitialization::ActionInitialization()
        : G4VUserActionInitialization()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::~ActionInitialization()
{
}

void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new RunActionMaster);
}

void ActionInitialization::Build() const
{
  G4String fileName = "brems_distributions.root";
  SetUserAction(new PrimaryGeneratorAction(fileName));
  SetUserAction(new EventAction);
  SetUserAction(new SteppingAction);
  SetUserAction(new StackingAction);
  if(G4Threading::IsMultithreadedApplication())
  {
    SetUserAction(new RunAction);
  }
  else
  {
    SetUserAction(new RunActionMaster);
  }
}
