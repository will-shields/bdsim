/** BDSIM, v0.4

Last modified 30.10.2007 by Steve Malton

**/

//==============================================================

#ifndef BDSGlobalConstants_h
#define BDSGlobalConstants_h 

#include "G4Timer.hh" // This must be the first in the include list

#include <fstream>
#include <string>
#include <set>
#include <list>
#include <deque>

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "G4ThreeVector.hh"
#include "globals.hh"
#include "G4FieldManager.hh"
#include "G4String.hh"
#include "G4AffineTransform.hh"
//#include "G4Track.hh"

#include "parser/gmad.h"

using std::istream;
using std::ostream;
using std::ifstream;
using std::ofstream;
using std::list;

struct strCmp {
  G4bool operator()( const G4String s1, const G4String s2 ) {
    return  strcmp(s1,s2) < 0;}
};

struct tmpParticle {
  G4double E;
  G4double x;
  G4double y;
  G4double z;
  G4double t;
  G4double xp;
  G4double yp;
  G4double zp;
};

/*
struct trackSort : public binary_function<G4Track*, G4Track*, G4bool>{
  G4bool operator()(G4Track* a, G4Track* b){
  return a->GetPosition().z() < b->GetPosition().z();}
};
*/

class BDSGlobalConstants 
{
public:
 
  BDSGlobalConstants(struct Options&);

  ~BDSGlobalConstants();
  

  G4bool DoTwiss();
  void SetDoTwiss(G4bool val); 

  G4double GetMinimumEpsilonStep();
  G4double GetMaximumEpsilonStep();
  G4double GetDeltaOneStep();

  void SetLogFile(ofstream & os);
  void StripHeader(istream& is);

  G4String StringFromInt(G4int anInt);
  G4String StringFromDigit(G4int anInt);

  G4ParticleDefinition* GetParticleDefinition();
  void SetParticleDefinition(G4ParticleDefinition* aBeamParticleDefinition);

  G4double GetBeamKineticEnergy();
  void SetBeamKineticEnergy(G4double val);

  G4double GetBeamTotalEnergy();
  void SetBeamTotalEnergy(G4double val);

  G4double GetBeamMomentum();
  void SetBeamMomentum(G4double val);
  
  G4double GetBackgroundScaleFactor();

  G4String GetOutputNtupleFileName();
 
  //  void SetWorldSizeZ(G4double WorldSizeZ);
  //G4double GetWorldSizeZ();

  void SetTotalS(G4double TotalS);
  G4double GetTotalS();  

 //  void SetVerticalComponentOffset(G4double VerticalComponentOffset);
//   void SetHorizontalComponentOffset(G4double HorizontalComponentOffset);
//   void AddVerticalComponentOffset(G4double VerticalComponentOffset);
//   void AddHorizontalComponentOffset(G4double HorizontalComponentOffset);
//   G4double GetVerticalComponentOffset(); 
//   G4double GetHorizontalComponentOffset(); 

  G4double GetComponentBoxSize();
  G4double GetMagnetPoleSize();
  G4double GetMagnetPoleRadius();

  G4double GetTunnelRadius(); 
  //G4double GetHorizontalBeamlineOffset(); 
  //G4double GetVerticalBeamlineOffset(); 

  G4double GetBeampipeRadius(); 
  G4double GetBeampipeThickness(); 

  G4double GetDeltaIntersection();
  G4double GetDeltaChord();

  G4double GetChordStepMinimum();

  G4double GetThresholdCutCharged();
  G4double GetThresholdCutPhotons();
  G4double GetTrackWeightFactor();

  G4double GetProdCutPhotons();
  G4double GetProdCutPhotonsP();
  G4double GetProdCutElectrons();
  G4double GetProdCutElectronsP();
  G4double GetProdCutPositrons();
  G4double GetProdCutPositronsP();


  // physical processes etc.

  G4String GetPhysListName();
  void SetPhysListName(G4String val);

  G4bool GetUseLowEMPhysics();

  G4bool GetSynchRadOn();
  G4bool GetSynchRescale();
  void SetSynchRescale(G4bool srRescale);
  G4bool GetSynchTrackPhotons();
  void SetSynchTrackPhotons(G4bool srTrackPhotons);
  G4double GetSynchLowX();
  G4double GetSynchLowGamE();
  G4int GetSynchPhotonMultiplicity();
  G4int GetSynchMeanFreeFactor();

