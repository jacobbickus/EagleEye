#include "eventInformation.hh"
#include "G4ios.hh"

eventInformation::eventInformation() {
        weight = 0.;
        beamEnergy = 0.;
}

eventInformation::eventInformation(const G4Event*) {
        weight = 0.; //aTrack->GetWeight();
        beamEnergy = 0.;
}

eventInformation::eventInformation(const eventInformation* anEventInfo) {
        weight = anEventInfo->GetWeight();
        beamEnergy = anEventInfo->GetBeamEnergy();
}

eventInformation::~eventInformation() {
        ;
}

void eventInformation::SetWeight(G4double x) {
        weight = x;
}

void eventInformation::SetBeamEnergy(G4double x) {
        beamEnergy = x;
}

void eventInformation::Print() const
{;}
