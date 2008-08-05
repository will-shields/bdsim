/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 5.3.2005
   Copyright (c) 2005 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/

#ifndef BDSSamplerCylinder_h
#define BDSSamplerCylinder_h 1

#include "globals.hh"
#include "BDSAcceleratorComponent.hh"
#include "BDSMaterials.hh"
#include "G4LogicalVolume.hh"

#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"

class BDSSamplerCylinder :public BDSAcceleratorComponent
{
public:
  BDSSamplerCylinder(G4String aName,G4double aLength, G4double aRadius);
  ~BDSSamplerCylinder();

  static const int GetNSamplers();
  static void AddExternalSampler();

protected:

private:
  void SamplerCylinderLogicalVolume();
  G4VisAttributes* SetVisAttributes();
  G4double itsRadius;

  // field related objects:
  G4UserLimits* itsUserLimits;
  G4VisAttributes* itsVisAttributes;

  int nThisSampler;
  static int nSamplers;
};

#endif
