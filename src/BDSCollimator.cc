/* BDSIM code.    Version beta
   Author: Grahame I. Agapov, Royal Holloway, Univ. of London.

*/
#include "BDSGlobalConstants.hh" // must be first in include list
#include "BDSCollimator.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"               
#include "G4SubtractionSolid.hh"
#include "G4UserLimits.hh"
#include "G4TransportationManager.hh"

#include "G4SDManager.hh"
#include "G4UserLimits.hh"
#include "parser/gmad.h"
#include <map>

//============================================================
typedef std::map<G4String,int> LogVolCountMap;
extern LogVolCountMap* LogVolCount;

typedef std::map<G4String,G4LogicalVolume*> LogVolMap;
extern LogVolMap* LogVol;

extern BDSMaterials* theMaterials;
//============================================================

BDSCollimator::BDSCollimator (G4String aName,G4double aLength,G4double bpRad,
			      G4double xAper,G4double yAper, G4int type,
			      G4Material *CollimatorMaterial, G4double outR, 
                              std::list<G4double> blmLocZ, std::list<G4double> blmLocTheta,
                              G4String aTunnelMaterial):
  BDSAcceleratorComponent(aName,
			  aLength,bpRad,xAper,yAper,
			  SetVisAttributes(), blmLocZ, blmLocTheta, aTunnelMaterial),
  itsCollimatorMaterial(CollimatorMaterial), itsOuterR(outR)
{
  if(type==_RCOL) itsType="rcol";
  if(type==_ECOL) itsType="ecol";

  if(itsOuterR==0) itsOuterR = BDSGlobals->GetComponentBoxSize()/2;

  if ( (*LogVolCount)[itsName]==0)
    {
  G4double xLength, yLength;
  xLength = yLength = std::max(itsOuterR,BDSGlobals->GetComponentBoxSize()/2);

  xLength = std::max(xLength, this->GetTunnelRadius()+2*std::abs(this->GetTunnelOffsetX()) + BDSGlobals->GetTunnelThickness()+BDSGlobals->GetTunnelSoilThickness() + 4*BDSGlobals->GetLengthSafety() );   
  yLength = std::max(yLength, this->GetTunnelRadius()+2*std::abs(BDSGlobals->GetTunnelOffsetY()) + BDSGlobals->GetTunnelThickness()+BDSGlobals->GetTunnelSoilThickness()+4*BDSGlobals->GetLengthSafety() );

  itsMarkerLogicalVolume=new G4LogicalVolume
    (new G4Box( itsName+"_marker_log",
                xLength,
		yLength,
		(itsLength+BDSGlobals->GetLengthSafety())/2), //z half length 
     theMaterials->GetMaterial("vacuum"),
     itsName+"_log");

      if(BDSGlobals->GetBuildTunnel()){
        BuildTunnel();
      }
      BuildInnerCollimator();
      BuildBLMs();

      itsSolidLogVol->SetVisAttributes(SetVisAttributes());

      // visual attributes
      G4VisAttributes* VisAtt1 =
        new G4VisAttributes(G4Colour(0., 0., 0.));
      VisAtt1->SetForceSolid(true);
      itsInnerLogVol->SetVisAttributes(VisAtt1);

      (*LogVolCount)[itsName]=1;
      (*LogVol)[itsName]=itsMarkerLogicalVolume;
    }
  else
    {
      (*LogVolCount)[itsName]++;
      itsMarkerLogicalVolume=(*LogVol)[itsName];
    }  
}

void BDSCollimator::BuildBLMs(){
  itsBlmLocationR=itsOuterR*2;
  BDSAcceleratorComponent::BuildBLMs();
}

G4VisAttributes* BDSCollimator::SetVisAttributes()
{
  itsVisAttributes=new G4VisAttributes(G4Colour(0.3,0.4,0.2));
  itsVisAttributes->SetForceSolid(true);
  return itsVisAttributes;
}

