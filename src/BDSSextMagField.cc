/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/
#include "BDSGlobalConstants.hh" 

#include "BDSSextMagField.hh"

#include "G4Navigator.hh"
#include "G4TransportationManager.hh"

BDSSextMagField::BDSSextMagField(G4double aBDblePrime):
  itsBDblePrime(aBDblePrime){}

BDSSextMagField::~BDSSextMagField(){}

void BDSSextMagField::GetFieldValue( const G4double Point[4],
		       G4double *Bfield ) const
{

  G4Navigator* SextNavigator=
    G4TransportationManager::GetTransportationManager()->
    GetNavigatorForTracking();

  // gab_dec03>>
  G4ThreeVector LocalR, GlobalR;

  GlobalR.setX(Point[0]);
  GlobalR.setY(Point[1]);
  GlobalR.setZ(Point[2]);
  //LocalR = SextNavigator->GetCurrentLocalCoordinate();
  G4AffineTransform GlobalAffine=SextNavigator->GetGlobalToLocalTransform();
  LocalR=GlobalAffine.TransformPoint(GlobalR); 
  // gab_dec03<<


  Bfield[0]=2*LocalR.x()*LocalR.y()*itsBDblePrime/2;
  Bfield[1]=(LocalR.x()*LocalR.x()-LocalR.y()*LocalR.y())*itsBDblePrime/2;
  Bfield[2]=0;

  // factor of 2 is actually 2-factorial.
}



