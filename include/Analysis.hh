#ifndef AnalysisManager_h
#define AnalysisManager_h 1

#include <tools/histo/h1d>
#include "globals.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#if defined (G4ANALYSIS_USE_ROOT)
#include "TFile.h"
#include "TROOT.h"
#include "TH1D.h"
#endif

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)


class Analysis
{
public:
~Analysis();
static Analysis* GetAnalysis();

void Book();
void EndOfRun();
void Save(const G4String& fname);
void Close(G4bool reset = true);
void Update();
void Clear();
void FillIncDet(G4double, G4double);
void FillDetected(G4double, G4double);
void FillIncWater(G4double, G4double);

private:
  Analysis();
  DISALLOW_COPY_AND_ASSIGN(Analysis);
  G4int fIncDetector_hist, fDetected_hist, fIncWater_hist;
  G4double xmax;
  G4String filename;
  #if defined (G4ANALYSIS_USE_ROOT)
  TH1D *hBrems;
  #endif

};

#endif
