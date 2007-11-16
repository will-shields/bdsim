#include "BDSGlobalConstants.hh" // must be first in include list

#include "BDSKicker.hh"
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

BDSKicker::BDSKicker(G4String aName, G4double aLength, 
		     G4double bpRad, G4double FeRad,
		     G4double bField, G4double angle, G4double outR,
		     G4double tilt, G4double bGrad, 
		     G4String aMaterial, G4int nSegments):
  BDSMultipole(aName, aLength, bpRad, FeRad, SetVisAttributes(), aMaterial,
	       0, 0, angle)
{
  SetOuterRadius(outR);
  itsTilt=tilt;
  itsBField=bField;
  itsBGrad=bGrad;
  if (tilt==0)
    itsType="hkick";
  else if (tilt==pi/2)
    itsType="vkick";
  else
    itsType="kick";

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
	  polePos[0]=0.;
	  polePos[1]=BDSGlobals->GetMagnetPoleRadius();
	  polePos[2]=0.;
	  polePos[0]=-999.;//flag to use polePos rather than local track
	  itsMagField->GetFieldValue(polePos,Bfield);
	  G4double BFldIron=
	    sqrt(Bfield[0]*Bfield[0]+Bfield[1]*Bfield[1])*
	    BDSGlobals->GetMagnetPoleSize()/
	    (BDSGlobals->GetComponentBoxSize()/2-
	     BDSGlobals->GetMagnetPoleRadius());

	  // Magnetic flux from a pole is divided in two directions
	  BFldIron/=2.;

	  BuildOuterFieldManager(2, BFldIron,0);
	}

      //
      // define sensitive volumes for hit generation
      //
      SetMultipleSensitiveVolumes(itsBeampipeLogicalVolume);
      SetMultipleSensitiveVolumes(itsOuterLogicalVolume);

      //
      // set visualization attributes
      //
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

      //
      // no rescaling ???
      //
 
      //
      // use already defined marker volume
      //
      itsMarkerLogicalVolume=(*LogVol)[itsName];
    }
}


G4VisAttributes* BDSKicker::SetVisAttributes()
{
  itsVisAttributes=new G4VisAttributes(G4Colour(0,0,1));
  return itsVisAttributes;
}


void BDSKicker::BuildBPFieldAndStepper()
{
  // set up the magnetic field and stepper
  G4ThreeVector Bfield(0.,-itsBField,0.); // note the - sign...
  itsMagField=new BDSSbendMagField(Bfield,itsLength,itsAngle);
  
  itsEqRhs=new G4Mag_UsualEqRhs(itsMagField);  
  
  itsStepper = new myQuadStepper(itsEqRhs);
  itsStepper->SetBField(-itsBField); // note the - sign...
  itsStepper->SetBGrad(itsBGrad);
}

BDSKicker::~BDSKicker()
{
  
  delete itsVisAttributes;
  delete itsMarkerLogicalVolume;
  delete itsOuterLogicalVolume;
  delete itsPhysiComp;
  delete itsMagField;
  delete itsEqRhs;
  delete itsStepper;
}
