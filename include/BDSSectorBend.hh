/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 18.10.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 

   IA: 12.10.05 , modified
*/

#ifndef BDSSectorBend_h
#define BDSSectorBend_h 

#include "globals.hh"
#include "BDSMaterials.hh"
#include "G4LogicalVolume.hh"
#include "myQuadStepper.hh"

#include "G4FieldManager.hh"
#include "G4ChordFinder.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"               

#include "BDSMultipole.hh"
#include "BDSSbendMagField.hh"
#include "G4Mag_EqRhs.hh"

class BDSSectorBend :public BDSMultipole
{
public:
  BDSSectorBend(G4String aName, G4double aLength,
		G4double bpRad, G4double FeRad,
		G4double bField, G4double angle, G4double outR,
		G4double tilt = 0, G4double bGrad=0, G4String aMaterial = "",
		G4int nSegments=1);
  ~BDSSectorBend();

  void SynchRescale(G4double factor);

protected:

private:
  G4double itsBField;
  G4double itsBGrad;

  void BuildBPFieldAndStepper();
  void BuildSBMarkerLogicalVolume();
  void BuildSBBeampipe();
  void BuildSBOuterLogicalVolume(G4bool OuterMaterialIsVacuum=false);

  G4VisAttributes* SetVisAttributes();
  G4Trd* markerSolidVolume;

  // field related objects:
  myQuadStepper* itsStepper;
  BDSSbendMagField* itsMagField;
  G4Mag_EqRhs* itsEqRhs;

};

#endif
