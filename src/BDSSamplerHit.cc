/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 

   Modified 22.03.05 by J.C.Carter, Royal Holloway, Univ. of London.
   Changed Samplers to account for plane and cylinder types (GABs code)
*/

#include "BDSSamplerHit.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"

G4Allocator<BDSSamplerHit> BDSSamplerHitAllocator;

BDSSamplerHit::BDSSamplerHit()
{;}

BDSSamplerHit::BDSSamplerHit(G4String aName,
G4double init_z,G4double init_mom,
G4double init_x, G4double init_xPrime, G4double init_y, G4double init_yPrime,
G4double z,G4double mom,
G4double x, G4double xPrime, G4double y, G4double yPrime, G4double time, 
			     G4double weight,
			     G4int PDGtype,G4int nEvent,
			     G4int ParentID, G4int TrackID)
  :itsName(aName),
   itsInit_Z(init_z),itsInit_Mom(init_mom),itsInit_X(init_x),
   itsInit_XPrime(init_xPrime),
   itsInit_Y(init_y),itsInit_YPrime(init_yPrime),
   itsZ(z),itsMom(mom),itsX(x),itsXPrime(xPrime),
   itsY(y),itsYPrime(yPrime),itsTime(time),itsWeight(weight),
   itsPDGtype(PDGtype),itsEventNo(nEvent),
   itsParentID(ParentID),    itsTrackID(TrackID)
  {;}

BDSSamplerHit::~BDSSamplerHit()
{;}

BDSSamplerHit::BDSSamplerHit(const BDSSamplerHit &right)
{
  itsName=right.itsName;

  itsInit_Z = right.itsInit_Z;
  itsInit_Mom = right.itsInit_Mom;
  itsInit_X = right.itsInit_X;
  itsInit_XPrime = right.itsInit_XPrime;
  itsInit_Y = right.itsInit_Y;
  itsInit_YPrime = right.itsInit_YPrime;

  itsZ = right.itsZ;
  itsMom = right.itsMom;
  itsX = right.itsX;
  itsXPrime = right.itsXPrime;
  itsY = right.itsY;
  itsYPrime = right.itsYPrime;
  itsTime = right.itsTime;
  itsWeight = right.itsWeight;

  itsPDGtype=right.itsPDGtype;
  itsEventNo=right.itsEventNo;

  itsParentID=right.itsParentID;
  itsTrackID=right.itsTrackID;

  itsSampType=right.itsSampType;

}

const BDSSamplerHit& BDSSamplerHit::operator=(const BDSSamplerHit &right)
{
  itsName= right.itsName;

  itsInit_Z = right.itsInit_Z;
  itsInit_Mom = right.itsInit_Mom;
  itsInit_X = right.itsInit_X;
  itsInit_XPrime = right.itsInit_XPrime;
  itsInit_Y = right.itsInit_Y;
  itsInit_YPrime = right.itsInit_YPrime;

  itsZ = right.itsZ;
  itsMom = right.itsMom;
  itsX = right.itsX;
  itsXPrime = right.itsXPrime;
  itsY = right.itsY;
  itsYPrime = right.itsYPrime;
  itsTime = right.itsTime;
  itsWeight = right.itsWeight;

  itsPDGtype=right.itsPDGtype;
  itsEventNo=right.itsEventNo;

  itsParentID=right.itsParentID;
  itsTrackID=right.itsTrackID;

  itsSampType=right.itsSampType;

  return *this;
}


