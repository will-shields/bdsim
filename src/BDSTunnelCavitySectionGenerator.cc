#include "BDSTunnelCavitySectionGenerator.hh"

BDSTunnelCavitySectionGenerator::BDSTunnelCavitySectionGenerator(){
}
BDSTunnelCavitySectionGenerator::~BDSTunnelCavitySectionGenerator(){
}

void BDSTunnelCavitySectionGenerator::tunnelCavitySectionBuilder(BDSTunnelCavitySectionBuilder* var){
  _tunnelCavitySectionBuilder = var;
}

void BDSTunnelCavitySectionGenerator::constructTunnelCavitySection(BDSAcceleratorComponent* var){
  _tunnelCavitySectionBuilder->createNewTunnelCavitySection(var);
  _tunnelCavitySectionBuilder->build();
}

BDSTunnelCavitySection* BDSTunnelCavitySectionGenerator::tunnelCavitySection(){
  return _tunnelCavitySectionBuilder->tunnelCavitySection();
}
