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
#include <string>

G4int seed;
G4int numCores;
G4String macro;

namespace
{
void PrintUsage()
{
   std::cerr << "Usage: " << std::endl;
   std::cerr << "EagleEye [macro] [MasterSeed] [numberCores]" << std::endl;
}
}

int main(int argc,char **argv)
{
  // Defaults
  G4int start_time = time(0);
  G4bool use_xsec_tables = true;
  G4bool use_xsec_integration = true;
  G4bool force_isotropic = false;
  G4bool addNRF = true;
  // Evaluate Arguments
  if ( argc != 4 )
  {
          PrintUsage();
          return 1;
  }

 macro = argv[1];
 seed = atoi(argv[2]);
 numCores = atoi(argv[3]);
  
  G4MPImanager* g4MPI = new G4MPImanager(argc,argv);
  
  g4MPI->SetVerbose(0);
  G4UImanager* UI= G4UImanager::GetUIpointer();
  for(G4int i=0;i<=numCores - 1;i++)
  {
    G4String thecommand = "/mpi/setSeed " + std::to_string(i) + " " + std::to_string(seed +i);
    UI->ApplyCommand(thecommand);
  }

  //UI->ApplyCommand("/mpi/showSeeds 1");
  G4MPIsession* session = g4MPI->GetMPIsession();

  G4RunManager *runManager = new G4RunManager;
  runManager->SetUserInitialization(new DetectorConstruction);
  runManager->SetUserInitialization(new physicsList(addNRF, use_xsec_tables, use_xsec_integration, force_isotropic));
  runManager->SetUserInitialization(new ActionInitialization);

  session->SessionStart();

  G4int stop_time = time(0);
  G4cout << "The MC took:\t\t" << stop_time - start_time << "s" <<G4endl;
  
  delete g4MPI;
  delete runManager;

  return 0;
}
