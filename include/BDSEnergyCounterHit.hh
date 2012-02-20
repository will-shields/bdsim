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
  BDSEnergyCounterHit(G4int nCopy, G4double Energy=0, G4double x=0, G4double y=0, G4double z=0, G4String name="", G4int partID=0, G4double weight=1);

  ~BDSEnergyCounterHit();

   BDSEnergyCounterHit(const BDSEnergyCounterHit& right);
   const BDSEnergyCounterHit& operator=(const BDSEnergyCounterHit& right);

   inline void* operator new(size_t) ;
   inline void operator delete(void *aHit);

  inline G4double GetEnergy();
  inline G4int GetCopyNumber();
  inline G4String GetName();
  inline void SetEnergy(G4double Energy);
  inline void AddEnergy(G4double Energy);
  inline void AddPos(G4double x, G4double y, G4double z);
  inline G4double GetZ(); 
  inline G4double GetX(); 
  inline G4double GetY(); 
  inline G4int GetPartID(); 
  inline G4double GetWeight(); 
  inline void SetWeight(G4double weight);
  
private:
  G4double itsEnergy;
  G4double itsX;
  G4double itsY;
  G4double itsZ;
  G4double itsWeight;
  //   G4String itsName
  G4int itsCopyNumber;
  G4int itsPartID;
  G4String itsName;
};

inline G4double BDSEnergyCounterHit::GetEnergy()
{return itsEnergy;}

inline G4double BDSEnergyCounterHit::GetX()
{return itsX;}

inline G4double BDSEnergyCounterHit::GetY()
{return itsY;}

inline G4double BDSEnergyCounterHit::GetZ()
{
  return itsZ;
}

inline G4double BDSEnergyCounterHit::GetWeight()
{
  return itsWeight;
}

inline G4int BDSEnergyCounterHit::GetCopyNumber()
{return itsCopyNumber;}

inline G4int BDSEnergyCounterHit::GetPartID()
{return itsPartID;}

inline G4String BDSEnergyCounterHit::GetName()
{return itsName;}

inline void BDSEnergyCounterHit::SetEnergy(G4double Energy)
{itsEnergy=Energy;}   


inline void BDSEnergyCounterHit::AddEnergy(G4double Energy)
{itsEnergy+=Energy;}   

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
