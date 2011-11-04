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
#include "G4EllipticalTube.hh"
#include "G4Cons.hh"
#include "G4Torus.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4UserLimits.hh"
#include "G4TransportationManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"

#include "G4MagIntegratorStepper.hh"

#include "BDSMultipoleOuterMagField.hh"
#include "G4MagneticField.hh"

#include <map>
#include <string>


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

BDSMultipole::BDSMultipole( G4String aName, 
			    G4double aLength,
			    G4double aBpRadius,
			    G4double aInnerIronRadius,
                            G4VisAttributes* aVisAtt,
			    std::list<G4double> blmLocZ, std::list<G4double> blmLocTheta,
                            G4String aTunnelMaterial,
                            G4String aMaterial,
			    G4double aXAper,
			    G4double aYAper,
			    G4double angle,
			    G4double tunnelRadius,
			    G4double tunnelOffsetX):
  BDSAcceleratorComponent(
			 aName, 
			 aLength,
			 aBpRadius,
			 aXAper,
			 aYAper,
			 aVisAtt,
			 blmLocZ,
			 blmLocTheta,
                         aTunnelMaterial,
			 aMaterial,
			 angle,
			 0.,
			 0.,
			 0.,
			 tunnelRadius,
			 tunnelOffsetX),
  itsInnerIronRadius(aInnerIronRadius)
{}

BDSMultipole::BDSMultipole( G4String aName, 
			    G4double aLength,
			    G4double aBpRadius,
			    G4double aInnerIronRadius,
                            G4VisAttributes* aVisAtt,
                            G4String aTunnelMaterial,
                            G4String aMaterial,
			    G4double aXAper,
			    G4double aYAper,
			    G4double angle,
			    G4double tunnelRadius,
			    G4double tunnelOffsetX):
  BDSAcceleratorComponent(
			 aName, 
			 aLength,
			 aBpRadius,
			 aXAper,
			 aYAper,
			 aVisAtt,
                         aTunnelMaterial,
			 aMaterial,
			 angle,
			 0.,
			 0.,
			 0.,
			 tunnelRadius,
			 tunnelOffsetX),
  itsInnerIronRadius(aInnerIronRadius)
{}

void BDSMultipole::BuildBLMs(){
  itsBlmLocationR=itsOuterR;
  BDSAcceleratorComponent::BuildBLMs();
}

