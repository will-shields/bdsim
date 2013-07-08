#ifndef _BDS_PIPE_HH__
#define _BDS_PIPE_HH__

#include "G4VSolid.hh"
#include "G4UnionSolid.hh"
#include "G4String.hh"

class BDSPipe : public G4VSolid{
public:
  BDSPipe(list<G4String> name, list<G4double> r, list<G4double> dPhi, list<G4double> l);
  ~BDSPipe();
private:
  //Transforms to move from the centre of the solid to the ends.
  G4Transform3D _pipeStart;
  G4Transform3D _pipeEnd;
  list<G4String> _name;
  list<G4double> _r;
  list<G4double> _dPhi;
  list<G4double> _l;
  list<G4String>::const_iterator _nameIter;
  list<G4double>::const_iterator _rIter;
  list<G4double>::const_iterator _dPhiIter;
  list<G4double>::const_iterator _lIter;
  list<BDSTunnelSection> _sections;
  list<G4Transform3D> _transforms;
}

#endif
