#ifndef BDSShowerModel_h
#define BDSShowerModel_h 1

#include "GFlashShowerModel.hh"
#include "G4FastTrack.hh"

#include  <vector>

class BDSShowerModel:public GFlashShowerModel
{
public:
  BDSShowerModel(G4String, G4Region*);
  BDSShowerModel(G4String);
  ~BDSShowerModel();
private: 
  G4bool CheckContainment(const G4FastTrack& fastTrack);
};
#endif

