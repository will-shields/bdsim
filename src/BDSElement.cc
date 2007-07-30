/* BDSIM code.    Version 1.0
   Author: John C. Carter, Royal Holloway, Univ. of London.
   Last modified 02.12.2004
   Copyright (c) 2004 by J.C.Carter.  ALL RIGHTS RESERVED. 
*/

const int DEBUG = 0;

#include "BDSGlobalConstants.hh" // must be first in include list
#include "BDSElement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4Trd.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UserLimits.hh"
#include "G4Mag_UsualEqRhs.hh"

#include "BDSAcceleratorComponent.hh"

// geometry drivers
#include "ggmad.hh"
#include "BDSGeometrySQL.hh"

#ifdef USE_XML
#include "BDSGeometryGDML.hh"
#endif

#include <map>

using namespace std;

//============================================================

typedef std::map<G4String,int> LogVolCountMap;
extern LogVolCountMap* LogVolCount;

typedef std::map<G4String,G4LogicalVolume*> LogVolMap;
extern LogVolMap* LogVol;

extern BDSMaterials* theMaterials;
extern G4RotationMatrix* RotY90;

//============================================================

BDSElement::BDSElement(G4String aName, G4String geometry, G4String bmap,
		       G4double aLength, G4double bpRad, G4double outR):
  BDSAcceleratorComponent(
			  aName,
			  aLength,bpRad,0,0,
			  SetVisAttributes()),
  itsField(NULL)
{
  itsOuterR = outR;
  SetType(_ELEMENT);

  // WARNING: ALign in and out will only apply to the first instance of the
  //          element. Subsequent copies will have no alignement set.
  align_in_volume = NULL;
  align_out_volume = NULL;

  if(!(*LogVolCount)[itsName])
    {
      if(DEBUG) G4cout<<"BDSElement : starting build logical volume "<<
		  itsName<<G4endl;

      BuildGeometry(); // build element box
      
      if(DEBUG) G4cout<<"BDSElement : end build logical volume "<<
		  itsName<<G4endl;

      PlaceComponents(geometry,bmap); // place components (from file) and build filed maps
    }
  else
    {
      (*LogVolCount)[itsName]++;
      
      itsMarkerLogicalVolume=(*LogVol)[itsName];
    }
}

void BDSElement::BuildGeometry()
{
  // multiple element instances not implemented yet!!!

  // Build the logical volume 

  if(DEBUG) G4cout<<"BDSElement : creating logical volume"<<G4endl;
  G4double elementSizeX=itsOuterR, elementSizeY = itsOuterR;
  if(itsOuterR==0)
    {
      elementSizeX =BDSGlobals->GetTunnelRadius() / 2;
      elementSizeY =BDSGlobals->GetTunnelRadius() / 2;
    }
      
  itsMarkerLogicalVolume = 
    new G4LogicalVolume(new G4Box(itsName+"generic_element",
				  elementSizeX,
				  elementSizeY,
				  itsLength/2),
			theMaterials->GetMaterial("Vacuum"),
			itsName);
  
  (*LogVolCount)[itsName] = 1;
  (*LogVol)[itsName] = itsMarkerLogicalVolume;
  
  itsOuterUserLimits = new G4UserLimits();
  itsOuterUserLimits->SetMaxAllowedStep(itsLength);
  itsMarkerLogicalVolume->SetUserLimits(itsOuterUserLimits);
 
  
}

