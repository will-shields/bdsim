/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 

   Modified 22.03.05 by J.C.Carter, Royal Holloway, Univ. of London.
   Removed StringFromInt function
   Added/Changed Sampler code for Plane Sampler or Cylinder Sampler (GABs Code)
*/


#ifndef BDSEventAction_h
#define BDSEventAction_h 


#include "BDSGlobalConstants.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"
#include "BDSAcceleratorComponent.hh"
#include "BDSEnergyCounterSD.hh"
#include <list>



class BDSEventAction : public G4UserEventAction
{
public:
  BDSEventAction();
  virtual ~BDSEventAction();

public:
  virtual void   BeginOfEventAction(const G4Event*);
  virtual void   EndOfEventAction(const G4Event*);

  void SetDrawFlag   (G4String val)  {drawFlag = val;};

private:
  G4int                       SamplerCollID_plane;                
  G4int                       SamplerCollID_cylin;                
  G4int                       LWCalorimeterCollID;
  //    G4int                       EnergyCounterCollID;                
  G4String                    drawFlag;

  G4int printModulo;
  G4int itsOutputFileNumber;
  G4int itsRecordSize;

  list<BDSAcceleratorComponent*>::const_iterator iBeam;
  list<BDSEnergyCounterSD*>::const_iterator iEC;

  BDSAcceleratorComponent* LastComp;
  
};

#endif

    