  G4bool GetPlanckOn();
  G4bool GetBDSeBremOn();

  G4double GetLaserwireWavelength();
  G4ThreeVector GetLaserwireDir();

  //use map to generate multiple laserwires with independent wavelength 
  //and direction
  G4double GetLaserwireWavelength(G4String aName); 
  G4ThreeVector GetLaserwireDir(G4String aName); 
  void SetLaserwireWavelength(G4String aName, G4double aWavelength);
  void SetLaserwireDir(G4String aName, G4ThreeVector aDirection);

  G4bool GetLaserwireTrackPhotons();
  G4bool GetLaserwireTrackElectrons();

  // G4bool GetReadBunchFile();
  // G4bool GetWriteBunchFile();
  //G4bool GetExtractBunchFile();

  G4bool GetTurnOnInteractions();
  G4bool GetUseTimer();
  G4bool GetUseEMHadronic();
  G4bool GetUseMuonPairProduction();

  G4bool GetStoreMuonTrajectories();
  G4bool GetStoreNeutronTrajectories();
  G4bool GetStoreTrajectory();
  G4bool GetUseMuonShowers();

  G4bool GetIncludeIronMagFields();


  G4bool GetStopTracks();
  void SetStopTracks(G4bool);
  G4bool stopTracks; // kill tracks after interactions

  // gab Dec04
  G4double GetMuonLowestGeneratedEnergy();
  void SetMuonEnergyCutScaleFactor(G4double aScaleFactor);
  G4double GetMuonEnergyCutScaleFactor();

  G4bool GetUseHaloRadius();
  G4double GetHaloInnerRadius();
  G4double GetHaloOuterRadius();

  G4double GetInnerHaloX();
  G4double GetOuterHaloX();
  G4double GetInnerHaloY();
  G4double GetOuterHaloY();

  G4double GetLengthSafety();
  G4double GetEnergyOffset();
  G4double GetMuonProductionScaleFactor();
  G4double GetHadronInelasticScaleFactor();

  G4Timer* GetTimer();

  ofstream GetEventOutput();

  G4long GetRandomSeed();
  G4bool GetUseBatch();
  G4int GetNumberToGenerate();

  G4int GetNumberOfEventsPerNtuple();

  G4int GetEventNumberOffset();

  // Internally transmitted variables:
  void SetGlobalBeamlineRotationY(G4double aYRotation);
  G4double GetGlobalBeamlineRotationY();

  G4FieldManager* GetZeroFieldManager();

  G4bool GetUseBeamGasPlug();
  G4double GetBeamGasPlugZ();

  G4bool GetUseSynchPrimaryGen();
  G4double GetSynchPrimaryAngle();
  G4double GetSynchPrimaryLength();

  G4bool GetUseLastMaterialPoint();

  G4double GetWedgeDisplacement();
  void SetWedgeDisplacement(G4double WedgeDisplacemnt);
  G4bool GetPreviousWasWedge();
  void SetPreviousWasWedge(G4bool PreviousWasWedge);

  // AI : for placet synchronization
  void setWaitingForDump(G4bool flag);
  G4bool getWaitingForDump();

  void setDumping(G4bool flag);
  G4bool getDumping();

  void setReading(G4bool flag);
  G4bool getReading();

  void setReadFromStack(G4bool flag);
  G4bool getReadFromStack();

  G4String GetFifo();
  void SetFifo(G4String fileName);

  G4AffineTransform GetDumpTransform();
  void SetDumpTransform(G4AffineTransform tf);

  G4String GetRefVolume();
  G4int GetRefCopyNo();

  const G4AffineTransform* GetRefTransform();
  void SetRefTransform(G4AffineTransform& aTransform);

  // SPM : temp filestream for placet to read and write
  ofstream fileDump;
  // ifstream fileRead; replaced with FILE* fifo in code for consistency with Placet. SPM

  G4String tmpParticleName; // particle name as given in options
                            // since the particle definition is looked up in 
                            // PhysicsList we need to store the name first

  std::deque<tmpParticle> holdingQueue;
  std::deque<tmpParticle> outputQueue;
  std::deque<G4double> referenceQueue;
  G4bool isReference;

protected:
private:
  // Data Members for Class Attributes
  ifstream ifs;
  ostream* log;

  // initial bunch parameters

