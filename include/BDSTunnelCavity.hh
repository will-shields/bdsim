#ifndef __BDSTUNNELCAVITY_H
#define __BDSTUNNELCAVITY_H

#include "BDSTunnelCavitySectionGenerator.hh"
#include "BDSTunnelCavitySectionBuilder.hh"
#include "BDSStraightTunnelCavitySectionBuilder.hh"
#include "BDSAngleTunnelCavitySectionBuilder.hh"
#include "BDSTunnelCavitySection.hh"
#include "BDSAcceleratorComponent.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"

class BDSTunnelCavity{
public:
  static BDSTunnelCavity* Instance();
  ~BDSTunnelCavity();

  void build();

  G4ThreeVector* widthMax();
    
    
private:
  BDSTunnelCavity();
  static BDSTunnelCavity* _instance;

  
  G4VSolid* _solidVolume;
  G4LogicalVolume* _logicalVolume;
  G4VPhysicalVolume* _physicalVolume;

  BDSTunnelCavitySectionGenerator _sectionGenerator;
  BDSTunnelCavitySectionBuilder* _straightSectionBuilder;
  BDSTunnelCavitySectionBuilder* _angleSectionBuilder;
  
  BDSAcceleratorComponent* _acceleratorComponent;

  void inspectBeamline();
  void constructSection();
  void calcWidthMax();
  void appendSection();
  void constructLogical();

  G4ThreeVector* _widthMax;

  G4bool _first;

};

#endif
