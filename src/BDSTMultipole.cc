/* BDSIM

19 May 2008 by Marchiori G.
17 Apr 2006 by Ilya Agapov

*/

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

#include "BDSMultipoleMagField.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "BDSRK4Stepper.hh"

#include <map>

const int DEBUG = 0;

//============================================================

typedef std::map<G4String,int> LogVolCountMap;
extern LogVolCountMap* LogVolCount;

typedef std::map<G4String,G4LogicalVolume*> LogVolMap;
extern LogVolMap* LogVol;

extern BDSMaterials* theMaterials;
//============================================================

BDSTMultipole::BDSTMultipole(G4String aName, G4double aLength, 
			     G4double bpRad, G4double FeRad,
			     G4double tilt, G4double outR,
			     list<G4double> akn, list<G4double> aks, 
			     G4String aMaterial):
  BDSMultipole(aName,aLength, bpRad, FeRad,SetVisAttributes(),aMaterial)
{
  SetOuterRadius(outR);
  itsTilt=tilt;
  itsType="multipole";

  if (DEBUG) G4cout<<"Building Multipole of order "<<akn.size()<<G4endl;

  if(akn.size() != aks.size()) { G4cerr<<"ERROR : skew and normal multipole coeficiens must be of the same numbers"<<G4endl;}
  if(akn.size() < 1 ) { G4cerr<<"ERROR : multipole order must be greater than 0"<<G4endl; }

  kn = akn;
  ks = aks;

  if (DEBUG) {
    int order = 0;
    G4cout<<"M: kn={ ";
    list<double>::iterator kit;
    for(kit=kn.begin();kit!=kn.end();kit++)
      {
	G4cout<<(*kit)<<"m^-"<<++order<<" ";
      }
    G4cout<<"}"<<G4endl;
    order = 0;
    G4cout<<"M: ks={ ";
    for(kit=ks.begin();kit!=ks.end();kit++)
      {
	G4cout<<(*kit)<<"m^-"<<++order<<" ";
      }
    G4cout<<"}"<<G4endl;
  }

  itsOrder = kn.size();
  
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
      //
      // use already defined marker volume
      //
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
  // set up the magnetic field and stepper
  itsMagField=new BDSMultipoleMagField(kn,ks);
  itsEqRhs=new G4Mag_UsualEqRhs(itsMagField);
  
  itsStepper=new BDSRK4Stepper(itsEqRhs);
  itsStepper->SetVolLength(itsLength);
}

BDSTMultipole::~BDSTMultipole()
{
  delete itsVisAttributes;
  delete itsMarkerLogicalVolume;
  delete itsOuterLogicalVolume;
  delete itsPhysiComp;
  delete itsMagField;
  delete itsEqRhs;
  delete itsStepper;
}