  G4ParticleDefinition* itsBeamParticleDefinition;

  G4double itsBeamTotalEnergy;

  G4double itsNumberOfParticles;

  G4double itsBeamMomentum, itsBeamKineticEnergy;

  G4double itsBackgroundScaleFactor;

  G4double itsMinimumEpsilonStep;
  G4double itsMaximumEpsilonStep;
  G4double itsDeltaOneStep;

  G4double itsComponentBoxSize;
  G4double itsMagnetPoleSize;
  G4double itsMagnetPoleRadius;

  G4double itsTunnelRadius;
  //G4double itsHorizontalBeamlineOffset;
  //G4double itsVerticalBeamlineOffset;
 
  //G4double itsVerticalComponentOffset;
  //G4double itsHorizontalComponentOffset;

  G4double itsBeampipeRadius; 
  G4double itsBeampipeThickness; 
  //G4double itsWorldSizeZ; 
  G4double itsTotalS;

  G4double itsDeltaIntersection;
  G4double itsDeltaChord;
 
  G4double itsChordStepMinimum;

  G4double itsThresholdCutCharged;
  G4double itsThresholdCutPhotons;

  G4double itsProdCutPhotons;
  G4double itsProdCutPhotonsP;
  G4double itsProdCutElectrons;
  G4double itsProdCutElectronsP;
  G4double itsProdCutPositrons;
  G4double itsProdCutPositronsP;

  G4String itsPhysListName;

  G4bool itsUseLowEMPhysics;

  G4bool itsSynchRadOn;
  G4bool itsSynchRescale;
  G4bool itsSynchTrackPhotons;
  G4double itsSynchLowX;
  G4double itsSynchLowGamE;
  G4int itsSynchMeanFreeFactor;
  G4int itsSynchPhotonMultiplicity;

  G4bool itsPlanckOn;
  G4bool itsBDSeBremOn;

  // test map container for laserwire parameters - Steve
  std::map<const G4String, G4double, strCmp> lwWavelength;
  std::map<const G4String, G4ThreeVector, strCmp> lwDirection;

  G4double itsLaserwireWavelength;
  G4ThreeVector itsLaserwireDir;

  G4bool itsLaserwireTrackPhotons;
  G4bool itsLaserwireTrackElectrons;
  G4bool itsTurnOnInteractions;

  G4bool doTwiss;

  //  G4bool itsReadBunchFile;
  // G4bool itsWriteBunchFile;
  // G4bool itsExtractBunchFile;
  G4bool itsUseTimer;
  G4bool itsUseEMHadronic;
  G4bool itsUseMuonPairProduction;
  G4bool itsStoreMuonTrajectories;
  G4bool itsUseMuonShowers;
  G4double itsMuonLowestGeneratedEnergy;
  G4double itsMuonEnergyCutScaleFactor;

  G4bool itsStoreTrajectory;
  G4bool itsStoreNeutronTrajectories;
  G4bool itsIncludeIronMagFields;

  G4double itsLengthSafety;
  G4double itsTrackWeightFactor;
  G4double itsMuonProductionScaleFactor;
  G4double itsHadronInelasticScaleFactor;

  G4Timer* itsTimer;

  G4double itsEnergyOffset;

  G4bool itsUseHaloRadius;
  G4double itsHaloInnerRadius;
  G4double itsHaloOuterRadius;

  G4double itsInnerHaloX;
  G4double itsOuterHaloX;
  G4double itsInnerHaloY;
  G4double itsOuterHaloY;

  G4bool itsUseBatch;
  G4long itsRandomSeed;
  G4int itsNumberToGenerate;

  G4String itsOutputNtupleFileName;
  G4int itsNumberOfEventsPerNtuple;
  G4int itsEventNumberOffset;

  // Internally used variables:
  G4double GlobalBeamlineRotationY;

  G4FieldManager* itsZeroFieldManager;

public:
  G4double GetLWCalWidth();
  G4double GetLWCalOffset();
  //G4String GetLWCalMaterial();

  G4String GetPipeMaterialName();
  G4String GetVacMaterialName();

private:
  G4double itsLWCalWidth;
  G4double itsLWCalOffset;
  //G4String itsLWCalMaterial;
  G4String itsPipeMaterial; //beampipe material
  G4String itsVacMaterial;  //vacuum inside beampipe

