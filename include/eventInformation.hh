#ifndef eventInformation_h
#define eventInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4Event.hh"
#include "G4VUserEventInformation.hh"

class eventInformation : public G4VUserEventInformation {
public:
eventInformation();
eventInformation(const G4Event*);
eventInformation(const eventInformation*);
virtual ~eventInformation();

inline G4double GetWeight() const {
        return weight;
}
void SetWeight(G4double);

inline G4double GetBeamEnergy() const {
        return beamEnergy;
}
void SetBeamEnergy(G4double);
void Print() const;
private:
G4double weight;
G4double beamEnergy;
};

#endif