void BDSCollimator::BuildInnerCollimator()
{

  // zero aperture --> no aperture
  if(itsXAper <= 0) itsXAper = BDSGlobals->GetComponentBoxSize()/2;
  if(itsYAper <= 0) itsYAper = BDSGlobals->GetComponentBoxSize()/2;

  if(itsType == "rcol")
    {
      itsInnerSolid=new G4Box(itsName+"_inner",
			      itsXAper,
			      itsYAper,
			      itsLength/2);
    }
  
  if(itsType == "ecol")
    {
      itsInnerSolid=new G4EllipticalTube(itsName+"_inner",
					 itsXAper,
					 itsYAper,
					 itsLength/2);
    }
  
  itsInnerLogVol=
    new G4LogicalVolume(itsInnerSolid,
			theMaterials->GetMaterial(BDSGlobals->GetVacuumMaterial()),
			itsName+"_inner_log");
  
  itsOuterSolid = new G4Box(itsName+"_outer_solid",
			    itsOuterR,
			    itsOuterR,
			    itsLength/2);

  G4ThreeVector nullThreeVector = G4ThreeVector(0,0,0);
  G4RotationMatrix *nullRotationMatrix = new G4RotationMatrix();  

  
  itsSolid = new G4SubtractionSolid(itsName+"_solid",itsOuterSolid,itsInnerSolid, nullRotationMatrix, nullThreeVector);

  itsSolidLogVol=
    new G4LogicalVolume(itsOuterSolid,
                        itsCollimatorMaterial,
			itsName+"_solid_log");
  
 #ifndef NOUSERLIMITS
  itsUserLimits = new G4UserLimits();
  itsUserLimits->SetMaxAllowedStep(itsLength);
  itsUserLimits->SetUserMaxTime(BDSGlobals->GetMaxTime());
  itsUserLimits->SetUserMinEkine(BDSGlobals->GetThresholdCutCharged());
  itsSolidLogVol-> SetUserLimits(itsUserLimits);
  itsInnerLogVol-> SetUserLimits(itsUserLimits);
  itsMarkerLogicalVolume->SetUserLimits(itsUserLimits);
#endif
  itsPhysiComp = 
    new G4PVPlacement(
		      nullRotationMatrix,   // no rotation
		      nullThreeVector,        // its position
		      itsSolidLogVol,    // its logical volume
		      itsName+"_solid_phys",	     // its name
		      itsMarkerLogicalVolume, // its mother  volume
		      false,		     // no boolean operation
		      0);		     // copy number  


    itsPhysiComp2 = 
    new G4PVPlacement(
    nullRotationMatrix,  // no rotation
    nullThreeVector,     // its position
    itsInnerLogVol,      // its logical volume
    itsName+"_inner_phys", // its name
    itsSolidLogVol,      // its mother  volume
    false,		   // no boolean operation
    0);		   // copy number 

  if(BDSGlobals->GetSensitiveComponents()){
    SetSensitiveVolume(itsSolidLogVol);
  }
  SetMultiplePhysicalVolumes(itsPhysiComp);
  SetMultiplePhysicalVolumes(itsPhysiComp2);
}


BDSCollimator::~BDSCollimator()
{
  if(itsVisAttributes) delete itsVisAttributes;
  if(itsUserLimits) delete itsUserLimits;
  if(itsInnerTunnelUserLimits) delete itsInnerTunnelUserLimits;
  if(itsTunnelUserLimits) delete itsTunnelUserLimits;
  if(itsSoilTunnelUserLimits) delete itsSoilTunnelUserLimits;

  if(itsMarkerLogicalVolume)delete itsMarkerLogicalVolume;
  if(itsSolidLogVol)delete itsSolidLogVol;
  if(itsInnerLogVol)delete itsInnerLogVol;

  if(itsPhysiComp) delete itsPhysiComp;
  if(itsPhysiComp2) delete itsPhysiComp2;

  if(itsSoilTube) delete itsSoilTube;
  if(itsTunnelTube) delete itsTunnelTube;
  if(itsInnerTunnelTube) delete itsInnerTunnelTube;
  if(itsInnerTunnelLogicalVolume) delete itsInnerTunnelLogicalVolume;
  if(itsSoilTunnelLogicalVolume) delete itsSoilTunnelLogicalVolume;
}
