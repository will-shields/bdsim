#ifndef BDSGeometryInterface_h
#define BDSGeometryInterface_h 1

#include "globals.hh"

class BDSGeometryInterface
{
public:
  BDSGeometryInterface(G4String filename);  
  ~BDSGeometryInterface();

  void Optics();
  void Survey();
  
protected:

  G4String itsFileName;

private:

};

#endif
