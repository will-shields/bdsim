/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 

   Modified 22.03.05 by J.C.Carter, Royal Holloway, Univ. of London.
   Added extra parameter to BuildDefaultOuterLogicalVolume so that it is 
     possible to set the material as either Iron or Vacuum
   Removed StringFromInt function
*/
#include "BDSGlobalConstants.hh" // must be first in include list

#include <cstdlib>

#include "BDSMultipole.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4UserLimits.hh"
#include "G4TransportationManager.hh"

#include "G4MagIntegratorStepper.hh"

#include "BDSMultipoleOuterMagField.hh"
#include "G4MagneticField.hh"

#include <map>
#include <string>


const int DEBUG= 0;

//============================================================

typedef std::map<G4String,int> LogVolCountMap;
extern LogVolCountMap* LogVolCount;

typedef std::map<G4String,G4LogicalVolume*> LogVolMap;
extern LogVolMap* LogVol;

extern BDSMaterials* theMaterials;

//============================================================

BDSMultipole::BDSMultipole( G4String aName, 
			    G4double aLength,
			    G4double aBpRadius,
			    G4double aInnerIronRadius,
			    G4VisAttributes* aVisAtt,
			    G4String aMaterial,
			    G4double aXAper,
			    G4double aYAper,
			    G4double angle):
  BDSAcceleratorComponent(
			 aName, 
			 aLength,
			 aBpRadius,
			 aXAper,
			 aYAper,
			 aVisAtt,
			 aMaterial,
			 angle),
  itsInnerIronRadius(aInnerIronRadius)
{}


void BDSMultipole::BuildBeampipe(G4double aLength,
				 G4String materialName,
				 G4int nSegments)
{

  itsNSegments=nSegments;
  G4double dSegments=double(nSegments);

  G4Material *material;

  if(materialName == "")
    {
      material = theMaterials->GetMaterial( BDSGlobals->GetPipeMaterialName() );
    }
  else
    {
      material = theMaterials->GetMaterial(materialName);
    }

  // build beampipe

  if(DEBUG) G4cout << "Outer pipe :"
		   << " r= " << itsBpRadius/m << " m"
		   << " l= " << aLength/(2.*dSegments)/m << " m"
		   << G4endl;

  itsBPTube=new G4Tubs(itsName+"_bmp_solid",
		       itsBpRadius-BDSGlobals->GetBeampipeThickness(),
		       itsBpRadius,
		       aLength/(2.*dSegments),
		       0,twopi*radian);

  if(DEBUG) G4cout << "Inner pipe :"
		   << " r= " << (itsBpRadius-BDSGlobals->GetBeampipeThickness() )/m
		   << " m"
		   << " l= " << aLength/(2.*dSegments)/m << " m"
		   << G4endl;
  
  if(nSegments==1)
    {
      itsInnerBPTube=new G4Tubs(itsName+"_inner_bmp_solid",
				0.,
				itsBpRadius-BDSGlobals->GetBeampipeThickness(),
				aLength/2,
				0,twopi*radian);
    }
  else
    {
      itsInnerBPTube=new G4Tubs(itsName+"_inner_bmp_solid",
				0.,
				itsBpRadius-BDSGlobals->GetBeampipeThickness(),
				aLength/(2.*dSegments)-
				  itsBpRadius*tan(itsAngle/(2.*dSegments)),
				0,twopi*radian); 
    }

  itsBeampipeLogicalVolume=	
    new G4LogicalVolume(itsBPTube,
			material,
			itsName+"_bmp_log");

  itsInnerBPLogicalVolume=	
    new G4LogicalVolume(itsInnerBPTube,
			theMaterials->GetMaterial("Vacuum"),
			itsName+"_inner_bmp_log");
  
  G4VPhysicalVolume* PhysiInner;
  PhysiInner = new G4PVPlacement(
		      0,		        // no rotation
		      0,	                // at (0,0,0)
		      itsInnerBPLogicalVolume,  // its logical volume
		      itsName+"_inner_bmp_phys",// its name
		      itsMarkerLogicalVolume,   // its mother  volume
		      false,		        // no boolean operation
		      0);		        // copy number
    
  if(nSegments==1)
    {
      G4VPhysicalVolume* PhysiComp;
      PhysiComp = new G4PVPlacement(
			  0,			     // no rotation
			  0,	                     // at (0,0,0)
			  itsBeampipeLogicalVolume,  // its logical volume
			  itsName+"_bmp_phys",	     // its name
			  itsMarkerLogicalVolume,    // its mother  volume
			  false,		     // no boolean operation
			  0);		             // copy number
    }
  else
    {
      itsSegRot=new G4RotationMatrix();
      G4double DeltaRot=fabs(itsAngle)/(double(nSegments));
      
      G4double angle=- fabs(itsAngle/2)-DeltaRot/2.;
      itsSegRot->rotateY(acos(0.) + angle );
      
      G4double X,Y,Z,R,Z0;
      
      R=itsLength/fabs(itsAngle);
      Z0=R*(1.-cos(fabs(itsAngle/2)-DeltaRot/2.));
      
      // loop:
      for(G4int iSeg=0;iSeg<nSegments;iSeg++)
	{
	  itsSegRot->rotateY(DeltaRot);
	  angle+=DeltaRot;

      	  X=R*sin(angle);
	  Y=0.;
	  Z=R*(1.-cos(angle))-Z0;

	  itsSegPos.setX(X);
	  itsSegPos.setY(Y);
	  itsSegPos.setZ(Z);

	  G4VPhysicalVolume* PhysiComp;
	  PhysiComp = new G4PVPlacement(
			      itsSegRot,		// its rotation
			      itsSegPos,	        // its position
			      itsBeampipeLogicalVolume, // its logical volume
			      itsName+"_bmp_phys",	// its name
			      itsMarkerLogicalVolume,   // its mother  volume
			      false,		        // no boolean operation
			      0);		        // copy number
	}
    }


  itsBeampipeUserLimits =
     new G4UserLimits("beampipe cuts",DBL_MAX,DBL_MAX,DBL_MAX,
  		     BDSGlobals->GetThresholdCutCharged());

  itsInnerBeampipeUserLimits =
    new G4UserLimits("inner beampipe cuts",DBL_MAX,DBL_MAX,DBL_MAX,
  		     BDSGlobals->GetThresholdCutCharged());

  itsBeampipeUserLimits->SetMaxAllowedStep(aLength);
  itsBeampipeLogicalVolume->SetUserLimits(itsBeampipeUserLimits);

  itsInnerBeampipeUserLimits->SetMaxAllowedStep(aLength);
  itsInnerBPLogicalVolume->SetUserLimits(itsInnerBeampipeUserLimits);

  itsBeampipeLogicalVolume->SetFieldManager(BDSGlobals->GetZeroFieldManager(),false);
  itsInnerBPLogicalVolume->SetFieldManager(itsBPFieldMgr,false) ;

  // now protect the fields inside the marker volume by giving the
  // marker a null magnetic field (otherwise G4VPlacement can
  // over-ride the already-created fields, by calling 
  // G4LogicalVolume::AddDaughter, which calls 
  // pDaughterLogical->SetFieldManager(fFieldManager, true) - the
  // latter 'true' over-writes all the other fields

  itsMarkerLogicalVolume->
    SetFieldManager(BDSGlobals->GetZeroFieldManager(),false);
  
  //
  // set visualization attributes
  //
  G4VisAttributes* VisAtt =  new G4VisAttributes(G4Colour(0., 0., 0));
  VisAtt->SetForceSolid(true);
  itsInnerBPLogicalVolume->SetVisAttributes(VisAtt);

  G4VisAttributes* VisAtt1 = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4));
  VisAtt1->SetForceSolid(true);
  itsBeampipeLogicalVolume->SetVisAttributes(VisAtt1);
}


