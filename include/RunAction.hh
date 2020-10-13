#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4MPImanager.hh"
#include "Analysis.hh"
#include "MyRun.hh"
#include "g4root.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "RunMessenger.hh"

class RunMessenger

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    virtual ~RunAction();
    virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
    void SetFilename(G4String val)
    {
      thefilename = val;
      std::cout << "Filename set to: " << filename << std::endl;
    }
  private:
    G4String thefilename; 
    RunMessenger* runM;
};


#endif
