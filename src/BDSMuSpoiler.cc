/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 12.12.2004
   Copyright (c) 2004 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/
#include "BDSGlobalConstants.hh" // must be first in include list
#include "BDSMuSpoiler.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"               
#include "G4SubtractionSolid.hh"
#include "G4UserLimits.hh"
#include "G4TransportationManager.hh"
#include "G4SDManager.hh"
#include "G4UserLimits.hh"
#include "G4Tubs.hh"


#include <map>

//============================================================
typedef std::map<G4String,int> LogVolCountMap;
extern LogVolCountMap* LogVolCount;

typedef std::map<G4String,G4LogicalVolume*> LogVolMap;
extern LogVolMap* LogVol;
//extern G4double BDS_Threshold_Energy;
extern BDSMaterials* theMaterials;
//============================================================

BDSMuSpoiler::BDSMuSpoiler (G4String& aName,G4double aLength,G4double bpRad,
			    G4double rInner, G4double rOuter,G4double aBField, 
                            std::list<G4double> blmLocZ, std::list<G4double> blmLocTheta,
                            G4String aTunnelMaterial):
  BDSAcceleratorComponent(aName,
			 aLength,bpRad,bpRad,bpRad,
                          SetVisAttributes(), blmLocZ, blmLocTheta, aTunnelMaterial),
  itsBeampipeRadius(bpRad),
  itsInnerRadius(rInner),
  itsOuterRadius(rOuter),
  itsBField(aBField)

{
  G4double minInnRad =  itsBeampipeRadius + BDSGlobals->GetBeampipeThickness() + BDSGlobals->GetLengthSafety()/2.0;
  if( itsInnerRadius < minInnRad ){
    G4cout << "BDSMuSpoiler - WARNING - inner radius less than minimum. Changing inner radius from " << itsInnerRadius << " to " << minInnRad << G4endl;
    itsInnerRadius = minInnRad;
  }

  itsType="muspoiler";
  SetVisAttributes();
  SetBPVisAttributes();
  if ( (*LogVolCount)[itsName]==0)
    {
  G4double xLength, yLength;
  G4double totalTunnelRadius = BDSGlobals->GetTunnelRadius()+BDSGlobals->GetTunnelThickness()+BDSGlobals->GetTunnelSoilThickness()+std::max(BDSGlobals->GetTunnelOffsetX(),BDSGlobals->GetTunnelOffsetY());
  
  xLength = yLength = std::max(itsOuterRadius,BDSGlobals->GetComponentBoxSize()/2);
  xLength = yLength = std::max(xLength,totalTunnelRadius);

#ifdef DEBUG 
  G4cout<<"marker volume : x/y="<<xLength/m<<
    " m, l= "<<  (itsLength+BDSGlobals->GetLengthSafety())/2/m <<" m"<<G4endl;
#endif

  itsMarkerLogicalVolume=new G4LogicalVolume
    (new G4Box( itsName+"_solid",
                xLength,
		yLength,
		(itsLength-BDSGlobals->GetLengthSafety())/2), //z half length 
     theMaterials->GetMaterial("vacuum"),
     itsName+"_log");

  // now protect the fields inside the marker volume by giving the
  // marker a null magnetic field (otherwise G4VPlacement can
  // over-ride the already-created fields, by calling 
  // G4LogicalVolume::AddDaughter, which calls 
  // pDaughterLogical->SetFieldManager(fFieldManager, true) - the
  // latter 'true' over-writes all the other fields
      itsMarkerLogicalVolume->
	SetFieldManager(BDSGlobals->GetZeroFieldManager(),false);
      
      if(BDSGlobals->GetBuildTunnel()){
        BuildTunnel();
      }
      BuildMuSpoiler();

      (*LogVolCount)[itsName]=1;
      (*LogVol)[itsName]=itsMarkerLogicalVolume;
    }
  else
    {
      (*LogVolCount)[itsName]++;
      itsMarkerLogicalVolume=(*LogVol)[itsName];
    }  
}

