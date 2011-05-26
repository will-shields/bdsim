//  
//   BDSIM, (C) 2001-2006 
//   
//   version 0.3
//  
//
//
//
//
//   Generic accelerator component class
//
//
//   History
//
//     24 Nov 2006 by Agapov,  v.0.3
//     x  x   2002 by Blair
//
//


#include "BDSGlobalConstants.hh" // must be first in include list

#include "BDSAcceleratorComponent.hh"
#include "BDSMaterials.hh"
#include <string>
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Torus.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UserLimits.hh"
#include "G4TransportationManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4AssemblyVolume.hh"
#include "G4Transform3D.hh"
#include <sstream>

extern BDSMaterials* theMaterials;
extern G4RotationMatrix* RotY90;
extern G4RotationMatrix* RotYM90;

void BDSAcceleratorComponent::PrepareField(G4VPhysicalVolume*)
{//do nothing by default
  return;
}

void BDSAcceleratorComponent::SynchRescale(G4double)
{
  return;
}

void BDSAcceleratorComponent::AlignComponent(G4ThreeVector& TargetPos,
					     G4RotationMatrix *TargetRot, 
					     G4RotationMatrix&,
					     G4ThreeVector&,
					     G4ThreeVector&,
					     G4ThreeVector&,
					     G4ThreeVector&,
					     G4ThreeVector&)
{//do nothing by default, except set rotation and position for BDSOutline
  itsRotation=TargetRot;
  itsPosition=TargetPos;
  return;
}

BDSAcceleratorComponent::~BDSAcceleratorComponent ()
{
  if(itsOuterLogicalVolume)delete itsOuterLogicalVolume;
  //  if(itsBeamPipe)delete itsBeamPipe;
  if(itsBDSEnergyCounter)delete itsBDSEnergyCounter;
  if(itsOuterUserLimits)delete itsOuterUserLimits;
  if(itsOuterLogicalVolume)delete itsOuterLogicalVolume;
  if(itsMarkerLogicalVolume)delete itsMarkerLogicalVolume;
  if(itsInnerBeampipeUserLimits)delete itsInnerBeampipeUserLimits;
  //Solid shapes used in building tunnel
  delete itsTunnelSolid;
  delete itsSoilSolid;
  delete itsInnerTunnelSolid;
  delete itsTunnelCavity;
  delete itsLargerTunnelCavity;
  delete itsLargerInnerTunnelSolid; 
  delete itsTunnelMinusCavity;
  delete itsTunnelSizedBlock;
  delete itsMarkerSolidVolume;
  //Tunnel logical volumes
  delete itsSoilTunnelLogicalVolume;
  delete itsTunnelCavityLogicalVolume;
  delete  itsTunnelMinusCavityLogicalVolume;
  //Tunnel physical volumes
  delete itsTunnelPhysiInner;
  delete itsTunnelPhysiComp;
  delete itsTunnelPhysiCompSoil;
  //Tunnel user limits
  delete itsTunnelUserLimits;
  delete itsSoilTunnelUserLimits;
  delete itsInnerTunnelUserLimits;
  
}

