#include "MyRootFileReader.hh"

MyRootFileReader::MyRootFileReader(std::string fileName)
{
  // file contains the normalized brems distribution p(E), sampling distribution s(E),
  // and binary 0/1 for off/on resonance useful in weighting
  const char* cfileName = fileName.c_str();
  TFile *fin = TFile::Open(cfileName);
  hBrems  = (TH1D*) fin->Get("hBrems");
  hSample = (TH1D*) fin->Get("hSample");
  if (hBrems && hSample)
  {
    //std::cout << "Imported brems and sampling distributions from " << fin->GetName() << std::endl << std::endl;
  }
  else
  {
    std::cerr << "Error reading from file " << fin->GetName() << std::endl;
    exit(1);
  }
}

MyRootFileReader::~MyRootFileReader()
{}

double MyRootFileReader::GetAnEvent()
{
  return hSample->GetRandom(); // sample the resonances specified by hSample
}

double MyRootFileReader::GettheWeight(double energy)
{
  double s = hSample->GetBinContent(hSample->GetXaxis()->FindBin(energy));
  double dNdE = hBrems->GetBinContent(hBrems->GetXaxis()->FindBin(energy));
  return dNdE/s;
}