void BDSMultipole::BuildBPFieldMgr(G4MagIntegratorStepper* aStepper,
				   G4MagneticField* aField)
{
  itsChordFinder= 
    new G4ChordFinder(aField,
		      BDSGlobals->GetChordStepMinimum(),
		      aStepper);

  itsChordFinder->SetDeltaChord(BDSGlobals->GetDeltaChord());

  itsBPFieldMgr= new G4FieldManager();
  itsBPFieldMgr->SetDetectorField(aField);
  itsBPFieldMgr->SetChordFinder(itsChordFinder);
  itsBPFieldMgr->SetDeltaIntersection(BDSGlobals->GetDeltaIntersection());
  
  if(BDSGlobals->GetMinimumEpsilonStep()>0)
    itsBPFieldMgr->SetMinimumEpsilonStep(BDSGlobals->GetMinimumEpsilonStep());

  if(BDSGlobals->GetMaximumEpsilonStep()>0)
    itsBPFieldMgr->SetMaximumEpsilonStep(BDSGlobals->GetMaximumEpsilonStep());

  if(BDSGlobals->GetDeltaOneStep()>0)
    itsBPFieldMgr->SetDeltaOneStep(BDSGlobals->GetDeltaOneStep());
  
}


void BDSMultipole::BuildDefaultMarkerLogicalVolume()
{
  G4double xLength, yLength;
  xLength = yLength = std::max(itsOuterR,BDSGlobals->GetComponentBoxSize()/2);

  if(DEBUG) G4cout<<"marker volume : x/y="<<xLength/m<<
	      " m, l= "<<  (itsLength+BDSGlobals->GetLengthSafety())/2/m <<" m"<<G4endl;

  itsMarkerLogicalVolume=new G4LogicalVolume
    (new G4Box( itsName+"_solid",
                xLength,
		yLength,
		(itsLength+BDSGlobals->GetLengthSafety())/2), //z half length 
     theMaterials->GetMaterial("Vacuum"),
     itsName+"_log");

  // itsMarkerUserLimits =
  //   new G4UserLimits(DBL_MAX,DBL_MAX,DBL_MAX,
  //		     BDSGlobals->GetThresholdCutCharged());     
  // itsMarkerUserLimits->SetMaxAllowedStep(itsLength);
  // itsMarkerLogicalVolume->SetUserLimits(itsMarkerUserLimits);

}


