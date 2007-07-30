/** BDSIM, v0.1   

Last modified 17.04.2006 by Ilya Agapov

**/

//==============================================================

#ifndef BDSTMultipole_h
#define BDSTMultipole_h 1

#include"globals.hh"
#include "BDSMaterials.hh"
#include "G4LogicalVolume.hh"

#include "G4FieldManager.hh"
#include "G4ChordFinder.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4UniformMagField.hh"
#include "G4PVPlacement.hh"               

#include "BDSMultipole.hh"
#include "BDSMultipoleMagField.hh"


#include <list>

class BDSTMultipole :public BDSMultipole
{
  public:
    BDSTMultipole(G4String aName, G4double aLength,
		  G4double bpRad,G4double FeRad,G4double outR,
		  list<G4double> aknl, list<double> aksl, G4String aMaterial = "");
    ~BDSTMultipole();

  protected:

  private:
  
  list<G4double> bnl;
  list<G4double> bsl;

  G4int itsOrder;
  

  //  void BuildOuterLogicalVolume();
  void BuildBPFieldAndStepper();
  //void BuildMarkerLogicalVolume();

  G4VisAttributes* SetVisAttributes();

  BDSMultipoleMagField *itsMagField;

  
};

#endif
