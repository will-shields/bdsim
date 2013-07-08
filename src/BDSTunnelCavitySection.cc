#include "BDSTunnelCavitySection.hh"
#include "BDSMaterials.hh"

BDSTunnelCavitySection::BDSTunnelCavitySection(){
  G4Exception("Error - BDSTunnelCavitySection must be initialized with an accelerator component as argument!", "-1", FatalErrorInArgument, "");
}

//Copy constructor
BDSTunnelCavitySection::BDSTunnelCavitySection(const BDSTunnelCavitySection& source){
  copy(source);
}

//Assignment operator overload
//bool BDSTunnelCavitySection::operator==(const BDSTunnelCavitySection& source){
//}

BDSTunnelCavitySection& BDSTunnelCavitySection::operator=(const BDSTunnelCavitySection& source){
  if(this == &source){
    return *this;
  }
  clear();
  copy(source);
  return *this;
}

void BDSTunnelCavitySection::copy(const BDSTunnelCavitySection& source){
  _name = source._name;
  _radius = source._radius;
  _length = source._length;
  _angle = source._angle;
  _materialName = source._materialName;
  _positionOffset = new G4ThreeVector((const CLHEP::Hep3Vector&)source._positionOffset);
  _nullRotationMatrix = new G4RotationMatrix((const CLHEP::HepRotation&)source._nullRotationMatrix);
  _rotation = new G4RotationMatrix((const CLHEP::HepRotation&)source._rotation);
  _acceleratorComponent = source._acceleratorComponent;
  _solid = source._solid->Clone();
}

void BDSTunnelCavitySection::clear(){
  delete _positionOffset;
  delete _rotation;
  delete _solid;
  delete _nullRotationMatrix;
  delete _acceleratorComponent;
}
BDSTunnelCavitySection::BDSTunnelCavitySection(BDSAcceleratorComponent* var){
  _acceleratorComponent=var;
  _name=_acceleratorComponent->GetName()+"_tunnel_cavity";
  _radius=_acceleratorComponent->GetTunnelRadius();
  _length=_acceleratorComponent->GetLength();
  _angle=_acceleratorComponent->GetAngle();
  G4double posX(_acceleratorComponent->GetTunnelOffsetX());
  G4double posY(BDSGlobalConstants::Instance()->GetTunnelOffsetY());
  _positionOffset=new G4ThreeVector(posX, posY, 0);
  G4cout << "BDSTunnelCavitySection::BDSTunnelCavitySection() - setting material to " << _acceleratorComponent->GetTunnelCavityMaterial() << G4endl;
  //  _material=BDSMaterials::Instance()->GetMaterial(_acceleratorComponent->GetTunnelCavityMaterial());
  _materialName="air";
  _nullRotationMatrix=new G4RotationMatrix(0,0,0);
}

BDSTunnelCavitySection::~BDSTunnelCavitySection(){
  clear();
}

//Setting functions
void BDSTunnelCavitySection::rotation(G4RotationMatrix* var){ _rotation = var; }
void BDSTunnelCavitySection::solid(G4VSolid* var){ _solid = var; }  

//Getting functions
G4ThreeVector* BDSTunnelCavitySection::positionOffset(){return _positionOffset;}
G4RotationMatrix* BDSTunnelCavitySection::rotation(){return _rotation;}
G4double BDSTunnelCavitySection::length(){return _length;}
G4double BDSTunnelCavitySection::angle(){return _angle;}
G4double BDSTunnelCavitySection::radius(){return _radius;}
G4String BDSTunnelCavitySection::name(){return _name;}
G4String BDSTunnelCavitySection::materialName(){return _materialName;}
G4VSolid* BDSTunnelCavitySection::solid() {return _solid;}
BDSAcceleratorComponent* BDSTunnelCavitySection::acceleratorComponent(){ return _acceleratorComponent; }

//Copy constructor