void BDSMuSpoiler::BuildMuSpoiler()
{
  itsSolidLogVol=
    new G4LogicalVolume(new G4Tubs(itsName+"_solid",
				   itsInnerRadius+BDSGlobals->GetLengthSafety()/2.0,
				   itsOuterRadius-BDSGlobals->GetLengthSafety()/2.0,
				   itsLength/2-BDSGlobals->GetLengthSafety()/2.0,
				   0,twopi*radian),
			theMaterials->GetMaterial("Iron"),
			itsName+"_solid");

  itsInnerLogVol=
    new G4LogicalVolume(new G4Tubs(itsName+"_inner",
				   0.,
				   itsInnerRadius,
				   itsLength/2,
				   0,twopi*radian),
			theMaterials->GetMaterial("Vacuum"),
			itsName+"_inner");

  G4Material *bpMaterial = theMaterials->GetMaterial( BDSGlobals->GetPipeMaterialName() );

  // build beampipe

#ifdef DEBUG 
  G4cout << "Outer pipe :"
         << " r= " << itsBpRadius/m << " m"
         << " l= " << itsLength/(2.)/m << " m"
         << G4endl;
#endif

  itsBPTube=new G4Tubs(itsName+"_bmp_solid",
		       itsBpRadius+BDSGlobals->GetLengthSafety()/2.0,
		       itsBpRadius+BDSGlobals->GetBeampipeThickness(),
		       itsLength/(2.),
		       0,twopi*radian);

#ifdef DEBUG
  G4cout << "Inner pipe :"
         << " r= " << (itsBpRadius-BDSGlobals->GetBeampipeThickness() )/m
         << " m"
         << " l= " << itsLength/(2.)/m << " m"
         << G4endl;
#endif
  
  itsInnerBPTube=new G4Tubs(itsName+"_inner_bmp_solid",
				0.,
				itsBpRadius,
			    itsLength/2 - BDSGlobals->GetLengthSafety()/2.0,
				0,twopi*radian);

  itsBeampipeLogicalVolume=	
    new G4LogicalVolume(itsBPTube,
			bpMaterial,
			itsName+"_bmp_log");

  itsInnerBPLogicalVolume=	
    new G4LogicalVolume(itsInnerBPTube,
			theMaterials->GetMaterial("Vacuum"),
			itsName+"_inner_bmp_log");



  G4UserLimits* AbsUserLimits =
    new G4UserLimits(DBL_MAX,DBL_MAX,DBL_MAX,
		     BDSGlobals->GetThresholdCutCharged());

  AbsUserLimits->SetMaxAllowedStep(itsLength);
  itsInnerLogVol->SetUserLimits(AbsUserLimits);

  itsSolidLogVol->
    SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,DBL_MAX,
				       BDSGlobals->GetThresholdCutCharged()));

  itsInnerBPLogicalVolume->SetUserLimits(AbsUserLimits);

  itsBeampipeLogicalVolume->
    SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,DBL_MAX,
				       BDSGlobals->GetThresholdCutCharged()));

  itsSolidLogVol->SetVisAttributes(itsVisAttributes);
  itsBeampipeLogicalVolume->SetVisAttributes(itsBPVisAttributes);

  if(itsBField)
    {
      itsMagField=new BDSMuSpoilerMagField(itsBField);
      itsFieldMgr=new G4FieldManager(itsMagField);
      itsSolidLogVol->SetFieldManager(itsFieldMgr,false);
    }
  
  itsPhysiComp2 = 
    new G4PVPlacement(
		      (G4RotationMatrix*)0,		   // no rotation
		      (G4ThreeVector)0,                   // its position
		      itsInnerLogVol,      // its logical volume
		      itsName+"_combined", // its name
		      itsSolidLogVol,      // its mother  volume
		      false,		   // no boolean operation
		      0);		   // copy number 

  //For geometric biasing etc.
  SetMultiplePhysicalVolumes(itsPhysiComp2);

  if(BDSGlobals->GetSensitiveComponents()){
    SetMultipleSensitiveVolumes(itsSolidLogVol);
  }
  if(BDSGlobals->GetSensitiveBeamPipe()){
    SetMultipleSensitiveVolumes(itsBeampipeLogicalVolume);
  }

  BuildBLMs();

  itsPhysiInnerBP = new G4PVPlacement(
				   (G4RotationMatrix*)0,		        // no rotation
				   (G4ThreeVector)0,	                // at (0,0,0)
		      itsInnerBPLogicalVolume,  // its logical volume
		      itsName+"_inner_bmp_phys",// its name
		      itsMarkerLogicalVolume,   // its mother  volume
		      false,		        // no boolean operation
		      0);		        // copy number
  
      itsPhysiBP = new G4PVPlacement(
				  (G4RotationMatrix*)0,			     // no rotation
				  (G4ThreeVector)0,	                     // at (0,0,0)
			  itsBeampipeLogicalVolume,  // its logical volume
			  itsName+"_bmp_phys",	     // its name
			  itsMarkerLogicalVolume,    // its mother  volume
			  false,		     // no boolean operation
			  0);		             // copy number

  itsPhysiComp = 
    new G4PVPlacement(
		      (G4RotationMatrix*)0,		     // no rotation
		      (G4ThreeVector)0,                     // its position
		      itsSolidLogVol,    // its logical volume
		      itsName+"_solid",	     // its name
		      itsMarkerLogicalVolume, // its mother  volume
		      false,		     // no boolean operation
		      0);		     // copy number  

  //For geometric biasing etc.
  SetMultiplePhysicalVolumes(itsPhysiInnerBP);
  SetMultiplePhysicalVolumes(itsPhysiBP);
  SetMultiplePhysicalVolumes(itsPhysiComp);


}

