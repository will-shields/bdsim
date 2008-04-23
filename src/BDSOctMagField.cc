/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 25.12.2003
   Copyright (c) 2003 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/
#include "BDSGlobalConstants.hh" // must be first in include list

#include "BDSOctMagField.hh"

#include "G4Navigator.hh"
#include "G4TransportationManager.hh"

BDSOctMagField::BDSOctMagField(G4double aBTrpPrime):itsBTrpPrime(aBTrpPrime){}

BDSOctMagField::~BDSOctMagField(){}

void BDSOctMagField::GetFieldValue( const G4double Point[4],
		       G4double *Bfield ) const
{

  // gab_dec03>>
  G4Navigator* OctNavigator=
    G4TransportationManager::GetTransportationManager()->
    GetNavigatorForTracking();

  G4ThreeVector LocalR, GlobalR;

  GlobalR.setX(Point[0]);
  GlobalR.setY(Point[1]);
  GlobalR.setZ(Point[2]);
  // LocalR = OctNavigator->GetCurrentLocalCoordinate();
  G4AffineTransform GlobalAffine=OctNavigator->GetGlobalToLocalTransform();
  LocalR=GlobalAffine.TransformPoint(GlobalR); 
  // gab_dec03<<


  Bfield[0]=(3*LocalR.x()*LocalR.x()*LocalR.y()-
	     pow(LocalR.y(),3))*itsBTrpPrime/6.;
  Bfield[1]=(pow(LocalR.x(),3)-LocalR.x()*LocalR.y()*LocalR.y())
    *itsBTrpPrime/6.;
  Bfield[2]=0;

  // factor of 6 is actually 3-factorial.
}



