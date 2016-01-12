#include "BDSMagnetStrength.hh"

#include "globals.hh" // geant4 globals / types

#include <algorithm>
#include <map>
#include <vector>

const std::vector<G4String> BDSMagnetStrength::keys = {
  "angle", "length",
  "k1", "k1s",
  "k2", "k2s",
  "k3", "k3s",
  "k4", "k4s",
  "k5", "k5s",
  "k6", "k6s",
  "k7", "k7s",
  "k8", "k8s",
  "k9", "k9s",
  "k10", "k10s",
  "k11", "k11s",
  "k12", "k12s"
};

const std::vector<G4String> BDSMagnetStrength::normalComponentKeys = {
  "k1", "k2", "k3", "k4", "k5", "k6", "k7", "k8", "k9", "k10", "k11", "k12"};

const std::vector<G4String> BDSMagnetStrength::skewComponentKeys = {
  "k1s", "k2s", "k3s", "k4s", "k5s", "k6s", "k7s", "k8s", "k9s", "k10s", "k11s", "k12s"};

const G4double BDSMagnetStrength::zero     = 0.0;
G4double       BDSMagnetStrength::variable = 0.0;

BDSMagnetStrength::BDSMagnetStrength(std::map<G4String, G4double> strengths)
{
  for (auto keyValue : strengths)
    {
      if (ValidKey(keyValue.first))
	{(*this)[keyValue.first] = keyValue.second;}
    }
}

std::ostream& operator<<(std::ostream& out, BDSMagnetStrength const &st)
{
  for (auto key : st.keys)
    {out << key << ": " << st.GetValue(key) << G4endl;}
  return out;
}

G4double& BDSMagnetStrength::operator[](const G4String key)
{
  if (ValidKey(key))
    {
      // check if this key is initialised (non const version of this operator)
      // if it's not, initialise it so assignment will work
      auto it = strengths.find(key);
      if (it != strengths.end())
	{return it->second;}
      else
	{
	  strengths[key] = 0.0;
	  return strengths[key];
	}
    }
  else
    {
      G4cerr << "Invalid key \"" << key << G4endl;
      return variable;
    }
}

const G4double& BDSMagnetStrength::operator[](G4String key) const
{
  if (ValidKey(key))
    {return GetValue(key);}
  else
    {
      G4cerr << "Invalid key \"" << key << G4endl;
      return variable;
    }
}

std::vector<G4double> BDSMagnetStrength::NormalComponents() const
{
  std::vector<G4double> result;
  for (auto key : normalComponentKeys)
    {result.push_back(GetValue(key));}
  return result;
}

std::vector<G4double> BDSMagnetStrength::SkewComponents() const
{
  std::vector<G4double> result;
  for (auto key : skewComponentKeys)
    {result.push_back(GetValue(key));}
  return result;
}

G4bool BDSMagnetStrength::ValidKey(const G4String key) const
{
  if (std::find(keys.begin(), keys.end(), key) != keys.end())
    {return true;}
  else
    {return false;}
}

const G4double& BDSMagnetStrength::GetValue(const G4String key) const
{
  auto it = strengths.find(key);
  if (it != strengths.end())
    {return it->second;}
  else
    {return zero;}
}
  