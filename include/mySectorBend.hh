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
	       G4double bField, G4double angle, G4double outR,
	       G4double tilt = 0, G4double bGrad=0, G4String aMaterial = ""
               );
  ~mySectorBend();
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

  // field related objects:
  myQuadStepper* itsStepper;
  BDSSbendMagField* itsMagField;
  G4Mag_EqRhs* itsEqRhs;

};

#endif
