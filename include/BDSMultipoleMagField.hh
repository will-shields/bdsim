/** BDSIM, v0.1   

Last modified 17.04.2006 by Ilya Agapov

**/

//==============================================================


#ifndef BDSMultipoleMagField_h
#define BDSMultipoleMagField_h

#include "G4Types.hh"
#include "G4MagneticField.hh"

class BDSMultipoleMagField : public G4MagneticField
{
public:  // with description

  BDSMultipoleMagField(list<G4double> knl, list<G4double> ksl);
  virtual ~BDSMultipoleMagField();
  // Constructor and destructor. No actions.


  virtual void  GetFieldValue( const G4double Point[4],
			       G4double *Bfield ) const;

public:
  
  list<G4double> bnl;
  list<G4double> bsl;

};


#endif 
