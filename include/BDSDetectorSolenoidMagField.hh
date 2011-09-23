#ifndef BDSDETECTORSOLENOIDMAGFIELD
#define BDSDETECTORSOLENOIDMAGFIELD

#include "G4Types.hh"
#include "G4MagneticField.hh"
#include "BDSField.hh"

class BDSDetectorSolenoidMagField : public BDSField
{
public: 

  BDSDetectorSolenoidMagField(G4double BIn, G4double Bout, G4double radiusIn, G4double radiusOut, G4double zMin, G4double zMax);
  virtual ~BDSDetectorSolenoidMagField();
  
  virtual void  GetFieldValue( const G4double Point[4],
			       G4double *Bfield ) const;
private:
  G4double itsBIn, itsBOut, itsRadiusIn, itsRadiusOut, itsZMin, itsZMax;
};

#endif /* BDSDETECTORSOLENOIDMAGFIELD */
