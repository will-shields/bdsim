//  
//   BDSIM, (C) 2001-2006 
//   
//   version 0.3
//  
//
//
//
//
//   Generic accelerator component class
//
//
//   History
//
//     24 Nov 2006 by Agapov,  v.0.3
//     x  x   2002 by Blair
//
//




#ifndef __BDSACCELERATORCOMPONENT_H
#define __BDSACCELERATORCOMPONENT_H

#include <cstring>
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "globals.hh"
#include "BDSBeamPipe.hh"
#include "BDSEnergyCounterSD.hh"

#include "G4MagneticField.hh"
#include "G4Mag_EqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4FieldManager.hh"
#include "G4UserLimits.hh"

#include <vector> 

//using namespace std;

class BDSAcceleratorComponent 
{
public:
  //destructor
  virtual ~BDSAcceleratorComponent ();

  //name
  const G4String GetName () const;
  void SetName(G4String aName);

  //type 
  const G4String GetType () const;
  void SetType(G4String aType);
  

  //
  //    Geometry features    
  //

  // angle - for bends etc.
  const G4double GetAngle () const;

   //	geometry length of the component.
  const G4double GetLength () const;
  void SetLength(G4double aLength);

  const G4double GetPhi () const; //polar angle with respect to original frame
  void SetPhi (G4double val);

  const G4double GetTheta () const; //azimuthal angle with respect to original frame
  void SetTheta(G4double val);

  const G4double GetPsi () const; //azimuthal angle with respect to original frame
  void SetPsi(G4double val);

  const G4double GetXOffset() const;  // frame offset 
  const G4double GetYOffset() const;
  const G4double GetZOffset() const;
  
  G4double GetAperX();
  G4double GetAperY();

  G4double GetK1();
  G4double GetK2();
  G4double GetK3();

  //Set is only for Outline readout purposes - doesn't change magnet strengths
  void SetK1(G4double K1);
  void SetK2(G4double K2);
  void SetK3(G4double K3);

  G4RotationMatrix* GetRotation();
  G4ThreeVector GetPosition();
  void SetPosition(G4ThreeVector);
  
  const G4double GetTilt() const;  // component tilt 
  
  
  



  G4LogicalVolume* GetMarkerLogicalVolume() const;
  
  BDSEnergyCounterSD* GetBDSEnergyCounter() const;
  
  void SetBDSEnergyCounter( BDSEnergyCounterSD* anBDSEnergyCounter);
  
  G4int GetCopyNumber() const;
  
  G4double GetSPos() const;
  
  void SetSPos(G4double spos);
  
  void SetCopyNumber(G4int nCopy);
  
  void SetBDSEnergyCounterID(G4int anID);
  
  G4int GetBDSEnergyCounterID();
  
  void SetSensitiveVolume(G4LogicalVolume* aLogVol);
  
  G4LogicalVolume* GetSensitiveVolume();

  void SetMultipleSensitiveVolumes(G4LogicalVolume* aLogVol);

  std::vector<G4LogicalVolume*> GetMultipleSensitiveVolumes();

  void SetInnerMostLogicalVolume(G4LogicalVolume* aLogVol);
  
  G4LogicalVolume* GetInnerMostLogicalVolume() const;
  
  G4UserLimits* GetInnerBPUserLimits();

  G4double GetZLower();
  G4double GetZUpper();
  void SetZLower(G4double aZLower);
  void SetZUpper(G4double aZUpper);
  void AddSynchEnergyLoss(G4double SynchEnergyLoss);
  G4double GetSynchEnergyLoss();
  
  BDSAcceleratorComponent();
  void BuildOuterFieldManager();

  // in case a mapped field is provided creates a field mesh in global coordinates
  virtual void PrepareField(G4VPhysicalVolume *referenceVolume); 
  virtual void SynchRescale(G4double factor); 

  // in case a component requires specific alignment (e.g. SQL/BDSElement)
  virtual void AlignComponent(G4ThreeVector& TargetPos, 
			      G4RotationMatrix *TargetRot,
			      G4RotationMatrix& globalRotation,
			      G4ThreeVector& rtot,
			      G4ThreeVector& rlast,
			      G4ThreeVector& localX,
			      G4ThreeVector& localY,
			      G4ThreeVector& localZ); 

  
  // get parameter value from the specification string

  G4double getParameterValue(G4String spec, G4String name) const;
  G4String getParameterValueString(G4String spec, G4String name) const;

  // constructor
  BDSAcceleratorComponent (
			  G4String& aName, 
			  G4double aLength,
			  G4double aBpRadius,
			  G4double aXAper,
			  G4double aYAper,
			  G4VisAttributes* aVisAtt,
			  G4double phi=0.,  // polar angle (used in hor. bends)
			  //G4double theta=0.,
			  G4double XOffset=0.,
			  G4double YOffset=0.,
			  G4double ZOffset=0.);