void BDSMuSpoiler::BuildBLMs(){
  itsBlmLocationR=itsOuterRadius;
  BDSAcceleratorComponent::BuildBLMs();
}


G4VisAttributes* BDSMuSpoiler::SetVisAttributes()
{
  itsVisAttributes=new G4VisAttributes(G4Colour(0.0,0.5,0.5));
  itsVisAttributes->SetForceSolid(true);
  itsVisAttributes->SetVisibility(true);
  return itsVisAttributes;
}

G4VisAttributes* BDSMuSpoiler::SetBPVisAttributes()
{
  itsBPVisAttributes=new G4VisAttributes(G4Colour(0.2,0.2,0.2));
  itsBPVisAttributes->SetForceSolid(true);
  itsBPVisAttributes->SetVisibility(true);
  return itsBPVisAttributes;
}


BDSMuSpoiler::~BDSMuSpoiler()
{
  if(itsVisAttributes) delete itsVisAttributes;
  if(itsBPVisAttributes) delete itsBPVisAttributes;
  if(itsUserLimits) delete itsUserLimits;

  if(itsInnerTunnelUserLimits) delete itsInnerTunnelUserLimits;
  if(itsTunnelUserLimits) delete itsTunnelUserLimits;
  if(itsSoilTunnelUserLimits) delete itsSoilTunnelUserLimits;

  if(itsMarkerLogicalVolume)delete itsMarkerLogicalVolume;
  if(itsSolidLogVol)delete itsSolidLogVol;
  if(itsInnerLogVol)delete itsInnerLogVol;
  if(itsBeampipeLogicalVolume) delete itsBeampipeLogicalVolume;
  if(itsInnerBPLogicalVolume) delete itsInnerBPLogicalVolume;
  
  if(itsBPTube) delete itsBPTube;
  if(itsInnerBPTube) delete itsInnerBPTube;

  if(itsPhysiComp) delete itsPhysiComp;
  if(itsPhysiComp2) delete itsPhysiComp2;
  if(itsPhysiInnerBP) delete itsPhysiInnerBP;
  if(itsPhysiBP) delete itsPhysiBP;
  if(itsPhysiCompSoil) delete itsPhysiCompSoil;

  if(itsMagField) delete itsMagField;
  if(itsFieldMgr) delete itsFieldMgr;

  if(itsSoilTube) delete itsSoilTube;
  if(itsTunnelTube) delete itsTunnelTube;
  if(itsInnerTunnelTube) delete itsInnerTunnelTube;
  if(itsInnerTunnelLogicalVolume) delete itsInnerTunnelLogicalVolume;
  if(itsSoilTunnelLogicalVolume) delete itsSoilTunnelLogicalVolume;
}
