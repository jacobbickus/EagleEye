#include "MyRun.hh"
#include <atomic>

namespace {
  std::atomic_int g_ctr(0);
}

MyRun::MyRun(): G4Run()
{
  fDummyCounter = g_ctr++;
}

MyRun::~MyRun()
{}

void MyRun::Merge(const G4Run* aRun)
{
  G4Run::Merge(aRun);
  fDummyCounter += static_cast<const MyRun*>(aRun)->fDummyCounter;
}