  G4bool itsUseBeamGasPlug;
  G4double itsBeamGasPlugZ;

  G4bool itsSynchPrimaryGen;
  G4double itsSynchPrimaryAngle;
  G4double itsSynchPrimaryLength;

  G4bool itsUseLastMaterialPoint;

  G4double itsWedgeDisplacement;
  G4bool itsPreviousWasWedge;

  G4bool isWaitingForDump;
  G4bool isDumping;
  G4bool isReading;
  G4bool isReadFromStack;

  G4String itsFifo; // fifo for BDSIM-placet
  G4AffineTransform itsDumpTransform; //transform of frame from start to current dump element

  G4String itsRefVolume;
  G4int itsRefCopyNo;
  G4AffineTransform itsRefTransform;
};

inline G4double BDSGlobalConstants::GetMinimumEpsilonStep()
{
  return itsMinimumEpsilonStep;
}

inline G4double BDSGlobalConstants::GetMaximumEpsilonStep()
{
  return itsMaximumEpsilonStep;
}

inline G4double BDSGlobalConstants::GetDeltaOneStep()
{
  return itsDeltaOneStep;
}

inline void BDSGlobalConstants::SetLogFile(ofstream & os)
{
  log=&os;
}


inline G4double BDSGlobalConstants::GetBeamKineticEnergy()
{
  return itsBeamKineticEnergy;
}

inline void BDSGlobalConstants::SetBeamKineticEnergy(G4double val)
{
  itsBeamKineticEnergy = val;
}

inline G4double BDSGlobalConstants::GetBeamTotalEnergy()
{
  return itsBeamTotalEnergy;
}

inline void BDSGlobalConstants::SetBeamTotalEnergy(G4double val)
{
  itsBeamTotalEnergy = val;
}


inline G4double BDSGlobalConstants::GetBeamMomentum()
{
  return itsBeamMomentum;
}

inline void BDSGlobalConstants::SetBeamMomentum(G4double val)
{
  itsBeamMomentum = val;
}


inline G4ParticleDefinition* BDSGlobalConstants::GetParticleDefinition()
{
  return itsBeamParticleDefinition;
}

inline void BDSGlobalConstants::SetParticleDefinition(G4ParticleDefinition* aBeamParticleDefinition)
{
  itsBeamParticleDefinition = aBeamParticleDefinition;
}

//inline G4double BDSGlobalConstants::GetSigmaT()
//{
//  return itsSigmaT;
//}

inline G4double BDSGlobalConstants::GetBackgroundScaleFactor()
{return itsBackgroundScaleFactor;}

inline G4double BDSGlobalConstants::GetComponentBoxSize()
{return itsComponentBoxSize;}

inline G4double BDSGlobalConstants::GetMagnetPoleSize()
{return itsMagnetPoleSize;}

inline G4double BDSGlobalConstants::GetMagnetPoleRadius()
{return itsMagnetPoleRadius;}

inline G4double BDSGlobalConstants::GetTunnelRadius()
{return itsTunnelRadius;}

// inline  G4double BDSGlobalConstants::GetHorizontalBeamlineOffset()
// {return itsHorizontalBeamlineOffset;}
// inline  G4double BDSGlobalConstants::GetVerticalBeamlineOffset()
// {return itsVerticalBeamlineOffset;}



inline G4double BDSGlobalConstants::GetBeampipeRadius() 
{return itsBeampipeRadius;}

inline G4double BDSGlobalConstants::GetBeampipeThickness() 
{return itsBeampipeThickness;}

inline G4double BDSGlobalConstants::GetDeltaChord() 
{return itsDeltaChord;}

inline G4double BDSGlobalConstants::GetDeltaIntersection() 
{return itsDeltaIntersection;}

inline G4double BDSGlobalConstants::GetChordStepMinimum() 
{return itsChordStepMinimum;}

inline G4double BDSGlobalConstants::GetThresholdCutCharged() 
{return itsThresholdCutCharged;}
inline G4double BDSGlobalConstants::GetThresholdCutPhotons() 
{return itsThresholdCutPhotons;}

