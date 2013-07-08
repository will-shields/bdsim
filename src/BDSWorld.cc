#include "BDSWorld.hh"
#include "BDSBeamlineNavigator.hh"
#include "BDSMaterials.hh"
#include "BDSTunnelCavity.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"

/*
BDSWorld* BDSWorld::_instance = 0;

BDSWorld* BDSWorld::Instance(){
  if(_instance==0) {
    _instance = new BDSWorld();
  }
  return _instance;
}
*/

BDSWorld::BDSWorld(){
  _material = BDSMaterials::Instance()->GetMaterial(BDSGlobalConstants::Instance()->GetSoilMaterialName()),
  build();
  G4cout << "BDSWorld - placing" << G4endl;
  place();
}

BDSWorld::~BDSWorld(){
  /*
  delete _solidVolume;
		  //		    gFlashRegion.back()->SetUserLimits(new G4UserLimits(var->GetLength()/10.0));
		  //		    MultipleSensVols[i]->SetUserLimits(new G4UserLimits(var->GetLength()/10.0));
b
  delete _logicalVolume;
  delete _physicalVolume;
  delete _instance;
  */
}

void BDSWorld::build(){
  G4cout << "BDSWorld::build() - constructSolid()" << G4endl;
  constructSolid();
  G4cout << "BDSWorld::build() - constructLogical()" << G4endl;
  constructLogical();
  G4cout << "BDSWorld::build() - setVisAttributes()" << G4endl;
  setVisAttributes();
  G4cout << "BDSWorld::build() - setUseLimits()" << G4endl;
  setUserLimits();
  G4cout << "BDSWorld::build() - finished()" << G4endl;
}


void BDSWorld::constructSolid(){
  calcSize();
  G4cout << "BDSWorld::constructSolid - making solid volume..." << G4endl;
  G4cout << "BDSWorld::constructSolid - x size =  " << _size->x() << G4endl;
  G4cout << "BDSWorld::constructSolid - y size =  " << _size->y() << G4endl;
  G4cout << "BDSWorld::constructSolid - z size =  " << _size->z() << G4endl;
  _solidVolume = new G4Box("World", _size->x(), _size->y(), _size->z());
  _solidVolume->DumpInfo();
  G4cout << "BDSWorld::constructSolid - finished." << G4endl;
}

void BDSWorld::calcSize(){
  G4cout << "BDSWorld::calcSize() - " << G4endl;
  G4cout << "BDSWorld::calcSize() - setting x width" << G4endl;
  _size->setX((BDSTunnelCavity::Instance()->widthMax().x()+BDSGlobalConstants::Instance()->GetTunnelSoilThickness())/2.0);
  G4cout << "BDSWorld::calcSize() - setting y width" << G4endl;
  _size->setY((BDSTunnelCavity::Instance()->widthMax().y()+BDSGlobalConstants::Instance()->GetTunnelSoilThickness())/2.0);
  G4cout << "BDSWorld::calcSize() - setting z width" << G4endl;
  G4cout << "BDSWorld::calcSize() - tunnel cavity max width = " << BDSTunnelCavity::Instance()->widthMax().z()/m << G4endl;
  _size->setZ((BDSTunnelCavity::Instance()->widthMax().z())/2.0);
  G4cout << "BDSWorld::calcSize() - size x, y, z = " << _size->x()/m << " " << _size->y()/m << " " << _size->z()/m << " m" << G4endl;
}

void BDSWorld::constructLogical(){
  G4cout << "BDSWorld::constructLogical - making logical volume..." << G4endl;
  G4cout << "BDSWorld::constructLogical - solid volume =  " << _solidVolume->GetName() << G4endl;
  G4cout << "BDSWorld::constructLogical - material =  " << _material->GetName() << G4endl;

  _logicalVolume = new G4LogicalVolume(_solidVolume,
				       _material,
				       "World");
  G4cout << "BDSWorld::constructLogical - finished." << G4endl;
}

void BDSWorld::setVisAttributes(){
  _visAttributes = new G4VisAttributes(true);
  _logicalVolume->SetVisAttributes(_visAttributes);
}

void BDSWorld::setUserLimits(){
#ifndef NOUSERLIMITS
  _userLimits =new G4UserLimits();
  _userLimits->SetMaxAllowedStep(10*m);
  _userLimits->SetUserMinEkine(BDSGlobalConstants::Instance()->GetThresholdCutCharged());
  _userLimits->SetUserMaxTime(BDSGlobalConstants::Instance()->GetMaxTime());
  _logicalVolume->SetUserLimits(_userLimits);
#endif
}

void BDSWorld::place(){
  G4cout << "BDSWorld::place - " << G4endl;
  G4ThreeVector zeroThreeVector = G4ThreeVector(0,0,0);
  _physicalVolume = new G4PVPlacement(0, // no rotation
				      (G4ThreeVector)0,     // at (0,0,0)
				      _logicalVolume,      
				      "world_physical",
				      NULL,                // its mother  volume
				      false,               // no boolean operation
				      0,                   // copy number
				      true
				      );
  
}

G4VPhysicalVolume* BDSWorld::physicalVolume(){
  return _physicalVolume;
}

G4LogicalVolume* BDSWorld::logicalVolume(){
  return _logicalVolume;

}

G4VSolid* BDSWorld::solidVolume(){
  return _solidVolume;
}


G4UserLimits* BDSWorld::userLimits(){
  return _userLimits;
}
