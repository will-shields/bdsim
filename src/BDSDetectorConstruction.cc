//  
//   BDSIM, (C) 2001-2007
//   
//   version 0.5-dev
//  
//
//
//   Geometry construction
//
//
//   History
//     19 May 2008 by Marchioni v.0.5-dev
//     18 Mar 2008 by Malton v.0.5-dev
//      3 Oct 2007 by Malton v.0.4
//     21 Nov 2006 by Agapov v.0.3
//     28 Mar 2006 by Agapov v.0.2
//     15 Dec 2005 by Agapov beta
//
//=================================================================

#include "BDSGlobalConstants.hh"

#include "BDSDetectorConstruction.hh"
#include "BDSTunnelCavity.hh"

#include "G4UserLimits.hh"
#include "G4GeometryManager.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"

#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UniformMagField.hh"
#include "G4TransportationManager.hh"
#include "G4PropagatorInField.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4ScoringBox.hh"
#include "G4ScoringManager.hh"
#include "G4PSCellFlux3D.hh"
#include "BDSScoreWriter.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "globals.hh"
#include "G4ios.hh"
#include <iostream>
#include <list>
#include <map>
#include "BDSAcceleratorComponent.hh"

#include "G4Navigator.hh"
#include "G4UniformMagField.hh"

#include "G4Material.hh"
#include "BDSEnergyCounterSD.hh"

// elements
#include "BDSWorld.hh"
//#include "BDSBeamPipe.hh"
#include "BDSDrift.hh"
#include "BDSPCLDrift.hh"
#include "BDSSectorBend.hh"
#include "BDSRBend.hh"
#include "BDSKicker.hh"
#include "BDSQuadrupole.hh"
#include "BDSSextupole.hh"
//#include "BDSSkewSextupole.hh"
#include "BDSOctupole.hh"
#include "BDSDecapole.hh"
#include "BDSTMultipole.hh"
#include "BDSRfCavity.hh"
#include "BDSSolenoid.hh"
#include "BDSSampler.hh"
#include "BDSSamplerCylinder.hh"
#include "BDSDump.hh"
#include "BDSLaserWire.hh"
#include "BDSLWCalorimeter.hh"
#include "BDSMuSpoiler.hh"
#include "BDSTransform3D.hh"
#include "BDSElement.hh"
#include "BDSComponentOffset.hh"
#include "BDSCollimator.hh"
//#include "BDSRealisticCollimator.hh"
// output interface
#include "BDSOutput.hh"
#include "BDSComponentFactory.hh"

//#include "BDSMultipoleOuterMagField.hh"
#include "G4MagneticField.hh"

// GMAD interface
#include "parser/gmad.h"
#include "ggmad.hh"

#include "G4VSampler.hh"
#include "G4GeometrySampler.hh"
#include "G4IStore.hh"

 
using namespace std;

//====================================

//typedef list<BDSAcceleratorComponent*>  BDSBeamline;

typedef list<BDSEnergyCounterSD*>  ECList;
ECList* theECList;

//extern BDSGlobalConstants* BDSGlobalConstants::Instance();
extern G4int gflash;
extern G4double gflashemax;
extern G4double gflashemin;

//--------------------------
// SYNCHROTRON RAD ***
G4double BDSLocalRadiusOfCurvature=DBL_MAX;// Used in Mean Free Path calc.
//--------------------------

G4Material* aMaterial;
extern G4double NumSpoilerRadLen;

typedef std::map<G4String,int> LogVolCountMap;
LogVolCountMap* LogVolCount;

typedef std::map<G4String,G4LogicalVolume*> LogVolMap;
LogVolMap* LogVol;

G4RotationMatrix* RotY90=new G4RotationMatrix();
G4RotationMatrix* RotYM90=new G4RotationMatrix();

G4RotationMatrix* RotX90=new G4RotationMatrix();
G4RotationMatrix* RotXM90=new G4RotationMatrix();

G4RotationMatrix* RotYM90X90=new G4RotationMatrix();
G4RotationMatrix* RotYM90XM90=new G4RotationMatrix();

//G4Navigator* StepperNavigator;
//G4Navigator* QuadNavigator;

//=========================================
extern BDSOutput* bdsOutput;
extern G4bool verbose;
extern G4bool outline;

#define DEBUG 1
#ifdef DEBUG
bool debug = true;
#else
bool debug = false;
#endif

//=================================================================



