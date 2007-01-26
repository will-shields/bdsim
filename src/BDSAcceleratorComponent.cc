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




#include "BDSAcceleratorComponent.hh"


void BDSAcceleratorComponent::PrepareField(G4VPhysicalVolume *referenceVolume)
{//do nothing by default
  return;
}

void BDSAcceleratorComponent::SynchRescale(G4double factor)
{
  return;
}

void BDSAcceleratorComponent::AlignComponent(G4ThreeVector& TargetPos,
					     G4RotationMatrix *TargetRot, 
					     G4RotationMatrix& globalRotation,
					     G4ThreeVector& rtot,
					     G4ThreeVector& rlast,
					     G4ThreeVector& localX,
					     G4ThreeVector& localY,
					     G4ThreeVector& localZ)
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

}

