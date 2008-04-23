/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 9.3.2003
   Copyright (c) 2003 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/
#ifndef BDSPlanckScatterBuilder_h
#define BDSPlanckScatterBuilder_h 1

#include "globals.hh"
#include "G4ios.hh"

#include "BDSPlanckScatter.hh"
#include "G4VProcess.hh"

class BDSPlanckScatterBuilder 
{
public: 
  BDSPlanckScatterBuilder();
  virtual ~BDSPlanckScatterBuilder();
  
public: 
  void Build();
  
protected:
  BDSPlanckScatter thePlanckScatterProcess;
};
// 2003 by G.A Blair (after J.P. Wellisch)
#endif





