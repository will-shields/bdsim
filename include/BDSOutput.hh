// Provide interface to output


#ifndef BDSOutput_h
#define BDSOutput_h 

#include "BDSGlobalConstants.hh"
#include "BDSSampler.hh"
#include "BDSSamplerHit.hh"
#include "BDSSamplerSD.hh"
#include "BDSSamplerCylinder.hh"
#include "BDSEnergyCounterHit.hh"

#include "BDSLWCalorimeter.hh"
#include "BDSLWCalorimeterHit.hh"

#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"

#include <fstream>
#include <vector>
// is the root output supported?
#ifdef USE_ROOT
#include "TROOT.h"
#include "TH1F.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TTree.h"
#endif

const G4int _ASCII = 0;
const G4int _ROOT = 1;
const G4int _ASCII_ROOT = 2;


class BDSOutput {

public: 
  BDSOutput(); // default constructor
  BDSOutput(G4int format);

  void SetFormat(G4int format);
  void Init(G4int FileNum);
  ~BDSOutput();

  void WriteHits(BDSSamplerHitsCollection*);
  void WriteEnergyLoss(BDSEnergyCounterHitsCollection*);
  G4int WriteTrajectory(TrajectoryVector* TrajVec);
  G4int WriteTrajectory(std::vector<G4VTrajectory*> TrajVec);

  void Echo(G4String str);

  G4int Commit(); //G4int FileNum);   // close the event
  void Write();           // close the event


  // for root output

#ifdef USE_ROOT
  TFile* theRootOutputFile;
  TTree *theLWCalorimeterTree;

  TH1F *EnergyLossHisto;
  TNtuple *EnergyLossNtuple;
#endif

  G4int nSamplers;
  G4double zMax;
  //BDSSamplerSD* BDSSamplerSensDet;
  std::vector <G4String> SampName;
  std::vector <G4String> CSampName;
private:
  G4int format;
  ofstream of;
  int outputFileNumber;

//#ifdef USE_ROOT
  float x0,xp0,y0,yp0,z0,zp0,E0,t0;
  float x,xp,y,yp,z,zp,E,Edep,t;
  float X,Xp,Y,Yp,Z,Zp,s,weight;
  int part,nev, pID, theID, track_id;
//#endif

};

extern BDSOutput* bdsOutput;
#endif