void BDSMultipole::BuildBeampipe(G4String materialName)
{
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

#ifdef DEBUG 
  G4cout << "Outer pipe :"
         << " r= " << itsBpRadius/m << " m"
         << " l= " << itsLength/(2.)/m << " m"
         << G4endl;
#endif

  G4cout << "Drift aperX: " << this->GetAperX()/m << " m" << G4endl;
  G4cout << "Drift aperY: " << this->GetAperY()/m << " m" << G4endl;

  G4EllipticalTube* tmp_tube =new G4EllipticalTube(itsName+"_bmp_solid_tmp",
						   this->GetAperX()+BDSGlobals->GetBeampipeThickness(),
						   this->GetAperY()+BDSGlobals->GetBeampipeThickness(),
						   itsLength/2);
  
  itsInnerBeampipeSolid=new G4EllipticalTube(itsName+"_inner_bmp_solid",
					     this->GetAperX(),
					     this->GetAperY(),
					     itsLength/2);

  G4EllipticalTube* largerInnerBeampipeSolid_tmp=new G4EllipticalTube(itsName+"larger_inner_bmp_solid",
								      this->GetAperX()+BDSGlobals->GetLengthSafety()/2.0,
								      this->GetAperY()+BDSGlobals->GetLengthSafety()/2.0,
								      itsLength+BDSGlobals->GetLengthSafety());
  
  itsBeampipeSolid = new G4SubtractionSolid(itsName + "_bmp_solid",
                                               tmp_tube,
                              
                                            largerInnerBeampipeSolid_tmp);
  
  itsBeampipeLogicalVolume=	
    new G4LogicalVolume(itsBeampipeSolid,
			material,
			itsName+"_bmp_log");
  
  itsInnerBPLogicalVolume=	
    new G4LogicalVolume(itsInnerBeampipeSolid,
			theMaterials->GetMaterial(BDSGlobals->GetVacuumMaterial()),
			itsName+"_inner_bmp_log");
  
  itsPhysiInner = new G4PVPlacement(
                                 (G4RotationMatrix*)0,		        // no rotation
                                 (G4ThreeVector)0,	                // at (0,0,0)
                                 itsInnerBPLogicalVolume,  // its logical volume
                                 itsName+"_inner_bmp_phys",// its name
                                 itsMarkerLogicalVolume,   // its mother  volume
                                 false,		        // no boolean operation
                                 0);		        // copy number
 

  itsPhysiComp = new G4PVPlacement(
                                (G4RotationMatrix*)0,			     // no rotation
                                (G4ThreeVector)0,	                     // at (0,0,0)
                                itsBeampipeLogicalVolume,  // its logical volume
                                itsName+"_bmp_phys",	     // its name
                                itsMarkerLogicalVolume,    // its mother  volume
                                false,		     // no boolean operation
                                0);		             // copy number

  //Add the physical volumes to a vector which can be used for e.g. geometrical biasing
  SetMultiplePhysicalVolumes(itsPhysiInner);
  SetMultiplePhysicalVolumes(itsPhysiComp);
  
#ifndef NOUSERLIMITS
  G4cout << "BDSMultipole::BuildBeamPipe() Using user limits." << G4endl;
  itsBeampipeUserLimits = new G4UserLimits("beampipe cuts");
  itsInnerBeampipeUserLimits = new G4UserLimits("inner beampipe cuts");
  
  G4double maxStepFactor=0.5;

  itsBeampipeUserLimits->SetMaxAllowedStep(itsLength*maxStepFactor);
  itsBeampipeUserLimits->SetUserMinEkine(BDSGlobals->GetThresholdCutCharged());
  itsBeampipeLogicalVolume->SetUserLimits(itsBeampipeUserLimits);
  
  G4double maxStepFactorIn=1e-3;

  itsInnerBeampipeUserLimits->SetMaxAllowedStep(itsLength*maxStepFactorIn);
  itsInnerBeampipeUserLimits->SetUserMinEkine(BDSGlobals->GetThresholdCutCharged());
  itsInnerBPLogicalVolume->SetUserLimits(itsInnerBeampipeUserLimits);
#endif
  
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
  VisAtt->SetVisibility(false);
  itsInnerBPLogicalVolume->SetVisAttributes(VisAtt);

  G4VisAttributes* VisAtt1 = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4));
  VisAtt1->SetForceSolid(true);
  VisAtt1->SetVisibility(true);
  itsBeampipeLogicalVolume->SetVisAttributes(VisAtt1);

}