inline G4double BDSGlobalConstants::GetProdCutPhotons() 
{return itsProdCutPhotons;}
inline G4double BDSGlobalConstants::GetProdCutPhotonsP() 
{return itsProdCutPhotonsP;}
inline G4double BDSGlobalConstants::GetProdCutElectrons() 
{return itsProdCutElectrons;}
inline G4double BDSGlobalConstants::GetProdCutElectronsP() 
{return itsProdCutElectronsP;}
inline G4double BDSGlobalConstants::GetProdCutPositrons() 
{return itsProdCutPositrons;}
inline G4double BDSGlobalConstants::GetProdCutPositronsP() 
{return itsProdCutPositronsP;}


//inline G4double BDSGlobalConstants::GetWorldSizeZ() 
//{return itsWorldSizeZ;}
//inline void BDSGlobalConstants::SetWorldSizeZ(G4double WorldSizeZ) 
//{itsWorldSizeZ=WorldSizeZ;}

inline G4double BDSGlobalConstants::GetTotalS() 
{return itsTotalS;}
inline void BDSGlobalConstants::SetTotalS(G4double TotalS) 
{itsTotalS=TotalS;}

// inline void BDSGlobalConstants::SetVerticalComponentOffset(G4double VerticalComponentOffset)
// {itsVerticalComponentOffset=VerticalComponentOffset;}
// inline void BDSGlobalConstants::SetHorizontalComponentOffset(G4double HorizontalComponentOffset)
// {itsHorizontalComponentOffset=HorizontalComponentOffset;}

// inline void BDSGlobalConstants::AddVerticalComponentOffset(G4double VerticalComponentOffset)
// {itsVerticalComponentOffset+=VerticalComponentOffset;}
// inline void BDSGlobalConstants::AddHorizontalComponentOffset(G4double HorizontalComponentOffset)
// {itsHorizontalComponentOffset+=HorizontalComponentOffset;}

// inline G4double BDSGlobalConstants::GetVerticalComponentOffset()
// {return itsVerticalComponentOffset;}
// inline G4double BDSGlobalConstants::GetHorizontalComponentOffset()
// {return itsHorizontalComponentOffset;}


inline G4String BDSGlobalConstants::GetPhysListName()
{
  return itsPhysListName;
}

inline void BDSGlobalConstants::SetPhysListName(G4String val)
{
  itsPhysListName = val;
}

inline G4bool BDSGlobalConstants::GetUseLowEMPhysics()
{return itsUseLowEMPhysics;}

inline G4bool BDSGlobalConstants::GetSynchRadOn()
{return itsSynchRadOn;}

inline G4bool BDSGlobalConstants::GetSynchRescale()
{return itsSynchRescale;}

inline void BDSGlobalConstants::SetSynchRescale(G4bool srRescale)
{itsSynchRescale = srRescale;}

inline G4bool BDSGlobalConstants::GetSynchTrackPhotons()
{return itsSynchTrackPhotons ;}

inline void BDSGlobalConstants::SetSynchTrackPhotons(G4bool srTrackPhotons)
{itsSynchTrackPhotons=srTrackPhotons ;}

inline G4double BDSGlobalConstants::GetSynchLowX()
{return itsSynchLowX ;}

inline G4double BDSGlobalConstants::GetSynchLowGamE()
{return itsSynchLowGamE ;}

inline G4int BDSGlobalConstants::GetSynchPhotonMultiplicity()
{return itsSynchPhotonMultiplicity ;}

inline G4int BDSGlobalConstants::GetSynchMeanFreeFactor()
{return itsSynchMeanFreeFactor ;}

inline G4bool BDSGlobalConstants::GetPlanckOn()
{return itsPlanckOn;}

inline G4bool BDSGlobalConstants::GetBDSeBremOn()
{return itsBDSeBremOn;}

inline G4double BDSGlobalConstants::GetLaserwireWavelength()
{return itsLaserwireWavelength ;}

inline G4ThreeVector BDSGlobalConstants::GetLaserwireDir()
{return itsLaserwireDir ;}

inline G4bool BDSGlobalConstants::GetLaserwireTrackPhotons()
{return itsLaserwireTrackPhotons ;}

inline G4bool BDSGlobalConstants::GetLaserwireTrackElectrons()
{return itsLaserwireTrackElectrons ;}

// inline G4bool BDSGlobalConstants::GetReadBunchFile()
// {return itsReadBunchFile;}

// inline G4bool BDSGlobalConstants::GetExtractBunchFile()
// {return itsExtractBunchFile;}

// inline G4bool BDSGlobalConstants::GetWriteBunchFile()
// {return itsWriteBunchFile ;}

