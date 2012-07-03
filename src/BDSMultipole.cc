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
#include "G4Trd.hh"
#include "G4Trap.hh"
#include "G4EllipticalTube.hh"
#include "G4Cons.hh"
#include "G4Torus.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4UserLimits.hh"
#include "G4TransportationManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
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

extern G4RotationMatrix* RotY90;
extern G4RotationMatrix* RotYM90;
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
{  
CalculateLengths();
}

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
			    G4double tunnelOffsetX,
			    G4double phiAngleIn,
			    G4double phiAngleOut):
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
{
  CalculateLengths();
  BDSAcceleratorComponent::itsPhiAngleIn=phiAngleIn;
  BDSAcceleratorComponent::itsPhiAngleOut=phiAngleOut;
}

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
{
CalculateLengths();
}

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
  
  G4RotationMatrix* RotY;

  if((itsPhiAngleIn==0)&&(itsPhiAngleOut==0)){
#ifdef DEBUG
    G4cout << "BDSMultipole.cc> Building ordinary beam pipe (not trapezoid) " << G4endl;
#endif
    RotY=NULL;
    
#ifdef DEBUG 
    G4cout << "Outer pipe :"
	   << " r= " << itsBpRadius/m << " m"
	   << " l= " << itsLength/(2.)/m << " m"
	   << G4endl;
    G4cout << "Drift aperX: " << this->GetAperX()/m << " m" << G4endl;
    G4cout << "Drift aperY: " << this->GetAperY()/m << " m" << G4endl;
#endif
    
    
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
								      itsLength);
  
  itsBeampipeSolid = new G4SubtractionSolid(itsName + "_bmp_solid",
					    tmp_tube,
                              
                                            largerInnerBeampipeSolid_tmp);
  
  } else {
    RotY=RotY90;
  //
  // compute some geometrical parameters
  //
  G4double boxSize = BDSGlobals->GetComponentBoxSize();

  G4double xHalfLengthPlus, xHalfLengthMinus, tubLen;
  xHalfLengthPlus = (itsLength + (boxSize/2.0)*(tan(itsPhiAngleIn) -tan(itsPhiAngleOut)))/2.0;
  xHalfLengthMinus = (itsLength +  (boxSize/2.0)*(tan(itsPhiAngleOut)-tan(itsPhiAngleIn )))/2.0;
  tubLen = std::max(xHalfLengthPlus,xHalfLengthMinus);
  
  if((xHalfLengthPlus<0) || (xHalfLengthMinus<0)){
    G4cerr << "Bend radius in " << itsName << " too small for this tunnel/component geometry. Exiting." << G4endl;
    exit(1);
  }

  //
  // build beampipe
  //
  G4VSolid *pipeTubsEnv = new G4SubtractionSolid("_pipe_outer_env",
                                               new G4EllipticalTube(itsName+"_pipe_outer_tmp_1",
                                                                    this->GetAperX()+BDSGlobals->GetBeampipeThickness(), 
                                                                    this->GetAperY()+BDSGlobals->GetBeampipeThickness(),          
                                                                    tubLen),
                                               new G4EllipticalTube(itsName+"_pipe_outer_tmp_2",
                                                                    this->GetAperX()+BDSGlobals->GetLengthSafety()/2.0, 
                                                                    this->GetAperY()+BDSGlobals->GetLengthSafety()/2.0,          
                                                                    tubLen*2)
                                               );



  
  G4VSolid *pipeInnerEnv = new G4EllipticalTube(itsName+"_pipe_outer_tmp_2",
                                                this->GetAperX(), 
                                                this->GetAperY(),          
                                                tubLen);
  
  itsBeampipeSolid =
    new G4IntersectionSolid(itsName+"_pipe_outer",
			    pipeTubsEnv,
 			    itsMarkerSolidVolume,
			    RotYM90,
			    (G4ThreeVector)0);
  
  itsInnerBeampipeSolid =
    new G4IntersectionSolid(itsName+"_pipe_inner",
			    pipeInnerEnv, 
 			    itsMarkerSolidVolume,
			    RotYM90,
			    (G4ThreeVector)0);
  
  }
  
  itsBeampipeLogicalVolume=	
    new G4LogicalVolume(itsBeampipeSolid,
			material,
			itsName+"_bmp_log");
  
  itsInnerBPLogicalVolume=	
    new G4LogicalVolume(itsInnerBeampipeSolid,
			theMaterials->GetMaterial(BDSGlobals->GetVacuumMaterial()),
			itsName+"_inner_bmp_log");
  
  itsPhysiInner = new G4PVPlacement(
                                 RotY,		        // rotation
                                 (G4ThreeVector)0,	                // at (0,0,0)
                                 itsInnerBPLogicalVolume,  // its logical volume
                                 itsName+"_inner_bmp_phys",// its name
                                 itsMarkerLogicalVolume,   // its mother  volume
                                 false,		        // no boolean operation
                                 0, BDSGlobals->GetCheckOverlaps());		        // copy number
 

  itsPhysiComp = new G4PVPlacement(
				   //                                (G4RotationMatrix*)0,			     // no rotation
                                RotY,			     // rotation
                                (G4ThreeVector)0,	                     // at (0,0,0)
                                itsBeampipeLogicalVolume,  // its logical volume
                                itsName+"_bmp_phys",	     // its name
                                itsMarkerLogicalVolume,    // its mother  volume
                                false,		     // no boolean operation
                                0, BDSGlobals->GetCheckOverlaps());		             // copy number

  //Add the physical volumes to a vector which can be used for e.g. geometrical biasing
  SetMultiplePhysicalVolumes(itsPhysiInner);
  SetMultiplePhysicalVolumes(itsPhysiComp);
  
