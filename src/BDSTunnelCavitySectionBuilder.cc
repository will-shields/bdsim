#include "BDSTunnelCavitySectionBuilder.hh"

BDSTunnelCavitySectionBuilder::BDSTunnelCavitySectionBuilder(){
}

BDSTunnelCavitySectionBuilder::~BDSTunnelCavitySectionBuilder(){
}

void BDSTunnelCavitySectionBuilder::createNewTunnelCavitySection(BDSAcceleratorComponent* var){ 
  _tunnelCavitySection = new BDSTunnelCavitySection(var); 
}

BDSTunnelCavitySection* BDSTunnelCavitySectionBuilder::tunnelCavitySection(){ 
  return _tunnelCavitySection; 
}



