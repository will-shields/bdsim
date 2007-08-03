/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 

   Modified 22.03.05 by J.C.Carter, Royal Holloway, Univ. of London.
   Added GABs LCBeamGasPlugMat Material
   Added LCLead
*/
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
  G4int ncomponents;
  G4double temperature, pressure;

  G4Material* tmpMaterial;
  G4Element* tmpElement;

  density = 2.700*g/cm3;
  a = 26.98*g/mole;
  tmpMaterial = new G4Material(name="Aluminium", z=13., a, density);
  materials[name] = tmpMaterial;

  density = 5.800*g/cm3;
  a = 50.9415*g/mole;
  tmpMaterial = new G4Material(name="Vanadium", z=23., a, density);
  materials[name] = tmpMaterial;

  density = 19.3*g/cm3;
  a = 183.84*g/mole;
  tmpMaterial=new G4Material(name="Tungsten", z=74., a, density);
  materials[name] = tmpMaterial;

  density = 7.87*g/cm3;
  a = 55.845*g/mole;
  tmpMaterial = new G4Material(name="Iron", z=26., a, density);
  materials[name] = tmpMaterial;

  density = 8.96*g/cm3;
  a = 63.546*g/mole;
  tmpMaterial = new G4Material(name="Copper", z=29., a, density);
  materials[name] = tmpMaterial;

  density = 7.87*g/cm3;
  a = 55.845*g/mole;
  tmpMaterial = new G4Material(name="WeightIron", z=26., a, density);
  materials[name] = tmpMaterial;

  density = 4.54*g/cm3;
  a = 47.867*g/mole;
  tmpMaterial = new G4Material(name="Titanium", z=22., a, density);
  materials[name] = tmpMaterial;

  density = 2.265*g/cm3;
  a = 12.011*g/mole;
  tmpMaterial = new G4Material(name="Graphite", z=6., a, density);
  materials[name] = tmpMaterial;

  density = 11.35*g/cm3;
  a = 207.2*g/mole;
  tmpMaterial = new G4Material(name="Lead",z=82., a, density);
  materials[name] = tmpMaterial;

  density = 2.33*g/cm3;
  a = 28.0855*g/mole;
  tmpMaterial = new G4Material(name="Silicon",z=14., a, density);
  materials[name] = tmpMaterial;

  density = 1.848*g/cm3;
  a = 9.0122*g/mole;
  tmpMaterial = new G4Material(name="Beryllium",z=4., a, density);
  materials[name] = tmpMaterial;

  a = 14.01*g/mole;
  tmpElement = new G4Element(name="Nitrogen",symbol="N" , z= 7., a);
  elements[symbol] = tmpElement;

  a = 16.00*g/mole;
  tmpElement = new G4Element(name="Oxygen"  ,symbol="O" , z= 8., a);
  elements[symbol] = tmpElement;

  a = 12.00*g/mole;
  tmpElement = new G4Element(name="Carbon"  ,symbol="C" , z= 6., a);
  elements[symbol] = tmpElement;

  a = 1.00*g/mole;
  tmpElement = new G4Element(name="Hydrogen"  ,symbol="H" , z= 1., a);
  elements[symbol] = tmpElement;

  a = 183.84*g/mole;
  tmpElement = new G4Element(name="Tungsten"  ,symbol="W" , z= 74., a);
  elements[symbol] = tmpElement;

  a = 207.2*g/mole;
  tmpElement = new G4Element(name="Lead"  ,symbol="Pb" , z= 82., a);
  elements[symbol] = tmpElement;

  a = 47.867*g/mole;
  tmpElement = new G4Element(name="Titanium", symbol="Ti", z=22., a);
  elements[symbol] = tmpElement;

  a = 92.906*g/mole;
  tmpElement = new G4Element(name="Niobium", symbol="Nb", z=41., a);
  elements[symbol] = tmpElement;

  a = 58.693*g/mole;
  tmpElement = new G4Element(name="Nickel", symbol="Ni", z=28., a); //LDeacon 6th Fe$
  elements[symbol] = tmpElement;

  a = 54.93805*g/mole;
  tmpElement = new G4Element(name="Manganese", symbol="Mn", z=25., a); //LDeacon 21 $
  elements[symbol] = tmpElement;

  a = 32.066*g/mole;
  tmpElement = new G4Element(name="Sulphur", symbol="S", z = 16., a); //LDeacon 21 Fe$
  elements[symbol] = tmpElement;

  a = 55.847*g/mole;
  tmpElement = new G4Element(name="Iron", symbol="Fe", z=26., a); //LDeacon 21 Feb 2$
  elements[symbol] = tmpElement;

  a = 30.973762*g/mole;
  tmpElement = new G4Element(name ="Phosphorous", symbol="P", z=15., a);//LDeacon 21 $
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Calcium",symbol="Ca" , z= 20., a=40.078*g/mole);
  elements[symbol] = tmpElement;

  // aluminium
  tmpElement = new G4Element
    (name="Aluminium"  ,symbol="Al" , z= 13., a=26.98*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Vanadium"  ,symbol="V" , z= 23., a=50.9415*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Silicon",symbol="Si" , z= 14., a=28.09*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Samarium",symbol="Sm", z= 62, a=150.4*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Cobalt",symbol="Co", z= 27, a=58.93*g/mole);
  elements[symbol] = tmpElement;

  tmpElement = new G4Element
    (name="Helium",symbol="He", z = 2, a=4.0026*g/mole);
  elements[symbol] = tmpElement;

  density = 0.12498*g/cm3;
  tmpMaterial = new G4Material(name="LiquidHelium",
                           density,
                           ncomponents=1,
                           kStateLiquid,
                           4.15*kelvin,
                           1*atmosphere);
  tmpMaterial->AddElement(elements["He"],1);
  materials[name] = tmpMaterial;


  density = 8.4*g/cm3;
  tmpMaterial = new G4Material(name="SmCo",
                           density,
                           ncomponents=2,
                           kStateSolid,
                           300*kelvin,
                           1*atmosphere);
  tmpMaterial->AddElement(elements["Sm"],fractionmass=0.338);
  tmpMaterial->AddElement(elements["Co"],fractionmass=0.662);
  materials[name] = tmpMaterial;

  density = 8.57*g/cm3;
  tmpMaterial = new G4Material(name="Niobium",
                           density,
                           1,
                           kStateSolid,
                           2*kelvin,
                           1*atmosphere);
  tmpMaterial->AddElement(elements["Nb"],1);
  materials[name] = tmpMaterial;

  //Titanium alloy (BDS collimator material SLAC-TN-03-069 p25) deacon 15 Jun 2007
  density = 4.48*g/cm3;
  tmpMaterial = new G4Material(name="TitaniumAlloy",                          
                                         density,
                                         ncomponents=3,
                                         kStateSolid,
                                         300*kelvin,
                                         1*atmosphere);
  tmpMaterial->AddElement(elements["V"],  fractionmass=0.025);
  tmpMaterial->AddElement(elements["Al"], fractionmass=0.03 );
  tmpMaterial->AddElement(elements["Ti"], fractionmass=0.945);
  materials[name] = tmpMaterial;

  //Carbon Steel (shell of cryomodule). LDeacon 21 Feb 2006
  density = 7.87*g/cm3;
  tmpMaterial = new G4Material(name="CarbonSteel",
                               density,
                               ncomponents=5,
                               kStateSolid,
                               100*kelvin,
                               1*atmosphere);

  tmpMaterial->AddElement(elements["C"], fractionmass=0.0017);
  tmpMaterial->AddElement(elements["Mn"], fractionmass=0.0045);
  tmpMaterial->AddElement(elements["P"], fractionmass=0.0004);
  tmpMaterial->AddElement(elements["S"], fractionmass=0.0005);
  tmpMaterial->AddElement(elements["Fe"], fractionmass=0.9929);
  materials[name] = tmpMaterial;

  pressure    = 1.e-9*(1.e-3*bar);
  // include scaling so that statistics are more reasonable:
  // (one bunch contains ~10^10 particles, so 1000 events (or whatever)
  //  should represent this number of particles - hence ~10^7:
  pressure*=BDSGlobals->GetBackgroundScaleFactor();

 
  // NB : temperature should be defined for individual elements
  // setting temporarily to 300K
  temperature = 300 * kelvin;

  density=1.205*g/(1.e-3*m3); // 1 litre= 1.e-3 m^3
  tmpMaterial = new G4Material(name="Air"  , density, ncomponents=2,kStateGas,temperature,pressure);
  tmpMaterial->AddElement(elements["O"], fractionmass=0.2);
  tmpMaterial->AddElement(elements["N"], fractionmass=0.8);
  materials[name] = tmpMaterial;

  density=(STP_Temperature/temperature)*(pressure/(1.*atmosphere))
    * (12.+16.)*g/(22.4*1.e-3*m3) ;

  tmpMaterial =  new G4Material(name="CarbonMonoxide",density,ncomponents=2);
  tmpMaterial->AddElement(elements["C"], 1);
  tmpMaterial->AddElement(elements["O"], 1);
  materials[name] = tmpMaterial;

  //Air->AddElement(elements["N"], fractionmass=0.7);
  //Air->AddElement(elements["O"], fractionmass=0.3);

  density = (STP_Temperature/temperature)*(pressure/(1.*atmosphere))
    // for H_2:
    * 2*g/(22.4*1.e-3*m3) ;


  // for air:
  //        * 30*g/(22.4*1.e-3*m3) ;
  G4cout<< " ***************** defining LCVacuum"<<G4endl;
  G4cout<< "pressure="<<pressure/bar<<" bar"<<G4endl;
  G4cout<< "temp="<<temperature/kelvin<<" K"<<G4endl;
  G4cout<< "density="<<density/(g/m3)<<"g/m^3"<<G4endl;
  
  //temperature = STP_Temperature;         //from PhysicalConstants.h
  tmpMaterial = new G4Material(name="Vacuum", density, ncomponents=1,
                            kStateGas,temperature,pressure);
  tmpMaterial->AddMaterial(materials["Air"], fractionmass=1.);
  materials[name] = tmpMaterial;

  tmpMaterial = new G4Material(name="LaserVac", density, ncomponents=1,
                            kStateGas,temperature,pressure);
  tmpMaterial->AddMaterial(materials["Air"], fractionmass=1.);
  materials[name] = tmpMaterial;

   tmpMaterial = new G4Material(name="NbTi", density=5.6*g/cm3, ncomponents=2,
     kStateSolid,4*kelvin,1*atmosphere);
  tmpMaterial->AddElement(elements["Nb"], 1);
  tmpMaterial->AddElement(elements["Ti"], 1);
  materials[name] = tmpMaterial;

  // concrete
  tmpMaterial = new G4Material(name="Concrete", density=2.3*g/cm3, ncomponents=6,
     kStateSolid,300*kelvin,1*atmosphere);
  // (name="Concrete", density=2.3*g/cm3, ncomponents=1);

  tmpMaterial->AddElement(elements["Si"], 0.227915);
  tmpMaterial->AddElement(elements["O"], 0.60541);
  tmpMaterial->AddElement(elements["H"], 0.09972);
  tmpMaterial->AddElement(elements["Ca"], 0.04986);
  tmpMaterial->AddElement(elements["Al"], 0.014245);
  tmpMaterial->AddElement(elements["Fe"], 0.00285);
  materials[name] = tmpMaterial;

 //  // concrete type 2 - FLUKA
