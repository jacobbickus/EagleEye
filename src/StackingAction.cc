#include "StackingAction.hh"


StackingAction::StackingAction()
{
}

StackingAction::~StackingAction()
{}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* currentTrack)
{
  G4ParticleDefinition *pdef = currentTrack->GetDefinition();
  // kill neutrons (probably not important)
  if(pdef == G4Neutron::Definition()) return fKill;
  return fUrgent;
}
