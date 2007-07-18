/** BDSIM, v0.1   

Last modified 01.02.2006 by Ilya Agapov

**/

const int DEBUG = 0;

#include "BDSGlobalConstants.hh"

#include "G4UniformMagField.hh"

#include <cstdlib>

#include <assert.h>

#include<string>
#include<stack>

using namespace std;

G4double NumSpoilerRadLen=1.;

namespace 
{

BDSGlobalConstants::BDSGlobalConstants(struct Options& opt)
{

  // defaults:
  itsEnergyOffset=0.;
  itsTrackWeightFactor=1.0;
  
  if(opt.physicsList == "") 
    itsPhysListName = "standard";
  else
    itsPhysListName = opt.physicsList;

  if(opt.pipeMaterial == "") 
    itsPipeMaterial = "Vacuum";
  else
    itsPipeMaterial = opt.pipeMaterial;
  
  
  tmpParticleName = G4String(opt.particleName);
  
      
  itsBeamTotalEnergy = opt.beamEnergy * GeV;

	      
  itsBackgroundScaleFactor = opt.backgroundScaleFactor;

  itsComponentBoxSize = opt.componentBoxSize *m;

  itsTunnelRadius = opt.tunnelRadius * m;

  itsBeampipeRadius = opt.beampipeRadius * m;
  
  itsBeampipeThickness = opt.beampipeThickness * m;
    
  itsThresholdCutCharged = opt.thresholdCutCharged * GeV;

  itsThresholdCutPhotons = opt.thresholdCutPhotons * GeV;

  itsTrackWeightFactor = opt.trackWeightFactor;

  itsProdCutPhotons = opt.prodCutPhotons * m;
  itsProdCutPhotonsP = opt.prodCutPhotonsP * m;
  itsProdCutElectrons = opt.prodCutElectrons * m;
  itsProdCutElectronsP = opt.prodCutElectronsP * m;
  itsProdCutPositrons = opt.prodCutPositrons * m;
  itsProdCutPositronsP = opt.prodCutPositronsP * m;


  itsDeltaChord = opt.deltaChord * m;

  itsChordStepMinimum = opt.chordStepMinimum * m;

  itsDeltaIntersection= opt.deltaIntersection * m;
  
  itsMinimumEpsilonStep = opt.minimumEpsilonStep;
  itsMaximumEpsilonStep = opt.maximumEpsilonStep;
  itsDeltaOneStep = opt.deltaOneStep * m;


  doTwiss = opt.doTwiss;

  itsTurnOnInteractions = opt.turnOnInteractions;

  itsUseLowEMPhysics = opt.useLowEMPhysics;

  itsSynchRadOn = opt.synchRadOn;

  itsSynchRescale = opt.synchRescale; // rescale due to synchrotron

  itsSynchTrackPhotons= opt.synchTrackPhotons;

  itsSynchPrimaryGen = 0;

  itsSynchLowX = opt.synchLowX;

  itsSynchLowGamE = opt.synchLowGamE * GeV;  // lowest gamma energy

  itsSynchPhotonMultiplicity = opt.synchPhotonMultiplicity;

  itsSynchMeanFreeFactor = opt.synchMeanFreeFactor;

  itsPlanckOn = opt.planckOn;

  itsBDSeBremOn = opt.eBremOn;

  itsLengthSafety = opt.lengthSafety * m;

  itsNumberOfParticles = opt.numberOfParticles;

  itsNumberToGenerate = opt.numberToGenerate;

  itsNumberOfEventsPerNtuple = opt.numberOfEventsPerNtuple;
  
  itsEventNumberOffset = opt.eventNumberOffset;
  
  itsRandomSeed = opt.randomSeed;
 
  if(opt.useTimer) {
    itsTimer=new G4Timer();
    itsUseTimer = opt.useTimer;
  }  

  itsUseEMHadronic = opt.useEMHadronic;

  itsUseMuonPairProduction = opt.useMuonPairProduction;

  itsMuonProductionScaleFactor = opt.muonProductionScaleFactor;

  itsHadronInelasticScaleFactor = opt.hadronInelasticScaleFactor;

  itsStoreMuonTrajectories = opt.storeMuonTrajectories;
  itsStoreNeutronTrajectories = opt.storeNeutronTrajectories;
  itsStoreTrajectory = opt.storeTrajectory;

  //G4cout<<"STOREA TRAJ = "<< itsStoreTrajectory<<G4endl;

  itsUseMuonShowers = opt.useMuonShowers;

  itsMuonLowestGeneratedEnergy = opt.muonLowestGeneratedEnergy * GeV;

  stopTracks = opt.stopTracks; 

  // tmp - parameters of the laserwire process - to be given in the element attributes

  itsLaserwireWavelength = 0.4 * micrometer;
  itsLaserwireDir = G4ThreeVector(1,0,0);
  itsLaserwireTrackPhotons = 1;
  itsLaserwireTrackElectrons = 1;


  isWaitingForDump = false;
  isDumping = false;
  isReading = false;
  isReadFromStack = false;

  itsFifo = opt.fifo;

 //  else if(name=="INCLUDE_IRON_MAG_FIELDS")
//     {
//       _READ(itsIncludeIronMagFields);
//     }


//  else if(name  =="OUTPUT_NTUPLE_FILE_NAME")
//     {_READ(itsOutputNtupleFileName);}

//   else if(name=="NUMBER_OF_EVENTS_PER_NTUPLE")
//     {_READ(itsNumberOfEventsPerNtuple);}


//   else if(name=="USE_SYNCH_PRIMARY_GEN")
//     {
//       _READ(itsSynchPrimaryGen);
//     }
//   else if(name=="SYNCH_PRIMARY_GEN_ANGLE")
//     {
//       _READ(itsSynchPrimaryAngle);
//       itsSynchPrimaryAngle*=radian;
//     }
//   else if(name=="SYNCH_PRIMARY_GEN_LENGTH")
//     {
//       _READ(itsSynchPrimaryLength);
//       itsSynchPrimaryLength*=m;
//     }

//   else if(name=="USE_LAST_MATERIAL_POINT")
//     {
//       _READ(itsUseLastMaterialPoint);
//     }

//   else
//     {
//       G4cout<<" Unknown card in BDSInput.cards:"<<name<<G4endl;
//       G4Exception("BDSGlobalConstants: UNRECOGNISED CARD");
//     } 
 

  itsOutputNtupleFileName="sampler_output.rz";
  // end of defaults

  
  // default value (can be renamed later)

  G4UniformMagField* magField = new G4UniformMagField(G4ThreeVector());
  itsZeroFieldManager=new G4FieldManager();
  itsZeroFieldManager->SetDetectorField(magField);
  itsZeroFieldManager->CreateChordFinder(magField);
   
}


// a robust compiler-invariant method to convert from integer to G4String
G4String BDSGlobalConstants::StringFromInt(G4int N) 
{
  if (N==0) return "0";
 
  G4int nLocal=N, nDigit=0, nMax=1;

  do { nDigit++;
      nMax*=10;} while(N > nMax-1);
  
  nMax/=10;
  G4String Cnum;
  do {Cnum+=StringFromDigit(nLocal/nMax);
      nLocal-= nLocal/nMax * nMax;
      nMax/=10;}   while(nMax>1);
  if(nMax!=0)Cnum+=StringFromDigit(nLocal/nMax);

  return Cnum;
}

// a robust compiler-invariant method to convert from digit to G4String
G4String BDSGlobalConstants::StringFromDigit(G4int N) 
{
  if(N<0 || N>9)
    G4Exception("Invalid Digit in BDSGlobalConstants::StringFromDigit");

  G4String Cnum;

  if(N==0)Cnum="0";
  else if(N==1)Cnum="1";
  else if(N==2)Cnum="2";
  else if(N==3)Cnum="3";
  else if(N==4)Cnum="4";
  else if(N==5)Cnum="5";
  else if(N==6)Cnum="6";
  else if(N==7)Cnum="7";
  else if(N==8)Cnum="8";
  else if(N==9)Cnum="9"; 

  return Cnum;
}

  BDSGlobalConstants::~BDSGlobalConstants()
  {  
    
  }

}