BDSDetectorConstruction::BDSDetectorConstruction()
{  
  
// create commands for interactive definition of the beamline  
  G4double pi_ov_2 = asin(1.);

  RotY90->rotateY(pi_ov_2);

  RotYM90->rotateY(-pi_ov_2);

  RotX90->rotateX(pi_ov_2);

  RotXM90->rotateX(-pi_ov_2);

  RotYM90X90->rotateY(-pi_ov_2);
  RotYM90X90->rotateX( pi_ov_2);

  RotYM90XM90->rotateY(-pi_ov_2);
  RotYM90XM90->rotateX(-pi_ov_2);

  // GlashStuff                                                                                                                                                         
  theParticleBounds  = new GFlashParticleBounds();              // Energy Cuts to kill particles                                                                
  theParticleBounds->SetMaxEneToParametrise(*G4Electron::ElectronDefinition(),gflashemax*GeV);
  theParticleBounds->SetMinEneToParametrise(*G4Electron::ElectronDefinition(),gflashemin*GeV);
  theParticleBounds->SetEneToKill(*G4Electron::ElectronDefinition(),BDSGlobalConstants::Instance()->GetThresholdCutCharged());
  
  theParticleBounds->SetMaxEneToParametrise(*G4Positron::PositronDefinition(),gflashemax*GeV);
  theParticleBounds->SetMinEneToParametrise(*G4Positron::PositronDefinition(),gflashemin*GeV);
  theParticleBounds->SetEneToKill(*G4Positron::PositronDefinition(),BDSGlobalConstants::Instance()->GetThresholdCutCharged());

  theParticleBoundsVac  = new GFlashParticleBounds();              // Energy Cuts to kill particles                                                                
  theParticleBoundsVac->SetMaxEneToParametrise(*G4Electron::ElectronDefinition(),0*GeV);
  theParticleBoundsVac->SetMaxEneToParametrise(*G4Positron::PositronDefinition(),0*GeV);

  G4cout << "BDSDetectorConstruction: theParticleBounds - min E - electron: " << theParticleBounds->GetMinEneToParametrise(*G4Electron::ElectronDefinition())/GeV<< " GeV" << G4endl;
  G4cout << "BDSDetectorConstruction: theParticleBounds - max E - electron: " << theParticleBounds->GetMaxEneToParametrise(*G4Electron::ElectronDefinition())/GeV<< G4endl;
  G4cout << "BDSDetectorConstruction: theParticleBounds - kill E - electron: " << theParticleBounds->GetEneToKill(*G4Electron::ElectronDefinition())/GeV<< G4endl;

G4cout << "BDSDetectorConstruction: theParticleBounds - min E - positron: " << theParticleBounds->GetMinEneToParametrise(*G4Positron::PositronDefinition())/GeV<< G4endl;
G4cout << "BDSDetectorConstruction: theParticleBounds - max E - positron: " << theParticleBounds->GetMaxEneToParametrise(*G4Positron::PositronDefinition())/GeV<< G4endl;
G4cout << "BDSDetectorConstruction: theParticleBounds - kill E - positron: " << theParticleBounds->GetEneToKill(*G4Positron::PositronDefinition())/GeV<< G4endl;

  theHitMaker          = new GFlashHitMaker();                    // Makes the EnergieSpots 

}

//=================================================================

G4VPhysicalVolume* BDSDetectorConstruction::Construct()
{
  theECList=new ECList;

  LogVolCount=new LogVolCountMap();

  LogVol=new LogVolMap();

  gasRegion = new G4Region("gasRegion");
  G4ProductionCuts* theGasProductionCuts = new G4ProductionCuts();
  theGasProductionCuts->SetProductionCut(1*m,G4ProductionCuts::GetIndex("gamma"));
  theGasProductionCuts->SetProductionCut(1*m,G4ProductionCuts::GetIndex("e-"));
  theGasProductionCuts->SetProductionCut(1*m,G4ProductionCuts::GetIndex("e+"));
  gasRegion->SetProductionCuts(theGasProductionCuts);


  if (verbose || debug) G4cout << "-->starting BDS construction \n"<<G4endl;

  return ConstructBDS();
}



G4VPhysicalVolume* BDSDetectorConstruction::ConstructBDS()
{
  //
  // set default output formats:
  //
  G4cout.precision(10);
  buildAtoms();
  buildMaterials();
  buildBeamline();
  buildWorld();
  G4cout << "BDSDetectorConstruction - returning physiWorld" << G4endl;
  G4cout << "BDSDetectorConstruction - the name of the world physical volume is " << _worldPhysicalVolume->GetName() <<  G4endl;
  if(verbose || debug) G4cout<< "BDSDetectorConstruction - returning physiWorld" << G4endl; 
  return   _worldPhysicalVolume;
}

//=================================================================
 