#ifndef NOUSERLIMITS
  itsBeampipeUserLimits = new G4UserLimits("beampipe cuts");
  itsInnerBeampipeUserLimits = new G4UserLimits("inner beampipe cuts");
  
  G4double maxStepFactor=0.5;

  itsBeampipeUserLimits->SetMaxAllowedStep(itsLength*maxStepFactor);
  itsBeampipeUserLimits->SetUserMinEkine(BDSGlobals->GetThresholdCutCharged());
  itsBeampipeLogicalVolume->SetUserLimits(itsBeampipeUserLimits);
  
  G4double maxStepFactorIn=0.5;

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

  G4RotationMatrix* RotY;
  RotY=NULL ;
  
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
				       itsLength/2-BDSGlobals->GetLengthSafety(),
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
				      RotY,		        // no rotation
				      (G4ThreeVector)0,	                // at (0,0,0)
				      itsInnerBPLogicalVolume,  // its logical volume
				      itsName+"_inner_bmp_phys",// its name
				      itsMarkerLogicalVolume,   // its mother  volume
				      false,		        // no boolean operation
				      0, BDSGlobals->GetCheckOverlaps());		        // copy number
    
      
    
    itsPhysiComp = new G4PVPlacement(
				     RotY,			     // no rotation
				     (G4ThreeVector)0,	                     // at (0,0,0)
				     itsBeampipeLogicalVolume,  // its logical volume
				     itsName+"_bmp_phys",	     // its name
				     itsMarkerLogicalVolume,    // its mother  volume
				     false,		     // no boolean operation
				     0, BDSGlobals->GetCheckOverlaps());		             // copy number
    
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
  if ((itsPhiAngleIn==0)&&(itsPhiAngleOut==0)){
    itsMarkerSolidVolume = new G4Box( itsName+"_marker_solid",
				      itsXLength,
				      itsYLength,
				      itsLength/2);
  } else {
    G4double xLength, yLength;
    xLength = yLength = std::max(itsOuterR,BDSGlobals->GetComponentBoxSize()/2);
    xLength = std::max(xLength, this->GetTunnelRadius()+2*std::abs(this->GetTunnelOffsetX()) + BDSGlobals->GetTunnelThickness()+BDSGlobals->GetTunnelSoilThickness() + 4*BDSGlobals->GetLengthSafety() );   
    yLength = std::max(yLength, this->GetTunnelRadius()+2*std::abs(BDSGlobals->GetTunnelOffsetY()) + BDSGlobals->GetTunnelThickness()+BDSGlobals->GetTunnelSoilThickness()+4*BDSGlobals->GetLengthSafety() );
    
    G4double transverseSize=std::max(xLength, yLength);
    G4double xHalfLengthPlus, xHalfLengthMinus;
    
    
    xHalfLengthPlus = (itsLength + (transverseSize/2.0)*(tan(itsPhiAngleIn) -tan(itsPhiAngleOut)))/2.0;
    xHalfLengthMinus = (itsLength +  (transverseSize/2.0)*(tan(itsPhiAngleOut)-tan(itsPhiAngleIn )))/2.0;
    
    /*
      if (itsPhiAngleIn >0){
      xHalfLengthPlus = ( (itsLength/itsPhiAngleIn) * sin(itsPhiAngleIn/2.0) 
      + fabs(cos(itsPhiAngleIn/2.0))*(transverseSize/2.0)*(tan(itsPhiAngleIn) -tan(itsPhiAngleOut)))/2.0;
      
      xHalfLengthMinus = ((itsLength/itsPhiAngleIn)*sin(itsPhiAngleIn/2.0)
      + fabs(cos(itsPhiAngleIn/2.0))*(transverseSize/2.0)*(tan(itsPhiAngleOut)-tan(itsPhiAngleIn )))/2.0;
      } else {
      xHalfLengthPlus = (itsLength + (transverseSize/2.0)*(tan(itsPhiAngleIn) -tan(itsPhiAngleOut)))/2.0;
      xHalfLengthMinus = (itsLength +  (transverseSize/2.0)*(tan(itsPhiAngleOut)-tan(itsPhiAngleIn )))/2.0;
      }
    */

    if((xHalfLengthPlus<0) || (xHalfLengthMinus<0)){
      G4cerr << "Bend radius in " << itsName << " too small for this tunnel/component geometry. Exiting." << G4endl;
      exit(1);
    }

    /*  
	itsMarkerSolidVolume = new G4Trd(itsName+"_marker",
	xHalfLengthPlus,     // x hlf lgth at +z
	xHalfLengthMinus,    // x hlf lgth at -z
	transverseSize/2,           // y hlf lgth at +z
	transverseSize/2,           // y hlf lgth at -z
	fabs(cos(itsAngle/2))*transverseSize/2);// z hlf lgth
    */
    
    G4cout << "BDSMultipole::MakeDefaultMarkerLogicalVolume> Trap parameters:  " << G4endl;
    G4cout  <<   
      //fabs(cos(itsPhiAngleIn/2))*transverseSize/2 << " " <<
      transverseSize/2 << " " <<
      itsPhiAngleOut-itsPhiAngleIn << " " <<
      0 << " " <<
      transverseSize/2.0 << " " <<
      xHalfLengthPlus << " " <<
      xHalfLengthPlus << " " <<
      0 << " " <<
      transverseSize/2.0 << " " <<
      xHalfLengthMinus << " " <<
      xHalfLengthMinus << " " <<
      0 << " " << G4endl;
    
    itsMarkerSolidVolume = new G4Trap(itsName+"_trapezoid_marker",
				      //			    fabs(cos(itsPhiAngleIn/2))*transverseSize/2,// z hlf lgth
				      transverseSize/2.0, // z hlf lgth Dz
				      atan((tan(itsPhiAngleOut)-tan(itsPhiAngleIn))/2.0), // pTheta
				      0,// pPhi
				      transverseSize/2.0, // pDy1
				      xHalfLengthPlus,    // pDx1
				      xHalfLengthPlus,    // pDx2
				      0, // pAlp1
				      transverseSize/2.0,  // pDy2
				      xHalfLengthMinus,     // pDx3
				      xHalfLengthMinus,     // pDx4
				      0); // pAlp2
    
  }
  
  
  itsMarkerLogicalVolume=new G4LogicalVolume
    (
     itsMarkerSolidVolume,
     theMaterials->GetMaterial("vacuum"),
     itsName+"_log");
  
#ifndef NOUSERLIMITS
  G4double maxStepFactor=0.5;
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
                                                            aLength/2-BDSGlobals->GetLengthSafety(),
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
				   0, BDSGlobals->GetCheckOverlaps());		      // copy number
  
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
			       aLength/2-BDSGlobals->GetLengthSafety(),
			       0,twopi*radian);

  G4EllipticalTube* etube_tmp= new G4EllipticalTube(itsName+"_etube_tmp",
                                                    this->GetAperX()+1*nm,
                                                    this->GetAperY()+1*nm,
                                                    aLength/2-BDSGlobals->GetLengthSafety());
  
 

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
				   0, BDSGlobals->GetCheckOverlaps());		      // copy number
  
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
