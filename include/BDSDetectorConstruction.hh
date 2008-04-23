/** BDSIM, v0.1   

Last modified 15.11.2005 by Ilya Agapov

**/

//==============================================================

#ifndef BDSDetectorConstruction_h
#define BDSDetectorConstruction_h 

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include"BDSMaterials.hh"

#include "parser/gmad.h"

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
     
public:

  private:
  G4Box*            solidWorld;    //pointer to the solid World 
  G4LogicalVolume*   logicWorld;    //pointer to the logical World
  G4VPhysicalVolume* physiWorld;    //pointer to the physical World

  void DefineMaterials();

  G4VPhysicalVolume* ConstructBDS(std::list<struct Element>& beamline_list);
  G4UniformMagField* magField;      //pointer to the magnetic field
  G4UserLimits* BDSUserLimits;

  G4VSensitiveDetector *  BDSSensitiveDetector;

};

#endif

