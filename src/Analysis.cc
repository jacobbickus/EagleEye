#include "Analysis.hh"
#include "G4AutoDelete.hh"
#include "g4root.hh"
#include "G4SystemOfUnits.hh"

G4ThreadLocal Analysis* the_analysis = 0;

Analysis* Analysis::GetAnalysis()
{
  if(!the_analysis)
  {
    the_analysis = new Analysis();
    G4AutoDelete::Register(the_analysis);
  }
  return the_analysis;
}
Analysis::Analysis(): fIncDetector_hist(0), fDetected_hist(0)
{}

Analysis::~Analysis()
{}

void Analysis::Book()
{
    G4AnalysisManager* manager = G4AnalysisManager::Instance();
  #if defined (G4ANALYSIS_USE_ROOT)

        TFile *fin = TFile::Open("brems_distributions.root");
        hBrems  = (TH1D*) fin->Get("hBrems");
        if (hBrems)
        {
            xmax = hBrems->GetXaxis()->GetXmax();
            fin->Close();
        }
        else
        {
                std::cerr << "Error reading from file " << fin->GetName() << std::endl;
                exit(1);
        }
    #else
        xmax = 5; // this is really just a placeholder
    #endif
    manager->SetVerboseLevel(0);
    G4int nbins = 10000;
    fIncDetector_hist = manager->CreateH1("IncDet", "Incident Detector Data", nbins, 0., xmax/2, "MeV");
    fDetected_hist = manager->CreateH1("Detected", "Detected Data", nbins, 0., 1E-3, "MeV");
}

void Analysis::Update()
{
  return;
}

void Analysis::Clear()
{
  return;
}

void Analysis::Save(const G4String& fname)
{
  G4AnalysisManager* manager = G4AnalysisManager::Instance();
  manager->OpenFile(fname.c_str());
  manager->Write();
  return;
}

void Analysis::Close(G4bool reset)
{
  G4AnalysisManager *manager = G4AnalysisManager::Instance();
  manager->CloseFile(reset);
  return;
}

void Analysis::FillIncDet(G4double E_incident, G4double weight)
{
  G4AnalysisManager*mgr = G4AnalysisManager::Instance();
  mgr->FillH1(fIncDetector_hist, E_incident, weight);
}

void Analysis::FillDetected(G4double Energy, G4double weight)
{
  G4AnalysisManager*mgr = G4AnalysisManager::Instance();
  mgr->FillH1(fDetected_hist, Energy, weight);
}
