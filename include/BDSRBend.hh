//  
//   BDSIM, (C) 2001-2007
//   
//   version 0.4
//  
//
//
//   Rectangular bending magnet class
//   - itsLength parameter internally stores the geometrical length 
//   - itsAngle parameter internally stores the bending angle
//   - to get the arc length use the GetArcLength() function
//   - the volume is a trapezoid with pole faces perpendicular to the ideal
//     orbit
//     
//   History
//
//

#ifndef BDSRBend_h
#define BDSRBend_h 

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
#include "G4PVPlacement.hh"               

#include "BDSMultipole.hh"
#include "BDSSbendMagField.hh"
#include "G4Mag_EqRhs.hh"

class BDSRBend :public BDSMultipole
{
public:
  BDSRBend(G4String aName, G4double aLength,
	   G4double bpRad, G4double FeRad,
	   G4double bField, G4double angle, G4double outR,
	   G4double tilt = 0, G4double bGrad=0, 
	   G4String aMaterial = "", G4int nSegments=1);
  ~BDSRBend();

  void SynchRescale(G4double factor);

  virtual const G4double GetArcLength() const;

protected:

private:
  G4double itsBField;
  G4double itsBGrad;

  void BuildBPFieldAndStepper();
  void BuildRBMarkerLogicalVolume();
  void BuildRBBeampipe();
  void BuildRBOuterLogicalVolume(G4bool OuterMaterialIsVacuum=false);

  G4VisAttributes* SetVisAttributes();
  G4Trd* markerSolidVolume;

  // field related objects:
  myQuadStepper* itsStepper;
  BDSSbendMagField* itsMagField;
  G4Mag_EqRhs* itsEqRhs;

};

inline const G4double BDSRBend::GetArcLength() const
{
  // arc length = radius*angle
  //            = (geometrical length/(2.0*sin(angle/2))*angle
  if (itsAngle == 0.0)
    return itsLength;
  else
    return (itsLength * (0.5*itsAngle) / sin(0.5*itsAngle));
}

#endif
