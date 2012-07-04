//Based on the Geant4 example examples/advanced/purging_magnet/src/PurgMagTabulatedField3D.cc
#include "G4MagneticField.hh"
#include "G4ios.hh"
#include "BDSMagField.hh"

#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

class BDSXYMagField2
#ifndef STANDALONE
// : public G4MagneticField
  : public BDSMagField
#endif
{
  
  // Storage space for the table
  vector< vector< double > > xField;
  vector< vector< double > > yField;
  vector< vector< double > > zField;
  // The dimensions of the table
  int nx,ny; 
  // The physical limits of the defined region
  double minx, maxx, miny, maxy;
  // The physical extent of the defined region
  double dx, dy;
  bool invertX, invertY;

public:
  BDSXYMagField2(const char* filename);
  void  GetFieldValue( const  double Point[4],
		       double *Bfield          ) const;
};

