#ifndef BDSINTEGRATORSOLENOID_H
#define BDSINTEGRATORSOLENOID_H

#include "BDSAuxiliaryNavigator.hh"

#include "globals.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4Mag_EqRhs.hh"
#include "G4ThreeVector.hh"

class BDSIntegratorSolenoid:
  public G4MagIntegratorStepper, public BDSAuxiliaryNavigator
{
public:
  BDSIntegratorSolenoid(G4Mag_EqRhs *EqRhs);
  ~BDSIntegratorSolenoid();

  void Stepper( const G4double y[],
		const G4double dydx[],
		const G4double h,
		G4double yout[],
		G4double yerr[]  );
  // The stepper for the Runge Kutta integration.
  // The stepsize is fixed, equal to h.
  // Integrates ODE starting values y[0 to 6]
  // Outputs yout[] and its estimated error yerr[].

  G4double DistChord() const;
  // Estimate maximum distance of curved solution and chord ... 
 
  void SetBField(G4double aBField);
  G4double GetBField();

  void StepperName();
  
  G4int IntegratorOrder()const { return 2; }

protected:
  void AdvanceHelix( const G4double  yIn[],
		     const G4double dydx[],
		     G4ThreeVector Bfld,
		     G4double  h,
		     G4double  yOut[], // output 
		     G4double  yErr[]);    
  // A first order Step along a solenoid inside the field.

private:
  G4MagIntegratorStepper* backupStepper; // use for high amplitude particles
  G4Mag_EqRhs* fPtrMagEqOfMot;
  G4double itsBField;
  G4double itsDist;
  const G4int nvar;
  //G4double pitch;
};

inline  void BDSIntegratorSolenoid::SetBField(G4double aBField)
{itsBField=aBField;
}

inline G4double BDSIntegratorSolenoid::GetBField()
{return itsBField;}

inline void BDSIntegratorSolenoid::StepperName()
{G4cout<<"BDSIntegratorSolenoid"<<G4endl;}

#endif