void BDSAcceleratorComponent::BuildTunnel()
{
  //Make sure length is not zero
  if(itsLength <= BDSGlobals->GetLengthSafety()){
    G4cout << "BDSAcceleratorComponent::BuildTunnel() - WARNING - " << G4endl;
    G4cout << "Components length, " << itsLength << ", less than or equal to safety length, " << BDSGlobals->GetLengthSafety() << ". Not building tunnel." << G4endl;
    return;
  }

  std::string tunnelMaterialName;
  if(itsTunnelMaterial!=""){
    tunnelMaterialName=itsTunnelMaterial;
  } else {
    tunnelMaterialName =BDSGlobals->GetTunnelMaterialName();
  }
  G4Material *tunnelMaterial=theMaterials->GetMaterial(tunnelMaterialName);
  
  std::string soilMaterialName =BDSGlobals->GetSoilMaterialName();
  G4Material *soilMaterial=theMaterials->GetMaterial(soilMaterialName);



   

  #ifdef DEBUG
  G4cout << "Soil :"
         << " r= " << (BDSGlobals->GetTunnelRadius()+BDSGlobals->GetTunnelThickness())/m + BDSGlobals->GetTunnelSoilThickness()/m<< " m"
         << " l= " << itsLength/m << " m"
         << " material = " << soilMaterialName << " m"
         << G4endl;
  G4cout << "Outer tunnel :"
         << " r= " << (BDSGlobals->GetTunnelRadius()+BDSGlobals->GetTunnelThickness())/m << " m"
         << " l= " << itsLength/m << " m"
         << " material = " << tunnelMaterialName << " m"
         << G4endl;
  G4cout << "Inner tunnel :"
         << " r= " << BDSGlobals->GetTunnelRadius()/m << " m"
         << " l= " << itsLength/m << " m"
         << G4endl;
#endif

  G4RotationMatrix *tunnelRot=new G4RotationMatrix();
  G4ThreeVector tunnelTrans;
  G4ThreeVector floorOffsetThreeVector;
  G4double blockSize = 4*BDSGlobals->GetTunnelRadius();
  G4ThreeVector nullThreeVector = G4ThreeVector(0,0,0);
  G4RotationMatrix *nullRotationMatrix = new G4RotationMatrix();


  //  if ( itsAngle/itsLength < 1.25e-4 ){ //Build a straight tunnel using tubes
  if ( itsAngle==0 ){ //Build a straight tunnel using tubes
  itsTunnelSizedBlock = new G4Box(
                                  itsName+"_bnd_sized_block_solid", 
                                  blockSize,
                                  blockSize,
                                  (itsLength-BDSGlobals->GetLengthSafety()/2.0)
                                  );

   itsTunnelSolid=new G4Tubs(itsName+"_tun_solid",
                             0,
                             BDSGlobals->GetTunnelRadius()+BDSGlobals->GetTunnelThickness(),
                             (itsLength-BDSGlobals->GetLengthSafety())/2.0,
                             0,twopi*radian);
   
   itsSoilSolid=new G4Tubs(itsName+"_tun_soil_solid",
                           BDSGlobals->GetTunnelRadius()+BDSGlobals->GetTunnelThickness()+BDSGlobals->GetLengthSafety(),
                           BDSGlobals->GetTunnelRadius()+BDSGlobals->GetTunnelThickness()+BDSGlobals->GetLengthSafety()+BDSGlobals->GetTunnelSoilThickness(),	
                           (itsLength-BDSGlobals->GetLengthSafety())/2.0,
                           0,twopi*radian);
   
   itsInnerTunnelSolid=new G4Tubs(itsName+"_inner_tun_solid",
                                  0.,
                                  BDSGlobals->GetTunnelRadius(),
                                  (itsLength-BDSGlobals->GetLengthSafety())/2,
                                  0,twopi*radian);
   
   itsLargerInnerTunnelSolid=new G4Tubs(itsName+"_inner_tun_solid",
					0.,
					BDSGlobals->GetTunnelRadius()+BDSGlobals->GetLengthSafety(),
					(itsLength-BDSGlobals->GetLengthSafety()),
					0,twopi*radian);
   
   tunnelTrans.setX(BDSGlobals->GetTunnelOffsetX());
   tunnelTrans.setY(BDSGlobals->GetTunnelOffsetY());
   tunnelTrans.setZ(0);
   floorOffsetThreeVector = G4ThreeVector(0,-blockSize-BDSGlobals->GetTunnelFloorOffset(),0);

   
   itsTunnelCavity = new G4SubtractionSolid(
                                         itsName +"_tun_cavity_solid", 
                                         itsInnerTunnelSolid,
                                         itsTunnelSizedBlock,
                                         nullRotationMatrix,
                                         floorOffsetThreeVector
                                         );

   itsLargerTunnelCavity = new G4SubtractionSolid(
                                         itsName +"_tun_cavity_solid", 
                                         itsLargerInnerTunnelSolid,
                                         itsTunnelSizedBlock,
                                         nullRotationMatrix,
                                         floorOffsetThreeVector
                                         );
   
  } else {//Build a trapezoidal tunnel to follow the bends


    G4double xHalfLengthPlus, xHalfLengthMinus, tunHalfLen;
    xHalfLengthMinus = (itsLength/itsAngle)*sin(itsAngle/2)
      - fabs(cos(itsAngle/2)) * (BDSGlobals->GetTunnelRadius() + BDSGlobals->GetTunnelThickness()) * tan(itsAngle/2)/2
      + BDSGlobals->GetLengthSafety()/2;
    
    xHalfLengthPlus = (itsLength/itsAngle)*sin(itsAngle/2)
      + fabs(cos(itsAngle/2)) * (BDSGlobals->GetTunnelRadius() + BDSGlobals->GetTunnelThickness()) * tan(itsAngle/2)/2
      + BDSGlobals->GetLengthSafety()/2;
    
    tunHalfLen = std::max(xHalfLengthPlus,xHalfLengthMinus);

    G4cout << "Building a block" << G4endl;
    itsTunnelSizedBlock = new G4Box(
                                    itsName+"_bnd_sized_block_solid", 
                                    blockSize,
                                    blockSize,
                                    tunHalfLen                                    );


    
    G4cout << "Building a tunnel solid" << G4endl;
    itsTunnelSolid = new G4IntersectionSolid(
                                             itsName+"_tun_solid",
                                             new G4Tubs(
                                                        itsName+"_temp_tun_solid",
                                                        0,
                                                        BDSGlobals->GetTunnelRadius()+BDSGlobals->GetTunnelThickness(),
                                                        tunHalfLen,
                                                        0,twopi*radian),			    
                                             itsMarkerLogicalVolume->GetSolid(),
                                             RotYM90,
                                             nullThreeVector
                                             ); 
    
    G4cout << "Building a soil tunnel solid" << G4endl;
    itsSoilSolid = new G4IntersectionSolid(
                                           itsName+"_soil_solid",
                                           new G4Tubs(
                                                      itsName+"_temp_soil_solid",
                                                      BDSGlobals->GetTunnelRadius()+BDSGlobals->GetTunnelThickness()+BDSGlobals->GetLengthSafety(),
                                                      BDSGlobals->GetTunnelRadius()+BDSGlobals->GetTunnelThickness()+BDSGlobals->GetLengthSafety()+BDSGlobals->GetTunnelSoilThickness(),
                                                      tunHalfLen,
                                                      0,twopi*radian),
                                           itsMarkerLogicalVolume->GetSolid(),
                                           RotYM90,
                                           nullThreeVector
                                           ); 
    
    G4cout << "Building inner tunnel solid" << G4endl;
    itsInnerTunnelSolid=new G4IntersectionSolid(
                                                itsName+"_inner_tun_solid",
                                                new G4Tubs(itsName+"_temp_inner_tun_solid",
                                                           0.,
                                                           BDSGlobals->GetTunnelRadius(),
                                                           tunHalfLen,
                                                           0,twopi*radian),
                                                itsMarkerLogicalVolume->GetSolid(),
                                                RotYM90,
                                                nullThreeVector
                                                ); 
    
    G4cout << "Building larger inner tunnel solid" << G4endl;
    itsLargerInnerTunnelSolid= new G4Tubs(itsName+"_temp_inner_tun_solid",
                                          0.,
                                          BDSGlobals->GetTunnelRadius()+BDSGlobals->GetLengthSafety(),
                                          2*tunHalfLen,
                                          0,twopi*radian);
                                                       
    
    tunnelTrans.setX(BDSGlobals->GetTunnelOffsetX());
    tunnelTrans.setY(BDSGlobals->GetTunnelOffsetY());
    tunnelTrans.setZ(0);
    floorOffsetThreeVector = G4ThreeVector(0,-blockSize-BDSGlobals->GetTunnelFloorOffset(),0);
    
    G4cout << "Building tunnel cavity" << G4endl;
    itsTunnelCavity = new G4SubtractionSolid(
                                         itsName +"_tun_cavity_solid", 
                                         itsInnerTunnelSolid,
                                         itsTunnelSizedBlock,
                                         nullRotationMatrix,
                                         floorOffsetThreeVector
                                         );
    G4cout << "Building larger tunnel cavity" << G4endl;
   itsLargerTunnelCavity = new G4SubtractionSolid(
                                         itsName +"_tun_cavity_solid", 
                                         itsLargerInnerTunnelSolid,
                                         itsTunnelSizedBlock,
                                         nullRotationMatrix,
                                         floorOffsetThreeVector
                                         );
  }
  
  
  G4cout << "Building tunnel minus cavity" << G4endl;
  itsTunnelMinusCavity = new G4SubtractionSolid(
                                                itsName+"_tun_floor_and_tun",
                                                itsTunnelSolid,
                                                itsLargerTunnelCavity
                                                );
  
  G4cout << "Building tunnel minus cavity logical volume" << G4endl;

  itsTunnelMinusCavityLogicalVolume=	
    new G4LogicalVolume(itsTunnelMinusCavity,
                        tunnelMaterial,
                        itsName+"_tun_flr_and_tun_log");
  
  
  G4cout << "Building soil tunnel logical volume" << G4endl;
  itsSoilTunnelLogicalVolume=	
    new G4LogicalVolume(itsSoilSolid,
                        soilMaterial,
                        itsName+"_tun_soil_log");
  G4cout << "Building tunnel logical volume" << G4endl;
  itsTunnelLogicalVolume=	
    new G4LogicalVolume(itsTunnelSolid,
                        tunnelMaterial,
                        itsName+"_tun_log");
  G4cout << "Building tunnel cavity logical volume" << G4endl;
  itsTunnelCavityLogicalVolume=	
    new G4LogicalVolume(itsLargerTunnelCavity,
                        theMaterials->GetMaterial("Air"),
                        itsName+"_tun_cav_log");
  
  G4cout << "Placing tunnel minus cavity" << G4endl;
  itsTunnelPhysiComp = new G4PVPlacement(
                                         tunnelRot,		     // no rotation
                                         tunnelTrans,	                     // at (0,0,0)
                                         itsTunnelMinusCavityLogicalVolume,  // its logical volume
                                         itsName+"_tun_phys",	     // its name
                                         itsMarkerLogicalVolume,    // its mother  volume
                                         true,		     // no boolean operation
                                         0);		             // copy number
  
  
  SetMultiplePhysicalVolumes(itsTunnelPhysiComp);

  G4cout << "Placing soil around tunnel" << G4endl;
  itsTunnelPhysiCompSoil = new G4PVPlacement(
                                             tunnelRot,		     // no rotation
                                             tunnelTrans,	                     // at (0,0,0)
                                             itsSoilTunnelLogicalVolume,  // its logical volume
                                             itsName+"_tun_phys_soil",	     // its name
                                             itsMarkerLogicalVolume,    // its mother  volume
                                             true,		     // no boolean operation
                                             0);		             // copy number
  
  SetMultiplePhysicalVolumes(itsTunnelPhysiCompSoil);
  /*
    itsTunnelPhysiInner = new G4PVPlacement(
    tunnelRot,
    tunnelTrans,	                // at (0,0,0)
    itsTunnelCavityLogicalVolume,  // its logical volume
    itsName+"_inner_tun_phys",// its name
    itsMarkerLogicalVolume,   // its mother  volume
    false,		        // no boolean operation
    0);		        // copy number
  */
  
    G4cout << "Setting user limits" << G4endl;
    itsTunnelUserLimits =
      new G4UserLimits("tunnel cuts",DBL_MAX,DBL_MAX,DBL_MAX,
                       BDSGlobals->GetThresholdCutCharged());
    itsSoilTunnelUserLimits =
      new G4UserLimits("tunnel soil cuts",DBL_MAX,DBL_MAX,DBL_MAX,
                       BDSGlobals->GetThresholdCutCharged());
    itsInnerTunnelUserLimits =
      new G4UserLimits("inner tunnel cuts",DBL_MAX,DBL_MAX,DBL_MAX,
                       BDSGlobals->GetThresholdCutCharged());
    
    itsTunnelUserLimits->SetMaxAllowedStep(itsLength);
    itsSoilTunnelUserLimits->SetMaxAllowedStep(itsLength);
    itsInnerTunnelUserLimits->SetMaxAllowedStep(itsLength);
    
    itsTunnelMinusCavityLogicalVolume->SetUserLimits(itsTunnelUserLimits);
    itsSoilTunnelLogicalVolume->SetUserLimits(itsSoilTunnelUserLimits);
    itsTunnelCavityLogicalVolume->SetUserLimits(itsInnerTunnelUserLimits);

    G4cout << "Setting vis attributes" << G4endl;
    //
    // set visualization attributes
    //
  G4VisAttributes* VisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));
  if (itsAngle==0){
    VisAtt->SetVisibility(false);  
  } else {
    VisAtt->SetVisibility(false);
  }
  VisAtt->SetForceSolid(true);
  itsSoilTunnelLogicalVolume->SetVisAttributes(VisAtt);
  G4VisAttributes* VisAtt1 = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4));
  VisAtt1->SetVisibility(BDSGlobals->GetShowTunnel());
  VisAtt1->SetForceSolid(true);
  G4VisAttributes* VisAtt2 = new G4VisAttributes(G4Colour(0.0, 0.5, 0.5));
  VisAtt2->SetVisibility(false);
  VisAtt2->SetForceSolid(true);
  G4VisAttributes* VisAtt3 = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4));
  VisAtt3->SetVisibility(false);
  VisAtt3->SetForceSolid(true);
  itsTunnelMinusCavityLogicalVolume->SetVisAttributes(VisAtt1);
  itsMarkerLogicalVolume->SetVisAttributes(VisAtt);
  itsTunnelCavityLogicalVolume->SetVisAttributes(VisAtt2);
}

