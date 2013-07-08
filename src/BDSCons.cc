#include "BDSCons.hh"
#include "BDSGlobalConstants.hh"

BDSCons::~BDSCons(){
}

BDSCons::BDSCons(const G4String& name,
		 G4double r,
		 G4double length):G4Cons(name,0.0,r,0.0,r,length/2.0,0,twopi*radian){
  _name=name;
  _r=r;
  _length=length;
}
