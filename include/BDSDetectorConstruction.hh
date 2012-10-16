/** BDSIM, v0.1   

Last modified 15.11.2005 by Ilya Agapov

**/

//==============================================================

#ifndef BDSDetectorConstruction_h
#define BDSDetectorConstruction_h 

#include "BDSGlobalConstants.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include"BDSMaterials.hh"

#include "parser/gmad.h"

#include "G4Region.hh"

#include "G4IStore.hh"
#include "G4GeometrySampler.hh"

//GFlash parameterisation                                                                                                                                                     
#include "GFlashHomoShowerParameterisation.hh"
#include "G4FastSimulationManager.hh"
#include "BDSShowerModel.hh"
#include "GFlashHitMaker.hh"
#include "GFlashParticleBounds.hh"


class G4Box;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4UniformMagField;
class BDSCalorimeterSD;
//class BDSMagField;
class G4UserLimits;
class G4VSensitiveDetector;

class G4Navigator;

//==============================================================

class BDSDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  
  BDSDetectorConstruction();
  ~BDSDetectorConstruction();

public:
     
  G4VPhysicalVolume* Construct();
  void SetMagField(const G4double afield);
  void UpdateGeometry();

  G4VIStore* CreateImportanceStore();
  inline G4IStore* GetIStore(){
    return itsIStore;
  }

  inline G4VPhysicalVolume* GetWorldVolume(){
    return physiWorld;
  }

  inline G4GeometrySampler* GetGeometrySampler(){
    return itsGeometrySampler;
  }

public:

  private:
  G4GeometrySampler* itsGeometrySampler;

  G4Region* precisionRegion;
  G4Region* gasRegion;

  G4Box*            solidWorld;    //pointer to the solid World 
  G4LogicalVolume*   logicWorld;    //pointer to the logical World
  G4VPhysicalVolume* physiWorld;    //pointer to the physical World
  std::vector< G4VPhysicalVolume * > fPhysicalVolumeVector; //a vector with all the physical volumes

  void DefineMaterials();

  int AddDriftToBeamline(G4String name, G4double l,std::list<G4double> blmLocZ, std::list<G4double> blmLocTheta, G4double startAper, G4double endAper, G4bool added_drift, G4String aTunnelMaterial="", G4double tunnelOffsetX=BDSGlobalConstants::Instance()->GetTunnelOffsetX() );
  G4VPhysicalVolume* ConstructBDS(std::list<struct Element>& beamline_list);
  G4UniformMagField* magField;      //pointer to the magnetic field
  G4UserLimits* BDSUserLimits;

  G4VSensitiveDetector *  BDSSensitiveDetector;
  
  G4IStore* itsIStore;

  // Gflash members                                                                                                                                                     
  std::vector<GFlashHomoShowerParameterisation*> theParameterisation;
  GFlashHitMaker *theHitMaker;
  GFlashParticleBounds *theParticleBounds;
  GFlashParticleBounds *theParticleBoundsVac;
  std::vector<BDSShowerModel*> theFastShowerModel;
  std::vector<G4Region*> gFlashRegion;

};

#endif

