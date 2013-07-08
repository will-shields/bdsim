/** BDSIM, v0.1   

Last modified 15.11.2005 by Ilya Agapov

**/

//==============================================================

#ifndef BDSDetectorConstruction_h
#define BDSDetectorConstruction_h 

#include "BDSGlobalConstants.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "BDSWorld.hh"
#include"BDSMaterials.hh"
#include "BDSBeamline.hh"

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
    return _worldPhysicalVolume;
  }

  inline G4GeometrySampler* GetGeometrySampler(){
    return itsGeometrySampler;
  }

private:
  void buildAtoms();
  void buildMaterials();
  void buildBeamline();
  void buildWorld();

  //====================================
  //Stuff to do with "world"
  void placeWorld();
  void calcWorldSize();
  void constructWorldSolid();
  void constructWorldLogical();
  void setWorldVisAttributes();
  void setWorldUserLimits();
  G4ThreeVector _worldSize;
  G4VSolid* _worldSolidVolume;
  G4LogicalVolume* _worldLogicalVolume;
  G4VPhysicalVolume* _worldPhysicalVolume;
  G4UserLimits* _worldUserLimits;
  G4VisAttributes* _worldVisAttributes;
  G4Material* _worldMaterial;
  //------------------------------------
  //Tunnel cavity
  void placeTunnelCavity();

  G4GeometrySampler* itsGeometrySampler;

  G4Region* precisionRegion;
  G4Region* gasRegion;

  std::vector< G4VPhysicalVolume * > fPhysicalVolumeVector; //a vector with all the physical volumes

  void DefineMaterials();

  G4VPhysicalVolume* ConstructBDS();
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

