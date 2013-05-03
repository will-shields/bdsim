#ifndef __BDSBEAMLINE_H
#define __BDSBEAMLINE_H

#include "parser/gmad.h"
#include "BDSAcceleratorComponent.hh"

class BDSBeamline{
public:

  static BDSBeamline* Instance();
  ~BDSBeamline();
  
  void addComponent(BDSAcceleratorComponent* var);
  void print();
  void printNavigation();
  BDSAcceleratorComponent* currentItem();
  BDSAcceleratorComponent* lastItem();
  void first();
  bool isDone();
  void next();

  G4double s_total();

  G4RotationMatrix* rotation();
  G4RotationMatrix* rotationGlobal();
  G4ThreeVector* position();
  G4double       positionS();
  G4ThreeVector* positionStart();
  G4ThreeVector* positionEnd();
  G4ThreeVector* positionFromCurrentCenter();

  G4int size();

private: 
  BDSBeamline();
  static BDSBeamline* _instance;
  list <BDSAcceleratorComponent*> _componentList;

  list<BDSAcceleratorComponent*>::const_iterator _iterComponent;
  list<BDSAcceleratorComponent*>::const_iterator _iterLastComponent;
  void setRefTransform(BDSAcceleratorComponent* var);



  //Navigation 
  void doNavigation();
  G4ThreeVector* _localX; 
  G4ThreeVector* _localY; 
  G4ThreeVector* _localZ; 
  G4ThreeVector* _position;
  G4ThreeVector* _positionStart;
  G4ThreeVector* _positionEnd;
  G4ThreeVector* _positionFromCurrentCenter;
  G4double _positionS;//Position along the curvilinear coordiante "s"
  G4ThreeVector* _zHalfAngle;
  G4RotationMatrix* _rotationLocal;
  G4RotationMatrix* _rotationGlobal;
  G4RotationMatrix* _rotation;

  list <G4RotationMatrix*> _rotationList;
  list <G4RotationMatrix*> _rotationGlobalList;
  list <G4ThreeVector*> _positionList;
  list <G4ThreeVector*> _positionStartList;
  list <G4ThreeVector*> _positionEndList;
  list <G4ThreeVector*> _positionFromCurrentCenterList;
  list <G4double> _positionSList; //Position along the curvilinear coordiante "s"

  list<G4RotationMatrix*>::const_iterator  _iterRotation;
  list<G4RotationMatrix*>::const_iterator  _iterRotationGlobal;
  list<G4ThreeVector*>::const_iterator  _iterPosition;
  list<G4ThreeVector*>::const_iterator  _iterPositionStart;
  list<G4ThreeVector*>::const_iterator  _iterPositionEnd;
  list<G4ThreeVector*>::const_iterator  _iterPositionFromCurrentCenter;
  list<G4double>::const_iterator _iterPositionS; //Position along the curvilinear coordinate "s"
  G4double _s_local, _s_total;
};

#endif
