#include "BDSFieldMag.hh"
#include "BDSFieldMagGlobal.hh"

#include "globals.hh" // geant4 types / globals
#include "G4ThreeVector.hh"

BDSFieldMagGlobal::BDSFieldMagGlobal(BDSFieldMag* fieldIn):
  field(fieldIn)
{;}

BDSFieldMagGlobal::~BDSFieldMagGlobal()
{
  delete field;
}

G4ThreeVector BDSFieldMagGlobal::GetFieldValue(const G4ThreeVector& position) const
{
  return field->GetFieldValue(position);
}

G4ThreeVector BDSFieldMagGlobal::GetFieldValueTransformed(const G4ThreeVector& position) const
{
  G4ThreeVector localPosition  = ConvertToLocal(position);
  G4ThreeVector localField     = field->GetFieldValueTransformed(localPosition);
  G4ThreeVector globalField    = ConvertAxisToGlobal(localField);
  return globalField;
}
