/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 

   Modified 22.03.05 by J.C.Carter, Royal Holloway, Univ. of London.
   Changed StringFromInt to be the BDSGlobal version (Line 86)
*/

#include "BDSGlobalConstants.hh" // must be first in include list
#include "BDSDecapole.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4UserLimits.hh"
#include "G4TransportationManager.hh"

#include <map>

//============================================================

typedef std::map<G4String,int> LogVolCountMap;
extern LogVolCountMap* LogVolCount;

typedef std::map<G4String,G4LogicalVolume*> LogVolMap;
extern LogVolMap* LogVol;

extern BDSMaterials* theMaterials;
//============================================================

BDSDecapole::BDSDecapole(G4String& aName,G4double aLength, 
			 G4double bpRad,G4double FeRad,
			 G4double BQuadPrime, G4String aMaterial):
  BDSMultipole(aName,aLength, bpRad, FeRad, SetVisAttributes(),aMaterial),
  itsBQuadPrime(BQuadPrime)
{
  if (!(*LogVolCount)[itsName])
    {
      BuildBPFieldAndStepper();
      BuildBPFieldMgr(itsStepper,itsMagField);
      BuildDefaultMarkerLogicalVolume();

      BuildBeampipe(itsLength);

      BuildDefaultOuterLogicalVolume(itsLength);

      SetSensitiveVolume(itsBeampipeLogicalVolume);// for synchrotron
      //SetSensitiveVolume(itsOuterLogicalVolume);// for laserwire

      // visual attributes
      G4VisAttributes* VisAtt =
        new G4VisAttributes(G4Colour(0., 0., 0.));
      VisAtt->SetForceSolid(true);
      itsInnerBPLogicalVolume->SetVisAttributes(VisAtt);

      G4VisAttributes* VisAtt1 =
        new G4VisAttributes(G4Colour(0.4, 0.4, 0.4));
      VisAtt1->SetForceSolid(true);
      itsBeampipeLogicalVolume->SetVisAttributes(VisAtt1);

      G4VisAttributes* VisAtt2 =
        new G4VisAttributes(G4Colour(0., 0., 1.0));
      VisAtt2->SetForceSolid(true);
      itsOuterLogicalVolume->SetVisAttributes(VisAtt2);

      if(BDSGlobals->GetIncludeIronMagFields())
	{
	  G4double polePos[4];
	  G4double Bfield[3];

	  polePos[0]=-BDSGlobals->GetMagnetPoleRadius()*sin(pi/8);
	  polePos[1]=BDSGlobals->GetMagnetPoleRadius()*cos(pi/8);
	  polePos[2]=0.;
	  polePos[3]=-999.;//flag to use polePos rather than local track
	                   //coordinate in GetFieldValue	    
	    
	  itsMagField->GetFieldValue(polePos,Bfield);
	  G4double BFldIron=
	    sqrt(Bfield[0]*Bfield[0]+Bfield[1]*Bfield[1])*
	    BDSGlobals->GetMagnetPoleSize()/
	    (BDSGlobals->GetComponentBoxSize()/2-
	     BDSGlobals->GetMagnetPoleRadius());
	  // Magnetic flux from a pole is divided in two directions
	  BFldIron/=2.;

	  BuildOuterFieldManager(10, BFldIron,pi/10);
	}

      (*LogVolCount)[itsName]=1;
      (*LogVol)[itsName]=itsMarkerLogicalVolume;
    }
  else
    {
      (*LogVolCount)[itsName]++;
      if(BDSGlobals->GetSynchRadOn()&& BDSGlobals->GetSynchRescale())
	{
	  // with synchrotron radiation, the rescaled magnetic field
	  // means elements with the same name must have different
	  //logical volumes, becuase they have different fields
	  itsName+=BDSGlobals->StringFromInt((*LogVolCount)[itsName]);
	  BuildBPFieldAndStepper();
	  BuildBPFieldMgr(itsStepper,itsMagField);
	  BuildDefaultMarkerLogicalVolume();
	  
	  BuildBeampipe(itsLength);
	  BuildDefaultOuterLogicalVolume(itsLength);

	  SetSensitiveVolume(itsBeampipeLogicalVolume);// for synchrotron
	  //SetSensitiveVolume(itsOuterLogicalVolume);// for laserwire      
	  (*LogVol)[itsName]=itsMarkerLogicalVolume;
	}
      else
	{
	  itsMarkerLogicalVolume=(*LogVol)[itsName];
	}      
    }
  
}


G4VisAttributes* BDSDecapole::SetVisAttributes()
{
  itsVisAttributes=new G4VisAttributes(G4Colour(0,1,1));
  return itsVisAttributes;
}


void BDSDecapole::BuildBPFieldAndStepper()
{
  // set up the magnetic field and stepper
  itsMagField=new BDSDecMagField(itsBQuadPrime);
  itsEqRhs=new G4Mag_UsualEqRhs(itsMagField);
  
  itsStepper=new BDSDecStepper(itsEqRhs);
  itsStepper->SetBQuadPrime(itsBQuadPrime);

}

BDSDecapole::~BDSDecapole()
{
  delete itsVisAttributes;
  delete itsMarkerLogicalVolume;
  delete itsOuterLogicalVolume;
  delete itsPhysiComp;
  delete itsMagField;
  delete itsEqRhs;
  delete itsStepper;
}
