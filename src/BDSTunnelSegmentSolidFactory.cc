#include "BDSTunnelSegmentSolidFactory.hh"
#include "BDSCons.hh"
#include "BDSTorus.hh"

BDSTunnelSegmentSolidFactory::BDSTunnelSegmentSolidFactory(){
}

BDSTunnelSegmentSolidFactory::~BDSTunnelSegmentSolidFactory(){
}

G4VSolid* BDSTunnelSegmentSolidFactory::create(G4String name, G4double r, G4double angle, G4double length){
  if (std::abs(angle)==0){
    return new BDSCons(name, r, length);
  } else {
    return new BDSTorus(name, r, angle, length);
  }
}