void BDSMultipole::BuildBeampipe(G4double startAper,
                                 G4double endAper, G4String materialName)
{
  //make sure length is not 0 
  if (itsLength==0){
    itsLength=itsLength+pow(1.0,-20.0);
  }

  G4Material* material;

  if(materialName == "")
    {
      material = theMaterials->GetMaterial( BDSGlobals->GetPipeMaterialName() );
    }
  else
    {
      material = theMaterials->GetMaterial(materialName);
    }

  // build beampipe

#ifdef DEBUG 
  G4cout << "Outer pipe :"
         << " start r= " << startAper/m << " m"
         << " end r= " << endAper/m << " m"
         << " l= " << itsLength/(2.)/m << " m"
         << G4endl;
#endif

  itsBeampipeSolid=new G4Cons(itsName+"_bmp_solid",
		       startAper-BDSGlobals->GetBeampipeThickness(),
		       startAper,
                       endAper-BDSGlobals->GetBeampipeThickness(),
		       endAper,
			      itsLength/(2.),
		       0,twopi*radian);

#ifdef DEBUG 
  G4cout << "Inner pipe :"
         << " r= " << (itsBpRadius-BDSGlobals->GetBeampipeThickness() )/m
         << " m"
         << " l= " << itsLength/(2.)/m << " m"
         << G4endl;
#endif
  
      itsInnerBeampipeSolid=new G4Cons(itsName+"_inner_bmp_solid",
				0.,
			        startAper-BDSGlobals->GetBeampipeThickness(),
				0.,
			        endAper-BDSGlobals->GetBeampipeThickness(),
				       itsLength/2+BDSGlobals->GetLengthSafety(),
				0,twopi*radian);
  itsBeampipeLogicalVolume=	
    new G4LogicalVolume(itsBeampipeSolid,
			material,
			itsName+"_bmp_log");

  itsInnerBPLogicalVolume=	
    new G4LogicalVolume(itsInnerBeampipeSolid,
			theMaterials->GetMaterial(BDSGlobals->GetVacuumMaterial()),
			itsName+"_inner_bmp_log");
  
  itsPhysiInner = new G4PVPlacement(
				 (G4RotationMatrix*)0,		        // no rotation
				 (G4ThreeVector)0,	                // at (0,0,0)
		      itsInnerBPLogicalVolume,  // its logical volume
		      itsName+"_inner_bmp_phys",// its name
		      itsMarkerLogicalVolume,   // its mother  volume
		      false,		        // no boolean operation
		      0);		        // copy number
    

  
      itsPhysiComp = new G4PVPlacement(
				    (G4RotationMatrix*)0,			     // no rotation
				    (G4ThreeVector)0,	                     // at (0,0,0)
			  itsBeampipeLogicalVolume,  // its logical volume
			  itsName+"_bmp_phys",	     // its name
			  itsMarkerLogicalVolume,    // its mother  volume
			  false,		     // no boolean operation
			  0);		             // copy number

  //Add the physical volumes to a vector which can be used for e.g. geometrical biasing
  SetMultiplePhysicalVolumes(itsPhysiInner);
  SetMultiplePhysicalVolumes(itsPhysiComp);

#ifndef NOUSERLIMITS
  G4double maxStepFactor=0.5;
  itsBeampipeUserLimits = new G4UserLimits("beampipe cuts");
  itsBeampipeUserLimits->SetUserMinEkine(BDSGlobals->GetThresholdCutCharged());
  itsBeampipeUserLimits->SetMaxAllowedStep(itsLength*maxStepFactor);

  itsBeampipeLogicalVolume->SetUserLimits(itsBeampipeUserLimits);
  itsInnerBPLogicalVolume->SetUserLimits(itsBeampipeUserLimits);

  itsBeampipeLogicalVolume->SetFieldManager(BDSGlobals->GetZeroFieldManager(),false);
  itsInnerBPLogicalVolume->SetFieldManager(itsBPFieldMgr,false) ;

  // now protect the fields inside the marker volume by giving the
  // marker a null magnetic field (otherwise G4VPlacement can
  // over-ride the already-created fields, by calling 
  // G4LogicalVolume::AddDaughter, which calls 
  // pDaughterLogical->SetFieldManager(fFieldManager, true) - the
  // latter 'true' over-writes all the other fields
#endif
  itsMarkerLogicalVolume->
    SetFieldManager(BDSGlobals->GetZeroFieldManager(),false);
  
  //
  // set visualization attributes
  //
  G4VisAttributes* VisAtt =  new G4VisAttributes(G4Colour(0., 0., 0));
  VisAtt->SetForceSolid(true);
  VisAtt->SetVisibility(false);
  itsInnerBPLogicalVolume->SetVisAttributes(VisAtt);

  G4VisAttributes* VisAtt1 = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4));
  VisAtt1->SetForceSolid(true);
  VisAtt->SetVisibility(true);
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
  if(BDSGlobals->GetDeltaIntersection()>0){
    itsBPFieldMgr->SetDeltaIntersection(BDSGlobals->GetDeltaIntersection());
  }
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

  xLength = std::max(xLength, this->GetTunnelRadius()+2*std::abs(this->GetTunnelOffsetX()) + BDSGlobals->GetTunnelThickness()+BDSGlobals->GetTunnelSoilThickness() + 4*BDSGlobals->GetLengthSafety() );   
  yLength = std::max(yLength, this->GetTunnelRadius()+2*std::abs(BDSGlobals->GetTunnelOffsetY()) + BDSGlobals->GetTunnelThickness()+BDSGlobals->GetTunnelSoilThickness()+4*BDSGlobals->GetLengthSafety() );

