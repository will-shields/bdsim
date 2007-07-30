/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 18.10.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/

#ifndef mySectorBend_h
#define mySectorBend_h 

#include "globals.hh"
#include "BDSMaterials.hh"
#include "G4LogicalVolume.hh"
#include "BDSHelixStepper.hh"

#include "myQuadStepper.hh"

#include "G4FieldManager.hh"
#include "G4ChordFinder.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4UniformMagField.hh"
#include "G4PVPlacement.hh"               

#include "BDSMultipole.hh"
#include "BDSSbendMagField.hh"
#include "G4Mag_EqRhs.hh"

class mySectorBend :public BDSMultipole
{
  public:
    mySectorBend(G4String aName, G4double aLength,
		  G4double bpRad,G4double FeRad,
		  G4double bField, G4double angle,G4double tilt = 0, G4double bGrad=0, 
		  G4String aMaterial = "", G4int nSegments=1);
    ~mySectorBend();

  protected:

  private:
  G4double itsBField;
  G4double itsBGrad;

  //  void BuildOuterLogicalVolume();
  void BuildBPFieldAndStepper();
  void BuildSBMarkerLogicalVolume();

  //void BuildBeampipe2(G4double length,G4double angle);
  friend void BuildBeampipe2(mySectorBend *sb,G4double length,G4double angle);

  G4VisAttributes* SetVisAttributes();

  // field related objects:
  //BDSHelixStepper* itsStepper;
  myQuadStepper* itsStepper;
  BDSSbendMagField* itsMagField;
  G4Mag_EqRhs* itsEqRhs;

};

void BuildBeampipe2(mySectorBend *sb,G4double length,G4double angle);

#endif
