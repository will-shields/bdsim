#include "BDSTunnelCavity.hh"
#include "BDSBeamline.hh"
#include "BDSMaterials.hh"
#include "BDSTunnelSolid.hh"
#include "G4UnionSolid.hh"



BDSTunnelCavity* BDSTunnelCavity::_instance = 0;

BDSTunnelCavity* BDSTunnelCavity::Instance(){
  if(_instance==0){
    _instance = new BDSTunnelCavity();
  }
  return _instance;
}

BDSTunnelCavity::BDSTunnelCavity(){
  G4cout << "BDSTunnelCavity::BDSTunnelCavity() ... " << G4endl;
  build();
  G4cout << "BDSTunnelCavity::BDSTunnelCavity() finished. " << G4endl;
}

BDSTunnelCavity::~BDSTunnelCavity(){
  G4cout << "BDSTunnelCavity::~BDSTunnelCavity() ... " << G4endl;
  delete _solidVolume;
  delete _logicalVolume;
  delete _physicalVolume;
  delete _instance;
}

void BDSTunnelCavity::build(){
  G4cout << "BDSTunnelCavity::build() ... " << G4endl;
  inspectBeamline();
  G4cout << "BDSTunnelCavity::build() making solid factory " << G4endl;
  BDSTunnelSolid solidFactory;
  G4cout << "BDSTunnelCavity::build() producing solid" << G4endl;
  _solidVolume=solidFactory.create();
  calcWidthMax(solidFactory);
  G4cout << "BDSTunnelCavity::build() constructing logical volume" << G4endl;
  constructLogical();
  G4cout << "BDSTunnelCavity::build() setting vis attributes" << G4endl;
  setVisAttributes();
  G4cout << "BDSTunnelCavity::build() - finished" << G4endl;
}

void BDSTunnelCavity::calcWidthMax(BDSTunnelSolid var){
  G4cout << "BDSTunnelCavity::calcWidthMax() ... " << G4endl;
  _widthMax=var.widthMax();
  /*
  _widthMax.setX(std::max(_widthMax.x(),(2*(_acceleratorComponent->GetTunnelRadius() + std::abs(_acceleratorComponent->GetTunnelOffsetX() + BDSBeamline::Instance()->positionEnd()->x())))));
  _widthMax.setY(std::max(_widthMax.y(),2*(_acceleratorComponent->GetTunnelRadius() + std::abs(BDSGlobalConstants::Instance()->GetTunnelOffsetY() + BDSBeamline::Instance()->positionEnd()->y()))));
  _widthMax.setZ(std::max(_widthMax.z(),BDSBeamline::Instance()->positionEnd()->z()));
  */
  G4cout << "BDSTunnelCavity::calcWidthMax() finished. " << G4endl;
}

G4ThreeVector BDSTunnelCavity::widthMax(){
  G4cout << "BDSTunnelCavity::widthmax() ... " << G4endl;
  G4cout << "BDSTunnelCavity::widthmax() x= " << _widthMax.x()/m << " m" << G4endl;
  G4cout << "BDSTunnelCavity::widthmax() y= " << _widthMax.y()/m << " m" << G4endl;
  G4cout << "BDSTunnelCavity::widthmax() z= " << _widthMax.z()/m << " m" << G4endl;
  return _widthMax;
}

void BDSTunnelCavity::constructLogical(){
  G4cout << "BDSTunnelCavity::constructLogical() - making logical volume" << G4endl;
  const G4String logicalVolumeName = "tunnelCavityLog";
  _logicalVolume = new G4LogicalVolume(_solidVolume,
				       //				       BDSMaterials::Instance()->GetMaterial(BDSGlobalConstants::Instance()->GetTunnelCavityMaterialName()),
				       BDSMaterials::Instance()->GetMaterial("air"),
				       logicalVolumeName);
  G4cout << "BDSTunnelCavity::constructLogical() - made logical volume" << G4endl;
}

void BDSTunnelCavity::inspectBeamline(){
  G4cout << "BDSTunnelCavity::inspectBeamline()" << G4endl;
  if (BDSBeamline::Instance()->size() == 0){
    G4Exception("BDSTunnelCavity: beam line is empty!", "-1", FatalException, "");
  }
}

void BDSTunnelCavity::place(G4LogicalVolume* motherVolume){
  G4cout << "BDSTunnelCavity::place() ..." << G4endl;
  _physicalVolume = new G4PVPlacement(0, // no rotation
				      (G4ThreeVector)0,     // at (0,0,0)
				      _logicalVolume,      
				      "tunnel_cavity_physical",
				      motherVolume,                // its mother  volume
				      false,               // no boolean operation
				      0,                   // copy number
				      true
				      );
  G4cout << "BDSTunnelCavity::place() finished." << G4endl;
}

void BDSTunnelCavity::setVisAttributes(){
  _visAttributes = new G4VisAttributes(G4Colour(0.2,0.2,0.2));;
  _visAttributes->SetVisibility(true);
  //  _visAttributes->SetForceWireframe(true);
  _visAttributes->SetForceSolid(true);
  _logicalVolume->SetVisAttributes(_visAttributes);
}
