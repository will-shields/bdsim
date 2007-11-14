//  
//   BDSIM, (C) 2001-2007
//   
//   version 0.4
//  
//
//
//   Solenoid Class
//
//
//   History
//
//     21 Oct 2007 by Marchiori, v.0.4
//

#ifndef __BDSSOLENOID_H
#define __BDSSOLENOID_H

//#define _USE_GEANT4_STEPPER_

#include "globals.hh"


#include "G4VisAttributes.hh"
#include "BDSMultipole.hh"

class G4Mag_UsualEqRhs;
class G4UniformMagField;
class BDSSolenoidStepper;
class BDSSolenoidMagField;

class BDSSolenoid : public BDSMultipole
{
public:
  BDSSolenoid(G4String aName, G4double aLength,
	      G4double bpRad, G4double FeRad,
	      G4double bField, G4double outR, 
	      G4String aMaterial= "", G4String spec="");
  ~BDSSolenoid();

  void SynchRescale(G4double factor);

protected:

private:
  G4double itsBField;

  void BuildBPFieldAndStepper();

  G4VisAttributes* SetVisAttributes();

  // field related objects:
#ifdef _USE_GEANT4_STEPPER_
  BDSSolenoidMagField* itsMagField;
  G4MagIntegratorStepper* itsStepper;
#else
  G4UniformMagField* itsMagField;
  BDSSolenoidStepper* itsStepper;
#endif
  G4Mag_UsualEqRhs* itsEqRhs;
};

#endif