inline G4double BDSGlobalConstants::GetLengthSafety()
{return itsLengthSafety;}

inline G4bool BDSGlobalConstants::GetTurnOnInteractions()
{return itsTurnOnInteractions;}

inline G4bool BDSGlobalConstants::GetUseTimer()
{return itsUseTimer;}

inline G4bool BDSGlobalConstants::GetUseEMHadronic()
{return itsUseEMHadronic;}

inline G4bool BDSGlobalConstants::GetIncludeIronMagFields()
{return itsIncludeIronMagFields;}

inline G4bool BDSGlobalConstants::GetUseMuonPairProduction()
{return itsUseMuonPairProduction;}

inline G4bool BDSGlobalConstants::GetStoreMuonTrajectories()
{return itsStoreMuonTrajectories;}

inline G4bool BDSGlobalConstants::GetStoreNeutronTrajectories()
{return itsStoreNeutronTrajectories;}

inline G4bool BDSGlobalConstants::GetStoreTrajectory()
{return itsStoreTrajectory;}

inline G4bool BDSGlobalConstants::GetStopTracks()
{ return stopTracks; }

inline void BDSGlobalConstants::SetStopTracks(G4bool val)
{ stopTracks = val; }

inline G4bool BDSGlobalConstants::GetUseMuonShowers()
{return itsUseMuonShowers;}

inline G4double BDSGlobalConstants::GetMuonLowestGeneratedEnergy()
{return itsMuonLowestGeneratedEnergy;}

inline G4double BDSGlobalConstants::GetMuonEnergyCutScaleFactor()
{return itsMuonEnergyCutScaleFactor;}

inline void BDSGlobalConstants::SetMuonEnergyCutScaleFactor(G4double aScaleFactor)
{itsMuonEnergyCutScaleFactor=aScaleFactor;}

inline G4Timer* BDSGlobalConstants::GetTimer()
{return itsTimer;}

inline G4double BDSGlobalConstants::GetEnergyOffset()
{return itsEnergyOffset;}

inline  G4double BDSGlobalConstants::GetTrackWeightFactor()
{return itsTrackWeightFactor;}

inline  G4double BDSGlobalConstants::GetMuonProductionScaleFactor()
{return itsMuonProductionScaleFactor;}

inline  G4double BDSGlobalConstants::GetHadronInelasticScaleFactor()
{return itsHadronInelasticScaleFactor;}

inline  G4bool BDSGlobalConstants::GetUseHaloRadius()
{return itsUseHaloRadius;}
inline  G4double BDSGlobalConstants::GetHaloInnerRadius()
{return itsHaloInnerRadius;}
inline  G4double BDSGlobalConstants::GetHaloOuterRadius()
{return itsHaloOuterRadius;}


inline G4double BDSGlobalConstants::GetInnerHaloX()
{return itsInnerHaloX;}
inline G4double BDSGlobalConstants::GetOuterHaloX()
{return itsOuterHaloX;}
inline G4double BDSGlobalConstants::GetInnerHaloY()
{return itsInnerHaloY;}
inline G4double BDSGlobalConstants::GetOuterHaloY()
{return itsOuterHaloY;}

inline G4long BDSGlobalConstants::GetRandomSeed()
{return itsRandomSeed;}
inline  G4bool BDSGlobalConstants::GetUseBatch()
{return itsUseBatch;}
inline G4int BDSGlobalConstants::GetNumberToGenerate()
{return itsNumberToGenerate;}

inline G4String BDSGlobalConstants::GetOutputNtupleFileName()
{return itsOutputNtupleFileName;}
inline G4int BDSGlobalConstants::GetNumberOfEventsPerNtuple()
{return itsNumberOfEventsPerNtuple;}

inline G4int BDSGlobalConstants::GetEventNumberOffset()
{return itsEventNumberOffset;}

// Internally transmitted variables
  // Internally transmitted variables:
inline void BDSGlobalConstants::SetGlobalBeamlineRotationY(G4double aYRotation)
{GlobalBeamlineRotationY=aYRotation;}
inline G4double BDSGlobalConstants::GetGlobalBeamlineRotationY()
{return GlobalBeamlineRotationY;}

inline G4FieldManager* BDSGlobalConstants::GetZeroFieldManager()
{return itsZeroFieldManager;}


inline  G4double BDSGlobalConstants::GetLWCalWidth()
{return itsLWCalWidth;}

