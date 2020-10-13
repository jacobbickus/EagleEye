#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "Analysis.hh"
#include "PrimaryGenActionMessenger.hh"
#include "globals.hh"
#include <vector>
#include "G4ParticleGun.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "eventInformation.hh"
#include "MyRootFileReader.hh"
#include "G4AutoLock.hh"

class G4Event;
class PrimaryGenActionMessenger;
class MyRootFileReader;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{

public:
PrimaryGeneratorAction(G4String fileName);
virtual ~PrimaryGeneratorAction();
virtual void GeneratePrimaries(G4Event*);
/*
G4ParticleGun* GetParticleGun(){
        return fParticleGun;
};
*/
void SetEnergyValue(G4double val){
        chosen_energy = val;
}

private:
G4double chosen_energy;
PrimaryGenActionMessenger* genM;
G4ParticleGun* fParticleGun;
static MyRootFileReader* fileReader;
G4double energy;

};


#endif
