/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 12.12.2004
   Copyright (c) 2004 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/
#include "BDSGlobalConstants.hh" 

#include "globals.hh"
#include "BDSMuSpoilerMagField.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"

BDSMuSpoilerMagField::BDSMuSpoilerMagField(G4double aField)
  :itsBField(aField)
{
  MuSpoilerNavigator = new G4Navigator();
}

BDSMuSpoilerMagField::~BDSMuSpoilerMagField(){
  delete MuSpoilerNavigator;
}

void BDSMuSpoilerMagField::GetFieldValue(const G4double Point[4],
					      G4double *Bfield ) const
{

  G4ThreeVector GlobalPosition= G4ThreeVector( Point[0], Point[1], Point[2]);  
  MuSpoilerNavigator->SetWorldVolume(G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->GetWorldVolume()); 
  MuSpoilerNavigator->LocateGlobalPointAndSetup(GlobalPosition);

  G4AffineTransform GlobalAffine=MuSpoilerNavigator->
    GetGlobalToLocalTransform();  
  G4ThreeVector LocalR=GlobalAffine.TransformPoint(GlobalPosition); 

  G4double BFactor=itsBField/LocalR.mag();

  //G4double phi = LocalR.phi();

  Bfield[0]= LocalR.y()*BFactor;
  Bfield[1]= -LocalR.x()*BFactor;
  Bfield[2]=0;
}



