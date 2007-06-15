/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 

   Added Get method to enable access to materials by passing a string
*/
#ifndef BDSMaterials_h
#define BDSMaterials_h 1

#include "globals.hh"
#include "G4Material.hh"


class BDSMaterials
{
public:

 
  BDSMaterials();

  G4Material*  GetMaterial(G4String aMaterial);

  G4Material*  LCTitanium;
  G4Material*  LCTungsten;
  G4Material*  LCGraphite;
  G4Material*  LCAir;
  G4Material*  LCVacuum;
  G4Material*  LCAluminium;
  G4Material*  LCIron;
  G4Material*  LCWeightIron;
  G4Material*  LaserVac;
  G4Material*  LCCopper;
  G4Material*  LCConcrete;
  G4Material*  LCSoil;
  G4Material*  LCWater;
  G4Material*  LCLeadTungstate;
  G4Material*  LCLead;
  G4Material*  LCBeamGasPlugMat;
  G4Material*  LCSilicon;
  G4Material*  LCBeryllium;
  G4Material*  LCNiobium;
  G4Material*  LCNbTi;
  G4Material*  LCInvar;
  G4Material*  LCCarbonSteel;
	G4Material*  LCVanadium;
	G4Material*  LCTitaniumAlloy;
  G4Material*  SmCo;
  G4Material*  LiquidHelium;

private:


};

inline G4Material* BDSMaterials::GetMaterial(G4String aMaterial)
{
  G4String::caseCompare cmpmode = G4String::ignoreCase;
  if(aMaterial.compareTo("Titanium",cmpmode)==0) return LCTitanium;
  else if(aMaterial.compareTo("Tungsten",cmpmode)==0) return LCTungsten;
  else if(aMaterial.compareTo("Graphite",cmpmode)==0) return LCGraphite;
  else if(aMaterial.compareTo("Air",cmpmode)==0) return LCAir;
  else if(aMaterial.compareTo("Vacuum",cmpmode)==0) return LCVacuum;
  else if(aMaterial.compareTo("Aluminium",cmpmode)==0) return LCAluminium;
  else if(aMaterial.compareTo("Iron",cmpmode)==0) return LCIron;
  else if(aMaterial.compareTo("WeightIron",cmpmode)==0) return LCWeightIron;
  else if(aMaterial.compareTo("LaserVac",cmpmode)==0) return LaserVac;
  else if(aMaterial.compareTo("Copper",cmpmode)==0) return LCCopper;
  else if(aMaterial.compareTo("Concrete",cmpmode)==0) return LCConcrete;
  else if(aMaterial.compareTo("Water",cmpmode)==0) return LCWater;
  else if(aMaterial.compareTo("LeadTungstate",cmpmode)==0) return LCLeadTungstate;
  else if(aMaterial.compareTo("Lead",cmpmode)==0) return LCLead;
  else if(aMaterial.compareTo("Beryllium",cmpmode)==0) return LCBeryllium;
  else if(aMaterial.compareTo("Silicon",cmpmode)==0) return LCSilicon;  
  else if(aMaterial.compareTo("Niobium",cmpmode)==0) return LCNiobium;  
  else if(aMaterial.compareTo("NbTi",cmpmode)==0) return LCNbTi;  
  else if(aMaterial.compareTo("Invar",cmpmode)==0) return LCInvar; 
  else if(aMaterial.compareTo("CarbonSteel", cmpmode)==0) return LCCarbonSteel;
  else if(aMaterial.compareTo("Vanadium", cmpmode)==0) return LCVanadium;
  else if(aMaterial.compareTo("TitaniumAlloy", cmpmode)==0) return LCTitaniumAlloy;
  else if(aMaterial.compareTo("SmCo", cmpmode)==0) return SmCo;
  else if(aMaterial.compareTo("LHe", cmpmode)==0) return LiquidHelium;
  else
    {
      G4cout <<aMaterial<<" not known. Using Vacuum instead!!" <<G4endl;
      return LCVacuum; //default if not recognized
    }

}

#endif
