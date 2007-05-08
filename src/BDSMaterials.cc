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

BDSMaterials::BDSMaterials()
{
 //This function illustrates the possible ways to define materials
 
  G4String name, symbol;             //a=mass of a mole;
  G4double a, z, density;            //z=mean number of protons;  
  G4int iz, n;                       //iz=number of protons  in an isotope; 
                                   // n=number of nucleons in an isotope;

  G4int ncomponents, natoms;
  G4double abundance, fractionmass;
  G4double temperature, pressure;


  //
  // define simple materials
  //

  density = 2.700*g/cm3;
  a = 26.98*g/mole;
  LCAluminium = new G4Material(name="LCAluminium", z=13., a, density);

  density = 19.3*g/cm3;
  a = 183.84*g/mole;
  LCTungsten=new G4Material(name="LCTungsten", z=74., a, density);

  density = 7.87*g/cm3;
  a = 55.845*g/mole;
  LCIron=new G4Material(name="LCIron", z=26., a, density);

  density = 8.96*g/cm3;
  a = 63.546*g/mole;
  LCCopper=new G4Material(name="LCCopper", z=29., a, density);

  density = 7.87*g/cm3;
  a = 55.845*g/mole;
  LCWeightIron=new G4Material(name="LCWeightIron", z=26., a, density);

  density = 4.54*g/cm3;
  a = 47.867*g/mole;
  LCTitanium=new G4Material(name="LCTitanium", z=22., a, density);

  density = 2.265*g/cm3;
  a = 12.011*g/mole;
  LCGraphite=new G4Material(name="LCGraphite", z=6., a, density);

  density = 11.35*g/cm3;
  a = 207.2*g/mole;
  LCLead=new G4Material(name="LCLead",z=82., a, density);

  density = 2.33*g/cm3;
  a = 28.0855*g/mole;
  LCSilicon=new G4Material(name="LCSilicon",z=14., a, density);

  density = 1.848*g/cm3;
  a = 9.0122*g/mole;
  LCBeryllium=new G4Material(name="LCBeryllium",z=4., a, density);

  a = 14.01*g/mole;
  G4Element* N  = new G4Element(name="Nitrogen",symbol="N" , z= 7., a);
  
  a = 16.00*g/mole;
  G4Element* O  = new G4Element(name="Oxygen"  ,symbol="O" , z= 8., a);

  a = 12.00*g/mole;
  G4Element* C  = new G4Element(name="Carbon"  ,symbol="C" , z= 6., a);

  a = 1.00*g/mole;
  G4Element* H  = new G4Element(name="Hydrogen"  ,symbol="H" , z= 1., a);

  a = 183.84*g/mole;
  G4Element* W  = new G4Element(name="Tungsten"  ,symbol="W" , z= 74., a);

  a = 207.2*g/mole;
  G4Element* Pb  = new G4Element(name="Lead"  ,symbol="Pb" , z= 82., a);

  a = 47.867*g/mole;
  G4Element* Ti = new G4Element(name="Titanium", symbol="Ti", z=22., a); 

  a = 92.906*g/mole;
  G4Element* Nb = new G4Element(name="Niobium", symbol="Nb", z=41., a);

  a = 58.693*g/mole;
  G4Element* Ni = new G4Element(name="Nickel", symbol="Ni", z=28., a); //LDeacon 6th Feb 2006
  
  a = 54.93805*g/mole;
  G4Element* Mn = new G4Element(name="Manganese", symbol="Mn", z=25., a); //LDeacon 21 Feb 2006
  
  a = 32.066*g/mole;
  G4Element* S = new G4Element(name="Sulphur", symbol="S", z = 16., a); //LDeacon 21 Feb 2006
  
  a = 55.847*g/mole;
  G4Element* Fe = new G4Element(name="Iron", symbol="Fe", z=26., a); //LDeacon 21 Feb 2006
  
  a = 30.973762*g/mole;
  G4Element* P = new G4Element(name ="Phosphorous", symbol="P", z=15., a);//LDeacon 21 Feb 2006

  G4Element* Ca = new G4Element
    (name="Calcium",symbol="Ca" , z= 20., a=40.078*g/mole);
  // aluminium
  G4Element* Al = new G4Element
    (name="Aluminium"  ,symbol="Al" , z= 13., a=26.98*g/mole);  
  
  G4Element* Si = new G4Element
    (name="Silicon",symbol="Si" , z= 14., a=28.09*g/mole);

  G4Element* Sm = new G4Element
    (name="Samarium",symbol="Sm", z= 62, a=150.4*g/mole);

  G4Element* Co = new G4Element
    (name="Cobalt",symbol="Co", z= 27, a=58.93*g/mole);
  
  G4Element* He = new G4Element
    (name="Helium",symbol="He", z = 2, a=4.0026*g/mole); 

  density = 0.12498*g/cm3;
  LiquidHelium = new G4Material(name="LiquidHelium",
			   density,
			   ncomponents=1,
			   kStateLiquid,
			   4.15*kelvin,
			   1*atmosphere);
  LiquidHelium->AddElement(He,1);

  density = 8.4*g/cm3;
  SmCo=new G4Material(name="SmCo",
			   density,
			   ncomponents=2,
			   kStateSolid,
			   300*kelvin,
			   1*atmosphere);
  SmCo->AddElement(Sm,fractionmass=0.338);
  SmCo->AddElement(Co,fractionmass=0.662);

  density = 8.57*g/cm3;
  LCNiobium=new G4Material(name="LCNiobium",
			   density,
			   1,
			   kStateSolid,
			   2*kelvin,
			   1*atmosphere);
  LCNiobium->AddElement(Nb,1);


  //Carbon Steel (shell of cryomodule). LDeacon 21 Feb 2006
  density = 7.87*g/cm3;
  LCCarbonSteel=new G4Material(name="LCCarbonSteel",
			       density,
			       ncomponents=5,
			       kStateSolid,
			       100*kelvin, 
			       1*atmosphere);
  
  LCCarbonSteel->AddElement(C, fractionmass=0.0017);
  LCCarbonSteel->AddElement(Mn, fractionmass=0.0045);
  LCCarbonSteel->AddElement(P, fractionmass=0.0004);
  LCCarbonSteel->AddElement(S, fractionmass=0.0005);
  LCCarbonSteel->AddElement(Fe, fractionmass=0.9929);


  pressure    = 1.e-9*(1.e-3*bar);
  // include scaling so that statistics are more reasonable:
  // (one bunch contains ~10^10 particles, so 1000 events (or whatever)
  //  should represent this number of particles - hence ~10^7:
  pressure*=BDSGlobals->GetBackgroundScaleFactor();

 
  // NB : temperature should be defined for individual elements
  // setting temporarily to 300K 
  temperature = 300 * kelvin;

  density=1.205*g/(1.e-3*m3); // 1 litre= 1.e-3 m^3
  LCAir = new G4Material(name="LCAir"  , density, ncomponents=2);
  LCAir->AddElement(O, fractionmass=0.2);
  LCAir->AddElement(N, fractionmass=0.8);

  density=(STP_Temperature/temperature)*(pressure/(1.*atmosphere))
    * (12.+16.)*g/(22.4*1.e-3*m3) ;
  G4Material* LCCarbonMonoxide=
    new G4Material("LCCarbonMonoxide",density,ncomponents=2);
  G4int nAtoms=1;
  LCCarbonMonoxide->AddElement(C, nAtoms);
  LCCarbonMonoxide->AddElement(O, nAtoms);

  //LCAir->AddElement(N, fractionmass=0.7);
  //LCAir->AddElement(O, fractionmass=0.3);


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
  LCVacuum = new G4Material(name="LCVacuum", density, ncomponents=1,
			    kStateGas,temperature,pressure);
  LCVacuum->AddMaterial(LCAir, fractionmass=1.);

  LaserVac = new G4Material(name="LaserVac", density, ncomponents=1,
			    kStateGas,temperature,pressure);
  LaserVac->AddMaterial(LCAir, fractionmass=1.);


 
  LCNbTi = new G4Material
    (name="LCNbTi", density=5.6*g/cm3, ncomponents=2,
     kStateSolid,4*kelvin,1*atmosphere);
  LCNbTi->AddElement(Nb, 1);
  LCNbTi->AddElement(Ti, 1);
  


  // concrete
  LCConcrete = new G4Material
    (name="LCConcrete", density=2.3*g/cm3, ncomponents=6,
     kStateSolid,300*kelvin,1*atmosphere);
  // (name="Concrete", density=2.3*g/cm3, ncomponents=1);


  LCConcrete->AddElement(Si, 0.227915);
  LCConcrete->AddElement(O, 0.60541);
  LCConcrete->AddElement(H, 0.09972);
  LCConcrete->AddElement(Ca, 0.04986);
  LCConcrete->AddElement(Al, 0.014245);
  LCConcrete->AddElement(Fe, 0.00285);

 //  // concrete type 2 - FLUKA
//   LCConcrete = new G4Material
//     (name="LCConcrete", density=2.3*g/cm3, ncomponents=6,
//      kStateSolid,300*kelvin,1*atmosphere);
//   // (name="Concrete", density=2.3*g/cm3, ncomponents=1);


//   LCConcrete->AddElement(Si, 0.227915);
//   LCConcrete->AddElement(O, 0.60541);
//   LCConcrete->AddElement(H, 0.09972);
//   LCConcrete->AddElement(Ca, 0.04986);
//   LCConcrete->AddElement(Al, 0.014245);
//   LCConcrete->AddElement(Fe, 0.00285);


  // water

  density = 1*g/cm3;
  LCWater=new G4Material(name="LCWater",density,ncomponents=2,
			 kStateLiquid,300*kelvin,1*atmosphere);
  LCWater->AddElement(O, 1);
  LCWater->AddElement(H, 2);

  //Invar.Temperature 2 kelvin. LDeacon 6th Feburary 2006
  LCInvar = new G4Material
    (name="LCInvar", density=8.1*kg/m3,ncomponents=2,kStateSolid,2*kelvin,1*atmosphere);
  LCInvar->AddElement(Ni, fractionmass=0.35);
  LCInvar->AddElement(Fe, fractionmass=0.65);
  
  
  // "standard" soil (dry)
	
  LCSoil = new G4Material
    (name="LCSoil", density=1.9*g/cm3, ncomponents=4,
     kStateSolid,300*kelvin,1*atmosphere);
   
  LCSoil->AddElement(Si,0.33377483443708611 );
  LCSoil->AddElement(O, 0.57218543046357617);
  LCSoil->AddElement(H, 0.022516556291390728);
  LCSoil->AddElement(Al, 0.071523178807947022);

 // making Lead Tungstate
  LCLeadTungstate = new G4Material
    (name="LCLeadTungstate", density=8.27*g/cm3, ncomponents=3);
  LCLeadTungstate->AddElement(Pb, natoms=1);
  LCLeadTungstate->AddElement(W,  natoms=1);
  LCLeadTungstate->AddElement(O,  natoms=4);

  density=37.403/10.*g/cm3;  //Choose such that 1mm length gives ~ one interaction
  //  density=1*g/cm3;  
  LCBeamGasPlugMat=new G4Material("LCBeamGasPlugMat",density,ncomponents=2);
  nAtoms=1;
  LCBeamGasPlugMat->AddElement(C, nAtoms);
  LCBeamGasPlugMat->AddElement(O, nAtoms);  

  //G4cout << *(G4Material::GetMaterialTable()) << G4endl;




}


