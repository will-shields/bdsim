/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/
#include "BDSGlobalConstants.hh" // must be first in include list

#include "BDSLaserWire.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"               
#include "G4UserLimits.hh"

#include <map>

typedef std::map<G4String,int> LogVolCountMap;
extern LogVolCountMap* LogVolCount;

typedef std::map<G4String,G4LogicalVolume*> LogVolMap;
extern LogVolMap* LogVol;

extern BDSMaterials* theMaterials;
//============================================================

BDSLaserWire::BDSLaserWire (G4String aName,G4double aLength,
G4double aWavelength, G4ThreeVector aDirection,
 G4ThreeVector aPosition, G4double xSigma, G4double ySigma):
  BDSAcceleratorComponent(
    aName,
    aLength,0,0,0,
    SetVisAttributes())
{
  LaserWireLogicalVolume();
  //  itsLaserCompton=new BDSLaserCompton(aWavelength, aDirection);

  //  G4cout << "*****" << itsMarkerLogicalVolume->GetName() << "*****\n";

  BDSGlobals->
    SetLaserwireWavelength(itsMarkerLogicalVolume->GetName(),aWavelength);
  BDSGlobals->
    SetLaserwireDir(itsMarkerLogicalVolume->GetName(),aDirection);
}

void BDSLaserWire::LaserWireLogicalVolume()
{
  if(!(*LogVolCount)[itsName])
    {
      itsMarkerLogicalVolume=new G4LogicalVolume(
						 new G4Box(itsName+"_solid",
							   BDSGlobals->
							   GetBeampipeRadius(),
							   BDSGlobals->
							   GetBeampipeRadius(),
							   itsLength/2),
						 theMaterials->GetMaterial("LaserVac"),
						 itsName);
      (*LogVolCount)[itsName]=1;
      (*LogVol)[itsName]=itsMarkerLogicalVolume;
    }
  else
    {
      (*LogVolCount)[itsName]++;
      itsMarkerLogicalVolume=(*LogVol)[itsName];
    }
}

G4VisAttributes* BDSLaserWire::SetVisAttributes()
{
  itsVisAttributes=new G4VisAttributes(G4Colour(1.,0.,0.));
  return itsVisAttributes;
}

BDSLaserWire::~BDSLaserWire()
{
  delete itsVisAttributes;
  //  delete itsUserLimits;
  //  delete itsLaserCompton;
}
