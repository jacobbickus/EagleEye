#include "ActionInitialization.hh"


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
  SetUserAction(new RunAction);
}

void ActionInitialization::Build() const
{
  G4String fileName = "brems_distributions.root";
  SetUserAction(new PrimaryGeneratorAction(fileName));
  SetUserAction(new EventAction);
  SetUserAction(new SteppingAction);
  SetUserAction(new StackingAction);
  SetUserAction(new RunAction);
}
