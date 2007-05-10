/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 12.12.2004
   Copyright (c) 2004 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/
#include "BDSGlobalConstants.hh" // must be first in include list
#include "BDSMuSpoiler.hh"
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
typedef std::map<G4String,int> LogVolCountMap;
extern LogVolCountMap* LogVolCount;

typedef std::map<G4String,G4LogicalVolume*> LogVolMap;
extern LogVolMap* LogVol;
//extern G4double BDS_Threshold_Energy;
extern BDSMaterials* theMaterials;
//============================================================

BDSMuSpoiler::BDSMuSpoiler (G4String& aName,G4double aLength,G4double bpRad,
			    G4double rOuter,G4double aBField):
  BDSAcceleratorComponent(aName,
			 aLength,bpRad,bpRad,bpRad,
			 SetVisAttributes()),
  itsInnerRadius(bpRad),
  itsOuterRadius(rOuter),
  itsBField(aBField)

{
  
  if ( (*LogVolCount)[itsName]==0)
    {
      itsMarkerLogicalVolume=
	new G4LogicalVolume(
			    new G4Box(itsName,
				      itsOuterRadius,
				      itsOuterRadius,
				      itsLength/2),
			    theMaterials->LCVacuum,
			    itsName);

  // now protect the fields inside the marker volume by giving the
  // marker a null magnetic field (otherwise G4VPlacement can
  // over-ride the already-created fields, by calling 
  // G4LogicalVolume::AddDaughter, which calls 
  // pDaughterLogical->SetFieldManager(fFieldManager, true) - the
  // latter 'true' over-writes all the other fields
      itsMarkerLogicalVolume->
	SetFieldManager(BDSGlobals->GetZeroFieldManager(),false);
	
      BuildMuSpoiler();

      (*LogVolCount)[itsName]=1;
      (*LogVol)[itsName]=itsMarkerLogicalVolume;
    }
  else
    {
      (*LogVolCount)[itsName]++;
      itsMarkerLogicalVolume=(*LogVol)[itsName];
    }  
}


void BDSMuSpoiler::BuildMuSpoiler()
{
  itsSolidLogVol=
    new G4LogicalVolume(new G4Tubs(itsName+"_solid",
				   itsInnerRadius,
				   itsOuterRadius,
				   itsLength/2,
				   0,twopi*radian),
			theMaterials->LCIron,
			itsName+"_solid");

  itsInnerLogVol=
    new G4LogicalVolume(new G4Tubs(itsName+"_inner",
				   0.,
				   itsInnerRadius,
				   itsLength/2,
				   0,twopi*radian),
			theMaterials->LCVacuum,
			itsName+"_inner");

  G4UserLimits* AbsUserLimits =
    new G4UserLimits(DBL_MAX,DBL_MAX,DBL_MAX,
		     BDSGlobals->GetThresholdCutCharged());

  AbsUserLimits->SetMaxAllowedStep(itsLength);
  itsInnerLogVol->SetUserLimits(AbsUserLimits);

  itsSolidLogVol->
    SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,DBL_MAX,
				       BDSGlobals->GetThresholdCutCharged()));

  if(itsBField)
    {
      itsMagField=new BDSMuSpoilerMagField(itsBField);
      itsFieldMgr=new G4FieldManager(itsMagField);
      itsSolidLogVol->SetFieldManager(itsFieldMgr,false);
    }
  
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




G4VisAttributes* BDSMuSpoiler::SetVisAttributes()
{
  itsVisAttributes=new G4VisAttributes(G4Colour(0.0,0.5,0.5));
  return itsVisAttributes;
}


BDSMuSpoiler::~BDSMuSpoiler()
{
  if(itsVisAttributes) delete itsVisAttributes;
  if(itsUserLimits) delete itsUserLimits;

  if(itsMarkerLogicalVolume)delete itsMarkerLogicalVolume;
  if(itsSolidLogVol)delete itsSolidLogVol;
  if(itsInnerLogVol)delete itsInnerLogVol;

  if(itsPhysiComp) delete itsPhysiComp;
  if(itsPhysiComp2) delete itsPhysiComp2;

  if(itsMagField) delete itsMagField;
  if(itsFieldMgr) delete itsFieldMgr;

}
