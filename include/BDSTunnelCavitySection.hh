#ifndef __BDSTUNNELCAVITYSECTION_H
#define __BDSTUNNELCAVITYSECTION_H
#include "BDSAcceleratorComponent.hh"
#include "G4ThreeVector.hh"

class BDSTunnelCavitySection
{
public:
  BDSTunnelCavitySection(); 
  ~BDSTunnelCavitySection();
  BDSTunnelCavitySection(BDSAcceleratorComponent* var);//A tunnel cavity section is associated with an accelerator component
  void visAttributes()const;
  //Setting functions
  void rotation(G4RotationMatrix* var);
  void logicalVolume(G4LogicalVolume* var);
  void solid(G4VSolid* var);  
  void physi(G4VPhysicalVolume* var);
  //Getting functions
  G4ThreeVector* positionOffset();
  G4RotationMatrix* rotation();
  G4double length();
  G4double angle();
  G4double radius();
  G4String name();
  G4Material* material();
  G4LogicalVolume* logicalVolume();
  G4VSolid* solid();
  BDSAcceleratorComponent* acceleratorComponent();
  
private: 
  G4String _name;
  G4double _radius;
  G4double _length;
  G4double _angle;
  G4ThreeVector* _positionOffset;
  G4RotationMatrix* _rotation;
  G4Material* _material;
  G4LogicalVolume* _logicalVolume;
  G4VSolid* _solid;  
  G4VPhysicalVolume* _physi;
  //Just a null rotation matrix...
  G4RotationMatrix* _nullRotationMatrix;
  //The accelerator component associated with the section of tunnel cavity...
  BDSAcceleratorComponent* _acceleratorComponent;
};

#endif


