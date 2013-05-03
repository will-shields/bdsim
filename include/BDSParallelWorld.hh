#ifndef BDSParallelWorld_h
#define BDSParallelWorld_h 1

#include "globals.hh"
#include "G4VUserParallelWorld.hh"
#include "G4ScoringBox.hh"
#include "G4PSCellFlux.hh"

class BDSParallelWorld : public G4VUserParallelWorld
{
public:
  BDSParallelWorld(G4String worldName);
  virtual ~BDSParallelWorld();

public:
  virtual void Construct();
  
  G4ScoringBox* GetScoringMesh();
private:
  G4ScoringBox* _scoringMesh;
  G4PSCellFlux* _primitiveScorer;
};

#endif