// place components 
void BDSElement::PlaceComponents(G4String geometry, G4String bmap)
{

  G4String gFormat="", bFormat="";
  G4String gFile="", bFile="";

  // get geometry format and file
  G4int pos = geometry.find(":");

  if(pos<0) { 
    G4cerr<<"WARNING: invalid geometry reference format : "<<geometry<<endl;
    gFormat="none";
  }
  else {
    gFormat = geometry.substr(0,pos);
    gFile = geometry.substr(pos+1,geometry.length() - pos); 
  }

  // get fieldmap format and file
  pos = bmap.find(":");

  if(pos<0) {
    G4cerr<<"WARNING: invalid B map reference format : "<<bmap<<endl; 
    bFormat="none";
  }
  else {
    bFormat = bmap.substr(0,pos);
    bFile = bmap.substr(pos+1,bmap.length() - pos); 
  }

  G4cout<<"placing components\n: geometry format - "<<gFormat<<G4endl<<
    "file - "<<gFile<<G4endl;

  G4cout<<"bmap format - "<<bFormat<<G4endl<<
    "file - "<<bFile<<G4endl;


  // get the geometry for the driver
  // different drivers may interpret the fieldmap differently
  // so a field map without geometry is not allowed

  GGmadDriver *ggmad;
  BDSGeometrySQL *Mokka;

#ifdef USE_XML
  BDSGeometryGDML *LCDD;
#endif

  if(gFormat=="gmad") {

    ggmad = new GGmadDriver(gFile);
    ggmad->Construct(itsMarkerLogicalVolume);

    // set sensitive volumes
    //vector<G4LogicalVolume*> SensComps = ggmad->SensitiveComponents;
    //for(G4int id=0; id<SensComps.size(); id++)
    //  SetMultipleSensitiveVolumes(SensComps[id]);

    
    SetMultipleSensitiveVolumes(itsMarkerLogicalVolume);

    // attach magnetic field if present

    if(bFormat=="XY")
      {
	itsField = new BDSXYMagField(bFile);

	// build the magnetic field manager and transportation
	BuildMagField();
      }
  }
  else if(gFormat=="lcdd") {
#ifdef USE_XML
    LCDD = new BDSGeometryGDML(gFile);
    LCDD->Construct(itsMarkerLogicalVolume);
#else
    G4cout << "XML support not selected during BDSIM configuration" << G4endl;
    G4Exception("Please re-compile BDSIM with USE_XML flag in Makefile");
#endif
  }
  else if(gFormat=="mokka") {

    Mokka = new BDSGeometrySQL(gFile,itsLength);
    Mokka->Construct(itsMarkerLogicalVolume);
    vector<G4LogicalVolume*> SensComps = Mokka->SensitiveComponents;
    for(G4int id=0; id<(G4int)SensComps.size(); id++)
      SetMultipleSensitiveVolumes(SensComps[id]);
    align_in_volume = Mokka->align_in_volume;
    align_out_volume = Mokka->align_out_volume;
    // attach magnetic field if present

    if(bFormat=="mokka" || bFormat=="none")
      {
	if(Mokka->HasFields || bFile!="")
	  // Check for field file or volumes with fields
	  // as there may be cases where there are no bFormats given
	  // in gmad file but fields might be set to volumes in SQL files
	  {
	    itsField = new BDSMagFieldSQL(bFile,itsLength,
					  Mokka->Quadvol, Mokka->QuadBgrad,
					  Mokka->Sextvol, Mokka->SextBgrad,
					  Mokka->Octvol, Mokka->OctBgrad,
					  Mokka->Fieldvol,Mokka->UniformField);
	    
	    // build the magnetic field manager and transportation
	    BuildMagField(6,true);
	  }
      }
  } 
  else if(gFormat=="gdml") {
    
    G4cout<<"GDML.... sorry, not implemented yet..."<<G4endl;

  }
  else {
    G4cerr<<"geometry format "<<gFormat<<" not supported"<<G4endl;
  }




  //   // test - dump field values
//     G4cout<<"dumping field values..."<<G4endl;
//     G4double Point[4];
//     G4double BField[6];
//     ofstream testf("fields.dat");

//     for(G4double x=-1*m;x<1*m;x+=1*cm)
//        for(G4double y=-1*m;y<1*m;y+=1*cm)
// 	 for(G4double z=-1*m;z<1*m;z+=1*cm)
//       {
// 	Point[0] = x;
// 	Point[1] = y;
// 	Point[2] = z;
// 	Point[3] = 0;
// 	itsField->GetFieldValue(Point,BField);
// 	testf<<Point[0]<<" "<<Point[1]<<" "<<Point[2]<<" "<<
// 	  BField[0]<<" "<<BField[1]<<" "<<BField[2]<<G4endl;
//       }

//     testf.close();
//     G4cout<<"done"<<G4endl;


}



G4VisAttributes* BDSElement::SetVisAttributes()
{
  itsVisAttributes=new G4VisAttributes(G4Colour(0.5,0.5,1));
  return itsVisAttributes;
}


void BDSElement::BuildMagField(G4int nvar, G4bool forceToAllDaughters)
{
  //itsField = new BDSField();

  // Create an equation of motion for this field
  G4EqMagElectricField* fEquation = new G4EqMagElectricField(itsField);

  G4MagIntegratorStepper* fStepper = new G4ClassicalRK4( fEquation, nvar );
  //G4MagIntegratorStepper* fStepper = new BDSRK4Stepper( fEquation, nvar );

  // create a field manager
  G4FieldManager* fieldManager = new G4FieldManager();
  fieldManager->SetDetectorField(itsField );


  G4double fMinStep  = BDSGlobals->GetChordStepMinimum(); 
  
  G4MagInt_Driver* fIntgrDriver = new G4MagInt_Driver(fMinStep, 
						      fStepper, 
						      fStepper->GetNumberOfVariables() );
  
  G4ChordFinder* fChordFinder = new G4ChordFinder(fIntgrDriver);

  fChordFinder->SetDeltaChord(BDSGlobals->GetDeltaChord());

  fieldManager->SetChordFinder( fChordFinder ); 

  itsMarkerLogicalVolume->SetFieldManager(fieldManager,forceToAllDaughters);
  
  G4UserLimits* fUserLimits =
    new G4UserLimits("element cuts",DBL_MAX,DBL_MAX,DBL_MAX,
  		     BDSGlobals->GetThresholdCutCharged());
  
  fUserLimits->SetMaxAllowedStep(1e-2 * m);
  
  itsMarkerLogicalVolume->SetUserLimits(fUserLimits);
  
}

// creates a field mesh in the reference frame of a physical volume
// from  b-field map value list 
// has to be called after the component is placed in the geometry
void BDSElement::PrepareField(G4VPhysicalVolume *referenceVolume)
{
  if(!itsField) return;
  itsField->Prepare(referenceVolume);
}

