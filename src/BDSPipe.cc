#include "BDSPipe.hh"

BDSPipe::BDSPipe(list<G4String> name, list<G4double> r, list<G4double> dPhi, list<G4double> l){
  _name=name;
  _r=r;
  _dPhi=dPhi;
  _l=l;
  
  BDSTunnelSegmentSolidFactory fact;

  for(first();isNotDone();next()){
    _sections.push_back(fact.create(*_nameIter,*_rIter,*_dPhiIter,*_lIter);
  }
}

void BDSPipe::first(){
  _nameIter=_name.begin();
  _rIter=_r.begin();
  _dPhiIter=_dPhi.begin();
  _lIter=_l.begin();
}

void BDSPipe::next(){
  _nameIter++;
  _rIter++;
  _dPhiIter++;
  _lIter++;
}


G4bool BDSPipe::isNotDone(){
  return (*_nameIter)!=_name.end();
}
