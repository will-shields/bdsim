//Based on the Geant4 example examples/advanced/purging_magnet/src/PurgMagTabulatedField3D.cc
#include "G4MagneticField.hh"
#include "G4ios.hh"
#include "BDSMagField.hh"

#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

class BDS3DMagField
#ifndef STANDALONE
// : public G4MagneticField
  : public BDSMagField
#endif
{
  
  // Storage space for the table
  vector< vector< vector< double > > > xField;
  vector< vector< vector< double > > > yField;
  vector< vector< vector< double > > > zField;
  // The dimensions of the table
  int nx,ny,nz; 
  // The physical limits of the defined region
  double minx, maxx, miny, maxy, minz, maxz;
  // The physical extent of the defined region
  double dx, dy, dz;
  double fZoffset;
  bool invertX, invertY, invertZ;

public:
  BDS3DMagField(const char* filename, double zOffset );
  void  GetFieldValue( const  double Point[4],
		       double *Bfield          ) const;
};