void BDSAcceleratorComponent::BuildBLMs()
{
 if(itsBlmLocZ.size() > 0){

   //declare materials
   G4Material *blmMaterial;
   blmMaterial = theMaterials->GetMaterial("nitrogen");
   G4Material *caseMaterial;
   caseMaterial = theMaterials->GetMaterial("nitrogen");
   
   /*
     blmCaseMaterial = theMaterials->GetMaterial("cement");
   */

   //declare Matrices & doubles
   G4RotationMatrix blmRot = G4RotationMatrix();
   G4ThreeVector blmTrans;
   G4double blmYRot;
   G4double blmXRot;
   G4double blmYTrans;
   G4double blmXTrans;
   G4double blmCaseThickness;
   
   G4AssemblyVolume* assemblyBlms= new G4AssemblyVolume();
   G4Transform3D blmTr3d;
   
   //declare vis attributes
   G4VisAttributes* VisAtt1 = new G4VisAttributes(G4Colour(0.5, 0.5, 0.0));
   VisAtt1->SetVisibility(true);
   VisAtt1->SetForceSolid(true);
   G4VisAttributes* VisAtt2 = new G4VisAttributes(G4Colour(0.5, 0.0, 0.5));
   VisAtt2->SetVisibility(true);
   VisAtt2->SetForceSolid(true);

   //set case thickness
   blmCaseThickness = 1e-20*m;

   itsBlmLocationR = itsBlmLocationR + BDSGlobals->GetBlmRad() + BDSGlobals->GetLengthSafety() +itsBpRadius + blmCaseThickness;
   
#ifdef DEBUG
   G4cout << "BDSAccleratorComponent::BuildBLMs() itsBlmLocationRadius = " << itsBlmLocationR/mm << " mm" << G4endl;
#endif
   
   G4double localLength;
   
   //Make sure BLM length can never exceed component length
#ifdef DEBUG
   G4cout << "BLM length = " << BDSGlobals->GetBlmLength()/m << " m" << G4endl;
   G4cout << "BLM radius = " << BDSGlobals->GetBlmRad()/m << " m" << G4endl;
   G4cout << "Component length = " << itsLength/m << " m" << G4endl;
#endif
   if(BDSGlobals->GetBlmLength()>=(itsLength-BDSGlobals->GetLengthSafety())){
     G4cout << itsName << "Warning: BLM longer than element. Setting BLM length to element length." << G4endl;
     localLength = itsLength-BDSGlobals->GetLengthSafety();
   } else {
     localLength=BDSGlobals->GetBlmLength();
   }
   
   G4int i = 0;
   list<double>::iterator zit;
   list<double>::iterator thetait;
   for (zit=itsBlmLocZ.begin(), thetait=itsBlmLocTheta.begin(); zit!=itsBlmLocZ.end(); zit++, thetait++){
#ifdef DEBUG
     G4cout << "Building BLM " << i << G4endl; 
#endif
     G4double indexInt = (G4double)i*blmYRot;
     string index;
     stringstream out;
     out << indexInt;
     index = out.str();
     
     blmYTrans=itsBlmLocationR*cos(*thetait);
     blmXTrans=-1*itsBlmLocationR*sin(*thetait);
     blmTrans.setZ(*zit*1000);
     blmTrans.setY(blmYTrans);
     blmTrans.setX(blmXTrans);
     
     //#ifdef DEBUG
     if((*zit*1000) < 0){
       G4cout << "itsLength is: " << itsLength << endl;
       G4cout << "z position defined is: " << (*zit*1000) << endl;
     }
     //#endif
     
     G4Tubs *itsBLMSolid = new G4Tubs(itsName + "_BLM_sol_" + index,
				      0,
				      BDSGlobals->GetBlmRad(),
				      localLength/2, 0, twopi*radian);
     
     
     G4Tubs *itsBlmOuterSolid = new G4Tubs(itsName + "_Blm_outCase_sol_" + index, 
					   0,
					   BDSGlobals->GetBlmRad() + blmCaseThickness,
					   localLength/2 + blmCaseThickness, 0, twopi*radian);
     
     G4SubtractionSolid *itsBlmCaseSolid = new G4SubtractionSolid(itsName + "_blm_case_subsolid_" + index,
								  itsBlmOuterSolid,
								  itsBLMSolid);       
       
       itsBLMLogicalVolume = new G4LogicalVolume(itsBLMSolid,
						 blmMaterial,
						 itsName + "_blm_logvol_" + index);
       
       //Set BLM gas as sensitive if option is chosen
       if(BDSGlobals->GetSensitiveBLMs()){
         SetMultipleSensitiveVolumes(itsBLMLogicalVolume);
       }

       itsBlmCaseLogicalVolume = new G4LogicalVolume(itsBlmCaseSolid,
						     caseMaterial,
						     itsName + "_blm_case_logvol_" + index);
       
       itsBlmCaseLogicalVolume->SetVisAttributes(VisAtt2);
       itsBLMLogicalVolume->SetVisAttributes(VisAtt1);
	 
       blmTr3d=G4Transform3D(blmRot,blmTrans);
     
       assemblyBlms->AddPlacedVolume(itsBLMLogicalVolume,blmTr3d);
       assemblyBlms->AddPlacedVolume(itsBlmCaseLogicalVolume,blmTr3d);
   }  
   if (itsType=="sbend" || itsType=="rbend"){
     blmRot.setTheta(twopi/4.0);
     blmRot.setPsi(-twopi/4.0);
     blmRot.setPhi(twopi/4.0);
   } else {
     blmRot.setTheta(0.0);
     blmRot.setPsi(0.0);
     blmRot.setPhi(0.0);
   }
   blmTrans.setX(0.0);
   blmTrans.setY(0.0);
   blmTrans.setZ(0.0);
   blmTr3d=G4Transform3D(blmRot,blmTrans);
   assemblyBlms->MakeImprint(itsMarkerLogicalVolume,blmTr3d);
 }
}


