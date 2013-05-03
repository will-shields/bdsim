#ifndef __BDSTUNNELCAVITYSECTIONGENERATOR_H_
#define __BDSTUNNELCAVITYSECTIONGENERATOR_H_

#include "BDSTunnelCavitySection.hh"
#include "BDSTunnelCavitySectionBuilder.hh"
#include "BDSAcceleratorComponent.hh"

class BDSTunnelCavitySectionGenerator{
public:
  BDSTunnelCavitySectionGenerator();
  ~BDSTunnelCavitySectionGenerator();

  //Setting function
  void tunnelCavitySectionBuilder(BDSTunnelCavitySectionBuilder* var);
  BDSTunnelCavitySection* tunnelCavitySection();
  void constructTunnelCavitySection(BDSAcceleratorComponent* var);

private:
  BDSTunnelCavitySectionBuilder* _tunnelCavitySectionBuilder;
};

#endif