void BDSDetectorConstruction::buildAtoms(){
  //
  // convert the parsed atom list to list of Geant4 G4Elements
  //
  list<struct Element>::iterator it;
  if (verbose || debug) G4cout << "parsing the atom list..."<< G4endl;
  for(it = atom_list.begin();it!=atom_list.end();it++)
    {
#ifdef DEBUG
      G4cout << "---->adding Atom, "
	     << "name= " << (*it).name << " "
	     << "symbol= " << (*it).symbol << " "
	     << "Z= " << (*it).Z << " "
	     << "A= " << (*it).A << "g/mole "
	     << G4endl;
#endif
      
      BDSMaterials::Instance()->AddElement((*it).name,(*it).symbol,(*it).Z,(*it).A);
    }
  if (verbose || debug) G4cout << "size of atom list: "<< atom_list.size() << G4endl;
}


void BDSDetectorConstruction::buildMaterials(){
  //
  // convert the parsed material list to list of Geant4 G4Materials
  //
  list<struct Element>::iterator it;
  if (verbose || debug) G4cout << "parsing the material list..."<< G4endl;
  for(it = material_list.begin();it!=material_list.end();it++)
    {
      if((*it).Z != 0) {
#ifdef DEBUG  
	G4cout << "---->adding Material, "
	       << "name= "<< (*it).name << " "
	       << "Z= " << (*it).Z << " "
	       << "A= " << (*it).A << "g/mole "
	       << "density= "<< (*it).density << "g/cm3 "
	       << G4endl;
#endif
	BDSMaterials::Instance()->AddMaterial((*it).name,(*it).Z,(*it).A,(*it).density);
      }
      else if((*it).components.size() != 0){
	
	G4State itsState;
	if ((*it).state=="solid") itsState = kStateSolid;
	else
	  if ((*it).state=="liquid") itsState = kStateLiquid;
	  else
	    if ((*it).state=="gas") itsState = kStateGas;
	    else {
	      G4cout << "Unknown material state "<< (*it).state 
		     << ", setting it to default (solid)"
		     << G4endl;
	      (*it).state="solid";
	      itsState = kStateSolid;
	    }
	
	if((*it).componentsWeights.size()==(*it).components.size()) {
	  
#ifdef DEBUG 
	  G4cout << "---->adding Material, "
		 << "name= "<< (*it).name << " "
		 << "density= "<< (*it).density << "g/cm3 "
		 << "state= " << (*it).state << " "
		 << "T= " << (*it).temper << "K "
		 << "P= " << (*it).pressure << "atm "
		 << "ncomponents= " << (*it).components.size() << " "
		 << G4endl;
#endif
	  
	  BDSMaterials::Instance()->AddMaterial((G4String)(*it).name,
						(G4double)(*it).density,
						(G4State)itsState,
						(G4double)(*it).temper,
						(G4double)(*it).pressure,
						(std::list<const char*>)(*it).components,
						(std::list<G4int>)(*it).componentsWeights);
	}
	else if((*it).componentsFractions.size()==(*it).components.size()) {
	  
#ifdef DEBUG 
	  G4cout << "---->adding Material, "
		 << "name= "<< (*it).name << " "
		 << "density= "<< (*it).density << "g/cm3 "
		 << "state= " << (*it).state << " "
		 << "T= " << (*it).temper << "K "
		 << "P= " << (*it).pressure << "atm "
		 << "ncomponents= " << (*it).components.size() << " "
		 << G4endl;
#endif
	  BDSMaterials::Instance()->AddMaterial((*it).name,
						(*it).density,
						itsState,
						(*it).temper,
						(*it).pressure,
						(*it).components,
						(*it).componentsFractions);
	}
	else {
	  G4Exception("Badly defined material - number of components is not equal to number of weights or mass fractions!", "-1", FatalErrorInArgument, "");
	  exit(1);
	}
      }
      else {
	G4Exception("Badly defined material - need more information!", "-1", FatalErrorInArgument, "");
	exit(1);
      }
    }
  if (verbose || debug) G4cout << "size of material list: "<< material_list.size() << G4endl;
}