//This Method is for investigating the Anomalous signal at LHc junction IP8

void BDSAcceleratorComponent::BuildGate()
{
  //Declare variables, matrices and constants to use
  G4RotationMatrix *gateRot = new G4RotationMatrix();
  G4ThreeVector gateTrans;
  G4ThreeVector floorOffsetThreeVector;
  G4Material *gateMaterial;

  //Set basic variables
  G4double gateX = 0.05*m;
  G4double gateY = 1.0*m;
  G4double gateZ = 0.63*0.99*m;//1.5*m;
  G4double gateXOffset = 3*m;
  G4double gateYOffset = -BDSGlobals->GetTunnelFloorOffset()/2 + gateY/2;
  G4double gateZOffset = -4.095*m; //Shifted along z 4.095m from centre of dipole
  gateMaterial = theMaterials->GetMaterial("iron");
  
  //Set position and rotation of gate
  gateTrans.setX(gateZOffset);
  gateTrans.setY(gateYOffset);
  gateTrans.setZ(gateXOffset);

  G4Box* itsGateSolid = new G4Box(itsName + "_gate_Solid",
			      gateZ,
			      gateY,
			      gateX
			      );
  G4LogicalVolume* itsGateLogicalVolume = new G4LogicalVolume(itsGateSolid,
							  gateMaterial,
							  itsName + "_gate_Log_Vol");

  G4PVPlacement*  itsGatePhysi = new G4PVPlacement(gateRot,
						  gateTrans,
						  itsGateLogicalVolume,
						  itsName + "_gate_physi",
						  itsMarkerLogicalVolume,
						  false,
						  0
						  );
  SetMultiplePhysicalVolumes(itsGatePhysi);
  
  G4VisAttributes* VisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
  VisAtt->SetVisibility(true);
  VisAtt->SetForceSolid(true);
  itsGateLogicalVolume->SetVisAttributes(VisAtt);
  }
