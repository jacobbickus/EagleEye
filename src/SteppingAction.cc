#include "SteppingAction.hh"
#include "Analysis.hh"

SteppingAction::SteppingAction()
        : G4UserSteppingAction()
{
        fExpectedNextStatus = Undefined;
}

SteppingAction::~SteppingAction()
{
}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{

        // Run Logical Checks
        if(aStep->GetPostStepPoint()==NULL) {
                return; // at the end of the world
        }
        else if(aStep->GetPostStepPoint()->GetPhysicalVolume()==NULL) {
                return;
        }

        G4Track* theTrack = aStep->GetTrack();

        // Run Cuts
        if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().compare(0, 10, "Collimator") == 0)
        {
                // kill photons in collimator
                theTrack->SetTrackStatus(fStopAndKill);
                //event->AddStatusKilled();
        }
        else if(theTrack->GetPosition().z()/(cm) < -10.*cm)
        {
                // Kill photons that go in behind beam origin
                theTrack->SetTrackStatus(fStopAndKill);
                //event->AddStatusKilled();
        }

// *********************** Checks and Cuts Complete ************************ //

        G4int isNRF = 0;
        eventInformation* info = (eventInformation*)(G4RunManager::GetRunManager()->GetCurrentEvent()->GetUserInformation());
        weight = info->GetWeight();
        G4String particleName = aStep->GetTrack()->GetDynamicParticle()
                                ->GetParticleDefinition()->GetParticleName();
        Analysis* manager = Analysis::GetAnalysis();
        if(theTrack->GetCreatorProcess() !=0)
        {
                G4String CPName = theTrack->GetCreatorProcess()->GetProcessName();
                if(CPName == "NRF")
                {
                        isNRF = 1;
                }
        }
        
        // Incident Water Analysis 
        if(aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName().compare(0, 5,"Water") == 0
           && aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().compare(0, 5, "Water") != 0 && isNRF)
        {
                G4double energy_inc_water = theTrack->GetKineticEnergy()/(MeV);
                manager->FillIncWater(energy_inc_water, weight);
        }

        // PMT Analysis

        G4StepPoint* endPoint   = aStep->GetPostStepPoint();
        G4StepPoint* startPoint = aStep->GetPreStepPoint();

        if(endPoint->GetStepStatus() == fGeomBoundary) {

                const G4DynamicParticle* theParticle = theTrack->GetDynamicParticle();

                G4ThreeVector oldMomentumDir = theParticle->GetMomentumDirection();

                G4ThreeVector m0 = startPoint->GetMomentumDirection(); // don't use these yet?
                G4ThreeVector m1 = endPoint->GetMomentumDirection();

                G4OpBoundaryProcessStatus theStatus = Undefined;
                G4ProcessManager* OpManager =
                        G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
                G4int MAXofPostStepLoops =
                        OpManager->GetPostStepProcessVector()->entries();
                G4ProcessVector* postStepDoItVector =
                        OpManager->GetPostStepProcessVector(typeDoIt);

                if(endPoint->GetPhysicalVolume()->GetName().compare(0,2,"PC")==0 && startPoint->GetPhysicalVolume()->GetName().compare(0,2,"PC")!=0) { // first time in photocathode

                        manager->FillIncDet(theParticle->GetKineticEnergy()/(MeV), weight);
                        for (G4int i=0; i<MAXofPostStepLoops; ++i) {
                                G4VProcess* currentProcess = (*postStepDoItVector)[i];

                                G4OpBoundaryProcess* opProc = dynamic_cast<G4OpBoundaryProcess*>(currentProcess);

                                if(opProc) {
                                        theStatus = opProc->GetStatus();
                                        if (theStatus == Detection) {
                                                det_energy = theParticle->GetKineticEnergy()/(MeV);
                                                manager->FillDetected(det_energy, weight);
                                        }
                                } // for if opProc
                        } // for for loop
                } // for if statement if first time in photocathode
        } // for if at boundary
} // end of user steepping action function
