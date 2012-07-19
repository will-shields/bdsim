#ifndef BDSSOLENOIDMAGFIELD
#define BDSSOLENOIDMAGFIELD

#include "G4Types.hh"
#include "G4MagneticField.hh"

class BDSSolenoidMagField : public G4MagneticField
{
public:  // with description

  BDSSolenoidMagField(G4double aBField);
  virtual ~BDSSolenoidMagField();
  // Constructor and destructor. No actions.

  /// Get Field Value (independent of position, since solenoid field is uniform)
  virtual void  GetFieldValue( const G4double* /*Point[4]*/,
			       G4double *Bfield ) const;
  void SetBField(G4double aBField);
private:
  G4double itsBField;

};

inline void BDSSolenoidMagField::SetBField(G4double aBField)
{ itsBField = aBField; }

#endif /* BDSSOLENOIDMAGFIELD */
