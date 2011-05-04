/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/

#include "BDSEnergyCounterHit.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"

G4Allocator<BDSEnergyCounterHit> BDSEnergyCounterHitAllocator;

BDSEnergyCounterHit::BDSEnergyCounterHit(G4int nCopy, G4double anEnergy, 
    G4double EnWeightZ, G4int partID, G4int parentID):
    itsEnergy(anEnergy),itsCopyNumber(nCopy),
    itsPartID(partID),itsParentID(parentID),
    itsEnergyWeightedPosition(EnWeightZ)
{;}


BDSEnergyCounterHit::BDSEnergyCounterHit()
{;}


BDSEnergyCounterHit::~BDSEnergyCounterHit()
{;}

BDSEnergyCounterHit::BDSEnergyCounterHit(const BDSEnergyCounterHit &right):G4VHit()
{
  itsEnergy = right.itsEnergy;
  itsCopyNumber = right.itsCopyNumber;
  itsPartID = right.itsPartID;
  itsParentID = right.itsParentID;
}

const BDSEnergyCounterHit& BDSEnergyCounterHit::operator=(const BDSEnergyCounterHit &right)
{
  itsEnergy = right.itsEnergy;
  itsCopyNumber = right.itsCopyNumber;
  itsPartID= right.itsPartID;
  itsParentID = right.itsParentID;
  return *this;
}


