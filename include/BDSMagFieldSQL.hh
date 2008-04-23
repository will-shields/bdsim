/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/

#ifndef BDSMagFieldSQL_h
#define BDSMagFieldSQL_h 

#include "G4Types.hh"
#include "G4MagneticField.hh"

#include "G4VPhysicalVolume.hh"
#include "BDSField.hh"
#include <string>
#include <vector>

//using namespace std;

class BDSMagFieldSQL : public BDSField
{
public:  // with description

  BDSMagFieldSQL(const G4String& FieldFile, G4double aMarkerLength,
		 std::vector<G4String> Quadvol, std::vector<G4double> QuadBgrad,
		 std::vector<G4String> Sextvol, std::vector<G4double> SextBgrad,
		 std::vector<G4String> Octvol, std::vector<G4double> OctBgrad,
		 std::vector<G4String> Fieldvol, std::vector<G4ThreeVector> UniformField);
  virtual ~BDSMagFieldSQL();
  // Constructor and destructor. No actions.


  void  GetFieldValue( const G4double Point[4],G4double *Bfield ) const;

  void Prepare(G4VPhysicalVolume *referenceVolume);

private:
  ofstream ofs;
  ifstream ifs;

  G4double itsMarkerLength;
  G4String FieldFile;

  // For Quad/Sext/Oct Fields
  std::vector<G4double> itsQuadBgrad;
  std::vector<G4String> itsQuadVol; 
  std::vector<G4double> itsSextBgrad;
  std::vector<G4String> itsSextVol; 
  std::vector<G4double> itsOctBgrad;
  std::vector<G4String> itsOctVol; 

  // For Uniform Fields
  std::vector<G4ThreeVector> itsUniformField;
  std::vector<G4String> itsFieldVol; 

  // For Solenoid FieldMaps
  std::vector<G4double> itsBz;
  std::vector<G4double> itsZ;
  std::vector<G4double> itsBr_over_r;
  std::vector<G4double> itsdBz_by_dz;
  double itsdz;
};

#endif
