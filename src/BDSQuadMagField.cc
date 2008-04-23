/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 25.12.2003
   Copyright (c) 2003 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/
#include "BDSGlobalConstants.hh" // must be first in include list

#include "BDSQuadMagField.hh"

#include "G4Navigator.hh"
#include "G4TransportationManager.hh"

BDSQuadMagField::BDSQuadMagField(G4double aBGrad):itsBGrad(aBGrad){}
BDSQuadMagField::~BDSQuadMagField(){}

void BDSQuadMagField::GetFieldValue( const G4double Point[4],
		       G4double *Bfield ) const
{
      G4Navigator* QuadNavigator=
	G4TransportationManager::GetTransportationManager()->
	GetNavigatorForTracking();
 
  // gab_dec03>>
  G4ThreeVector LocalR, GlobalR;

  GlobalR.setX(Point[0]);
  GlobalR.setY(Point[1]);
  GlobalR.setZ(Point[2]);
  //LocalR = QuadNavigator->GetCurrentLocalCoordinate();
  G4AffineTransform GlobalAffine=QuadNavigator->GetGlobalToLocalTransform();
  LocalR=GlobalAffine.TransformPoint(GlobalR); 
  // gab_dec03<<
     
      


  Bfield[0]=LocalR.y()*itsBGrad;
  Bfield[1]=LocalR.x()*itsBGrad;
  Bfield[2]=0;

  // factor of 2 is actually 2-factorial.
}



