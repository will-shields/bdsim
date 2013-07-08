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
  BDSTunnelCavitySection(const BDSTunnelCavitySection& source);
  BDSTunnelCavitySection& operator=(const BDSTunnelCavitySection& source);

  void visAttributes()const;
  //Setting functions
  void rotation(G4RotationMatrix* var);
  void solid(G4VSolid* var);  
  //Getting functions
  G4ThreeVector* positionOffset();
  G4RotationMatrix* rotation();
  G4double length();
  G4double angle();
  G4double radius();
  G4String name();
  G4String materialName();
  G4VSolid* solid();
  BDSAcceleratorComponent* acceleratorComponent();

private: 
  void copy(const BDSTunnelCavitySection& source);
  void clear();
  G4String _name;
  G4double _radius;
  G4double _length;
  G4double _angle;
  G4ThreeVector* _positionOffset;
  G4RotationMatrix* _rotation;
  G4String _materialName;
  G4VSolid* _solid;  
  //Just a null rotation matrix...
  G4RotationMatrix* _nullRotationMatrix;
  //The accelerator component associated with the section of tunnel cavity...
  BDSAcceleratorComponent* _acceleratorComponent;
};

#endif


