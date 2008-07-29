/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 

   Modified 22.03.05 by J.C.Carter, Royal Holloway, Univ. of London.
   Added GABs LCBeamGasPlugMat Material
   Added LCLead

   Modified 13.09.07 by S.P.Malton, Royal Holloway, Univ. of London.
   Created maps of materials and elements
   Added Initialise function
   Added Add/GetElement/Material functions
   Cleanup in destructor
   Added ListMaterials function
*/
const int DEBUG = 0;

#include "BDSGlobalConstants.hh" // must be first in include list

#include "BDSMaterials.hh"

using namespace std;

BDSMaterials::BDSMaterials()
{
  Initialise();
}

void BDSMaterials::Initialise()
{
  G4String name, symbol;             //a=mass of a mole;
  G4double a, z, density;            //z=mean number of protons;
  //G4int iz, n;                       //iz=number of protons  in an isotope;
                                   // n=number of nucleons in an isotope;

  //G4double abundance;
  G4double fractionmass;
  //G4int ncomponents;
  G4double temperature, pressure;

  G4Material* tmpMaterial;
  G4Element* tmpElement;

  //
  // Define elements
  //

  tmpElement = new G4Element
    (name="Hydrogen"   , symbol="H" , z=  1., a=   1.00*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Helium"     , symbol="He", z=  2., a=   4.0026*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Beryllium"  , symbol="Be", z=  4., a=   9.0122*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Carbon"     , symbol="C" , z=  6., a=  12.00*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Nitrogen"   , symbol="N" , z=  7., a=  14.01*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Oxygen"     , symbol="O" , z=  8., a=  16.00*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Aluminium"  , symbol="Al", z= 13., a=  26.98*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Silicon"    , symbol="Si", z= 14., a=  28.09*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Phosphorous", symbol="P" , z= 15., a=  30.973762*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Sulphur"    , symbol="S" , z= 16., a=  32.066*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Calcium"    , symbol="Ca", z= 20., a=  40.078*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Titanium"   , symbol="Ti", z= 22., a=  47.867*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Vanadium"   , symbol="V" , z= 23., a=  50.9415*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Manganese"  , symbol="Mn", z= 25., a=  54.93805*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Iron"       , symbol="Fe", z= 26., a=  55.847*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Cobalt"     , symbol="Co", z= 27., a=  58.93*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Nickel"     , symbol="Ni", z= 28., a=  58.693*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Copper"     , symbol="Cu", z= 29., a=  63.546*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Niobium"    , symbol="Nb", z= 41., a=  92.906*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Samarium"   , symbol="Sm", z= 62., a= 150.4*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Tungsten"   , symbol="W" , z= 74., a= 183.84*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Lead"       , symbol="Pb", z= 82., a= 207.2*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Molybdenum"       , symbol="Mo", z= 42., a= 95.94*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Chromium"       , symbol="Cr", z= 24., a= 51.9961*g/mole);
  elements[symbol] = tmpElement;


  //
  // Define materials
  //

  //default Geant4 temperature = 273.15 K
  //default Geant4 pressure = 1atm

  // solid materials

  tmpMaterial = new G4Material
    (name="aluminium"     , density=  2.700*g/cm3, 1, kStateSolid);
  tmpMaterial->AddElement(elements["Al"],1);
  materials[name] = tmpMaterial;

  tmpMaterial = new G4Material
    (name="vanadium"      , density=  5.800*g/cm3, 1, kStateSolid);
  tmpMaterial->AddElement(elements["V"],1);
  materials[name] = tmpMaterial;

  tmpMaterial = new G4Material
    (name="tungsten"      , density= 19.3  *g/cm3, 1, kStateSolid);
  tmpMaterial->AddElement(elements["W"],1);
  materials[name] = tmpMaterial;

  tmpMaterial = new G4Material
    (name="iron"          , density=  7.87 *g/cm3, 1, kStateSolid);
  tmpMaterial->AddElement(elements["Fe"],1);
  materials[name] = tmpMaterial;

  tmpMaterial = new G4Material
    (name="copper"        , density=  8.96 *g/cm3, 1, kStateSolid);
  tmpMaterial->AddElement(elements["Cu"],1);
  materials[name] = tmpMaterial;

  tmpMaterial = new G4Material
    (name="weightiron"    , density=  7.87 *g/cm3, 1, kStateSolid);
  tmpMaterial->AddElement(elements["Fe"],1);
  materials[name] = tmpMaterial;

  tmpMaterial = new G4Material
    (name="titanium"      , density=  4.54 *g/cm3, 1, kStateSolid);
  tmpMaterial->AddElement(elements["Ti"],1);
  materials[name] = tmpMaterial;

  tmpMaterial = new G4Material
    (name="graphite"      , density=  2.265*g/cm3, 1, kStateSolid);
  tmpMaterial->AddElement(elements["C"],1);
  materials[name] = tmpMaterial;

  tmpMaterial = new G4Material
    (name="lead"          , density= 11.35 *g/cm3, 1, kStateSolid);
  tmpMaterial->AddElement(elements["Pb"],1);
  materials[name] = tmpMaterial;

  tmpMaterial = new G4Material
    (name="silicon"       , density=  2.33 *g/cm3, 1, kStateSolid);
  tmpMaterial->AddElement(elements["Si"],1);
  materials[name] = tmpMaterial;

  tmpMaterial = new G4Material
    (name="beryllium"     , density=  1.848*g/cm3, 1, kStateSolid);
  tmpMaterial->AddElement(elements["Be"],1);
  materials[name] = tmpMaterial;

  tmpMaterial = new G4Material
    (name="smco"          , density=  8.4  *g/cm3, 2, kStateSolid, 300*kelvin);
  tmpMaterial->AddElement(elements["Sm"],fractionmass=0.338);
  tmpMaterial->AddElement(elements["Co"],fractionmass=0.662);
  materials[name] = tmpMaterial;

  tmpMaterial = new G4Material
    (name="niobium"       , density=  8.57 *g/cm3, 1, kStateSolid, 2*kelvin);
  tmpMaterial->AddElement(elements["Nb"],1);
  materials[name] = tmpMaterial;

  //Titanium alloy (BDS collimator material SLAC-TN-03-069 p25) deacon 15 Jun 2007
  tmpMaterial = new G4Material
    (name="titaniumalloy" , density=  4.48 *g/cm3, 3, kStateSolid, 300*kelvin);
  tmpMaterial->AddElement(elements["V"],  fractionmass=0.025);
  tmpMaterial->AddElement(elements["Al"], fractionmass=0.03 );
  tmpMaterial->AddElement(elements["Ti"], fractionmass=0.945);
  materials[name] = tmpMaterial;

  //Carbon Steel (shell of cryomodule). LDeacon 21 Feb 2006
  tmpMaterial = new G4Material
    (name="carbonsteel"   , density=  7.87 *g/cm3, 5, kStateSolid, 100*kelvin);
  tmpMaterial->AddElement(elements["C"], fractionmass=0.0017);
  tmpMaterial->AddElement(elements["Mn"], fractionmass=0.0045);
  tmpMaterial->AddElement(elements["P"], fractionmass=0.0004);
  tmpMaterial->AddElement(elements["S"], fractionmass=0.0005);
  tmpMaterial->AddElement(elements["Fe"], fractionmass=0.9929);
  materials[name] = tmpMaterial;

  //Stainless Steel 316L
  tmpMaterial = new G4Material
    (name="stainlesssteel"   , density=  8000 *kg/m3, 10, kStateSolid, 295*kelvin);
  tmpMaterial->AddElement(elements["C"], fractionmass=0.0003);
  tmpMaterial->AddElement(elements["Mn"], fractionmass=0.02);
  tmpMaterial->AddElement(elements["Si"], fractionmass=0.0075);
  tmpMaterial->AddElement(elements["P"], fractionmass=0.00045);
  tmpMaterial->AddElement(elements["S"], fractionmass=0.0003);
  tmpMaterial->AddElement(elements["Cr"], fractionmass=0.17);
  tmpMaterial->AddElement(elements["Mo"], fractionmass=0.025);
  tmpMaterial->AddElement(elements["Ni"], fractionmass=0.12);
  tmpMaterial->AddElement(elements["N"], fractionmass=0.001);
  tmpMaterial->AddElement(elements["Fe"], fractionmass=0.65545);
  materials[name] = tmpMaterial;

  //Kapton polyimide film
  tmpMaterial = new G4Material
    (name="kapton", density=  1.42 *g/cm3, 4, kStateSolid, 295*kelvin);
  tmpMaterial->AddElement(elements["H"], fractionmass=0.026362);
  tmpMaterial->AddElement(elements["C"], fractionmass=0.691133);
  tmpMaterial->AddElement(elements["N"], fractionmass=0.073270);
  tmpMaterial->AddElement(elements["O"], fractionmass=0.209235);
  materials[name] = tmpMaterial;


  tmpMaterial = new G4Material
    (name="nbti"          , density=  5.6  *g/cm3, 2, kStateSolid, 4*kelvin);
  tmpMaterial->AddElement(elements["Nb"], 1);
  tmpMaterial->AddElement(elements["Ti"], 1);
  materials[name] = tmpMaterial;

  tmpMaterial = new G4Material
    (name="concrete"      , density=  2.3  *g/cm3, 6, kStateSolid, 300*kelvin);
  tmpMaterial->AddElement(elements["Si"], 0.227915);
  tmpMaterial->AddElement(elements["O"], 0.60541);
  tmpMaterial->AddElement(elements["H"], 0.09972);
  tmpMaterial->AddElement(elements["Ca"], 0.04986);
  tmpMaterial->AddElement(elements["Al"], 0.014245);
  tmpMaterial->AddElement(elements["Fe"], 0.00285);
  materials[name] = tmpMaterial;

  //Invar.Temperature 2 kelvin. LDeacon 6th Feburary 2006
  tmpMaterial = new G4Material
    (name="invar"         , density=  8.1  *kg/m3, 2, kStateSolid, 2*kelvin);
  tmpMaterial->AddElement(elements["Ni"], fractionmass=0.35);
  tmpMaterial->AddElement(elements["Fe"], fractionmass=0.65);
  materials[name] = tmpMaterial;

  // "standard" soil (dry)
  tmpMaterial = new G4Material
    (name="soil"          , density=  1.9  *g/cm3, 4, kStateSolid, 300*kelvin);
  tmpMaterial->AddElement(elements["Si"],0.33377483443708611 );
  tmpMaterial->AddElement(elements["O"], 0.57218543046357617);
  tmpMaterial->AddElement(elements["H"], 0.022516556291390728);
  tmpMaterial->AddElement(elements["Al"], 0.071523178807947022);
  materials[name] = tmpMaterial;

  tmpMaterial = new G4Material
    (name="leadtungstate" , density=  8.27 *g/cm3, 3, kStateSolid);
  tmpMaterial->AddElement(elements["Pb"], 1);
  tmpMaterial->AddElement(elements["W"], 1);
  tmpMaterial->AddElement(elements["O"], 4);
  materials[name] = tmpMaterial;

	tmpMaterial = new G4Material 
		(name="fusedsilica", density=1.032*g/cm3, 2, kStateSolid);
	tmpMaterial->AddElement(elements["O"],2);
	tmpMaterial->AddElement(elements["Si"],1);
	const G4int FusedSilica_NUMENTRIES = 3; //Number of entries in the material properties table
  G4double FusedSilica_RIND[FusedSilica_NUMENTRIES]={1.49,1.49,1.49};
  G4double FusedSilica_AbsLength[FusedSilica_NUMENTRIES]={420.*cm,420.*cm,420.*cm};
  G4MaterialPropertiesTable *FusedSilica_mt = new G4MaterialPropertiesTable();
	G4double FusedSilica_Energy[FusedSilica_NUMENTRIES] = {2.0*eV,7.0*eV,7.14*eV};
  FusedSilica_mt->AddProperty("ABSLENGTH",FusedSilica_Energy,FusedSilica_AbsLength,FusedSilica_NUMENTRIES);

  FusedSilica_mt->AddProperty("RINDEX",FusedSilica_Energy,FusedSilica_RIND,FusedSilica_NUMENTRIES);
  tmpMaterial->SetMaterialPropertiesTable(FusedSilica_mt);
	materials[name] = tmpMaterial;

  // liquid materials

  tmpMaterial = new G4Material
    (name="liquidhelium"  , density=  0.12498*g/cm3, 1, kStateLiquid, 4.15*kelvin);
  tmpMaterial->AddElement(elements["He"],1);
  materials[name] = tmpMaterial;

  tmpMaterial=new G4Material
    (name="water"         , density=  1.00 *g/cm3, 2, kStateLiquid, 300*kelvin);
  tmpMaterial->AddElement(elements["O"], 1);
  tmpMaterial->AddElement(elements["H"], 2);
  materials[name] = tmpMaterial;

  // gaseous materials

  pressure    = 1.e-9*(1.e-3*bar);
  // include scaling so that statistics are more reasonable:
  // (one bunch contains ~10^10 particles, so 1000 events (or whatever)
  //  should represent this number of particles - hence ~10^7:
  pressure*=BDSGlobals->GetBackgroundScaleFactor();
 
  // NB : temperature should be defined for individual elements
  // setting temporarily to 300K
  temperature = 300*kelvin;

  // For air: do we want it at p=1atm or at p inside beampipe?
  // 1st case:
  // tmpMaterial = new G4Material
  //    (name="Air"           , density=  1.205*g/dm3, 2, kStateGas, 300*kelvin);
  // 2nd case:
  // ideal gas: 
  //   1) density * temperature / pressure = constant
  //   2) at T=STP_Temperature, p=1*atmosphere, n=1mole occupies a volume
  //      V=22.4l=22.4e-3*m3 and therefore has density=molar weight/(22.4e-3*m3)
  density = (STP_Temperature/temperature) * (pressure/(1.*atmosphere))
    * 30*g/(22.4*1.e-3*m3) ;
  tmpMaterial = new G4Material
    (name="air"           , density, 2, kStateGas, temperature, pressure);
  tmpMaterial->AddElement(elements["O"], fractionmass=0.2);
  tmpMaterial->AddElement(elements["N"], fractionmass=0.8);
	G4MaterialPropertiesTable *Air_mt = new G4MaterialPropertiesTable(); //Material properties table for air
	const G4int Air_NUMENTRIES = 3; //Number of entries in the material properties table
	G4double Air_RIND[Air_NUMENTRIES] = {1.000292,1.000292,1.000292};//Source: NPL Tables of Physical & Chemical Constants
	G4double Air_Energy[Air_NUMENTRIES] = {2.0*eV,7.0*eV,7.14*eV};
	Air_mt->AddProperty("RINDEX",Air_Energy, Air_RIND, Air_NUMENTRIES);
	tmpMaterial->SetMaterialPropertiesTable(Air_mt);
  materials[name] = tmpMaterial;

  density = (STP_Temperature/temperature) * (pressure/(1.*atmosphere))
    * (12.+16.)*g/(22.4*1.e-3*m3) ;
  tmpMaterial =  new G4Material
    (name="carbonmonoxide", density, 2, kStateGas, temperature, pressure);
  tmpMaterial->AddElement(elements["C"], 1);
  tmpMaterial->AddElement(elements["O"], 1);
  materials[name] = tmpMaterial;

  density = (STP_Temperature/temperature) * (pressure/(1.*atmosphere))
    // for H_2:
    //* 2*g/(22.4*1.e-3*m3) ;
    // for air:
    * 30*g/(22.4*1.e-3*m3) ;
  G4cout<< " ***************** defining Vacuum"<<G4endl;
  G4cout<< "pressure="<<pressure/bar<<" bar"<<G4endl;
  G4cout<< "temp="<<temperature/kelvin<<" K"<<G4endl;
  G4cout<< "density="<<density/(g/m3)<<"g/m^3"<<G4endl;
  tmpMaterial = new G4Material
    (name="vacuum"        , density, 1, kStateGas, temperature, pressure);
  tmpMaterial->AddMaterial(materials["air"], fractionmass=1.);
	G4MaterialPropertiesTable *Vacuum_mt = new G4MaterialPropertiesTable(); //Material properties table for vacuum
	const G4int Vacuum_NUMENTRIES = 3; //Number of entries in the material properties table
	G4double Vacuum_RIND[Vacuum_NUMENTRIES] = {1.,1.,1.};
	Vacuum_mt->AddProperty("RINDEX",Air_Energy, Vacuum_RIND, Air_NUMENTRIES);
	tmpMaterial->SetMaterialPropertiesTable(Vacuum_mt);
  materials[name] = tmpMaterial;

  tmpMaterial = new G4Material
    (name="laservac"      , density, 1, kStateGas, temperature, pressure);
  tmpMaterial->AddMaterial(materials["air"], fractionmass=1.);
  materials[name] = tmpMaterial;

  density=37.403/10.*g/cm3;  //Choose such that 1mm length gives ~ one interaction
  //  density=1*g/cm3;
  tmpMaterial = new G4Material
    (name="beamgasplugmat", density, 2);
  //shouldn't it be a gas?
  //(name="BeamGasPlugMat", density, 2, kStateGas);
  tmpMaterial->AddElement(elements["C"], 1);
  tmpMaterial->AddElement(elements["O"], 1);
  materials[name] = tmpMaterial;

}

void BDSMaterials::AddMaterial(G4Material* aMaterial, G4String aName)
{
  aName.toLower();
  if(materials.insert(make_pair(aName,aMaterial)).second)
    G4cout << "New material : " << aName << " added to material table" << G4endl;
  else G4Exception("Material "+aName+" already exists\n");
}

// add material with default state (kSolidState), temperature (273.15K)
// and pressure (1atm)
void BDSMaterials::AddMaterial(G4String aName, G4double itsZ, G4double itsA, G4double itsDensity)
{
  aName.toLower();
  G4Material* tmpMaterial = new G4Material(aName, itsZ, itsA*g/mole, itsDensity*g/cm3);
  if(materials.insert(make_pair(aName,tmpMaterial)).second)
    G4cout << "New material : " << aName << " added to material table" << G4endl;
  else G4Exception("Material "+aName+" already exists\n");
}

void BDSMaterials::AddMaterial(G4String aName, G4double itsDensity, G4State itsState,
G4double itsTemp, G4double itsPressure,
list<char*> itsComponents, list<G4double> itsComponentsFractions)
{
  aName.toLower();
  G4Material* tmpMaterial = new G4Material(aName, itsDensity*g/cm3, 
		(G4int)itsComponents.size(),itsState, itsTemp*kelvin, itsPressure*atmosphere);
  list<char*>::iterator sIter;
  list<G4double>::iterator dIter;
  for(sIter = itsComponents.begin(), dIter = itsComponentsFractions.begin();
        sIter != itsComponents.end();
        sIter++, dIter++)
  {
    tmpMaterial->AddElement(GetElement(*sIter),(*dIter));
  }
  if(materials.insert(make_pair(aName,tmpMaterial)).second)
    G4cout << "New material : " << aName << " added to material table" << G4endl;
  else G4Exception("Material "+aName+" already exists\n");
}

void BDSMaterials::AddMaterial(G4String aName, G4double itsDensity, G4State itsState,
G4double itsTemp, G4double itsPressure,      
list<char*> itsComponents, list<G4int> itsComponentsWeights)       
{
  aName.toLower();
  G4Material* tmpMaterial = new G4Material(aName, itsDensity*g/cm3, 
		(G4int)itsComponents.size(),itsState, itsTemp*kelvin, itsPressure*atmosphere);
  list<char*>::iterator sIter;
  list<G4int>::iterator iIter;
  for(sIter = itsComponents.begin(), iIter = itsComponentsWeights.begin(); 
	sIter != itsComponents.end();
	sIter++, iIter++)
  {
    tmpMaterial->AddElement(GetElement(*sIter),(*iIter));
  }     
  if(materials.insert(make_pair(aName,tmpMaterial)).second)
    G4cout << "New material : " << aName << " added to material table" << G4endl;
  else G4Exception("Material "+aName+" already exists\n");
}

void BDSMaterials::AddElement(G4Element* aElement, G4String aSymbol)
{
  if(elements.insert(make_pair(aSymbol,aElement)).second)
    G4cout << "New atom : " << aSymbol << G4endl;
  else G4Exception("Atom "+aSymbol+" already exists\n");
}

void BDSMaterials::AddElement(G4String aName, G4String aSymbol, G4double itsZ, G4double itsA)
{
  G4Element* tmpElement = new G4Element(aName, aSymbol, itsZ, itsA*g/mole);
  if(elements.insert(make_pair(aSymbol,tmpElement)).second)
    G4cout << "New atom : " << aSymbol << G4endl;
  else G4Exception("Atom "+aSymbol+" already exists\n");
}

G4Material* BDSMaterials::GetMaterial(G4String aMaterial)
{
  aMaterial.toLower();
  map<G4String,G4Material*>::iterator iter = materials.find(aMaterial);
  if(iter != materials.end()) return (*iter).second;
  else{
    G4Exception("Material "+aMaterial+" not known. Aborting.");
    exit(1);
  }
}

G4Element* BDSMaterials::GetElement(G4String aSymbol)
{
  map<G4String,G4Element*>::iterator iter = elements.find(aSymbol);
  if(iter != elements.end()) return (*iter).second;
  else{
    G4Exception("Element "+aSymbol+" not known. Aborting.");
    exit(1);
  }
}

void BDSMaterials::ListMaterials(){
  G4cout << "Available elements are:" << G4endl;
  G4cout << "Aluminium  - Al" << G4endl;
  G4cout << "Calcium    - Ca" << G4endl;
  G4cout << "Carbon     - C" << G4endl;
  G4cout << "Cobalt     - Co" << G4endl;
  G4cout << "Helium     - He" << G4endl;
  G4cout << "Hydrogen   - H" << G4endl;
  G4cout << "Iron       - Fe" << G4endl;
  G4cout << "Lead       - Pb" << G4endl;
  G4cout << "Manganese  - Mn" << G4endl;
  G4cout << "Niobium    - Nb" << G4endl;
  G4cout << "Nitrogen   - N" << G4endl;
  G4cout << "Oxygen     - O" << G4endl;
  G4cout << "Phosphorus - P" << G4endl;
  G4cout << "Samarium   - Sm" << G4endl;
  G4cout << "Silicon    - Si" << G4endl;
  G4cout << "Sulphur    - S" << G4endl;
  G4cout << "Titanium   - Ti" << G4endl;
  G4cout << "Tungsten   - W" << G4endl;
  G4cout << "Vanadium   - V" << G4endl;
  G4cout << "Molybdenum   - Mo" << G4endl;
  G4cout << "Chromium   - Cr" << G4endl;
  G4cout << "***************" << G4endl;
  G4cout << "Available materials are:" << G4endl;
  G4cout << "Air" << G4endl;
  G4cout << "Aluminium" << G4endl;
  G4cout << "Beryllium" << G4endl;
  G4cout << "CarbonMonoxide" << G4endl;
  G4cout << "CarbonSteel" << G4endl;
  G4cout << "Concrete" << G4endl;
  G4cout << "Copper" << G4endl;
  G4cout << "Graphite" << G4endl;
  G4cout << "Invar" << G4endl;
  G4cout << "Iron" << G4endl;
  G4cout << "LaserVac" << G4endl;
  G4cout << "Lead" << G4endl;
  G4cout << "LeadTungstate" << G4endl;
  G4cout << "LiquidHelium" << G4endl;
  G4cout << "NbTi" << G4endl;
  G4cout << "Niobium" << G4endl;
  G4cout << "Silicon" << G4endl;
  G4cout << "SmCo" << G4endl;
  G4cout << "Soil" << G4endl;
  G4cout << "StainlessSteel" << G4endl;
  G4cout << "Titanium" << G4endl;
  G4cout << "TitaniumAlloy" << G4endl;
  G4cout << "Tungsten" << G4endl;
  G4cout << "Vacuum" << G4endl;
  G4cout << "Vanadium" << G4endl;
  G4cout << "Water" << G4endl;
  G4cout << "WeightIron" << G4endl;
}

BDSMaterials::~BDSMaterials(){
  map<G4String,G4Material*>::iterator mIter;
  for(mIter = materials.begin(); mIter!=materials.end(); mIter++)
    delete (*mIter).second;
  materials.clear();

  map<G4String,G4Element*>::iterator eIter;
  for(eIter = elements.begin(); eIter!=elements.end(); eIter++)
    delete (*eIter).second;
  elements.clear();
}
