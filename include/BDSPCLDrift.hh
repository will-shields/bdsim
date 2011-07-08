/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/

#ifndef BDSPCLDrift_h
#define BDSPCLDrift_h 1

#include "globals.hh"
#include "BDSMaterials.hh"
#include "G4LogicalVolume.hh"

#include "G4FieldManager.hh"
#include "G4ChordFinder.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"               

#include "BDSMultipole.hh"

class BDSPCLDrift :public BDSMultipole
{
public:
  BDSPCLDrift(G4String aName, G4double aLength,
           std::list<G4double> blmLocZ, std::list<G4double> blmLocTheta, 
	      G4double aperX, G4double aperyUp, G4double aperYDown, G4double aperDy,  G4String aTunnelMaterial="", G4bool aperset=false, G4double aper=0, G4double tunnelRadius=0., G4double tunnelOffsetX=BDSGlobals->GetTunnelOffsetX());
  ~BDSPCLDrift();

protected:

private:
  void BuildBLMs();
  void BuildBeampipe();
  G4VisAttributes* SetVisAttributes();
  G4double itsYAperUp, itsYAperDown, itsDyAper;

  G4VSolid* upper_outer_solid;
  G4VSolid* middle_outer_solid;
  G4VSolid* lower_outer_solid;
  G4VSolid* upper_inner_solid;
  G4VSolid* middle_inner_solid;
  G4VSolid* lower_inner_solid;

  G4LogicalVolume* itsUpperBeamPipeLogicalVolume;
  G4LogicalVolume* itsMiddleBeamPipeLogicalVolume;
  G4LogicalVolume* itsLowerBeamPipeLogicalVolume;

  G4LogicalVolume* itsUpperInnerBeamPipeLogicalVolume;
  G4LogicalVolume* itsMiddleInnerBeamPipeLogicalVolume;
  G4LogicalVolume* itsLowerInnerBeamPipeLogicalVolume;
  
  G4VPhysicalVolume* itsPhysiUpperInner;
  G4VPhysicalVolume* itsPhysiUpper;
  G4VPhysicalVolume* itsPhysiMiddleInner;
  G4VPhysicalVolume* itsPhysiMiddle;
  G4VPhysicalVolume* itsPhysiLowerInner;
  G4VPhysicalVolume* itsPhysiLower;

  G4VisAttributes* itsBeampipeVisAtt;
  G4VisAttributes* itsInnerBeampipeVisAtt;
};

#endif