  G4VisAttributes* GetVisAttributes()const;
  G4LogicalVolume* itsOuterLogicalVolume;
  G4LogicalVolume* itsMarkerLogicalVolume;

protected:
  G4String itsName;
  G4String itsType;
  G4double itsLength;
  G4double itsBpRadius;
  G4double itsXAper;
  G4double itsYAper;
  G4double itsAngle;
  G4double itsTilt;
  
  G4double itsMagScaleFactor;
  G4double itsPhi;
  G4double itsTheta;
  G4double itsPsi;
  G4double itsK1, itsK2, itsK3;
  G4RotationMatrix* itsRotation;
  G4ThreeVector itsPosition;
  BDSBeamPipe* itsBeamPipe;
  G4MagneticField* itsOuterMagField;
  G4Mag_EqRhs* itsOuterEqRhs;
  G4MagIntegratorStepper*  itsOuterStepper;
  G4ChordFinder* itsOuterChordFinder;
  G4FieldManager*  itsOuterFieldMgr;
  G4VisAttributes* itsVisAttributes;
  G4UserLimits* itsOuterUserLimits;
  G4UserLimits* itsMarkerUserLimits;
  G4UserLimits* itsInnerBeampipeUserLimits;
  G4LogicalVolume* itsInnerMostLogicalVolume;

  // JCC Mar05 >>
  G4double itsXOffset;
  G4double itsYOffset;
  // << JCC Mar05
  G4double itsZOffset;

private:
  G4double itsSPos;
  G4int itsCopyNumber;
  BDSEnergyCounterSD* itsBDSEnergyCounter;
  G4int itsCollectionID;
  G4LogicalVolume* itsSensitiveVolume;
  std::vector<G4LogicalVolume*> itsMultipleSensitiveVolumes;
  G4double itsZLower;
  G4double itsZUpper;
  G4double itsSynchEnergyLoss;

};

// Class BDSAcceleratorComponent 

inline BDSAcceleratorComponent::
BDSAcceleratorComponent (
			G4String& aName,G4double aLength, 
			G4double aBpRadius,G4double aXAper,G4double aYAper, 
			G4VisAttributes* aVisAtt,G4double angle,
			G4double XOffset, G4double YOffset,G4double ZOffset):
  itsName(aName),itsLength(aLength),itsBpRadius(aBpRadius),
  itsXAper(aXAper),itsYAper(aYAper),itsAngle(angle),
  itsVisAttributes(aVisAtt),
  itsXOffset(XOffset),itsYOffset(YOffset), itsZOffset(ZOffset)
{
  itsSensitiveVolume=NULL;
  itsInnerBeampipeUserLimits =new G4UserLimits();
  itsPhi = 0;
  itsTheta = 0;
  itsPsi = 0;
  itsTilt = 0;
  itsMagScaleFactor = 1;
}


inline const G4double BDSAcceleratorComponent::GetLength () const
{return itsLength;}

inline const G4double BDSAcceleratorComponent::GetAngle () const
{return itsAngle;}

inline const G4double BDSAcceleratorComponent::GetPhi () const
{return itsPhi;}

inline void BDSAcceleratorComponent::SetPhi (G4double val)
{itsPhi = val;}

inline const G4double BDSAcceleratorComponent::GetTheta () const
{return itsTheta;}

inline void BDSAcceleratorComponent::SetTheta (G4double val)
{itsTheta = val;}

inline const G4double BDSAcceleratorComponent::GetPsi () const
{return itsPsi;}

inline void BDSAcceleratorComponent::SetPsi (G4double val)
{itsPsi = val;}

inline G4double BDSAcceleratorComponent::GetAperX()
{
  if(itsXAper==0) // i.e. it has not been set
    return itsBpRadius;
  else return itsXAper;
}

inline G4double BDSAcceleratorComponent::GetAperY()
{
  if(itsYAper==0) // i.e. it has not been set
    return itsBpRadius;
  else return itsYAper;
}

inline G4double BDSAcceleratorComponent::GetK1()
{ return itsK1; }

inline G4double BDSAcceleratorComponent::GetK2()
{ return itsK2; }

inline G4double BDSAcceleratorComponent::GetK3()
{ return itsK3; }

inline void BDSAcceleratorComponent::SetK1(G4double K1)
{ itsK1 = K1; }

inline void BDSAcceleratorComponent::SetK2(G4double K2)
{ itsK2 = K2; }

inline void BDSAcceleratorComponent::SetK3(G4double K3)
{ itsK3 = K3; }

inline G4RotationMatrix* BDSAcceleratorComponent::GetRotation()
{ return itsRotation;}

inline G4ThreeVector BDSAcceleratorComponent::GetPosition()
{ return itsPosition;}

inline void BDSAcceleratorComponent::SetPosition(G4ThreeVector pos)
{ itsPosition = pos;}

inline const G4String BDSAcceleratorComponent::GetName () const
{return itsName;}

inline void BDSAcceleratorComponent::SetName (G4String aName)
{itsName=aName;}