inline  G4double BDSGlobalConstants::GetLWCalOffset()
{return itsLWCalOffset;}

//inline  G4String BDSGlobalConstants::GetLWCalMaterial()
//{return itsLWCalMaterial;}

inline G4String BDSGlobalConstants::GetPipeMaterialName()
{return itsPipeMaterial;}

inline G4String BDSGlobalConstants::GetVacMaterialName()
{return itsVacMaterial;}

inline G4bool BDSGlobalConstants::GetUseBeamGasPlug()
{return itsUseBeamGasPlug;}
inline G4double BDSGlobalConstants::GetBeamGasPlugZ()
{return itsBeamGasPlugZ;}

inline G4bool BDSGlobalConstants::GetUseSynchPrimaryGen()
{return itsSynchPrimaryGen;}
inline G4double BDSGlobalConstants::GetSynchPrimaryAngle()
{return itsSynchPrimaryAngle;}
inline G4double BDSGlobalConstants::GetSynchPrimaryLength()
{return itsSynchPrimaryLength;}

inline G4bool BDSGlobalConstants::GetUseLastMaterialPoint()
{return itsUseLastMaterialPoint;}

inline G4double BDSGlobalConstants::GetWedgeDisplacement()
{return itsWedgeDisplacement;}
inline void BDSGlobalConstants::SetWedgeDisplacement(G4double WedgeDisplacement)
{itsWedgeDisplacement=WedgeDisplacement;}
inline G4bool BDSGlobalConstants::GetPreviousWasWedge()
{return itsPreviousWasWedge;}
inline void BDSGlobalConstants::SetPreviousWasWedge(G4bool PreviousWasWedge)
{itsPreviousWasWedge=PreviousWasWedge;}

inline G4bool BDSGlobalConstants::DoTwiss() 
{return doTwiss;}

inline void BDSGlobalConstants::SetDoTwiss(G4bool val) 
{doTwiss = val;}

//for map of laserwire wavelengths
inline G4double BDSGlobalConstants::GetLaserwireWavelength(G4String aName)
{return lwWavelength[aName];}

//for map of laserwire wavelengths
inline G4ThreeVector BDSGlobalConstants::GetLaserwireDir(G4String aName)
{return lwDirection[aName];}

inline void BDSGlobalConstants::SetLaserwireWavelength(G4String aName, G4double aWavelength)
{lwWavelength[aName]=aWavelength;}

inline void BDSGlobalConstants::SetLaserwireDir(G4String aName, G4ThreeVector aDirection)
{lwDirection[aName]=aDirection;}

inline void BDSGlobalConstants::setWaitingForDump(G4bool flag) { isWaitingForDump = flag; } // waiting before all tracks arrive at a dump element
inline G4bool BDSGlobalConstants::getWaitingForDump() { return isWaitingForDump; }

inline void BDSGlobalConstants::setDumping(G4bool flag) { isDumping = flag; } // all tracks are pending - for stacking manager 
inline G4bool BDSGlobalConstants::getDumping() { return isDumping; }

inline void BDSGlobalConstants::setReading(G4bool flag) { isReading = flag; }
inline G4bool BDSGlobalConstants::getReading() { return isReading; }

inline void BDSGlobalConstants::setReadFromStack(G4bool flag) { isReadFromStack = flag; }
inline G4bool BDSGlobalConstants::getReadFromStack() { return isReadFromStack; }

inline G4String BDSGlobalConstants::GetFifo() {return itsFifo;}
inline void BDSGlobalConstants::SetFifo(G4String aFileName) {itsFifo = aFileName;}

inline G4AffineTransform BDSGlobalConstants::GetDumpTransform() {return itsDumpTransform;}
inline void BDSGlobalConstants::SetDumpTransform(G4AffineTransform tf)
{itsDumpTransform=tf;}

inline G4String BDSGlobalConstants::GetRefVolume() { return itsRefVolume; }
inline G4int BDSGlobalConstants::GetRefCopyNo() { return itsRefCopyNo; }

inline const G4AffineTransform* BDSGlobalConstants::GetRefTransform() { return &itsRefTransform; }
inline void BDSGlobalConstants::SetRefTransform(G4AffineTransform& aTransform)
{itsRefTransform=aTransform;}

extern BDSGlobalConstants* BDSGlobals;
#endif

