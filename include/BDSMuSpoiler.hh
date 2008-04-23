/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 12.12.2004
   Copyright (c) 2004 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/

#ifndef BDSMuSpoiler_h
#define BDSMuSpoiler_h 1

#include"globals.hh"
#include"BDSAcceleratorComponent.hh"
#include "BDSMaterials.hh"
#include "G4LogicalVolume.hh"

#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4Box.hh"
#include "BDSEnergyCounterSD.hh"
#include "BDSMuSpoilerMagField.hh"
#include "G4FieldManager.hh"

class BDSMuSpoiler :public BDSAcceleratorComponent
{
public:
  BDSMuSpoiler(G4String& aName, G4double aLength,G4double bpRad, 
	       G4double rOuter,G4double BField);


  ~BDSMuSpoiler();

protected:

private:
  G4VisAttributes* SetVisAttributes();

  // Geometrical objects:

  void BuildMuSpoiler();

  G4VPhysicalVolume* itsPhysiComp;
  G4VPhysicalVolume* itsPhysiComp2;
  G4LogicalVolume* itsSolidLogVol;
  G4LogicalVolume* itsInnerLogVol;
                    
  G4UserLimits* itsUserLimits;
  G4VisAttributes* itsVisAttributes;
  G4Mag_UsualEqRhs* itsEqRhs;

private:
  BDSEnergyCounterSD* itsEnergyCounterSD;
  G4double itsInnerRadius;
  G4double itsOuterRadius;
  G4double itsBField;

  BDSMuSpoilerMagField* itsMagField;
  G4FieldManager* itsFieldMgr;
};

#endif
