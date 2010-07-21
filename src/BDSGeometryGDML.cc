#ifdef USE_GDML
#include "BDSGlobalConstants.hh"
#include "BDSGeometryGDML.hh"
#include "G4SDManager.hh"
#include "BDSMaterials.hh"
#include "BDSSamplerSD.hh"
#include "G4SDManager.hh"
#include "BDSSamplerSD.hh"
#include "BDSOutput.hh"
#include "G4GDMLParser.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include <vector>
#include <map>
#include <cstdlib>
#include "G4ClassicalRK4.hh"
#include <cstring>

using namespace std;

extern BDSSamplerSD* BDSSamplerSensDet;

extern BDSMaterials* theMaterials;
extern G4RotationMatrix* RotY90;
extern BDSOutput* bdsOutput;
extern BDSGlobalConstants* BDSGlobals;

BDSGeometryGDML::BDSGeometryGDML(G4String GDMLfile){
  itsGDMLfile = GDMLfile;
}

void BDSGeometryGDML::Construct(G4LogicalVolume *marker){
  itsMarkerVol = marker;
  G4GDMLParser *parser = new G4GDMLParser();
  parser->Read(itsGDMLfile);
  
  G4LogicalVolume* topvol = parser->GetWorldVolume()->GetLogicalVolume();
  new G4PVPlacement(NULL,
                    G4ThreeVector(0.,0.,0.),
                    topvol,
                    topvol->GetName()+"_PhysiComp",
                    itsMarkerVol,
                    false,
                    0);
    
}
#endif
