#ifndef __BDSPIPESECTIONFACTORY_
#define __BDSPIPESECTIONFACTORY_

#include "BDSPipeSection.hh"

class BDSPipesectionFactory{
public:
  BDSPipesectionFactory();
  ~BDSPipesectionFactory();

  BDSPipeSection* create(G4String name, G4double r, G4double angle, G4double length);
};
#endif
