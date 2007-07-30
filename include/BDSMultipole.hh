/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 

   Modified 22.03.05 by J.C.Carter, Royal Holloway, Univ. of London.
   Added extra parameter to BuildDefaultOuterLogicalVolume so that it is 
     possible to set the material as either Iron or Vacuum
   Removed StringFromInt function
*/

#ifndef BDSMultipole_h
#define BDSMultipole_h 

#include "globals.hh"
#include "BDSAcceleratorComponent.hh"
#include "BDSMaterials.hh"
#include "G4LogicalVolume.hh"

#include "G4FieldManager.hh"
#include "G4ChordFinder.hh"
#include "G4Mag_EqRhs.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4MagneticField.hh"
#include "G4PVPlacement.hh"               


class BDSMultipole :public BDSAcceleratorComponent
{
public:
  
  BDSMultipole( G4String aName, 
		G4double aLength,
		G4double aBpRadius,
		G4double aInnerIronRadius,
		G4VisAttributes* aVisAtt,
		G4String aMaterial = "",
		G4double aXAper=0.,
		G4double aYAper=0.,
		G4double angle=0.);

  virtual ~BDSMultipole();

  void BuildBeampipe(G4double aLength,G4String materialName = "", G4int nSegments=1);

  void BuildDefaultMarkerLogicalVolume();

//  void BuildDefaultOuterLogicalVolume();
  void BuildDefaultOuterLogicalVolume(G4double aLength,G4bool OuterMaterialIsVacuum=false);

  void BuildBPFieldMgr(G4MagIntegratorStepper* aStepper,
		       G4MagneticField* aField);

  G4FieldManager* GetBPFieldMgr();

  void BuildOuterFieldManager(G4int nPoles, G4double poleField, 
			      G4double phiOffset);

  void SetOuterRadius(G4double outR);

protected:
  G4LogicalVolume* itsBeampipeLogicalVolume;
  G4LogicalVolume* itsInnerBPLogicalVolume;
  G4VisAttributes* itsVisAttributes;
  G4UserLimits* itsUserLimits;
  G4UserLimits* itsBeampipeUserLimits;
  G4PVPlacement* itsPhysiComp;
  G4FieldManager* itsBPFieldMgr;
  G4FieldManager* itsOuterFieldMgr;

protected:   // these might need to be accessed from the child classes
  G4double itsInnerIronRadius;
  G4Tubs* itsBPTube;
  G4Tubs* itsInnerBPTube;
  G4ChordFinder* itsChordFinder;
  G4int itsNPoles;
  G4MagneticField* itsOuterMagField;

  G4RotationMatrix* itsSegRot;
  G4ThreeVector itsSegPos;
  G4int itsNSegments;
  G4double itsOuterR;
};

inline G4FieldManager* BDSMultipole::GetBPFieldMgr()
{return itsBPFieldMgr;}

inline void BDSMultipole::SetOuterRadius(G4double outR)
{itsOuterR = outR;}

#endif
