#include "PrimaryGeneratorAction.hh"
namespace {G4Mutex myHEPPrimGenMutex = G4MUTEX_INITIALIZER;}
MyRootFileReader* PrimaryGeneratorAction::fileReader = 0;

PrimaryGeneratorAction::PrimaryGeneratorAction(G4String fileName) : G4VUserPrimaryGeneratorAction(),
        chosen_energy(-1), genM(NULL), fParticleGun(0)
{

        genM = new PrimaryGenActionMessenger(this);
        G4AutoLock lock(&myHEPPrimGenMutex);
        if(!fileReader)
        {
          fileReader = new MyRootFileReader(fileName);
        }
        fParticleGun = new G4ParticleGun(1);
        // Default Kinematics
        fParticleGun->SetParticleDefinition(G4Gamma::Definition());
        fParticleGun->SetParticleTime(0.0*ns);

}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
        delete fParticleGun;
        delete genM;
        G4AutoLock lock(&myHEPPrimGenMutex);
        if(fileReader) {delete fileReader; fileReader = 0;}

}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4double w = 1.0;

  if(fileReader)
  {
    G4AutoLock lock(&myHEPPrimGenMutex);
    energy = fileReader->GetAnEvent();
    w = fileReader->GettheWeight(energy);
  }

  if(chosen_energy > 0)
  {
    energy = chosen_energy;
  }
  energy = energy*MeV;
  fParticleGun->SetParticleEnergy(energy);


  const float pi=acos(-1);

  // Set beam position
  G4double beam_size = 1.3*mm;
  G4double r = beam_size*acos(G4UniformRand())/pi*2.;
  G4double ph = 360.*G4UniformRand()*CLHEP::deg;
  G4double x_r = r*cos(ph);
  G4double y_r = r*sin(ph);
  fParticleGun->SetParticlePosition(G4ThreeVector(x_r,y_r,0));

  // Set beam momentum
  G4double theta = 0.*CLHEP::deg;
  G4double phi = 0.*CLHEP::deg;
  G4ThreeVector vDir = G4ThreeVector(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
  fParticleGun->SetParticleMomentumDirection(vDir);

  fParticleGun->GeneratePrimaryVertex(anEvent);

// Pass the event information
  eventInformation *anInfo = new eventInformation(anEvent);
  anInfo->SetWeight(w);
  anInfo->SetBeamEnergy(energy);
  anEvent->SetUserInformation(anInfo);

}
