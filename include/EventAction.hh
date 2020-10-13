#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"

class EventAction : public G4UserEventAction
{
public:
EventAction();
~EventAction();

public:
void BeginOfEventAction(const G4Event* anEvent);
void EndOfEventAction(const G4Event* anEvent);

};

#endif
