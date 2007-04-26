/* BDSIM code.    Version 1.0
   IA: 12.10.05 , modified
*/

#ifndef BDSRBend_h
#define BDSRBend_h 

#include"globals.hh"
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

class BDSRBend:public BDSMultipole
{
  public:
    BDSRBend(G4String aName, G4double aLength,
		  G4double bpRad,G4double FeRad,
		  G4double bField, G4double angle, G4double outR,
		  G4double tilt = 0, G4double bGrad=0, G4int nSegments=1);
    ~BDSRBend();

  protected:

  private:
  G4double itsBField;
  G4double itsBGrad;

  //  void BuildOuterLogicalVolume();
  void BuildBPFieldAndStepper();
  void BuildSBMarkerLogicalVolume();

  //void BuildBeampipe2(G4double length,G4double angle);
  friend void BuildBeampipe2(BDSRBend *sb,G4double length,G4double angle);
  //  friend void BuildDefaultOuterLogicalVolume2(BDSRBend* sb,G4double aLength, G4double angle, G4bool OuterMaterialIsVacuum=false);

  G4VisAttributes* SetVisAttributes();

  // field related objects:
  //BDSHelixStepper* itsStepper;
  myQuadStepper* itsStepper;
  BDSSbendMagField* itsMagField;
  G4Mag_EqRhs* itsEqRhs;

};

void BuildBeampipe2(BDSRBend *sb,G4double length,G4double angle);
//void BuildDefaultOuterLogicalVolume2(BDSRBend* sb,G4double aLength, G4double angle, G4bool OuterMaterialIsVacuum=false);

#endif
