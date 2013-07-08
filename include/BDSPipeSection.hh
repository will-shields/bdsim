#ifndef __BDSPIPESECTION_H_
#define __BDSPIPESECTION_H_

#include "G4VSolid.hh"
#include "G4Transform3D.hh"

class BDSPipeSection{
public:
  ~BDSPipeSection();
  BDSPipeSection(const G4String& name,
		 G4double r,
		 G4double length,
		 G4double angle=0);
  inline G4VSolid* solid() return _solid;  
  inline G4Transform3D transform3D() return _transform3D;
  inline G4String name() return _name;
  inline G4double r() return _r;
  inline G4double length() return _length;
  inline G4String angle() return _angle;
private:
  G4VSolid* _solid;
  G4Transform3D _transform3D;
  G4String _name;
  G4double _r;
  G4double _length;
  G4double _angle;
};
#endif