// Rotates and positions the marker volume before it is placed in
// BDSDetectorConstruction. It aligns the marker volume so that the
// the beamline goes through the specified daugther volume (e.g. for mokka)
void BDSElement::AlignComponent(G4ThreeVector& TargetPos, 
				G4RotationMatrix *TargetRot, 
				G4RotationMatrix& globalRotation,
				G4ThreeVector& rtot,
				G4ThreeVector& rlast,
				G4ThreeVector& localX,
				G4ThreeVector& localY,
				G4ThreeVector& localZ)
{
  
  
  if(align_in_volume == NULL)
    {
      if(align_out_volume == NULL)
	{
	  // advance co-ords in usual way if no alignment volumes found
	  
	  rtot = rlast + localZ*(itsLength/2 + BDSGlobals->GetLengthSafety()/2);
	  rlast = rtot + localZ*(itsLength/2 + BDSGlobals->GetLengthSafety()/2);
	  return;
	}
      else 
	{
	  G4cout << "BDSElement : Aligning outgoing to SQL element " 
		 << align_out_volume->GetName() << G4endl;
	  G4RotationMatrix Trot = *TargetRot;
	  G4RotationMatrix trackedRot;
	  G4RotationMatrix outRot = *(align_out_volume->GetFrameRotation());
	  trackedRot.transform(outRot.inverse());
	  trackedRot.transform(Trot.inverse());
	  globalRotation = trackedRot;

	  G4ThreeVector outPos = align_out_volume->GetFrameTranslation();
	  G4ThreeVector diff = outPos;

	  G4ThreeVector zHalfAngle = G4ThreeVector(0.,0.,1.);

	  zHalfAngle.transform(globalRotation);

	  //moving positioning to outgoing alignment volume
	  rlast = TargetPos - ((outPos.unit()).transform(Trot.inverse()) )*diff.mag();
	  localX.transform(outRot.inverse());
	  localY.transform(outRot.inverse());
	  localZ.transform(outRot.inverse());

	  localX.transform(Trot.inverse());
	  localY.transform(Trot.inverse());
	  localZ.transform(Trot.inverse());

	  //moving position in Z be at least itsLength/2 away
	  rlast +=zHalfAngle*(itsLength/2 + diff.z() + BDSGlobals->GetLengthSafety()/2);
	  return;
	}
    }

  if(align_in_volume != NULL)
    {
      G4cout << "BDSElement : Aligning incoming to SQL element " 
      	     << align_in_volume->GetName() << G4endl;
      
      const G4RotationMatrix* inRot = align_in_volume->GetFrameRotation();
      TargetRot->transform((*inRot).inverse());
      
      G4ThreeVector inPos = align_in_volume->GetFrameTranslation();
      inPos.transform((*TargetRot).inverse());
      TargetPos+=G4ThreeVector(inPos.x(), inPos.y(), 0.0);
      
      if(align_out_volume == NULL)
	{
	  // align outgoing (i.e. next component) to Marker Volume
	  
	  G4RotationMatrix Trot = *TargetRot;
	  globalRotation.transform(Trot.inverse());
	  
	  G4ThreeVector zHalfAngle = G4ThreeVector(0.,0.,1.);
	  zHalfAngle.transform(Trot.inverse());
	  
	  rlast = TargetPos + zHalfAngle*(itsLength/2 + BDSGlobals->GetLengthSafety()/2);
	  localX.transform(Trot.inverse());
	  localY.transform(Trot.inverse());
	  localZ.transform(Trot.inverse());
	  return;
	}

      else
	{
	  G4cout << "BDSElement : Aligning outgoing to SQL element " 
		 << align_out_volume->GetName() << G4endl;
	  G4RotationMatrix Trot = *TargetRot;
	  G4RotationMatrix trackedRot;
	  G4RotationMatrix outRot = *(align_out_volume->GetFrameRotation());
	  trackedRot.transform(outRot.inverse());
	  trackedRot.transform(Trot.inverse());
	  globalRotation = trackedRot;

	  G4ThreeVector outPos = align_out_volume->GetFrameTranslation();
	  G4ThreeVector diff = outPos;

	  G4ThreeVector zHalfAngle = G4ThreeVector(0.,0.,1.);

	  zHalfAngle.transform(globalRotation);

	  //moving positioning to outgoing alignment volume
	  rlast = TargetPos - ((outPos.unit()).transform(Trot.inverse()) )*diff.mag();
	  localX.transform(outRot.inverse());
	  localY.transform(outRot.inverse());
	  localZ.transform(outRot.inverse());

	  localX.transform(Trot.inverse());
	  localY.transform(Trot.inverse());
	  localZ.transform(Trot.inverse());

	  //moving position in Z be at least itsLength/2 away
	  rlast +=zHalfAngle*(itsLength/2 + diff.z() + BDSGlobals->GetLengthSafety()/2);
	  return;
	}
    }
  
}

BDSElement::~BDSElement()
{

  delete itsVisAttributes;
 
}