//   Concrete = new G4Material
//     (name="Concrete", density=2.3*g/cm3, ncomponents=6,
//      kStateSolid,300*kelvin,1*atmosphere);
//   // (name="Concrete", density=2.3*g/cm3, ncomponents=1);
//   Concrete->AddElement(elements["Si"], 0.227915);
//   Concrete->AddElement(elements["O"], 0.60541);
//   Concrete->AddElement(H, 0.09972);
//   Concrete->AddElement(Ca, 0.04986);
//   Concrete->AddElement(Al, 0.014245);
//   Concrete->AddElement(Fe, 0.00285);


  // water

  density = 1*g/cm3;
  tmpMaterial=new G4Material(name="Water",density,ncomponents=2,
                         kStateLiquid,300*kelvin,1*atmosphere);
  tmpMaterial->AddElement(elements["O"], 1);
  tmpMaterial->AddElement(elements["H"], 2);
  materials[name] = tmpMaterial;

  //Invar.Temperature 2 kelvin. LDeacon 6th Feburary 2006
  tmpMaterial = new G4Material
    (name="Invar", density=8.1*kg/m3,ncomponents=2,kStateSolid,2*kelvin,1*atmosphere);
  tmpMaterial->AddElement(elements["Ni"], fractionmass=0.35);
  tmpMaterial->AddElement(elements["Fe"], fractionmass=0.65);
  materials[name] = tmpMaterial;

  // "standard" soil (dry)

  tmpMaterial = new G4Material
    (name="Soil", density=1.9*g/cm3, ncomponents=4,
     kStateSolid,300*kelvin,1*atmosphere);

  tmpMaterial->AddElement(elements["Si"],0.33377483443708611 );
  tmpMaterial->AddElement(elements["O"], 0.57218543046357617);
  tmpMaterial->AddElement(elements["H"], 0.022516556291390728);
  tmpMaterial->AddElement(elements["Al"], 0.071523178807947022);
  materials[name] = tmpMaterial;

 // making Lead Tungstate
  tmpMaterial = new G4Material
    (name="LeadTungstate", density=8.27*g/cm3, ncomponents=3);
  tmpMaterial->AddElement(elements["Pb"], 1);
  tmpMaterial->AddElement(elements["W"], 1);
  tmpMaterial->AddElement(elements["O"], 4);
  materials[name] = tmpMaterial;

  density=37.403/10.*g/cm3;  //Choose such that 1mm length gives ~ one interaction
  //  density=1*g/cm3;
  tmpMaterial = new G4Material(name="BeamGasPlugMat",density,ncomponents=2);
  tmpMaterial->AddElement(elements["C"], 1);
  tmpMaterial->AddElement(elements["O"], 1);
  materials[name] = tmpMaterial;
  //G4cout << *(G4Material::GetMaterialTable()) << G4endl;

}

