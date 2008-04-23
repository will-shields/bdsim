// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: BDSSteppingVerbose.hh,v 1.1.1.1 2004/12/14 18:57:41 agapov Exp $
// GEANT4 tag $Name:  $
//
//  
//---------------------------------------------------------------
//
// BDSSteppingVerbose.hh
//
// Description:
//   This class manages the verbose outputs in G4SteppingManager. 
//   It inherits from G4SteppingVerbose   
//
//---------------------------------------------------------------

class BDSSteppingVerbose;

#ifndef BDSSteppingVerbose_h
#define BDSSteppingVerbose_h 1

#include "G4SteppingVerbose.hh"

class BDSSteppingVerbose : public G4SteppingVerbose {
public:   
// Constructor/Destructor
  BDSSteppingVerbose();
 ~BDSSteppingVerbose();
//
  void StepInfo();
  void TrackingStarted();
//


};

#endif
