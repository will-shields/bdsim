#include "BDSCons.hh"
#include "BDSTorus.hh"
#include "BDSGlobalConstants.hh"
#include "G4PhysicalConstants.hh"

BDSPipeSection::~BDSPipeSection(){
  delete _solid;
}

BDSPipeSection::BDSPipeSection(const G4String& name,
			       G4double r,
			       G4double length,
			       G4double angle){
  _name=name;
  _r=r;
  _angle=angle;
  _length=length;

  _transform3D.setIdentity(); //Initialization
  
  if(_angle==0){
    _solid=new BDSCons(_name,_r,_length);
  } else {
    _solid=new BDSTorus(_name,_r,_length,_angle);
    //Transform required to move torus to correct location and orientation for beam pipe or tunnel
    _transform3D.RotateX3D(pi/2.0);
    _transform3D.ReflectY3D();
    _transform3D.ReflectX3D();
    _transform3D.TranslateX3D(_length/_angle);    
  }
}
