/* BDSIM code.    Version 0.1
*/

#include "BDSGlobalConstants.hh" // must be first in include list

#include "BDSRBend.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4IntersectionSolid.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4UserLimits.hh"
#include "G4TransportationManager.hh"
#include "G4PropagatorInField.hh"

#include <map>

//============================================================

typedef std::map<G4String,int> LogVolCountMap;
extern LogVolCountMap* LogVolCount;

typedef std::map<G4String,G4LogicalVolume*> LogVolMap;
extern LogVolMap* LogVol;

extern BDSMaterials* theMaterials;
extern G4RotationMatrix* RotY90;

//============================================================

BDSRBend::BDSRBend(G4String aName,G4double aLength, 
		   G4double bpRad,G4double FeRad,
		   G4double bField, G4double angle, G4double outR,
		   G4double tilt,  G4double bGrad, G4int nSegments):
  BDSMultipole(aName,aLength, bpRad, FeRad,SetVisAttributes(),0,0,angle)
{
  SetOuterRadius(outR);
  itsTilt = tilt;
  itsBField = bField;
  itsBGrad = bGrad;

  itsType="rbend";

  if (!(*LogVolCount)[itsName])
    {
      BuildDefaultMarkerLogicalVolume();

      BuildBeampipe(itsLength);

      BuildBPFieldAndStepper();

      BuildBPFieldMgr(itsStepper,itsMagField);

      BuildDefaultOuterLogicalVolume(itsLength);
      
      //SetSensitiveVolume(itsBeampipeLogicalVolume);// for synchrotron
      SetSensitiveVolume(itsOuterLogicalVolume);// otherwise





      if(BDSGlobals->GetIncludeIronMagFields())
        {
          G4double polePos[4];
          G4double Bfield[3];

          polePos[0]=0.;
          polePos[1]=BDSGlobals->GetMagnetPoleRadius();
          polePos[2]=0.;
          polePos[0]=-999.;//flag to use polePos rather than local track
          //coordinate in GetFieldValue

          itsMagField->GetFieldValue(polePos,Bfield);
          G4double BFldIron=
          sqrt(Bfield[0]*Bfield[0]+Bfield[1]*Bfield[1])*
          BDSGlobals->GetMagnetPoleSize()/
          (BDSGlobals->GetComponentBoxSize()/2-
          BDSGlobals->GetMagnetPoleRadius());

          // Magnetic flux from a pole is divided in two directions
          BFldIron/=2.;

          //BuildOuterFieldManager(2, BFldIron,0);
        }

      (*LogVolCount)[itsName]=1;
      (*LogVol)[itsName]=itsMarkerLogicalVolume;


      G4VisAttributes* VisAtt1 = 
	new G4VisAttributes(G4Colour(0.1, 0.1, 0.1));
      VisAtt1->SetForceSolid(true);
      itsBeampipeLogicalVolume->SetVisAttributes(VisAtt1);
      
      G4VisAttributes* VisAtt2 = 
	new G4VisAttributes(G4Colour(0., 0., 1.));
      VisAtt2->SetForceSolid(true);
      itsOuterLogicalVolume->SetVisAttributes(VisAtt2);
      

    }
  else
    {
      (*LogVolCount)[itsName]++;

	itsMarkerLogicalVolume=(*LogVol)[itsName];

    }


}


G4VisAttributes* BDSRBend::SetVisAttributes()
{
  itsVisAttributes=new G4VisAttributes(G4Colour(0,1,1));
  return itsVisAttributes;
}


void BDSRBend::BuildBPFieldAndStepper()
{
  // set up the magnetic field and stepper
  G4ThreeVector Bfield(0.,-itsBField,0.);
  itsMagField=new BDSSbendMagField(Bfield,itsLength,itsAngle);
  
  
  itsEqRhs=new G4Mag_UsualEqRhs(itsMagField);  
  
  
  //itsStepper=new BDSHelixStepper(itsEqRhs);
  itsStepper = new myQuadStepper(itsEqRhs); // note the - sign...
  itsStepper->SetBField(-itsBField);
 
  itsStepper->SetBGrad(itsBGrad);

  BuildBPFieldMgr(itsStepper,itsMagField);

  itsBeampipeLogicalVolume->SetFieldManager(BDSGlobals->GetZeroFieldManager(),false);
  itsInnerBPLogicalVolume->SetFieldManager(itsBPFieldMgr,false);
  
}

BDSRBend::~BDSRBend()
{
  
  delete itsVisAttributes;
  delete itsMarkerLogicalVolume;
  delete itsOuterLogicalVolume;
  delete itsPhysiComp;
  delete itsMagField;
  delete itsEqRhs;
  delete itsStepper;
}