#ifdef DEBUG 
  G4cout<<"marker volume : x/y="<<xLength/m<<
    " m, l= "<<  (itsLength)/2/m <<" m"<<G4endl;
#endif

  itsMarkerSolidVolume = new G4Box( itsName+"_marker_solid",
                                    xLength,
                                    yLength,
                                    itsLength/2);

  itsMarkerLogicalVolume=new G4LogicalVolume
    (
     itsMarkerSolidVolume,
     theMaterials->GetMaterial("vacuum"),
     itsName+"_log");

#ifndef NOUSERLIMITS
  G4double maxStepFactor=1e-3;
   itsMarkerUserLimits =  new G4UserLimits();
   itsMarkerUserLimits->SetMaxAllowedStep(itsLength*maxStepFactor);
   itsMarkerUserLimits->SetUserMinEkine(BDSGlobals->GetThresholdCutCharged());
   itsMarkerLogicalVolume->SetUserLimits(itsMarkerUserLimits);
#endif

}

void BDSMultipole::BuildDefaultOuterLogicalVolume(G4double aLength,
						  G4bool OuterMaterialIsVacuum)
{
  //OuterMaterialIsVacuum parameter is useless: one can set
  //itsMaterial = BDSGlobals->GetVacuumMaterial() and obtain the same result. Or cannot?

  G4Material* material;
  if(itsMaterial != "") material = theMaterials->GetMaterial(itsMaterial);
  else material = theMaterials->GetMaterial("Iron");

  G4double outerRadius = itsOuterR;
  if(itsOuterR==0) outerRadius = BDSGlobals->GetComponentBoxSize()/2;

#ifdef DEBUG 
  G4cout << "Outer volume inner radius :"
         << " r= " << (itsInnerIronRadius)/m << " m"
         << " l= " << aLength/2./m << " m"
         << G4endl;
#endif

#ifdef DEBUG 
  G4cout << "Outer radius :"
         << " r= " << outerRadius/m << " m"
         << " l= " << aLength/2./m << " m"
         << G4endl;
#endif

  if(OuterMaterialIsVacuum){
    material=  theMaterials->GetMaterial(BDSGlobals->GetVacuumMaterial());
  }
   

  itsOuterLogicalVolume=
    new G4LogicalVolume(  new G4SubtractionSolid(itsName+"_outer_solid",
                                                 new G4Tubs(itsName+"_outer_solid_tmp_1",
                                                            itsInnerIronRadius+BDSGlobals->GetLengthSafety()/2.0,
                                                            outerRadius,
                                                            aLength/2-BDSGlobals->GetLengthSafety()/2,
                                                            0,twopi*radian),
                                                 new G4EllipticalTube(itsName+"_outer_solid_tmp_2",
                                                                      this->GetAperX()+BDSGlobals->GetBeampipeThickness()+BDSGlobals->GetLengthSafety()/2.0,
                                                                      this->GetAperY()+BDSGlobals->GetBeampipeThickness()+BDSGlobals->GetLengthSafety()/2.0,
                                                                      itsLength)
                                                 ),
                          material,
                          itsName+"_outer_log");
  
  
  itsPhysiComp = new G4PVPlacement(
				   (G4RotationMatrix*)0,		      // no rotation
				   (G4ThreeVector)0,                      // its position
		      itsOuterLogicalVolume,  // its logical volume
		      itsName+"_outer_phys",  // its name
		      itsMarkerLogicalVolume, // its mother  volume
		      false,		      // no boolean operation
		      0);		      // copy number
  
  //Add the physical volumes to a vector which can be used for e.g. geometrical biasing
  SetMultiplePhysicalVolumes(itsPhysiComp);

#ifndef NOUSERLIMITS
  G4double maxStepFactor=0.5;
  itsOuterUserLimits =  new G4UserLimits("multipole cut");
  itsOuterUserLimits->SetUserMinEkine( BDSGlobals->GetThresholdCutCharged());
  itsOuterUserLimits->SetMaxAllowedStep(itsLength*maxStepFactor);
  itsOuterLogicalVolume->SetUserLimits(itsOuterUserLimits);
#endif
 

}

