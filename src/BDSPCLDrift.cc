/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/
#include "BDSGlobalConstants.hh" // must be first in include list

#include "BDSPCLDrift.hh"
#include "BDSMagField.hh"
#include "BDSDriftStepper.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4UserLimits.hh"
#include "G4TransportationManager.hh"
#include "G4UnionSolid.hh"

#include <map>

//============================================================

typedef std::map<G4String,int> LogVolCountMap;
extern LogVolCountMap* LogVolCount;

typedef std::map<G4String,G4LogicalVolume*> LogVolMap;
extern LogVolMap* LogVol;

extern BDSMaterials* theMaterials;
//============================================================

BDSPCLDrift::BDSPCLDrift (G4String aName, G4double aLength, 
			  std::list<G4double> blmLocZ, std::list<G4double> blmLocTheta, G4double aperX, G4double aperYUp, G4double aperYDown, G4double aperDy, G4String tunnelMaterial, G4double aper, G4double tunnelRadius, G4double tunnelOffsetX):
  BDSMultipole(aName, aLength, aper, aper, SetVisAttributes(),  blmLocZ, blmLocTheta, tunnelMaterial, "", aper, aper, 0, tunnelRadius, tunnelOffsetX),
  itsYAperUp(aperYUp), itsYAperDown(aperYDown), itsDyAper(aperDy)
{
  itsType="pcldrift";
  itsXAper=aperX;

  if (!(*LogVolCount)[itsName])
    {
      //
      // build external volume
      // 
      BuildDefaultMarkerLogicalVolume();

      //
      // build beampipe (geometry + magnetic field)
      //
      if(BDSGlobals->GetBuildTunnel()){
        BuildTunnel();
      }
      BuildBpFieldAndStepper();
      BuildBPFieldMgr(itsStepper, itsMagField);
      BuildBeampipe();
      BuildBLMs();
  
      //
      // define sensitive volumes for hit generation
      //
      if(BDSGlobals->GetSensitiveBeamPipe()){
	SetMultipleSensitiveVolumes(itsUpperBeamPipeLogicalVolume)
;	SetMultipleSensitiveVolumes(itsMiddleBeamPipeLogicalVolume);
	SetMultipleSensitiveVolumes(itsLowerBeamPipeLogicalVolume);
      }
      
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

void BDSPCLDrift::BuildBeampipe(G4String materialName){
  G4Material *material;
  if(materialName != ""){
    material = theMaterials->GetMaterial( materialName );
  } else {
    material = theMaterials->GetMaterial( BDSGlobals->GetPipeMaterialName());
  }
  
  // build beampipe

#ifdef DEBUG 
  G4cout << "Outer pipe :"
         << " r= " << itsBpRadius/m << " m"
         << " l= " << itsLength/(2.)/m << " m"
         << G4endl;
#endif

  G4cout << "PCLDrift aperX: " << itsXAper/m << " m" << G4endl;
  G4cout << "PCLDrift aperYUp: " << itsYAperUp/m << " m" << G4endl;
  G4cout << "PCLDrift aperYDown: " << itsYAperDown/m << " m" << G4endl;
  G4cout << "PCLDrift Dy: " << itsDyAper/m << " m" << G4endl;

  G4RotationMatrix* nullrot  = new G4RotationMatrix();
  G4ThreeVector threeVector1;

  threeVector1.setX(0);
  threeVector1.setZ(0);

  //Make the upper ellipse
  threeVector1.setY(0);
  G4VSolid* tmp_solid_1 =new G4SubtractionSolid("tmp_solid_1",
						new G4EllipticalTube(itsName+"_tmp_solid_1_a",
								     itsXAper+BDSGlobals->GetBeampipeThickness(),
								     itsYAperUp+BDSGlobals->GetBeampipeThickness(),
								     itsLength/2-BDSGlobals->GetLengthSafety()/2.0),
						new G4EllipticalTube(itsName+"_tmp_solid_1_b",
								     itsXAper+BDSGlobals->GetLengthSafety()/2.0,
								     itsYAperUp+BDSGlobals->GetLengthSafety()/2.0,
								     itsLength),					
						nullrot,
						threeVector1);

  threeVector1.setY(-1*(itsYAperUp*2));
   upper_outer_solid = 

     //  new G4EllipticalTube(itsName+"_tester",
     //			  itsXAper+BDSGlobals->GetBeampipeThickness(),
     //			  itsYAperUp+BDSGlobals->GetBeampipeThickness(),
     //			  itsLength/2-BDSGlobals->GetLengthSafety()/2.0);
   
    new G4SubtractionSolid("upper_outer_solid",
  			 tmp_solid_1,
  			 new G4Box(itsName+"_upper_outer_solid_a",
  				   itsXAper*2.0,
  				   itsYAperUp*2.0,
  				   itsLength),
  			 nullrot,
  			 threeVector1);

   upper_inner_solid =new G4SubtractionSolid("upper_inner_solid",
						      new G4EllipticalTube(itsName+"_upper_inner_solid_a",
									   itsXAper-BDSGlobals->GetLengthSafety()/2.0,
									   itsYAperUp-BDSGlobals->GetLengthSafety()/2.0,
									   itsLength/2.0-BDSGlobals->GetLengthSafety()/2.0),					
						      new G4Box(itsName+"_upper_inner_solid_b",
								itsXAper*2.0,
								itsYAperUp*2.0,
								itsLength),
						      nullrot,
						      threeVector1);
  


  

  threeVector1.setY(0);
  //Make the middle box
   middle_outer_solid =new G4SubtractionSolid(itsName+"middle_outer_solid",
						new G4Box(itsName+"_middle_outer_solid_a",
							  itsXAper+BDSGlobals->GetBeampipeThickness(),
							  itsDyAper/2.0-BDSGlobals->GetLengthSafety()/2.0,
							  itsLength/2.0-BDSGlobals->GetLengthSafety()/2.0),
						new G4Box(itsName+"_middle_outer_solid_b",
							  itsXAper+BDSGlobals->GetLengthSafety()/2.0,
							  itsDyAper,
							  itsLength),
						nullrot,
						threeVector1);
  
   middle_inner_solid = new G4Box(itsName+"middle_inner_solid", 
				  itsXAper,
				  itsDyAper/2.0-10*BDSGlobals->GetLengthSafety(),
				  itsLength/2.0-BDSGlobals->GetLengthSafety()/2.0);
    

//Make the lower ellipse
    threeVector1.setY(0);
  G4VSolid* tmp_solid_2 =new G4SubtractionSolid("tmp_solid_2",
						new G4EllipticalTube(itsName+"_tmp_solid_2_a",
								     itsXAper+BDSGlobals->GetBeampipeThickness(),
								     itsYAperDown+BDSGlobals->GetBeampipeThickness(),
								     itsLength/2-BDSGlobals->GetLengthSafety()/2.0),
						new G4EllipticalTube(itsName+"_tmp_solid_2_b",
								     itsXAper+BDSGlobals->GetLengthSafety()/2.0,
								     itsYAperDown+BDSGlobals->GetLengthSafety()/2.0,
								     itsLength),					
						nullrot,
						threeVector1);

  threeVector1.setY(itsYAperDown*2);
  lower_outer_solid =new G4SubtractionSolid("lower_outer_solid",
					    tmp_solid_2,
					    new G4Box(itsName+"_lower_outer_solid_a",
						      itsXAper*2.0,
						      itsYAperDown*2.0,
						      itsLength),
					    nullrot,
					    threeVector1);
  lower_inner_solid =new G4SubtractionSolid("lower_inner_solid",
					    new G4EllipticalTube(itsName+"_lower_inner_solid_a",
								 itsXAper,
								 itsYAperDown,
								 itsLength/2.0-BDSGlobals->GetLengthSafety()/2.0),					
					    new G4Box(itsName+"_lower_inner_solid_b",
						      itsXAper*2.0,
						      itsYAperDown*2.0-BDSGlobals->GetLengthSafety()/2.0,
						      itsLength),
					    nullrot,
						      threeVector1);
  
  
  G4cout << "Making logical..." << G4endl;
  
  itsUpperBeamPipeLogicalVolume=	
    new G4LogicalVolume(upper_outer_solid,
			material,
			itsName+"_upper_bmp_log");

  itsMiddleBeamPipeLogicalVolume=	
    new G4LogicalVolume(middle_outer_solid,
			material,
			itsName+"_middle_bmp_log");

  itsLowerBeamPipeLogicalVolume=	
    new G4LogicalVolume(lower_outer_solid,
			material,
			itsName+"_lower_bmp_log");

  itsUpperInnerBeamPipeLogicalVolume=	
    new G4LogicalVolume(upper_inner_solid,
			theMaterials->GetMaterial(BDSGlobals->GetVacuumMaterial()),
			itsName+"_upper_inner_bmp_log");

  itsMiddleInnerBeamPipeLogicalVolume=	
    new G4LogicalVolume(middle_inner_solid,
			theMaterials->GetMaterial(BDSGlobals->GetVacuumMaterial()),
			itsName+"_middle_inner_bmp_log");

  itsLowerInnerBeamPipeLogicalVolume=	
    new G4LogicalVolume(lower_inner_solid,
			theMaterials->GetMaterial(BDSGlobals->GetVacuumMaterial()),
			itsName+"_lower_inner_bmp_log");
  //
  // set visualization attributes
  //
  itsInnerBeampipeVisAtt =  new G4VisAttributes(G4Colour(0., 0., 0));
  itsInnerBeampipeVisAtt->SetForceSolid(true);
  itsInnerBeampipeVisAtt->SetVisibility(false);
  itsUpperInnerBeamPipeLogicalVolume->SetVisAttributes(itsInnerBeampipeVisAtt);
  itsMiddleInnerBeamPipeLogicalVolume->SetVisAttributes(itsInnerBeampipeVisAtt);
  itsLowerInnerBeamPipeLogicalVolume->SetVisAttributes(itsInnerBeampipeVisAtt);
  
  itsBeampipeVisAtt = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4));
  itsBeampipeVisAtt->SetForceSolid(true);
  itsBeampipeVisAtt->SetVisibility(true);
  itsUpperBeamPipeLogicalVolume->SetVisAttributes(itsBeampipeVisAtt);
  itsMiddleBeamPipeLogicalVolume->SetVisAttributes(itsBeampipeVisAtt);
  itsLowerBeamPipeLogicalVolume->SetVisAttributes(itsBeampipeVisAtt);



  G4cout << "Placing..." << G4endl;

  threeVector1.setY(itsDyAper+BDSGlobals->GetLengthSafety()*2);

  itsPhysiUpperInner = new G4PVPlacement(
					 (G4RotationMatrix*)0,		        // no rotation
					 threeVector1,	               
					 itsUpperInnerBeamPipeLogicalVolume,  // its logical volume
					 itsName+"_upper_inner_bmp_phys",// its name
					 itsMarkerLogicalVolume,   // its mother  volume
					 false,		        // no boolean operation
					 0);		        // copy number

  itsPhysiUpper = new G4PVPlacement(
				    (G4RotationMatrix*)0,			     // no rotation
				    threeVector1,	               
				    itsUpperBeamPipeLogicalVolume,  // its logical volume
				    itsName+"_bmp_phys",	     // its name
				    itsMarkerLogicalVolume,    // its mother  volume
				    false,		     // no boolean operation
				    0);		             // copy number
  
  threeVector1.setY(itsDyAper/2.0);
  
  itsPhysiMiddleInner = new G4PVPlacement(
					  (G4RotationMatrix*)0,		        // no rotation
					  threeVector1,	               
					  itsMiddleInnerBeamPipeLogicalVolume,  // its logical volume
					  itsName+"_middle_inner_bmp_phys",// its name
					  itsMarkerLogicalVolume,   // its mother  volume
					  false,		        // no boolean operation
					  0);		        // copy number
  
  itsPhysiMiddle = new G4PVPlacement(
				     (G4RotationMatrix*)0,			     // no rotation
				     threeVector1,	               
				     itsMiddleBeamPipeLogicalVolume,  // its logical volume
				     itsName+"_middle_bmp_phys",	     // its name
				     itsMarkerLogicalVolume,    // its mother  volume
				     false,		     // no boolean operation
				     0);		             // copy number

  threeVector1.setY(-1*BDSGlobals->GetLengthSafety()/2.0);
  itsPhysiLowerInner = new G4PVPlacement(
					 (G4RotationMatrix*)0,		        // no rotation
					 threeVector1,	               
					 itsLowerInnerBeamPipeLogicalVolume,  // its logical volume
					 itsName+"_lower_inner_bmp_phys",// its name
					 itsMarkerLogicalVolume,   // its mother  volume
					 false,		        // no boolean operation
					 0);		        // copy number
  
  itsPhysiLower = new G4PVPlacement(
				   (G4RotationMatrix*)0,			     // no rotation
				   threeVector1,	               
				   itsLowerBeamPipeLogicalVolume,  // its logical volume
				   itsName+"_lower_bmp_phys",	     // its name
				   itsMarkerLogicalVolume,    // its mother  volume
				   false,		     // no boolean operation
				   0);		             // copy number


  //Add the physical volumes to a vector which can be used for e.g. geometrical biasing
  SetMultiplePhysicalVolumes(itsPhysiUpperInner);
  SetMultiplePhysicalVolumes(itsPhysiUpper);
  SetMultiplePhysicalVolumes(itsPhysiMiddleInner);
  SetMultiplePhysicalVolumes(itsPhysiMiddle);
  SetMultiplePhysicalVolumes(itsPhysiLowerInner);
  SetMultiplePhysicalVolumes(itsPhysiLower);

#ifndef NOUSERLIMITS
  itsBeampipeUserLimits =  new G4UserLimits("beampipe cuts");
  itsBeampipeUserLimits->SetUserMinEkine(BDSGlobals->GetThresholdCutCharged());

  itsInnerBeampipeUserLimits =  new G4UserLimits("inner beampipe cuts");
  itsInnerBeampipeUserLimits->SetUserMinEkine( BDSGlobals->GetThresholdCutCharged());

  itsBeampipeUserLimits->SetMaxAllowedStep(itsLength*1000);
  itsInnerBeampipeUserLimits->SetMaxAllowedStep(itsLength*1000);

  itsUpperBeamPipeLogicalVolume->SetUserLimits(itsBeampipeUserLimits);
  itsUpperInnerBeamPipeLogicalVolume->SetUserLimits(itsInnerBeampipeUserLimits);

  itsUpperBeamPipeLogicalVolume->SetFieldManager(BDSGlobals->GetZeroFieldManager(),false);

   itsMiddleBeamPipeLogicalVolume->SetUserLimits(itsBeampipeUserLimits);
   itsMiddleInnerBeamPipeLogicalVolume->SetUserLimits(itsInnerBeampipeUserLimits);

   itsMiddleBeamPipeLogicalVolume->SetFieldManager(BDSGlobals->GetZeroFieldManager(),false);

   itsLowerBeamPipeLogicalVolume->SetUserLimits(itsBeampipeUserLimits);
   itsLowerInnerBeamPipeLogicalVolume->SetUserLimits(itsInnerBeampipeUserLimits);

#endif


   itsUpperInnerBeamPipeLogicalVolume->SetFieldManager(itsBPFieldMgr,false);
   //---
   itsMiddleInnerBeamPipeLogicalVolume->SetFieldManager(itsBPFieldMgr,false);
   //---
   itsLowerBeamPipeLogicalVolume->SetFieldManager(BDSGlobals->GetZeroFieldManager(),false);
   itsLowerInnerBeamPipeLogicalVolume->SetFieldManager(itsBPFieldMgr,false);
   
  
  // now protect the fields inside the marker volume by giving the
  // marker a null magnetic field (otherwise G4VPlacement can
  // over-ride the already-created fields, by calling 
  // G4LogicalVolume::AddDaughter, which calls 
  // pDaughterLogical->SetFieldManager(fFieldManager, true) - the
  // latter 'true' over-writes all the other fields
  
  itsMarkerLogicalVolume->
    SetFieldManager(BDSGlobals->GetZeroFieldManager(),false);
  
  G4cout << "Finished making beam pipe..." << G4endl;
}