void BDSMaterials::AddMaterial(G4Material* aMaterial, G4String aName)
{
  materials[aName] = aMaterial;
}

void BDSMaterials::AddMaterial(G4String aName, G4double itsZ, G4double itsA, G4double itsDensity)
{
  materials[aName] = new G4Material(aName, itsZ, itsA, itsDensity);
}

void BDSMaterials::AddMaterial(G4String aName, G4double itsDensity, G4State itsState,
G4double itsTemp, G4double itsPressure,
list<char*> itsComponents, list<G4double> itsComponentsFractions)
{
  G4Material* tmpMaterial = new G4Material(aName, itsDensity, 
		(G4int)itsComponents.size(),itsState, itsTemp, itsPressure);
  list<char*>::iterator sIter;
  list<G4double>::iterator dIter;
  for(sIter = itsComponents.begin(), dIter = itsComponentsFractions.begin();
        sIter != itsComponents.end();
        sIter++, dIter++)
  {
    tmpMaterial->AddElement(GetElement(*sIter),(*dIter));
  }
  materials[aName] = tmpMaterial;
}

void BDSMaterials::AddMaterial(G4String aName, G4double itsDensity, G4State itsState,
G4double itsTemp, G4double itsPressure,      
list<char*> itsComponents, list<G4int> itsComponentsWeights)       
{
  G4Material* tmpMaterial = new G4Material(aName, itsDensity, 
		(G4int)itsComponents.size(),itsState, itsTemp, itsPressure);
  list<char*>::iterator sIter;
  list<G4int>::iterator iIter;
  for(sIter = itsComponents.begin(), iIter = itsComponentsWeights.begin(); 
	sIter != itsComponents.end();
	sIter++, iIter++)
  {
    tmpMaterial->AddElement(GetElement(*sIter),(*iIter));
  }     

  materials[aName] = tmpMaterial;
}

void BDSMaterials::AddElement(G4Element* aElement, G4String aName)
{
  elements[aName] = aElement;
}

void BDSMaterials::AddElement(G4String aName, G4String aSymbol, G4double itsZ, G4double itsA)
{
  elements[aName] = new G4Element(aName, aSymbol, itsZ, itsA);
}

G4Material* BDSMaterials::GetMaterial(G4String aMaterial)
{
  map<G4String,G4Material*>::iterator iter = materials.find(aMaterial);
  if(iter != materials.end()) return (*iter).second;
  else{
    G4Exception("Material not known. Aborting.");
    exit(1);
  }
}

G4Element* BDSMaterials::GetElement(G4String aSymbol)
{
  map<G4String,G4Element*>::iterator iter = elements.find(aSymbol);
  if(iter != elements.end()) return (*iter).second;
  else{
    G4Exception("Element not known. Aborting.");
    exit(1);
  }
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
