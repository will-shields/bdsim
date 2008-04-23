/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/

#ifndef BDSEnergyCounterHit_h
#define BDSEnergyCounterHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class BDSEnergyCounterHit :public G4VHit
{
  public:
   BDSEnergyCounterHit();
   BDSEnergyCounterHit(G4int nCopy, G4double Energy=0, G4double EnWeightZ=0.,
	G4int partID=0, G4int parentID=0);

  ~BDSEnergyCounterHit();

   BDSEnergyCounterHit(const BDSEnergyCounterHit& right);
   const BDSEnergyCounterHit& operator=(const BDSEnergyCounterHit& right);

   inline void* operator new(size_t) ;
   inline void operator delete(void *aHit);

   inline G4double GetEnergy();
   inline G4int GetCopyNumber();
   inline G4int GetPartID();
   inline G4int GetParentID();
   inline void SetEnergy(G4double Energy);
   inline void AddEnergy(G4double Energy);
   inline void AddEnergyWeightedPosition(G4double EnergyWeightedPosition);
   G4double GetEnergyWeightedPosition(); 

  private:
   G4double itsEnergy;
  //   G4String itsName;
   G4int itsCopyNumber;
   G4int itsPartID;
   G4int itsParentID;
   G4double itsEnergyWeightedPosition;
};

    inline G4double BDSEnergyCounterHit::GetEnergy()
    {return itsEnergy;}

  //    inline G4String GetName()
  //   {return itsName;}

    inline G4int BDSEnergyCounterHit::GetCopyNumber()
    {return itsCopyNumber;}

    inline G4int BDSEnergyCounterHit::GetPartID()
    {return itsPartID;}

    inline G4int BDSEnergyCounterHit::GetParentID()
    {return itsParentID;}

   inline void BDSEnergyCounterHit::SetEnergy(G4double Energy)
    {itsEnergy=Energy;}   
   inline void BDSEnergyCounterHit::AddEnergy(G4double Energy)
    {
itsEnergy+=Energy;}   

    inline void BDSEnergyCounterHit::
       AddEnergyWeightedPosition(G4double EnergyWeightedPosition)
{
 itsEnergyWeightedPosition += EnergyWeightedPosition;
}

    inline G4double BDSEnergyCounterHit::
       GetEnergyWeightedPosition()
{
return itsEnergyWeightedPosition;
}

typedef G4THitsCollection<BDSEnergyCounterHit> BDSEnergyCounterHitsCollection;
extern G4Allocator<BDSEnergyCounterHit> BDSEnergyCounterHitAllocator;

inline void* BDSEnergyCounterHit::operator new(size_t)
{
  void* aHit;
  aHit=(void*) BDSEnergyCounterHitAllocator.MallocSingle();
  return aHit;
}

inline void BDSEnergyCounterHit::operator delete(void *aHit)
{
 BDSEnergyCounterHitAllocator.FreeSingle((BDSEnergyCounterHit*) aHit);
}

#endif

///
