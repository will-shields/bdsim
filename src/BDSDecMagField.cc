/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 25.12.2003
   Copyright (c) 2003 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/
#include "BDSGlobalConstants.hh" // must be first in include list

#include "BDSDecMagField.hh"

#include "G4Navigator.hh"
#include "G4TransportationManager.hh"

BDSDecMagField::BDSDecMagField(G4double aBQuadPrime):itsBQuadPrime(aBQuadPrime){}
 
BDSDecMagField::~BDSDecMagField(){}

void BDSDecMagField::GetFieldValue( const G4double Point[4],
		       G4double *Bfield ) const
{

     
  G4Navigator* DecNavigator=
    G4TransportationManager::GetTransportationManager()->
    GetNavigatorForTracking();

  // gab_dec03>>
  G4ThreeVector LocalR, GlobalR;

  GlobalR.setX(Point[0]);
  GlobalR.setY(Point[1]);
  GlobalR.setZ(Point[2]);
  //      LocalR = DecNavigator->GetCurrentLocalCoordinate();
  G4AffineTransform GlobalAffine=DecNavigator->GetGlobalToLocalTransform();
  LocalR=GlobalAffine.TransformPoint(GlobalR); 
  // gab_dec03<<

  Bfield[0]= 4*LocalR.x()*LocalR.y()*(LocalR.x()*LocalR.x()
				      -LocalR.y()*LocalR.y())*itsBQuadPrime/24;
  Bfield[1]=(pow(LocalR.x(),4)-6*LocalR.x()*LocalR.x()*LocalR.y()*LocalR.y()+
	     pow(LocalR.y(),4))*itsBQuadPrime/24;
  Bfield[2]=0;

  // factor of 24 is actually 4-factorial.
}



