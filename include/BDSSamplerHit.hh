/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 5.11.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 

   Modified 22.03.05 by J.C.Carter, Royal Holloway, Univ. of London.
   Changed Samplers to account for plane and cylinder types (GABs code)
*/

#ifndef BDSSamplerHit_h
#define BDSSamplerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class BDSSamplerHit :public G4VHit
{
  public:
   BDSSamplerHit();
   BDSSamplerHit(G4String aName,
		 G4double init_z,G4double init_mom,
		 G4double init_x, G4double init_xPrime, 
		 G4double init_y, G4double init_yPrime,
		 G4double z,G4double mom,
		 G4double x, G4double xPrime, G4double y, 
		 G4double yPrime, G4double time, G4double weight,
		 G4int PDGtype,G4int nEvent,
		 G4int ParentID, G4int TrackID);


  ~BDSSamplerHit();
  
   BDSSamplerHit(const BDSSamplerHit& right);
  const BDSSamplerHit& operator=(const BDSSamplerHit& right);
  
  inline void* operator new(size_t) ;
  inline void operator delete(void *aHit);
  
private:
  G4String itsName;
  G4double itsInit_Z;
  G4double itsInit_Mom;
  G4double itsInit_X;
  G4double itsInit_XPrime;
  G4double itsInit_Y;
  G4double itsInit_YPrime;
  G4double itsZ;
  G4double itsMom;
  G4double itsX;
  G4double itsXPrime;
  G4double itsY;
  G4double itsYPrime;
  G4double itsTime;
  G4double itsWeight;
  G4int itsPDGtype;
  G4int itsEventNo;
  G4int itsParentID;
  G4int itsTrackID;
  G4String itsSampType;
  
public:
  inline void SetInitMom(G4double mom)
    {itsInit_Mom=mom;}
  inline G4double GetInitMom() const
    {return itsInit_Mom;}
  inline void SetInitX(G4double x)
    {itsInit_X=x;}
  inline G4double GetInitX() const
    {return itsInit_X;}
  inline void SetInitXPrime(G4double xPrime)
    {itsInit_XPrime=xPrime;}
  inline G4double GetInitXPrime() const
    {return itsInit_XPrime;}
  inline G4double GetInitY() const
    {return itsInit_Y;}
  inline void SetInitYPrime(G4double yPrime)
    {itsInit_YPrime=yPrime;}
  inline G4double GetInitYPrime() const
    {return itsInit_YPrime;}
  inline void SetInitZ(G4double z)
    {itsInit_Z=z;}
  inline G4double GetInitZ() const
    {return itsInit_Z;}
  
  inline void SetMom(G4double mom)
    {itsMom=mom;}
  inline G4double GetMom() const
    {return itsMom;}
  inline void SetX(G4double x)
    {itsX=x;}
  inline G4double GetX() const
    {return itsX;}
  inline void SetXPrime(G4double xPrime)
    {itsXPrime=xPrime;}
  inline G4double GetXPrime() const
    {return itsXPrime;}
  inline G4double GetY() const
    {return itsY;}
  inline void SetYPrime(G4double yPrime)
    {itsYPrime=yPrime;}
  inline G4double GetYPrime() const
    {return itsYPrime;}
  inline G4double GetTime() const
    {return itsTime;}
  inline void SetTime(G4double time)
    {itsYPrime=time;}
  inline G4double GetWeight() const
    {return itsWeight;}
  inline void SetZ(G4double z)
    {itsZ=z;}
  inline G4double GetZ() const
    {return itsZ;}
  
  inline void SetName(G4String aName)
    {itsName=aName;}
  inline void SetType(G4String aSampType)
    {itsSampType=aSampType;}
  inline G4String GetType()
    {return itsSampType;}
  inline G4String GetName() const
    {return itsName;}
  inline G4int GetPDGtype() const
    {return itsPDGtype;}
  inline void SetEventNo(G4int nEvent)
    {itsEventNo=nEvent;}
  inline G4int GetEventNo() const
    {return itsEventNo;}
  inline G4int GetParentID() const
    {return itsParentID;}
  inline G4int GetTrackID() const
    {return itsTrackID;}
};

typedef G4THitsCollection<BDSSamplerHit> BDSSamplerHitsCollection;
extern G4Allocator<BDSSamplerHit> BDSSamplerHitAllocator;

inline void* BDSSamplerHit::operator new(size_t)
{
  void* aHit;
  aHit=(void*) BDSSamplerHitAllocator.MallocSingle();
  return aHit;
}

inline void BDSSamplerHit::operator delete(void *aHit)
{
  BDSSamplerHitAllocator.FreeSingle((BDSSamplerHit*) aHit);
}

#endif

///
