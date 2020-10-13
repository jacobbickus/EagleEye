#ifndef MyRootFileReader_h
#define MyRootFileReader_h 1

#include <iostream>
#include <cstring>
#if defined (G4ANALYSIS_USE_ROOT)
#include "TFile.h"
#include "TROOT.h"
#include "TH1D.h"
#include "TRandom1.h"
#endif

class MyRootFileReader
{
public:
  MyRootFileReader(std::string fileName);
  ~MyRootFileReader();
  double GetAnEvent();
  double GettheWeight(double);

private:
  #if defined (G4ANALYSIS_USE_ROOT)
  TRandom1 Random;
  TH1D *hBrems;
  TH1D *hSample;
  #endif
};


#endif
