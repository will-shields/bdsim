#include "BDSGlobalConstants.hh" // must be first in include list

#include "BDSRfCavity.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4UserLimits.hh"
#include "G4TransportationManager.hh"


#include "G4MagIntegratorDriver.hh"

#include "G4ExplicitEuler.hh"


#include <map>

//============================================================

typedef std::map<G4String,int> LogVolCountMap;
extern LogVolCountMap* LogVolCount;

typedef std::map<G4String,G4LogicalVolume*> LogVolMap;
extern LogVolMap* LogVol;

extern BDSMaterials* theMaterials;
//============================================================

BDSRfCavity::BDSRfCavity (G4String aName,G4double aLength, G4double bpRad, 
			  G4double grad):
  BDSMultipole(aName,aLength, bpRad, bpRad, SetVisAttributes())
{

  itsGrad = grad;
  
  if (!(*LogVolCount)[itsName])
    {
      itsBPFieldMgr=NULL;
      BuildDefaultMarkerLogicalVolume();

      BuildBeampipe(itsLength);

      BuildDefaultOuterLogicalVolume(itsLength);

      // doesn't have an outer volume - but include it for laserwire
      //      BuildDefaultOuterLogicalVolume(itsLength);
      SetSensitiveVolume(itsBeampipeLogicalVolume);// for laserwire
      //SetSensitiveVolume(itsOuterLogicalVolume);// for laserwire



      G4VisAttributes* VisAtt = 
	new G4VisAttributes(G4Colour(0., 0., 0));
      VisAtt->SetForceSolid(true);
      itsInnerBPLogicalVolume->SetVisAttributes(VisAtt);
      
      G4VisAttributes* VisAtt1 = 
	new G4VisAttributes(G4Colour(0.4, 0.4, 0.4));
      VisAtt1->SetForceSolid(true);
      itsBeampipeLogicalVolume->SetVisAttributes(VisAtt1);
      
      G4VisAttributes* VisAtt2 = 
	new G4VisAttributes(G4Colour(1.0, 1.0, 0.));
      VisAtt2->SetForceSolid(true);
      itsOuterLogicalVolume->SetVisAttributes(VisAtt2);

 
      (*LogVolCount)[itsName]=1;
      (*LogVol)[itsName]=itsMarkerLogicalVolume;
      BuildMarkerFieldAndStepper();
    }
  else
    {
      (*LogVolCount)[itsName]++;
      itsMarkerLogicalVolume=(*LogVol)[itsName];
    }
  
}


G4VisAttributes* BDSRfCavity::SetVisAttributes()
{
  itsVisAttributes=new G4VisAttributes(G4Colour(0.25,0.25,0.5));
  return itsVisAttributes;
}


void BDSRfCavity::BuildMarkerFieldAndStepper()
{

  G4int nvar = 8;

  // set up the magnetic field and stepper
  G4ThreeVector Efield(0.,0.,itsGrad * megavolt / m);
  itsField=new G4UniformElectricField(Efield);

  G4EqMagElectricField* fEquation = new G4EqMagElectricField(itsField);

  fieldManager = new G4FieldManager();
  
  fStepper = new G4ExplicitEuler( fEquation, nvar );
  //itsStepper = new G4ClassicalRK4( fEquation, nvar );

  G4double fMinStep     = BDSGlobals->GetChordStepMinimum();
 

  fieldManager->SetDetectorField(itsField );

  if(fChordFinder) delete fChordFinder;

  fIntgrDriver = new G4MagInt_Driver(fMinStep,
                                     fStepper,
                                     fStepper->GetNumberOfVariables() );
  
  fChordFinder = new G4ChordFinder(fIntgrDriver);

  fChordFinder->SetDeltaChord(BDSGlobals->GetDeltaChord());
  fieldManager->SetChordFinder( fChordFinder );


  itsInnerBPLogicalVolume->SetFieldManager(fieldManager,false);

}


BDSRfCavity::~BDSRfCavity()
{
  if(itsVisAttributes) delete itsVisAttributes;
  if(itsMarkerLogicalVolume) delete itsMarkerLogicalVolume;
  if(itsOuterLogicalVolume) delete itsOuterLogicalVolume;
  if(itsPhysiComp) delete itsPhysiComp;
  if(itsField) delete itsField;
  //if(itsEqRhs) delete itsEqRhs;
  if(itsStepper) delete itsStepper;
}
