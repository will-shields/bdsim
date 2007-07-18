/** BDSIM, v0.1   

Last modified 17.04.2006 by Ilya Agapov

**/

//==============================================================


#include "BDSGlobalConstants.hh" // must be first in include list

#include "BDSTMultipole.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4UserLimits.hh"
#include "G4TransportationManager.hh"

#include "G4EqMagElectricField.hh"
#include "BDSRK4Stepper.hh"

#include <map>

//============================================================

typedef std::map<G4String,int> LogVolCountMap;
extern LogVolCountMap* LogVolCount;

typedef std::map<G4String,G4LogicalVolume*> LogVolMap;
extern LogVolMap* LogVol;

extern BDSMaterials* theMaterials;
//============================================================

BDSTMultipole::BDSTMultipole(G4String aName,G4double aLength, 
			     G4double bpRad,G4double FeRad,G4double outR,
			     list<G4double> knl, list<G4double> ksl):
  BDSMultipole(aName,aLength, bpRad, FeRad,SetVisAttributes())
{

  SetOuterRadius(outR);

  G4cout<<"Building Multipole of order "<<knl.size()<<G4endl;

  if(knl.size() != ksl.size()) { G4cerr<<"ERROR : skew and normal multipole coeficiens must be of the same numbers"<<G4endl;}
  if(knl.size() < 1 ) { G4cerr<<"ERROR : multipole order must be greater than 0"<<G4endl; }

  bnl = knl;
  bsl = ksl;

  G4cout<<"M: bnl=";
  list<double>::iterator kit;
  for(kit=bnl.begin();kit!=bnl.end();kit++)
    {
      G4cout<<(*kit)<<" ";
    }
  G4cout<<G4endl;

  itsOrder = bnl.size();

  G4cout<<"now the order is"<<bnl.size()<<G4endl;
  
  if (!(*LogVolCount)[itsName])
    {
      //BuildBPFieldAndStepper();
      //BuildBPFieldMgr();
      BuildDefaultMarkerLogicalVolume();
      
      BuildBeampipe(itsLength);
      BuildDefaultOuterLogicalVolume(itsLength);

      BuildBPFieldAndStepper();
      
      G4VisAttributes* VisAtt =
        new G4VisAttributes(G4Colour(0., 0., 0.));
      VisAtt->SetForceSolid(true);
      itsInnerBPLogicalVolume->SetVisAttributes(VisAtt);

      G4VisAttributes* VisAtt1 =
        new G4VisAttributes(G4Colour(0.4, 0.4, 0.4));
      VisAtt1->SetForceSolid(true);
      itsBeampipeLogicalVolume->SetVisAttributes(VisAtt1);

      G4VisAttributes* VisAtt2 =
        new G4VisAttributes(G4Colour(0.1, 0.5, 0.9));
      VisAtt2->SetForceSolid(true);
      itsOuterLogicalVolume->SetVisAttributes(VisAtt2);

      SetMultipleSensitiveVolumes(itsBeampipeLogicalVolume);
      SetMultipleSensitiveVolumes(itsOuterLogicalVolume);
    }
  else
    {
      itsMarkerLogicalVolume=(*LogVol)[itsName];
    }      
  

  
}
  
G4VisAttributes* BDSTMultipole::SetVisAttributes()
{
  itsVisAttributes=new G4VisAttributes(G4Colour(0.1,0.4,0.2));
  return itsVisAttributes;
}


void BDSTMultipole::BuildBPFieldAndStepper()
{
  G4cout<<"Building Multipole Field and stepper"<<G4endl;

  //G4int nvar = 6;

  itsMagField = new BDSMultipoleMagField(bnl,bsl);

   
  //G4MagneticField* fEquation = new BDSMultipoleMagMagnetiField(itsMagField);
  
  G4EqMagElectricField* fEquation = new G4EqMagElectricField(itsMagField);

  BDSRK4Stepper * fStepper = new BDSRK4Stepper( fEquation );


  fStepper->SetVolLength(itsLength);

  // create a field manager
  G4FieldManager* fieldManager = new G4FieldManager();
  fieldManager->SetDetectorField(itsMagField );


  G4double fMinStep  = BDSGlobals->GetChordStepMinimum(); 
  
  G4MagInt_Driver* fIntgrDriver = new G4MagInt_Driver(fMinStep, 
						      fStepper, 
						      fStepper->GetNumberOfVariables() );
  
  G4ChordFinder* fChordFinder = new G4ChordFinder(fIntgrDriver);

  fChordFinder->SetDeltaChord(BDSGlobals->GetDeltaChord());

  fieldManager->SetChordFinder( fChordFinder ); 

  G4bool forceToAllDaughters=true;

  itsInnerBPLogicalVolume->SetFieldManager(fieldManager,forceToAllDaughters);
  
  G4UserLimits* fUserLimits =
    new G4UserLimits("element cuts",DBL_MAX,DBL_MAX,DBL_MAX,
  		     BDSGlobals->GetThresholdCutCharged());
  
  fUserLimits->SetMaxAllowedStep(1e-2 * m);
  
  itsMarkerLogicalVolume->SetUserLimits(fUserLimits);


  
}

BDSTMultipole::~BDSTMultipole()
{
  delete itsVisAttributes;
  delete itsMarkerLogicalVolume;
  delete itsOuterLogicalVolume;
  delete itsPhysiComp;

}
