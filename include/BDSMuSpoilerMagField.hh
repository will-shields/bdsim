/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 23.12.2004
   Copyright (c) 2004 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/
#ifndef BDSMUSPOILERMAGFIELD
#define BDSMUSPOILERMAGFIELD

#include "G4Types.hh"
#include "G4MagneticField.hh"

class BDSMuSpoilerMagField : public G4MagneticField
{
public:  // with description

  BDSMuSpoilerMagField();
  BDSMuSpoilerMagField(G4double aField);

  void SetFieldValue(G4double aField);

  virtual ~BDSMuSpoilerMagField();
  // Constructor and destructor. No actions.



  virtual void  GetFieldValue( const G4double Point[4],
			       G4double *Bfield ) const;

private:
  G4double itsBField;
};

inline void BDSMuSpoilerMagField::SetFieldValue(G4double aField)
{itsBField=aField;}

#endif /* BDSMUSPOILERERMAGFIELD */
