#ifndef __BDSTORUS_H_
#define __BDSTORUS_H_

#include "G4Torus.hh"
#include "G4Transform3D.hh"

class BDSTorus : public G4Torus {
public:
  ~BDSTorus();
  BDSTorus(const G4String& name,
	   G4double r,
	   G4double length,
	   G4double angle);
  
  inline G4Transform3D transform3D(){
    return _transform3D;
  }
  
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
    return _angle;
  }

private:
  G4Transform3D _transform3D;
  G4String _name;
  G4double _r=r;
  G4double _length;
  G4double _angle;
};
#endif
