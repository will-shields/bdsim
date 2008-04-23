/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 9.3.2003
   Copyright (c) 2003 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/
#ifndef BDSSpecificProcesses_h
#define BDSSpecificProcesses_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4VPhysicsConstructor.hh"
#include "BDSLaserWireBuilder.hh"

#include "BDSPlanckScatterBuilder.hh"

//#include "BDSSynchrotronRadiationBuilder.hh"
//#include "BDSeBremsstrahlungBuilder.hh"
//#include "G4GammaConversionToMuonsBuilder.hh"

class BDSSpecificProcesses : public G4VPhysicsConstructor
{
public: 
  BDSSpecificProcesses(const G4String& name ="BDSSpecific");
  virtual ~BDSSpecificProcesses();
  
public: 
  virtual void ConstructParticle();
  virtual void ConstructProcess();
  
private:
  
  BDSLaserWireBuilder  theBDSLaserWireBuilder;
  BDSPlanckScatterBuilder theBDSPlanckScatter;

  //  BDSSynchrotronRadiationBuilder the BDSSynchrotronRadiation;
  //  BDSeBremsstrahlungBuilder theBDSeBremsstrahlung;
  //  BDSGammaConversionBuilder theBDSGammaConversionToMuons;
  
};

// 2002 by J.P. Wellisch

#endif