void BDSPCLDrift::BuildBpFieldAndStepper(){
    // set up the magnetic field and stepper
  itsMagField=new BDSMagField(); //Zero magnetic field.
  itsEqRhs=new G4Mag_UsualEqRhs(itsMagField);
  itsStepper=new BDSDriftStepper(itsEqRhs);
}

G4VisAttributes* BDSPCLDrift::SetVisAttributes()
{
  itsVisAttributes=new G4VisAttributes(G4Colour(0,1,0)); //useless
  itsVisAttributes->SetVisibility(true);
  itsVisAttributes->SetForceSolid(true);
  return itsVisAttributes;
}

void BDSPCLDrift::BuildBLMs(){
  itsBlmLocationR = std::max(itsStartOuterR, itsEndOuterR) - itsBpRadius;
  BDSAcceleratorComponent::BuildBLMs();
}

BDSPCLDrift::~BDSPCLDrift()
{
  if(itsVisAttributes) delete itsVisAttributes;
  if(itsMarkerLogicalVolume) delete itsMarkerLogicalVolume;
  if(itsOuterLogicalVolume) delete itsOuterLogicalVolume;
  if(itsPhysiComp) delete itsPhysiComp;
  delete upper_outer_solid;
  delete middle_outer_solid;
  delete lower_outer_solid;
  delete upper_inner_solid;
  delete middle_inner_solid;
  delete lower_inner_solid;
  delete itsUpperBeamPipeLogicalVolume;
  delete itsMiddleBeamPipeLogicalVolume;
  delete itsLowerBeamPipeLogicalVolume;
  delete itsUpperInnerBeamPipeLogicalVolume;
  delete itsMiddleInnerBeamPipeLogicalVolume;
  delete itsLowerInnerBeamPipeLogicalVolume;
  delete itsPhysiUpperInner;
  delete itsPhysiUpper;
  delete itsPhysiMiddleInner;
  delete itsPhysiMiddle;
  delete itsPhysiLowerInner;
  delete itsPhysiLower;
  delete itsBeampipeVisAtt;
  delete itsInnerBeampipeVisAtt;
  //if(itsField) delete itsField;
  //if(itsEqRhs) delete itsEqRhs;
  //if(itsStepper) delete itsStepper;
}
