// Always include
#include "G4RunManager.hh"

#include "G4MPImanager.hh"
#include "G4MPIsession.hh"
#include "G4UImanager.hh"

#include "DetectorConstruction.hh"
#include "physicsList.hh"
#include "ActionInitialization.hh"
// Typcially include
#include "time.h"
#include "Randomize.hh"
#include <iostream>
#include "G4Types.hh"
#include <stdio.h>

// declare global variables
//G4long seed;

int main(int argc,char **argv)
{
  // Defaults
  G4int start_time = time(0);
  G4bool use_xsec_tables = true;
  G4bool use_xsec_integration = true;
  G4bool force_isotropic = false;
  G4bool addNRF = true;
  //seed = 1;

  G4MPImanager* g4MPI = new G4MPImanager(argc,argv);
  g4MPI->SetVerbose(0);
  G4MPIsession* session = g4MPI->GetMPIsession();
  G4String prompt= " [40;01;33m";
  prompt+= "G4MPI";
  prompt+= " [40;31m(%s) [40;36m[%/] [00;30m:";
  session-> SetPrompt(prompt);

        // choose the Random engine
  //CLHEP::HepRandom::setTheEngine(new CLHEP::RanluxEngine);
  //CLHEP::HepRandom::setTheSeed(seed);

  G4RunManager *runManager = new G4RunManager;
  runManager->SetUserInitialization(new DetectorConstruction);
  runManager->SetUserInitialization(new physicsList(addNRF, use_xsec_tables, use_xsec_integration, force_isotropic));
  runManager->SetUserInitialization(new ActionInitialization);

  //runManager->Initialize();

  session->SessionStart();

  G4int stop_time = time(0);
  G4cout << "The MC took:\t\t" << stop_time - start_time << "s" <<G4endl;
  
  delete g4MPI;
  delete runManager;

  return 0;
}