inline const G4String BDSAcceleratorComponent::GetType () const
{return itsType;}

inline void BDSAcceleratorComponent::SetType (G4String aType)
{itsType=aType;}

inline G4LogicalVolume* BDSAcceleratorComponent::GetMarkerLogicalVolume() const
{return itsMarkerLogicalVolume;}

inline G4LogicalVolume* BDSAcceleratorComponent::GetInnerMostLogicalVolume() const
{return itsInnerMostLogicalVolume;}

inline void BDSAcceleratorComponent::
SetInnerMostLogicalVolume(G4LogicalVolume* aLogVol)
{itsInnerMostLogicalVolume = aLogVol;}

inline G4VisAttributes* BDSAcceleratorComponent::GetVisAttributes() const
{return itsVisAttributes;}

inline BDSEnergyCounterSD* BDSAcceleratorComponent::GetBDSEnergyCounter() const
{return itsBDSEnergyCounter;}

inline G4int BDSAcceleratorComponent::GetCopyNumber() const
{return itsCopyNumber;}

inline G4double BDSAcceleratorComponent::GetSPos() const
{return itsSPos;}

inline void BDSAcceleratorComponent::SetCopyNumber(G4int nCopy)
{itsCopyNumber=nCopy;}

inline void BDSAcceleratorComponent::SetSPos(G4double spos)
{itsSPos=spos;}

inline void 
BDSAcceleratorComponent::SetBDSEnergyCounter(BDSEnergyCounterSD* anBDSEnergyCounter)
{itsBDSEnergyCounter=anBDSEnergyCounter;}

inline void BDSAcceleratorComponent::SetBDSEnergyCounterID(G4int anID)
{itsCollectionID=anID;}

inline G4int BDSAcceleratorComponent::GetBDSEnergyCounterID()
{return itsCollectionID;}

inline  
void BDSAcceleratorComponent::SetSensitiveVolume(G4LogicalVolume* aLogVol)
{itsSensitiveVolume=aLogVol;}

inline  G4LogicalVolume* BDSAcceleratorComponent::GetSensitiveVolume()
{return itsSensitiveVolume;}

inline void BDSAcceleratorComponent::SetMultipleSensitiveVolumes(G4LogicalVolume* aLogVol)
{ itsMultipleSensitiveVolumes.push_back(aLogVol);}

inline  std::vector<G4LogicalVolume*> BDSAcceleratorComponent::GetMultipleSensitiveVolumes()
{return itsMultipleSensitiveVolumes;}

inline  G4double BDSAcceleratorComponent::GetZLower()
{return itsZLower;}

inline  G4double BDSAcceleratorComponent::GetZUpper()
{return itsZUpper;}

inline  void BDSAcceleratorComponent::SetZLower(G4double aZLower)
{itsZLower=aZLower;}

inline  void BDSAcceleratorComponent::SetZUpper(G4double aZUpper)
{itsZUpper=aZUpper;}

inline void 
BDSAcceleratorComponent::AddSynchEnergyLoss(G4double SynchEnergyLoss)
{itsSynchEnergyLoss+=SynchEnergyLoss;}

inline  G4double BDSAcceleratorComponent::GetSynchEnergyLoss()
{return itsSynchEnergyLoss;}

inline  G4UserLimits* BDSAcceleratorComponent::GetInnerBPUserLimits()
  {return itsInnerBeampipeUserLimits;}

inline  const G4double BDSAcceleratorComponent::GetXOffset() const
{return itsXOffset;}

inline  const G4double BDSAcceleratorComponent::GetYOffset() const
{return itsYOffset;}

inline  const G4double BDSAcceleratorComponent::GetZOffset() const
{return itsZOffset;}

inline  const G4double BDSAcceleratorComponent::GetTilt() const
{return itsTilt;}


inline  G4double BDSAcceleratorComponent::getParameterValue(G4String spec, G4String name) const
{
  G4double value = 0;

  std::string delimiters = "&";
  std::string param = name + "=";

  int pos = spec.find(param);
  if( pos >= 0 )
    {
      
      int pos2 = spec.find("&",pos);
      int pos3 = spec.length();
      int tend = pos2 < 0 ? pos3 : pos2; 
      int llen = tend - pos - param.length();
      
      std::string val = spec.substr(pos + param.length(), llen);
      
      value = atof(val.c_str());

  }

  return value;

}

inline  G4String BDSAcceleratorComponent::getParameterValueString(G4String spec, G4String name) const
{
  G4String value = "";

  std::string delimiters = "&";
  std::string param = name + "=";

  int pos = spec.find(param);
  if( pos >= 0 )
    {
      
      int pos2 = spec.find("&",pos);
      int pos3 = spec.length();
      int tend = pos2 < 0 ? pos3 : pos2; 
      int llen = tend - pos - param.length();
      
      value = spec.substr(pos + param.length(), llen);
  }

  return value;

}

#endif
