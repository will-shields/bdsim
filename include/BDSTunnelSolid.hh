#ifndef __BDSTUNNELSOLID_H_
#define __BDSTUNNELSOLID_H_

#include "G4VSolid.hh"
#include "G4UnionSolid.hh"
#include "G4Transform3D.hh"
#include "BDSPipeSection.hh"
#include <vector>

using namespace std;

class BDSTunnelSolid {
public:
  ~BDSTunnelSolid();
  BDSTunnelSolid();
  list<G4VSolid*> create();
  G4ThreeVector widthMax();

  
private:
  void clear();
  void fill();
  void  process();
  void  removeZeroLengthSegs();
  void  consolidate();
  list<G4VSolid*> buildSolidList();
  list<G4VSolid*> _solidList;
  G4int _numSegments;
  G4double _r;
  vector<G4double> _vecLengthSegment;
  vector<G4double> _vecDPhiSegment;
  vector<G4double> _vecR;
  vector<G4double> _vecOffsetX;
  vector<G4double> _vecOffsetY;
  G4double maxAbs(vector<G4double> var);
  void calcWidthMax();
  G4ThreeVector _widthMax;
  list<BDSPipeSection*> _pipeSectionList;
};
#endif
