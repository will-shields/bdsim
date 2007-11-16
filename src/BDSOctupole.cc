/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 

   Modified 22.03.05 by J.C.Carter, Royal Holloway, Univ. of London.
   Changed StringFromInt to be the BDSGlobal version
*/

#include "BDSGlobalConstants.hh" // must be first in include list

#include "BDSOctupole.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4UserLimits.hh"
#include "G4TransportationManager.hh"

#include <map>

const int DEBUG = 0;

//============================================================

typedef std::map<G4String,int> LogVolCountMap;
extern LogVolCountMap* LogVolCount;

typedef std::map<G4String,G4LogicalVolume*> LogVolMap;
extern LogVolMap* LogVol;

extern BDSMaterials* theMaterials;
//============================================================

BDSOctupole::BDSOctupole(G4String aName, G4double aLength, 
			 G4double bpRad, G4double FeRad,
			 G4double BTrpPrime, G4double tilt, 
			 G4double outR, G4String aMaterial):
  BDSMultipole(aName, aLength, bpRad, FeRad, SetVisAttributes(), aMaterial),
  itsBTrpPrime(BTrpPrime)
{
  SetOuterRadius(outR);
  itsTilt=tilt;
  itsType="octu";

  if (!(*LogVolCount)[itsName])
    {
      //
      // build external volume
      //
      BuildDefaultMarkerLogicalVolume();

      //
      // build beampipe (geometry + magnetic field)
      //
      BuildBPFieldAndStepper();
      BuildBPFieldMgr(itsStepper,itsMagField);
      BuildBeampipe(itsLength);

      //
      // build magnet (geometry + magnetic field)
      //
      BuildDefaultOuterLogicalVolume(itsLength);
      if(BDSGlobals->GetIncludeIronMagFields())
	{
	  G4double polePos[4];
	  G4double Bfield[3];

	  //coordinate in GetFieldValue
	  polePos[0]=-BDSGlobals->GetMagnetPoleRadius()*sin(pi/8);
	  polePos[1]=BDSGlobals->GetMagnetPoleRadius()*cos(pi/8);
	  polePos[2]=0.;
	  polePos[3]=-999.;//flag to use polePos rather than local track

	  itsMagField->GetFieldValue(polePos,Bfield);
	  G4double BFldIron=
	    sqrt(Bfield[0]*Bfield[0]+Bfield[1]*Bfield[1])*
	    BDSGlobals->GetMagnetPoleSize()/
	    (BDSGlobals->GetComponentBoxSize()/2-
	     BDSGlobals->GetMagnetPoleRadius());

	  // Magnetic flux from a pole is divided in two directions
	  BFldIron/=2.;

	  BuildOuterFieldManager(8, BFldIron,pi/8);
	}

      //
      // define sensitive volumes for hit generation
      //
      SetMultipleSensitiveVolumes(itsBeampipeLogicalVolume);
      SetMultipleSensitiveVolumes(itsOuterLogicalVolume);

      //
      // set visualization attributes
      //
      itsVisAttributes=SetVisAttributes();
      itsVisAttributes->SetForceSolid(true);
      itsOuterLogicalVolume->SetVisAttributes(itsVisAttributes);

      //
      // append marker logical volume to volume map
      //
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

	  //
	  // build external volume
	  // 
	  BuildDefaultMarkerLogicalVolume();

	  //
	  // build beampipe (geometry + magnetic field)
	  //
	  BuildBPFieldAndStepper();
	  BuildBPFieldMgr(itsStepper,itsMagField);
	  BuildBeampipe(itsLength);

	  //
	  // build magnet (geometry + magnetic field)
	  //
	  BuildDefaultOuterLogicalVolume(itsLength);
	  if(BDSGlobals->GetIncludeIronMagFields())
	    {
	      G4double polePos[4];
	      G4double Bfield[3];
	      
	      //coordinate in GetFieldValue
	      polePos[0]=-BDSGlobals->GetMagnetPoleRadius()*sin(pi/8);
	      polePos[1]=BDSGlobals->GetMagnetPoleRadius()*cos(pi/8);
	      polePos[2]=0.;
	      polePos[3]=-999.;//flag to use polePos rather than local track

	      itsMagField->GetFieldValue(polePos,Bfield);
	      G4double BFldIron=
		sqrt(Bfield[0]*Bfield[0]+Bfield[1]*Bfield[1])*
		BDSGlobals->GetMagnetPoleSize()/
		(BDSGlobals->GetComponentBoxSize()/2-
		 BDSGlobals->GetMagnetPoleRadius());

	      // Magnetic flux from a pole is divided in two directions
	      BFldIron/=2.;
	      
	      BuildOuterFieldManager(8, BFldIron,pi/8);
	    }
	  //When is SynchRescale(factor) called?

	  //
	  // define sensitive volumes for hit generation
	  //
	  SetSensitiveVolume(itsBeampipeLogicalVolume);// for synchrotron
	  //SetSensitiveVolume(itsOuterLogicalVolume);// for laserwire
	  
	  //
	  // set visualization attributes
	  //
	  itsVisAttributes=SetVisAttributes();
	  itsVisAttributes->SetForceSolid(true);
	  itsOuterLogicalVolume->SetVisAttributes(itsVisAttributes);
	  
	  //
	  // append marker logical volume to volume map
	  //
	  (*LogVol)[itsName]=itsMarkerLogicalVolume;
	}
      else
	{
	  //
	  // use already defined marker volume
	  //
	  itsMarkerLogicalVolume=(*LogVol)[itsName];
	}      
    }
}

void BDSOctupole::SynchRescale(G4double factor)
{
  itsStepper->SetBTrpPrime(factor*itsBTrpPrime);
  itsMagField->SetBTrpPrime(factor*itsBTrpPrime);
  if(DEBUG) G4cout << "Oct " << itsName << " has been scaled" << G4endl;
}

G4VisAttributes* BDSOctupole::SetVisAttributes()
{
  itsVisAttributes=new G4VisAttributes(G4Colour(0,1,1));
  return itsVisAttributes;
}

void BDSOctupole::BuildBPFieldAndStepper()
{
  // set up the magnetic field and stepper
  itsMagField=new BDSOctMagField(itsBTrpPrime);
  itsEqRhs=new G4Mag_UsualEqRhs(itsMagField);
  
  itsStepper=new BDSOctStepper(itsEqRhs);
  itsStepper->SetBTrpPrime(itsBTrpPrime);
}

BDSOctupole::~BDSOctupole()
{
  delete itsVisAttributes;
  delete itsMarkerLogicalVolume;
  delete itsOuterLogicalVolume;
  delete itsPhysiComp;
  delete itsMagField;
  delete itsEqRhs;
  delete itsStepper;
}
