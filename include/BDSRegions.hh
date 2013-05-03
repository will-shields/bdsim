#ifndef __BDSREGIONS_H
#define __BDSREGIONS_H

#include "BDSShowerModel.hh"
#include "BDSGlobalConstants.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "GFlashHitMaker.hh"
#include "GFlashParticleBounds.hh"
#include "GFlashHomoShowerParameterisation.hh"
#include <vector>
#include "BDSAcceleratorComponent.hh"

class BDSRegions{
public:
  BDSRegions();
  ~BDSRegions();
  G4Region* precisionRegion();
  G4Region* worldRegion();

private: 
  void buildRegions();
  void buildNormalRegion();
  void buildPrecisionRegion();
  void buildGasRegion();
  void buildGFlashRegion(BDSAcceleratorComponent* var);
  void buildGFlashRegion();
  
  G4Region* _precisionRegion;
  G4Region* _gasRegion;
  G4Region* _worldRegion;
  std::vector<G4Region*> _gFlashRegion;
 
  G4ProductionCuts* _precisionProductionCuts;
  G4ProductionCuts* _gasProductionCuts;
  G4ProductionCuts* _gFlashProductionCuts;

  std::vector<GFlashHomoShowerParameterisation*> _gFlashParameterisation;
  GFlashHitMaker *_gFlashHitMaker;
  GFlashParticleBounds *_gFlashParticleBounds;
  GFlashParticleBounds *_gFlashParticleBoundsVac;
  std::vector<BDSShowerModel*> _gFlashFastShowerModel;
};

#endif
