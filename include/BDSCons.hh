#ifndef __BDSCONS_H_
#define __BDSCONS_H_

#include "G4Cons.hh"

class BDSCons : public G4Cons{
public:
  ~BDSCons();
  BDSCons(const G4String& name,G4double r,G4double length);
  inline G4String name(){
    return _name;
  }
  inline G4double r(){
    return _r;
  }
  inline G4double length(){
    return _length;
  }
  inline G4String angle(){
    return 0;
  }
  inline G4Transform3D transform3D(){
    _transform3D.setIdentity();
    return _transform3D;
  }
private:
  G4Transform3D _transform3D;
  G4String _name;
  G4double _r;
  G4double _length;
};
#endif
