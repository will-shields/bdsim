#ifndef __BDSTUNNELSEGMENTSOLIDFACTORY_
#define __BDSTUNNELSEGMENTSOLIDFACTORY_

#include "G4VSolid.hh"

class BDSTunnelSegmentSolidFactory{
public:
  BDSTunnelSegmentSolidFactory();
  ~BDSTunnelSegmentSolidFactory();

  G4VSolid* create(G4String name, G4double r, G4double angle, G4double length);
};
#endif
