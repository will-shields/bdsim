/* BDSIM code.    Version 1.0
  
*/

#ifndef BDSCollimator_h
#define BDSCollimator_h 

#include"globals.hh"
#include"BDSAcceleratorComponent.hh"
#include "BDSMaterials.hh"
#include "G4LogicalVolume.hh"

#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4Box.hh"
#include "G4EllipticalTube.hh"
#include "BDSEnergyCounterSD.hh"

#include "parser/gmad.h" // for type definitions


class BDSCollimator :public BDSAcceleratorComponent
{
public:
  BDSCollimator(G4String aName, G4double aLength,G4double bpRad, 
	     G4double xAper, G4double yAper,G4int type,G4Material *collimatorMaterial, G4double outR);
  ~BDSCollimator();

protected:

private:
  void BuildInnerCollimator();
  G4VisAttributes* SetVisAttributes();

  // Geometrical objects:

  G4VPhysicalVolume* itsPhysiComp;
  G4VPhysicalVolume* itsPhysiComp2;
  G4LogicalVolume* itsSolidLogVol;
  G4LogicalVolume* itsInnerLogVol;
                    
  G4UserLimits* itsUserLimits;
  G4VisAttributes* itsVisAttributes;
  G4Mag_UsualEqRhs* itsEqRhs;

private:
  G4Material* itsCollimatorMaterial;
  BDSEnergyCounterSD* itsEnergyCounterSD;
  G4double itsOuterR;
};

#endif
