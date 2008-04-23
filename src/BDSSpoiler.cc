/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/
#include "BDSGlobalConstants.hh" // must be first in include list
#include "BDSSpoiler.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"               
#include "G4UserLimits.hh"
#include "G4TransportationManager.hh"

#include "G4SDManager.hh"
#include "G4UserLimits.hh"

#include <map>

//============================================================
//typedef std::map<G4String,MagFieldFunction*> PhysFieldMap;
//extern PhysFieldMap* MagFieldMap;

typedef std::map<G4String,int> LogVolCountMap;
extern LogVolCountMap* LogVolCount;

typedef std::map<G4String,G4LogicalVolume*> LogVolMap;
extern LogVolMap* LogVol;
//extern G4double BDS_Threshold_Energy;
extern BDSMaterials* theMaterials;
//============================================================

BDSSpoiler::BDSSpoiler (G4String& aName,G4double aLength,G4double bpRad,
			  G4double xAper,G4double yAper,
			  G4Material* SpoilerMaterial):
  BDSAcceleratorComponent(aName,
			 aLength,bpRad,xAper,yAper,
			 SetVisAttributes()),
  itsSpoilerMaterial(SpoilerMaterial)
{
  
  if ( (*LogVolCount)[itsName]==0)
    {
      itsMarkerLogicalVolume=
	new G4LogicalVolume(
			    new G4Box(itsName,
				      BDSGlobals->GetComponentBoxSize()/2,
				      BDSGlobals->GetComponentBoxSize()/2,
				      itsLength/2),
			    theMaterials->GetMaterial("Vacuum"),
			    itsName);
      BuildInnerSpoiler();

      (*LogVolCount)[itsName]=1;
      (*LogVol)[itsName]=itsMarkerLogicalVolume;
    }
  else
    {
      (*LogVolCount)[itsName]++;
      itsMarkerLogicalVolume=(*LogVol)[itsName];
    }  
}


G4VisAttributes* BDSSpoiler::SetVisAttributes()
{
  itsVisAttributes=new G4VisAttributes(G4Colour(0.3,0.4,0.2));
  return itsVisAttributes;
}


void BDSSpoiler::BuildInnerSpoiler()
{
  itsSolidLogVol=
    new G4LogicalVolume(new G4Box(itsName+"_solid",
				  BDSGlobals->GetComponentBoxSize()/2,
				  BDSGlobals->GetComponentBoxSize()/2,
				  itsLength/2),
			itsSpoilerMaterial,
			itsName+"_solid");

  itsInnerLogVol=
    new G4LogicalVolume(new G4Box(itsName+"_inner",
				  itsXAper,
				  itsYAper,
				  itsLength/2),
			theMaterials->GetMaterial("Vacuum"),
			itsName+"_inner");
  
  itsPhysiComp2 = 
    new G4PVPlacement(
		      0,		   // no rotation
		      0,                   // its position
		      itsInnerLogVol,      // its logical volume
		      itsName+"_combined", // its name
		      itsSolidLogVol,      // its mother  volume
		      false,		   // no boolean operation
		      0);		   // copy number 

  SetSensitiveVolume(itsSolidLogVol);

  itsSolidLogVol->
    SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,DBL_MAX,
				       BDSGlobals-> GetThresholdCutCharged()));
  
  itsPhysiComp = 
    new G4PVPlacement(
		      0,		     // no rotation
		      0,                     // its position
		      itsSolidLogVol,    // its logical volume
		      itsName+"_solid",	     // its name
		      itsMarkerLogicalVolume, // its mother  volume
		      false,		     // no boolean operation
		      0);		     // copy number  
}


BDSSpoiler::~BDSSpoiler()
{
  if(itsVisAttributes) delete itsVisAttributes;
  if(itsUserLimits) delete itsUserLimits;

  if(itsMarkerLogicalVolume)delete itsMarkerLogicalVolume;
  if(itsSolidLogVol)delete itsSolidLogVol;
  if(itsInnerLogVol)delete itsInnerLogVol;

  if(itsPhysiComp) delete itsPhysiComp;
  if(itsPhysiComp2) delete itsPhysiComp2;
}