void  BDSDetectorConstruction::buildBeamline(){
  //
  // convert the parsed element list to list of BDS elements
  //
  BDSComponentFactory* theComponentFactory = new BDSComponentFactory();
  list<struct Element>::iterator it;  
  if (verbose || debug) G4cout << "parsing the beamline element list..."<< G4endl;
  for(it = beamline_list.begin();it!=beamline_list.end();it++){
    G4cout << "BDSDetectorConstruction creating component..." << G4endl;
    BDSAcceleratorComponent* temp = theComponentFactory->createComponent(it, beamline_list);
    G4cout << "pushing onto back of beamline..." << G4endl;
    if(temp){
      BDSBeamline::Instance()->addComponent(temp);
      //For the outline file...
      BDSBeamline::Instance()->currentItem()->SetK1((*it).k1);
      BDSBeamline::Instance()->currentItem()->SetK2((*it).k2);
      BDSBeamline::Instance()->currentItem()->SetK3((*it).k3);
    }
    G4cout << "done." << G4endl;
  }
  
  if (verbose || debug) G4cout << "size of beamline element list: "<< beamline_list.size() << G4endl;
  if (verbose || debug) G4cout << "size of theBeamline: "<< BDSBeamline::Instance()->size() << G4endl;
  if (verbose || debug) G4cout << "now constructing geometry" << G4endl;
  if (verbose || debug) G4cout << "first, getting the world size" << G4endl;
  
  list<BDSAcceleratorComponent*>::const_iterator iBeam;
  
  // free the parser list
  beamline_list.clear();
  
  if(verbose || debug) G4cout<<"end placement, size="<<BDSBeamline::Instance()->size()<<G4endl;
  
  if(verbose || debug) G4cout<<"Beamline Construction done"<<G4endl; 
  if(verbose || debug) G4cout<<"Printing beamline..."<<G4endl; 
  BDSBeamline::Instance()->print();
								 
  
#ifdef DEBUG 
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
#endif
}

void BDSDetectorConstruction::buildWorld(){
  //
  // set global magnetic field first
  //
  SetMagField(0.0); // necessary to set a global field; so chose zero
  constructWorldSolid();
  constructWorldLogical();
  setWorldVisAttributes();
  setWorldUserLimits();
  placeWorld();
  placeTunnelCavity();
  G4cout << "BDSDetectorConstruction::buildWorld() finished." << G4endl;
}

void  BDSDetectorConstruction::placeTunnelCavity(){
  G4cout << "BDSDetectorConstruction::placeTunnelCavity() ..." << G4endl;
  BDSTunnelCavity::Instance()->place(_worldLogicalVolume);
  G4cout << "BDSDetectorConstruction::placeTunnelCavity() - finished" << G4endl;
}

void BDSDetectorConstruction::placeWorld(){
  _worldPhysicalVolume = new G4PVPlacement(0, // no rotation
					   (G4ThreeVector)0,     // at (0,0,0)
					   _worldLogicalVolume,      
					   "world_physical",
					   NULL,                // its mother  volume
					   false,               // no boolean operation
					   0,                   // copy number
					   true
					   );
}

void BDSDetectorConstruction::calcWorldSize(){
  G4cout << "BDSDetectorConstruction::calcWorldSize() ..." << G4endl;
  G4cout << "BDSDetectorConstruction::calcWorldSize() setting x" << G4endl;
  G4double worldX = (BDSTunnelCavity::Instance()->widthMax().x()+BDSGlobalConstants::Instance()->GetTunnelSoilThickness())/2.0;
  _worldSize.setX(worldX);
  G4cout << "BDSDetectorConstruction::calcWorldSize() setting y" << G4endl;
  _worldSize.setY((BDSTunnelCavity::Instance()->widthMax().y()+BDSGlobalConstants::Instance()->GetTunnelSoilThickness())/2.0);
  G4cout << "BDSDetectorConstruction::calcWorldSize() setting z" << G4endl;
  _worldSize.setZ((BDSTunnelCavity::Instance()->widthMax().z())/2.0);
  G4cout << "BDSDetectorConstruction::calcWorldSize() finished." << G4endl;
}

void BDSDetectorConstruction::constructWorldSolid(){
  G4cout << "BDSDetectorConstruction::constructWorldSolid() ..." << G4endl;
  calcWorldSize();
  G4cout << "BDSDetectorConstruction::constructWorldSolid() making volume..." << G4endl;
  _worldSolidVolume = new G4Box("World", _worldSize.x(), _worldSize.y(), _worldSize.z());
  G4cout << "BDSDetectorConstruction::constructWorldSolid() dumping info....." << G4endl;
  _worldSolidVolume->DumpInfo();
  G4cout << "BDSDetectorConstruction::constructWorldSolid() finished." << G4endl;
}
void BDSDetectorConstruction::constructWorldLogical(){
  _worldMaterial = BDSMaterials::Instance()->GetMaterial(BDSGlobalConstants::Instance()->GetSoilMaterialName());
  G4cout << "BDSDetectorConstruction::constructWorldLogical() ..." << G4endl;
  _worldLogicalVolume = new G4LogicalVolume(_worldSolidVolume,
				       _worldMaterial,
				       "World");
  G4cout << "BDSDetectorConstruction::constructWorldLogical() finished." << G4endl;
}

