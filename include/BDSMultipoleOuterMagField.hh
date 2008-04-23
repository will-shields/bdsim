/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/
#ifndef BDSMULTIPOLEOUTERMAGFIELD
#define BDSMULTIPOLEOUTERMAGFIELD

#include "G4Types.hh"
#include "G4MagneticField.hh"

class BDSMultipoleOuterMagField : public G4MagneticField
{
public:  // with description

  BDSMultipoleOuterMagField(G4int nPole,G4double polField, G4double phiOffset);
  virtual ~BDSMultipoleOuterMagField();
  // Constructor and destructor. No actions.


  virtual void  GetFieldValue( const G4double Point[4],
			       G4double *Bfield ) const;

private:
  G4int itsNPoles;
  G4double itsIronField;
  G4double itsPhiOffset;
  G4double itsSectorPhi;
};


#endif /* BDSMULTIPOELOUTERMAGFIELD */
