/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 5.3.2005
   Copyright (c) 2005 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/
#include "BDSGlobalConstants.hh" // must be first in include list
#include "BDSSamplerCylinder.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"               
#include "G4UserLimits.hh"

#include <map>

#include "BDSSamplerSD.hh"
#include "G4SDManager.hh"


typedef std::map<G4String,int> LogVolCountMap;
extern LogVolCountMap* LogVolCount;

typedef std::map<G4String,G4LogicalVolume*> LogVolMap;
extern LogVolMap* LogVol;



extern BDSMaterials* theMaterials;
//============================================================

BDSSamplerCylinder::
BDSSamplerCylinder (G4String aName,G4double aLength,G4double aRadius):
  BDSAcceleratorComponent(
			 aName,
			 aLength,0,0,0,
			 SetVisAttributes()),
  itsRadius(aRadius)
{
  SamplerCylinderLogicalVolume();
  //G4int nSamplers=(*LogVolCount)[itsName];
  //BDSRoot->SetSampCylinderNumber(nSamplers);
}


void BDSSamplerCylinder::SamplerCylinderLogicalVolume()
{
  if(!(*LogVolCount)[itsName])
    {
      G4double SampTransSize;
      SampTransSize=2.*BDSGlobals->GetTunnelRadius();

      itsMarkerLogicalVolume=
	new G4LogicalVolume(new G4Tubs(itsName+"_body",
				       itsRadius-1.e-6*m,
				       itsRadius,
				       itsLength/2,
				       0,twopi*radian),
			    theMaterials->LCVacuum,
			    itsName);
      
      (*LogVolCount)[itsName]=1;
      (*LogVol)[itsName]=itsMarkerLogicalVolume;

      itsOuterUserLimits =new G4UserLimits();
      itsOuterUserLimits->SetMaxAllowedStep(itsLength);
      itsMarkerLogicalVolume->SetUserLimits(itsOuterUserLimits);

      // Sensitive Detector:
      G4SDManager* SDMan = G4SDManager::GetSDMpointer();
      BDSSamplerSD* SensDet=new BDSSamplerSD(itsName,"cylinder");
       
      SDMan->AddNewDetector(SensDet);
      itsMarkerLogicalVolume->SetSensitiveDetector(SensDet);
    }
  else
    {
      (*LogVolCount)[itsName]++;
      itsMarkerLogicalVolume=(*LogVol)[itsName];
    }
}

G4VisAttributes* BDSSamplerCylinder::SetVisAttributes()
{
  itsVisAttributes=new G4VisAttributes(G4Colour(1,0,1));
  return itsVisAttributes;
}

BDSSamplerCylinder::~BDSSamplerCylinder()
{
  delete itsVisAttributes;
  delete itsUserLimits;
}
