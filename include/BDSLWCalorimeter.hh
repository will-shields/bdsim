/* BDSIM code for LW Calorimeter.    Version 1.0
   Author: John Carter, Royal Holloway, Univ. of London.
   Last modified 26.7.2004
   Copyright (c) 2004 by J.C.Carter.  ALL RIGHTS RESERVED. 
*/
#ifndef BDSLWCalorimeter_h
#define BDSLWCalorimeter_h 1

#include "globals.hh"
#include "BDSAcceleratorComponent.hh"
#include "BDSMaterials.hh"
#include "G4LogicalVolume.hh"

#include "G4FieldManager.hh"
#include "G4ChordFinder.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"

class BDSLWCalorimeter :public BDSAcceleratorComponent
{
public:
  BDSLWCalorimeter(G4String& aName,G4double aLength, G4double aBpRad, G4String aTunnelMaterial="");
  ~BDSLWCalorimeter();
  void BuildBeampipe(G4double aLength);
  void BuildCal(G4double aLength);

protected:
  G4LogicalVolume* itsBeampipeLogicalVolume;
  G4LogicalVolume* itsInnerBPLogicalVolume;
  G4LogicalVolume* itsLWCalLogicalVolume;
  G4UserLimits* itsBeampipeUserLimits;
  G4FieldManager* itsBPFieldMgr;

private:
  G4Tubs* itsBPTube;
  G4Tubs* itsInnerBPTube;
  G4Box*  itsLWCal;
  G4VisAttributes* SetVisAttributes();
  void LWCalorimeterLogicalVolume();

  // field related objects:
  G4VisAttributes* itsVisAttributes;
};

#endif