void BDSMultipole::BuildDefaultOuterLogicalVolume(G4double aLength,
						  G4bool OuterMaterialIsVacuum)
{
  //OuterMaterialIsVacuum parameter is useless: one can set
  //itsMaterial = "Vacuum" and obtain the same result. Or cannot?

  // compute sagitta:
  G4double sagitta=0.;

  G4Material* material;
  if(itsMaterial != "") material = theMaterials->GetMaterial(itsMaterial);
  else material = theMaterials->GetMaterial("Iron");

  G4double outerRadius = itsOuterR;
  if(itsOuterR==0) outerRadius = BDSGlobals->GetComponentBoxSize()/2;

  if(itsNSegments>1)
    {
      sagitta=itsLength/itsAngle*(1.-cos(itsAngle/2.));
    }

  if(DEBUG) G4cout << "Outer volume inner radius :"
		   << " r= " << (itsInnerIronRadius+sagitta)/m << " m"
		   << " l= " << aLength/2./m << " m"
		   << G4endl;

  if(DEBUG) G4cout << "Outer radius :"
		   << " r= " << outerRadius/m << " m"
		   << " l= " << aLength/2./m << " m"
		   << G4endl;

  if(OuterMaterialIsVacuum)
    {
      itsOuterLogicalVolume=
	new G4LogicalVolume(new G4Tubs(itsName+"_outer_solid",
				       itsInnerIronRadius+sagitta+1*nm,
				       outerRadius,
				       aLength/2,
				       0,twopi*radian),
			    theMaterials->GetMaterial("Vacuum"),
			    itsName+"_outer_log");
    }
  if(!OuterMaterialIsVacuum)
    {
      itsOuterLogicalVolume=
	new G4LogicalVolume(new G4Tubs(itsName+"_outer_solid",
				       itsInnerIronRadius+sagitta+1*nm,
				       outerRadius,
				       aLength/2,
				       0,twopi*radian),
			    material,
			    itsName+"_outer_log");
    }
  
  itsPhysiComp = new G4PVPlacement(
		      0,		      // no rotation
		      0,                      // its position
		      itsOuterLogicalVolume,  // its logical volume
		      itsName+"_outer_phys",  // its name
		      itsMarkerLogicalVolume, // its mother  volume
		      false,		      // no boolean operation
		      0);		      // copy number

  itsOuterUserLimits =
    new G4UserLimits("multipole cut",aLength,DBL_MAX,DBL_MAX,
		     BDSGlobals->GetThresholdCutCharged());
  //  itsOuterUserLimits->SetMaxAllowedStep(aLength);
  itsOuterLogicalVolume->SetUserLimits(itsOuterUserLimits);

 

}

void BDSMultipole::BuildOuterFieldManager(G4int nPoles, G4double poleField,
					  G4double phiOffset)
{
  if(nPoles<=0 || nPoles>10 || nPoles%2 !=0)
    G4Exception("BDSMultipole: Invalid number of poles");
  itsNPoles=nPoles;
  itsOuterFieldMgr=NULL;
  if (poleField==0) return;

  itsOuterMagField=new BDSMultipoleOuterMagField(nPoles,poleField,phiOffset);

  itsOuterFieldMgr=new G4FieldManager(itsOuterMagField);

  //  (*theOuterFieldMap)[itsOuterLogicalVolume->GetName()]=itsOuterMagField;

  itsOuterLogicalVolume->SetFieldManager(itsOuterFieldMgr,false);


}


BDSMultipole::~BDSMultipole()
{
  if(itsPhysiComp) delete itsPhysiComp;
  if(itsBPFieldMgr) delete itsBPFieldMgr;
  if(itsBeampipeLogicalVolume) delete itsBeampipeLogicalVolume;
  if(itsBPTube) delete itsBPTube;
  if(itsChordFinder) delete itsChordFinder;
  if(itsOuterUserLimits) delete itsOuterUserLimits;
  if(itsBeampipeUserLimits) delete itsBeampipeUserLimits;
  if(itsOuterFieldMgr) delete itsOuterFieldMgr;
  if(itsOuterMagField) delete itsOuterMagField;
  if(itsSegRot)delete itsSegRot;
}
