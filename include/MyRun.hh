#ifndef MyRun_h
#define MyRun_h 1

#include "G4Run.hh"

class MyRun: public G4Run
{
  friend class RunMerger;
public:
  MyRun();
  virtual ~MyRun();
  virtual void Merge(const G4Run*);
  G4int GetCounter() const {return fDummyCounter;}

private:
  G4int fDummyCounter;
};
#endif
