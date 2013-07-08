#ifndef __BDSTUNNELCAVITY_H
#define __BDSTUNNELCAVITY_H

#include "BDSTunnelCavitySectionGenerator.hh"
#include "BDSTunnelCavitySectionBuilder.hh"
#include "BDSStraightTunnelCavitySectionBuilder.hh"
#include "BDSAngleTunnelCavitySectionBuilder.hh"
#include "BDSTunnelCavitySection.hh"
#include "BDSAcceleratorComponent.hh"
#include "BDSTunnelSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"

class BDSTunnelCavity{
public:
  static BDSTunnelCavity* Instance();
  ~BDSTunnelCavity();

  void build();
  void place(G4LogicalVolume* motherVolume);
  G4ThreeVector widthMax();
    
    
private:
  BDSTunnelCavity();
  static BDSTunnelCavity* _instance;

  
  G4VSolid* _solidVolume;
  G4LogicalVolume* _logicalVolume;
  G4VPhysicalVolume* _physicalVolume;

  void inspectBeamline();
  void calcWidthMax(BDSTunnelSolid var);
  void constructLogical();

  G4ThreeVector _widthMax;
  G4VisAttributes* _visAttributes;
  void setVisAttributes();

  G4double maxAbs(vector<G4double> var);
};

#endif