void BDSMultipole::BuildEllipticalOuterLogicalVolume(G4double aLength,
						  G4bool OuterMaterialIsVacuum)
{
  //OuterMaterialIsVacuum parameter is useless: one can set
  //itsMaterial = BDSGlobals->GetVacuumMaterial() and obtain the same result. Or cannot?

  G4Material* material;
  if(itsMaterial != "") material = theMaterials->GetMaterial(itsMaterial);
  else material = theMaterials->GetMaterial("Iron");
  if(OuterMaterialIsVacuum) material = theMaterials->GetMaterial(BDSGlobals->GetVacuumMaterial());
  G4double outerRadius = itsOuterR;
  if(itsOuterR==0) outerRadius = BDSGlobals->GetComponentBoxSize()/2;

  G4Tubs* tubs_tmp= new G4Tubs(itsName+"_tubs_tmp",
			       0,
			       outerRadius,
			       aLength/2-BDSGlobals->GetLengthSafety()/2,
			       0,twopi*radian);

  G4EllipticalTube* etube_tmp= new G4EllipticalTube(itsName+"_etube_tmp",
                                                    this->GetAperX()+1*nm,
                                                    this->GetAperY()+1*nm,
                                                    aLength/2-BDSGlobals->GetLengthSafety()/2);
  
 

  G4LogicalVolume* itsOuterLogicalVolume=
    new G4LogicalVolume(new G4SubtractionSolid(itsName+"_outer_solid",
					       tubs_tmp, etube_tmp),
			material,
			itsName+"_outer_log");
  
  itsPhysiComp = new G4PVPlacement(
				   (G4RotationMatrix*)0,		      // no rotation
				   (G4ThreeVector)0,                      // its position
				   itsOuterLogicalVolume,  // its logical volume
				   itsName+"_outer_phys",  // its name
				   itsMarkerLogicalVolume, // its mother  volume
				   false,		      // no boolean operation
				   0);		      // copy number
  
  //Add the physical volumes to a vector which can be used for e.g. geometrical biasing
  SetMultiplePhysicalVolumes(itsPhysiComp);
  
#ifndef NOUSERLIMITS
  G4double maxStepFactor=0.5;
  itsOuterUserLimits = new G4UserLimits("multipole cut");
  itsOuterUserLimits->SetUserMinEkine(BDSGlobals->GetThresholdCutCharged());
  itsOuterUserLimits->SetMaxAllowedStep(itsLength*maxStepFactor);
  itsOuterLogicalVolume->SetUserLimits(itsOuterUserLimits);
#endif
 

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
  if(BDSGlobals->GetDeltaIntersection()>0){
    itsOuterFieldMgr->SetDeltaIntersection(BDSGlobals->GetDeltaIntersection());
  }
  if(BDSGlobals->GetMinimumEpsilonStep()>0)
    itsOuterFieldMgr->SetMinimumEpsilonStep(BDSGlobals->GetMinimumEpsilonStep());
  if(BDSGlobals->GetMaximumEpsilonStep()>0)
    itsOuterFieldMgr->SetMaximumEpsilonStep(BDSGlobals->GetMaximumEpsilonStep());
  if(BDSGlobals->GetDeltaOneStep()>0)
    itsOuterFieldMgr->SetDeltaOneStep(BDSGlobals->GetDeltaOneStep());
  itsOuterLogicalVolume->SetFieldManager(itsOuterFieldMgr,false);
}


BDSMultipole::~BDSMultipole()
{
  if(itsPhysiComp) delete itsPhysiComp;
  if(itsPhysiInner) delete itsPhysiInner;
  if(itsBPFieldMgr) delete itsBPFieldMgr;
  if(itsBeampipeLogicalVolume) delete itsBeampipeLogicalVolume;
  if(itsChordFinder) delete itsChordFinder;
  if(itsOuterChordFinder) delete itsOuterChordFinder;
  if(itsOuterUserLimits) delete itsOuterUserLimits;
  if(itsBeampipeUserLimits) delete itsBeampipeUserLimits;
  if(itsOuterFieldMgr) delete itsOuterFieldMgr;
  if(itsOuterMagField) delete itsOuterMagField;
  if(itsSegRot)delete itsSegRot;
}