void BDSDetectorConstruction::setWorldVisAttributes(){
  _worldVisAttributes = new G4VisAttributes(true);
  _worldVisAttributes->SetColor(0,1,0);
  //  _worldVisAttributes->SetLineStyle(1);
  _worldVisAttributes->SetLineWidth(1);
  //_worldVisAttributes->SetForceSolid(1);
  _worldVisAttributes->SetForceWireframe(1);
  _worldLogicalVolume->SetVisAttributes(_worldVisAttributes);
}
void BDSDetectorConstruction::setWorldUserLimits(){
#ifndef NOUSERLIMITS
  _worldUserLimits =new G4UserLimits();
  _worldUserLimits->SetMaxAllowedStep(10*m);
  _worldUserLimits->SetUserMinEkine(BDSGlobalConstants::Instance()->GetThresholdCutCharged());
  _worldUserLimits->SetUserMaxTime(BDSGlobalConstants::Instance()->GetMaxTime());
  _worldLogicalVolume->SetUserLimits(_worldUserLimits);
#endif
}


void BDSDetectorConstruction::SetMagField(const G4double fieldValue){
  
  G4FieldManager* fieldMgr =
    G4TransportationManager::GetTransportationManager()->GetFieldManager();
  magField = new G4UniformMagField(G4ThreeVector(0.,fieldValue,0.));  
  fieldMgr->SetDetectorField(magField);
  fieldMgr->CreateChordFinder(magField);
}

//=================================================================
  
void BDSDetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructBDS());
}

//=================================================================
BDSDetectorConstruction::~BDSDetectorConstruction()
{ 
  LogVolCount->clear();
  delete LogVolCount;

  LogVolMap::iterator iter;
  for(iter=LogVol->begin();iter!=LogVol->end();iter++){
    delete (*iter).second;
  }
  LogVol->clear();
  delete LogVol;

  delete precisionRegion;
  gFlashRegion.clear();
}



  //Set up geometric importance sampling.
//  G4GeometrySampler importanceSampler(physiWorld,"electron");
//  importanceSampler.SetParallel(false);
//  G4VIStore* BDSDetectorConstruction:: = new G4IStore(*physiWorld);
//  istore->AddImportanceGeometryCell(1, *physiWorld);
  //importanceSampler.PrepareImportanceSampling(istore);
  //  importanceSampler.Configure();
G4VIStore *BDSDetectorConstruction::CreateImportanceStore(){
//Create a geometry sampler  
  G4GeometrySampler* itsGeometrySampler = new G4GeometrySampler(GetWorldVolume(),"electron");
  itsGeometrySampler->SetParallel(false);


   if (!fPhysicalVolumeVector.size())
    {
      G4Exception("B01-DetectorConstruction: no physical volumes created yet!", "-1", FatalException, "");
    }
  
  // creating and filling the importance store
  
   G4IStore *itsIStore = new G4IStore(*_worldPhysicalVolume);
  
  G4double imp =1.0;
  G4int n=1;
  itsIStore->AddImportanceGeometryCell(1,*_worldPhysicalVolume);
  
  for (std::vector<G4VPhysicalVolume *>::iterator
	 it =  fPhysicalVolumeVector.begin();
       it != fPhysicalVolumeVector.end(); it++)
    {
    	  G4cout << "Going to assign importance: " << imp << ", to volume: "
		 << (*it)->GetName() << G4endl;
	  itsIStore->AddImportanceGeometryCell(imp, *(*it), (*it)->GetCopyNo());
 	  n++;
	  imp=imp+1.0;
    }



  //  for(  list<BDSAcceleratorComponent*>::const_iterator iBeam=theBeamline->begin();iBeam!=theBeamline->end();iBeam++){
  // itsIStore->AddImportanceGeometryCell(imp, *fPhysicalVolumeVector.at(0));
  //
  //  vector<G4VPhysicalVolume*> MultiplePhysicalVolumes = (*iBeam)->GetMultiplePhysicalVolumes();
  //  for (G4int i=0;i<MultiplePhysicalVolumes.size(); i++)
  //    {
  //G4cout << "Going to assign importance: " << imp << ", to accelerator component: "
  //	       << MultiplePhysicalVolumes.at(i)->GetName() << G4endl;
  //	G4cout << "Adding importance geometry cell: " << MultiplePhysicalVolumes.at(i)->GetName() << G4endl;       
  //	itsIStore->AddImportanceGeometryCell(imp, *MultiplePhysicalVolumes.at(i));
  //  }
  //  imp++;
  // }

  return itsIStore;
}

//=================================================================
