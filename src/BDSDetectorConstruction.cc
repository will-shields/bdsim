//  
//   BDSIM, (C) 2001-2007
//   
//   version 0.4
//  
//
//
//   Geometry construction
//
//
//   History
//      3 Oct 2007 by Malton v.0.4
//     21 Nov 2006 by Agapov v.0.3
//     28 Mar 2006 by Agapov v.0.2
//     15 Dec 2005 by Agapov beta
//


const int DEBUG = 0;

//=================================================================

#include "BDSGlobalConstants.hh"

#include "BDSDetectorConstruction.hh"

#include "G4UserLimits.hh"

#include "G4Region.hh"
#include "G4ProductionCuts.hh"

#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4PropagatorInField.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"

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
#include "BDSBeamPipe.hh"
#include "BDSDrift.hh"
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

// output interface
#include "BDSOutput.hh"

//#include "BDSMultipoleOuterMagField.hh"
#include "G4MagneticField.hh"

// GMAD interface
#include "parser/gmad.h"
#include "ggmad.hh"

using namespace std;


//====================================

typedef list<BDSAcceleratorComponent*>  BDSBeamline;
BDSBeamline theBeamline;

typedef list<BDSEnergyCounterSD*>  ECList;
ECList* theECList;

BDSMaterials* theMaterials;

extern BDSGlobalConstants* BDSGlobals;

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

G4Navigator* StepperNavigator;
G4Navigator* QuadNavigator;

//=========================================
G4FieldManager* theOuterFieldManager;

extern BDSOutput bdsOutput;
extern G4bool verbose;
extern G4bool outline;
//=================================================================



BDSDetectorConstruction::BDSDetectorConstruction()
{  // create commands for interactive definition of the beamline  
  G4double pi_ov_2 = asin(1.);

  RotY90->rotateY(pi_ov_2);
  RotYM90->rotateY(-pi_ov_2);
}

//=================================================================

G4VPhysicalVolume* BDSDetectorConstruction::Construct()
{
  theECList=new ECList;

  LogVolCount=new LogVolCountMap();

  LogVol=new LogVolMap();

  theMaterials=new BDSMaterials();

  if (verbose || DEBUG) G4cout << "-->starting BDS construction \n"<<G4endl;

  return ConstructBDS(beamline_list);

}


G4VPhysicalVolume* BDSDetectorConstruction::ConstructBDS(list<struct Element>& beamline_list)
{
  //
  // set default output formats:
  //
  G4cout.precision(10);
  
  
  //
  // convert the parsed atom list to list of Geant4 G4Elements
  //
  list<struct Element>::iterator it;


  if (verbose || DEBUG) G4cout << "parsing the atom list..."<< G4endl;
  for(it = atom_list.begin();it!=atom_list.end();it++)
  {
    if (DEBUG) G4cout << "---->adding Atom, "
		      << "name= " << (*it).name << " "
		      << "symbol= " << (*it).symbol << " "
		      << "Z= " << (*it).Z << " "
		      << "A= " << (*it).A << "g/mole "
		      << G4endl;

    theMaterials->AddElement((*it).name,(*it).symbol,(*it).Z,(*it).A);
  }
  if (verbose || DEBUG) G4cout << "size of atom list: "<< atom_list.size() << G4endl;


  //
  // convert the parsed material list to list of Geant4 G4Materials
  //
  if (verbose || DEBUG) G4cout << "parsing the material list..."<< G4endl;
  for(it = material_list.begin();it!=material_list.end();it++)
  {
    if((*it).Z != 0) {
      if (DEBUG) G4cout << "---->adding Material, "
			<< "name= "<< (*it).name << " "
			<< "Z= " << (*it).Z << " "
			<< "A= " << (*it).A << "g/mole "
			<< "density= "<< (*it).density << "g/cm3 "
			<< G4endl;

      theMaterials->AddMaterial((*it).name,(*it).Z,(*it).A,(*it).density);
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

	if (DEBUG) G4cout << "---->adding Material, "
			  << "name= "<< (*it).name << " "
			  << "density= "<< (*it).density << "g/cm3 "
			  << "state= " << (*it).state << " "
			  << "T= " << (*it).temper << "K "
			  << "P= " << (*it).pressure << "atm "
			  << "ncomponents= " << (*it).components.size() << " "
			  << G4endl;

	theMaterials->AddMaterial((*it).name,
				  (*it).density,
				  itsState,
				  (*it).temper,
				  (*it).pressure,
				  (*it).components,
				  (*it).componentsWeights);
      }
      else if((*it).componentsFractions.size()==(*it).components.size()) {

	if (DEBUG) G4cout << "---->adding Material, "
			  << "name= "<< (*it).name << " "
			  << "density= "<< (*it).density << "g/cm3 "
			  << "state= " << (*it).state << " "
			  << "T= " << (*it).temper << "K "
			  << "P= " << (*it).pressure << "atm "
			  << "ncomponents= " << (*it).components.size() << " "
			  << G4endl;

        theMaterials->AddMaterial((*it).name,
				  (*it).density,
				  itsState,
				  (*it).temper,
				  (*it).pressure,
				  (*it).components,
				  (*it).componentsFractions);
      }
      else {
	G4Exception("Badly defined material - number of components is not equal to number of weights or mass fractions!");
	exit(1);
      }
    }
    else {
      G4Exception("Badly defined material - need more information!");
      exit(1);
    }
  }
  if (verbose || DEBUG) G4cout << "size of material list: "<< material_list.size() << G4endl;


  //
  // set global magnetic field first
  //
  SetMagField(0.0); // necessary to set a global field; so chose zero

  
  //
  // compute magnetic rigidity brho
  // formula: B(Tesla)*rho(m) = p(GeV)/(0.299792458 * |charge(e)|)
  //
  // charge (in |e| units)
  G4double charge = BDSGlobals->GetParticleDefinition()->GetPDGCharge();
  // momentum (in GeV/c)
  G4double momentum = BDSGlobals->GetBeamMomentum();
  // rigidity (in T*m)
  G4double brho = ( (momentum/GeV) / (0.299792458 * charge));
  // rigidity (in Geant4 units)
  brho *= (tesla*m);
  if (verbose || DEBUG) G4cout << "Rigidity (Brho) : "<< fabs(brho)/(tesla*m) << " T*m"<<G4endl;


  //
  // beampipe default outer radius (if not overridden by "aper" option)
  //
  G4double bpRad=BDSGlobals->GetBeampipeRadius();
  if (verbose || DEBUG) G4cout<<"Default pipe outer radius= "<<bpRad/m<< "m"
			      << G4endl;


  // I suspect FeRad is planned to be offered as an option for the inner radius
  // of the iron in case it is different from the beampipe outer radius
  // Not done yet.
  G4double FeRad = bpRad;
  if (verbose || DEBUG) G4cout<<"Default magnet inner radius= "<<FeRad/m<< "m"
			      << G4endl;


  // magnetic field moments (depending on the magnet type)
  G4double bField;       // dipole (constant) field (G4 units)
  G4double bPrime;       // quadrupole field gradient dBy/dx (G4 units)
  G4double bDoublePrime; // sextupole field coefficient d^2 By/dx^2 (G4 units)
  G4double bTriplePrime; // octupole field coefficient d^3 By/dy^3 (G4 units)


  // stuff for rescaling due to synchrotron radiation, IGNORING
  G4double synch_factor = 1;


  //
  G4double samplerLength = 1.E-11 * m;


  //
  // convert the parsed element list to list of BDS elements
  //
  G4bool added_comp = false;
  if (verbose || DEBUG) G4cout << "parsing the beamline element list..."<< G4endl;
  for(it = beamline_list.begin();it!=beamline_list.end();it++)
    {
      added_comp = false;

      if((*it).type==_SAMPLER ) {
	if(DEBUG) G4cout << "---->adding Sampler,"
			 << " name= " << (*it).name
			 << G4endl;

	theBeamline.push_back(new BDSSampler( (*it).name,
					      samplerLength ) );
	bdsOutput.nSamplers++;
      
	added_comp=true;
      }

      if((*it).type==_CSAMPLER ) {

	// replace short cylinders with 1 meter cylinders
	if( (*it).l < 1.E-4 ) (*it).l = 1.0 ;

	if(DEBUG) G4cout << "---->adding CSampler,"
			 << " name= " << (*it).name
			 << " l= " << (*it).l << "m"
			 << " r= " << (*it).r << "m"
			 << G4endl;

	theBeamline.push_back(new BDSSamplerCylinder( (*it).name,
						      (*it).l * m,
						      (*it).r * m ) );
	bdsOutput.nSamplers++;
      
	added_comp=true;
      }

      if((*it).type==_DUMP ) {
        if(DEBUG) G4cout << "---->adding Dump,"
			 << " name= " << (*it).name
			 << G4endl;

        theBeamline.push_back(new BDSDump( (*it).name,
					   samplerLength ) );

	added_comp=true;
      }

      if((*it).type==_DRIFT ) {
	G4double aper = bpRad;
	if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;

	if((*it).l > 0) // skip zero-length elements
	  {
	    if(DEBUG) G4cout << "---->adding Drift,"
			     << " name= " << (*it).name
			     << " l= " << (*it).l << "m"
			     << " aper= " << aper/m << "m"
			     << G4endl;

	    theBeamline.push_back(new BDSDrift( (*it).name,
						(*it).l*m,
						aper ) );
	    added_comp=true;
	  } else {
	    G4cerr << "---->NOT adding Drift,"
		   << " name= " << (*it).name
		   << ", TOO SHORT LENGTH:"
		   << " l= " << (*it).l << "m"
		   << G4endl;
	  }
      }

      if((*it).type==_RF ) {
	G4double aper = bpRad;
	if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;

	if((*it).l > 0) // skip zero-length elements
	  {
	    if(DEBUG) G4cout << "---->adding RF,"
			     << " name= " << (*it).name
			     << " l= " << (*it).l << "m"
			     << " aper= " << aper/m << "m"
			     << " grad= " << (*it).gradient << "MV/m"
			     << G4endl;

	    theBeamline.push_back(new BDSRfCavity( (*it).name,
						   (*it).l * m,
						   aper,
						   (*it).gradient,
						   (*it).material ) );
	    added_comp=true;
	  } else {
	    G4cerr << "---->NOT adding RF,"
		   << " name= " << (*it).name
		   << ", TOO SHORT LENGTH:"
		   << " l= " << (*it).l << "m"
		   << G4endl;
	  }
      }
      
      if((*it).type==_SBEND ) {

	//
	// geometry
	//
	G4double aper = bpRad;
	if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;

	FeRad = aper;

	if( (*it).outR < aper/m)
	  {
	    G4cerr << (*it).name << ": outer radius smaller than aperture: "
		   << "aper= "<<aper/m<<"m outR= "<<(*it).outR<<"m"<<G4endl;
	    G4cerr << (*it).name << ": setting outer radius to default = "
		   << "aper+22*cm"<<G4endl;
	    (*it).outR = aper/m + 0.22;
	  }

	// arc length
	G4double length = (*it).l*m;

	//
	// magnetic field
	//

	// MAD conventions:
	// 1) a positive bend angle represents a bend to the right, i.e.
	// towards negative x values (even for negative charges??)
	// 2) a positive K1 = 1/|Brho| dBy/dx means horizontal focusing of
	// positive charges
	// CHECK SIGNS 
	//
	if((*it).B != 0){
	  // angle = arc length/radius of curvature = L/rho = (B*L)/(B*rho)
	  bField = (*it).B * tesla;
	  (*it).angle  = - bField * length / brho;
	}
	else{
	  (*it).angle *= -1;
	  // B = Brho/rho = Brho/(arc length/angle)
	  bField = - brho * (*it).angle / length;
	  (*it).B = bField/tesla;
	}

	// synch factor??

	// B' = dBy/dx = Brho * (1/Brho dBy/dx) = Brho * k1
	// Brho is already in G4 units, but k1 is not -> multiply k1 by m^-2
	bPrime = - brho * ((*it).k1 / (m*m)) * synch_factor;

	if( fabs((*it).angle) < 1.e-7 * rad) {
	  if(DEBUG)
	  G4cerr << "---->NOT adding Sbend,"
		 << " name= " << (*it).name
		 << ", TOO SMALL ANGLE"
		 << " angle= " << (*it).angle << "rad"
		 << ": REPLACED WITH Drift,"
		 << " l= " << length/m << "m"
		 << " aper= " << aper/m << "m"
		 << G4endl;

	  theBeamline.push_back(new BDSDrift( (*it).name,
					      length,
					      aper ) );
	}
	else {
	  if(DEBUG) G4cout << "---->adding Sbend "
			   << " name= " << (*it).name
			   << " l= " << length/m << "m"
			   << " angle= " << (*it).angle << "rad"
			   << " B= " << bField/tesla << "T"
			   << " B'= " << bPrime/(tesla/m) << "T/m"
			   << " tilt= " << (*it).tilt 
			   << " aper= " << aper/m << "m"
			   << " outR= " << (*it).outR << "m"
			   << " FeRad= " << FeRad/m << "m"
			   << " material= " << (*it).material
			   << G4endl;


	  // REPLACE WITH BDSRBend FOR THE MOMENT (UNTIL TOROIDAL GEOMETRY IS
	  // IMPLEMENTED!
	  length *= sin((*it).angle/2.0) / ((*it).angle/2.0);

	  theBeamline.push_back(new BDSRBend( (*it).name,
						   length,
						   aper,
						   FeRad,
						   bField,
						   (*it).angle,
						   (*it).outR*m,
						   (*it).tilt,
						   bPrime,
						   (*it).material ) );
/*	  
	  theBeamline.push_back(new BDSSectorBend( (*it).name,
						   length,
						   aper,
						   FeRad,
						   bField,
						   (*it).angle,
						   (*it).outR*m,
						   (*it).tilt,
						   bPrime,
						   (*it).material ) );
*/	  
	}
      
	added_comp=true;
      }

      if((*it).type==_RBEND ) {

	//
	// geometry
	//
        G4double aper = bpRad;
        if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;

        FeRad = aper;

        if( (*it).outR < aper/m)
          {
	    G4cerr << (*it).name << ": outer radius smaller than aperture: "
		   << "aper= "<<aper/m<<"m outR= "<<(*it).outR<<"m"<<G4endl;
	    G4cerr << (*it).name << ": setting outer radius to default = "
		   << "aper+22*cm"<<G4endl;
	    (*it).outR = aper/m + 0.22;
	  }

	G4double length = (*it).l*m; //geometrical length

	//
	// magnetic field
	//

	// CHECK SIGNS OF B, B', ANGLE
        if((*it).B != 0){
	  // angle = arc length/radius of curvature = L/rho = (B*L)/(B*rho)
          bField = (*it).B * tesla;
	  G4double rho = brho/bField;
	  //(*it).angle  = - bField * length / brho;
	  (*it).angle  = - 2.0*asin(length/2.0/rho);
        }
        else{
	  (*it).angle *= -1;
	  // arc length = radius*angle
	  //            = (geometrical length/(2.0*sin(angle/2))*angle
	  G4double arclength = 0.5*length * (*it).angle / sin((*it).angle/2.0);
	  // B = Brho/rho = Brho/(arc length/angle)
	  bField = - brho * (*it).angle / arclength;
	  (*it).B = bField/tesla;
        }

	// synch factor???

	// B' = dBy/dx = Brho * (1/Brho dBy/dx) = Brho * k1
	// Brho is already in G4 units, but k1 is not -> multiply k1 by m^-2
	bPrime = - brho * ((*it).k1 / (m*m)) * synch_factor;

        if( fabs((*it).angle) < 1.e-7 * rad) {
	  if(DEBUG)
	    G4cerr << "---->NOT adding Rbend,"
		 << " name= " << (*it).name
		 << ", TOO SMALL ANGLE"
		 << " angle= " << (*it).angle << "rad"
		 << ": REPLACED WITH Drift,"
		 << " l= " << length/m << "m"
		 << " aper= " << aper/m << "m"
		 << G4endl;
	  
          theBeamline.push_back(new BDSDrift( (*it).name,
					      length,
					      aper ) );
        }
        else {
	  if(DEBUG) G4cout << "---->adding Rbend,"
			   << " name= " << (*it).name 
			   << " l= " << length/m << "m"
			   << " angle= " << (*it).angle << "rad"
			   << " B= " << bField/tesla << "T"
			   << " B'= " << bPrime/(tesla/m) << "T/m"
			   << " tilt= " << (*it).tilt 
			   << " aper= " << aper/m << "m"
			   << " outR= " << (*it).outR << "m"
			   << " FeRad= " << FeRad/m << "m"
			   << " material= " << (*it).material
			   << G4endl;

          theBeamline.push_back(new BDSRBend( (*it).name,
					      length,
					      aper,
					      FeRad,
					      bField,
					      (*it).angle,
					      (*it).outR*m,
					      (*it).tilt,
					      bPrime,
					      (*it).material ) );
        }

	added_comp=true;
      }

      if((*it).type==_HKICK ) {

	//
	// geometry
	//
	G4double aper = bpRad;
        if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;

	FeRad = aper;

	if( (*it).outR < aper/m)
	  {
	    G4cerr << (*it).name << ": outer radius smaller than aperture: "
		   << "aper= "<<aper/m<<"m outR= "<<(*it).outR<<"m"<<G4endl;
	    G4cerr << (*it).name << ": setting outer radius to default = "
		   << "aper+22*cm"<<G4endl;
            (*it).outR = aper/m + 0.22;
	  }
	
	G4double length = (*it).l*m;

	//
	// magnetic field
	//
        if((*it).B != 0){
	  // angle = arc length/radius of curvature = L/rho = (B*L)/(B*rho)
          bField = (*it).B * tesla;
          (*it).angle  = -bField * length / brho;
        }
        else{
	  // B = Brho/rho = Brho/(arc length/angle)
          bField = - brho * (*it).angle / length;
	  (*it).B = bField/tesla;
        }

	// synch factor??
	
	// B' = dBy/dx = Brho * (1/Brho dBy/dx) = Brho * k1
	// Brho is already in G4 units, but k1 is not -> multiply k1 by m^-2
	bPrime = - brho * ((*it).k1 / (m*m)) * synch_factor;
	
	if( fabs((*it).angle) < 1.e-7 * rad ) {
	  G4cerr << "---->NOT adding Hkick,"
		 << " name= " << (*it).name
		 << ", TOO SMALL ANGLE"
		 << " angle= " << (*it).angle << "rad"
		 << ": REPLACED WITH Drift,"
		 << " l= " << length/m << "m"
		 << " aper= " << aper/m << "m"
		 << G4endl;

	  theBeamline.push_back(new BDSDrift( (*it).name,
					      length,
					      aper ) );
	} 
	else {
	  if(DEBUG) G4cout << "---->adding HKick,"
			   << " name= " << (*it).name 
			   << " l= " << length/m << "m"
			   << " angle= " << (*it).angle << "rad"
			   << " B= " << bField/tesla << "T"
			   << " B'= " << bPrime/(tesla/m) << "T/m"
			   << " tilt= " << (*it).tilt 
			   << " aper= " << aper/m << "m"
			   << " outR= " << (*it).outR << "m"
			   << " FeRad= " << FeRad/m << "m"
			   << " material= " << (*it).material
			   << G4endl;

	  theBeamline.push_back(new BDSKicker( (*it).name,
					       length,
					       aper,
					       FeRad,
					       bField,
					       (*it).angle,
					       (*it).outR*m,
					       (*it).tilt,
					       bPrime,
					       (*it).material ) );
	}
      
	added_comp=true;
      }

      if((*it).type==_VKICK ) {

	//
	// geometry
	//
        G4double aper = bpRad;
        if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;

	FeRad = aper;

	if( (*it).outR < aper/m)
	  {
	    G4cerr << (*it).name << ": outer radius smaller than aperture: "
		   << "aper= "<<aper/m<<"m outR= "<<(*it).outR<<"m"<<G4endl;
	    G4cerr << (*it).name << ": setting outer radius to default = "
		   << "aper+22*cm"<<G4endl;
            (*it).outR = aper/m + 0.22;
	  }

	G4double length = (*it).l*m;

	//
	// magnetic field
	//
        if((*it).B != 0){
	  // angle = arc length/radius of curvature = L/rho = (B*L)/(B*rho)
          bField = (*it).B * tesla;
          (*it).angle  = -bField * length / brho;
        }
        else{
	  // B = Brho/rho = Brho/(arc length/angle)
          bField = - brho * (*it).angle / length;
	  (*it).B = bField/tesla;
        }

	// synch factor???

	// B' = dBy/dx = Brho * (1/Brho dBy/dx) = Brho * k1
	// Brho is already in G4 units, but k1 is not -> multiply k1 by m^-2
	bPrime = - brho * ((*it).k1 / (m*m)) * synch_factor;

	if( fabs((*it).angle) < 1.e-7 * rad ) {
	  G4cerr << "---->NOT adding Vkick,"
		 << " name= " << (*it).name
		 << ", TOO SMALL ANGLE"
		 << " angle= " << (*it).angle << "rad"
		 << ": REPLACED WITH Drift,"
		 << " l= " << (*it).l << "m"
		 << " aper= " << aper/m << "m"
		 << G4endl;

	  theBeamline.push_back(new BDSDrift( (*it).name,
					      (*it).l * m,
					      aper ) );
	} 
	else {
	  if(DEBUG) G4cout << "---->adding VKick,"
			   << " name= " << (*it).name 
			   << " l= " << (*it).l << "m"
			   << " angle= " << (*it).angle << "rad"
			   << " B= " << bField/tesla << "T"
			   << " B'= " << bPrime/(tesla/m) << "T/m"
			   << " tilt= " << (*it).tilt 
			   << " aper= " << aper/m << "m"
			   << " outR= " << (*it).outR << "m"
			   << " FeRad= " << FeRad/m << "m"
			   << " material= " << (*it).material
			   << G4endl;

	  theBeamline.push_back(new BDSKicker( (*it).name,
					       (*it).l * m,
					       aper,
					       FeRad,
					       bField,
					       (*it).angle,
					       (*it).outR * m,
					       pi/2,
					       bPrime,
					       (*it).material ) );
	}
      
	added_comp=true;
      }


      if((*it).type==_QUAD ) {

	//
	// geometry
	//
	G4double aper = bpRad;
	if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;

	FeRad = aper;
	
	if( (*it).outR < aper/m)
	  {
	    G4cerr << (*it).name << ": outer radius smaller than aperture: "
		   << "aper= "<<aper/m<<"m outR= "<<(*it).outR<<"m"<<G4endl;
	    G4cerr << (*it).name << ": setting outer radius to default = "
		   << "aper+22*cm"<<G4endl;
	    (*it).outR = aper/m + 0.22;
	  }
	
	//
	// magnetic field
	//

	// B' = dBy/dx = Brho * (1/Brho dBy/dx) = Brho * k1
	// Brho is already in G4 units, but k1 is not -> multiply k1 by m^-2
	bPrime = - brho * ((*it).k1 / (m*m)) * synch_factor;

	if(DEBUG) G4cout << "---->adding Quadrupole,"
			 << " name= " << (*it).name
			 << " l= " << (*it).l << "m"
			 << " k1= " << (*it).k1 << "m^-2"
			 << " brho= " << fabs(brho)/(tesla*m) << "T*m"
			 << " B'= " << bPrime/(tesla/m) << "T/m"
			 << " aper= " << aper/m << "m"
			 << " outR= " << (*it).outR << "m"
			 << " FeRad= " << FeRad/m << "m"
			 << " material= " << (*it).material
			 << G4endl;
	
	theBeamline.push_back(new BDSQuadrupole( (*it).name,
						 (*it).l * m,
						 aper,
						 FeRad,
						 bPrime, 
						 (*it).tilt,
						 (*it).outR * m, 
						 (*it).material,
						 (*it).spec ) );
	
	added_comp=true;
      }
      
      if((*it).type==_SEXTUPOLE ) {

	//
	// geometry
	//
	G4double aper = bpRad;
        if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;

	FeRad = aper;

	if( (*it).outR < aper/m)
	  {
	    G4cerr << (*it).name << ": outer radius smaller than aperture: "
		   << "aper= "<<aper/m<<"m outR= "<<(*it).outR<<"m"<<G4endl;
	    G4cerr << (*it).name << ": setting outer radius to default = "
		   << "aper+22*cm"<<G4endl;
	    (*it).outR = aper/m + 0.22;
	  }

	//
	// magnetic field
	//

	// B'' = d^2By/dx^2 = Brho * (1/Brho d^2By/dx^2) = Brho * k2
	// brho is in Geant4 units, but k2 is not -> multiply k2 by m^-3
	bDoublePrime = - brho * ((*it).k2 / (m*m*m)) * synch_factor;

	if(DEBUG) G4cout << "---->adding Sextupole,"
			 << " name= " << (*it).name
			 << " l= " << (*it).l << "m"
			 << " k2= " << (*it).k2 << "m^-3"
			 << " brho= " << fabs(brho)/(tesla*m) << "T*m"
			 << " B''= " << bDoublePrime/(tesla/(m*m)) << "T/m^2"
			 << " aper= " << aper/m << "m"
			 << " outR= " << (*it).outR << "m"
			 << " FeRad= " << FeRad/m << "m"
			 << " material= " << (*it).material
			 << G4endl;

	theBeamline.push_back(new BDSSextupole( (*it).name,
						(*it).l * m,
						aper,
						FeRad,
						bDoublePrime,
						(*it).tilt,
						(*it).outR * m,
						(*it).material ) );
      
	added_comp=true;
      }

      if((*it).type==_OCTUPOLE ) {

	//
	// geometry
	//
	G4double aper = bpRad;
	if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;

	FeRad = aper;

	if( (*it).outR < aper/m)
	  {
	    G4cerr << (*it).name << ": outer radius smaller than aperture: "
		   << "aper= "<<aper/m<<"m outR= "<<(*it).outR<<"m"<<G4endl;
	    G4cerr << (*it).name << ": setting outer radius to default = "
		   << "aper+22*cm"<<G4endl;
            (*it).outR = aper/m + 0.22;
	  }

	//
	// magnetic field
	//

	// B''' = d^3By/dx^3 = Brho * (1/Brho d^3By/dx^3) = Brho * k3
	// brho is in Geant4 units, but k3 is not -> multiply k3 by m^-4
	bTriplePrime = - brho * ((*it).k3 / (m*m*m*m)) * synch_factor;

	if(DEBUG) G4cout << "---->adding Octupole,"
			 << " name= " << (*it).name
			 << " l= " << (*it).l << "m"
			 << " k3= " << (*it).k3 << "m^-4"
			 << " brho= " << fabs(brho)/(tesla*m) << "T*m"
			 << " B'''= " << bTriplePrime/(tesla/(m*m*m)) << "T/m^3"
			 << " aper= " << aper/m << "m"
			 << " outR= " << (*it).outR << "m"
			 << " FeRad= " << FeRad/m << "m"
			 << " material= " << (*it).material
			 << G4endl;

	theBeamline.push_back(new BDSOctupole( (*it).name,
					       (*it).l * m,
					       aper,
					       FeRad,
					       bTriplePrime,
					       (*it).tilt,
					       (*it).outR * m,
					       (*it).material ) );
      
	added_comp=true;
      }

      if((*it).type==_MULT ) {
	
	//
	// geometry
	//
	G4double aper = bpRad;
	if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;

	FeRad = aper;

        if( (*it).outR < aper/m)
          {
	    G4cerr << (*it).name << ": outer radius smaller than aperture: "
		   << "aper= "<<aper/m<<"m outR= "<<(*it).outR<<"m"<<G4endl;
	    G4cerr << (*it).name << ": setting outer radius to default = "
		   << "aper+22*cm"<<G4endl;
            (*it).outR = aper/m + 0.22;
          }

	if(DEBUG) G4cout << "---->adding Multipole,"
			 << " name= " << (*it).name
			 << " l= " << (*it).l << "m"
			 << " aper= " << aper/m << "m"
			 << " outR= " << (*it).outR << "m"
			 << " FeRad= " << FeRad/m << "m"
			 << " material= " << (*it).material
			 << G4endl;

	//
	// magnetic field
	//

	list<double>::iterator kit;
	
	if(DEBUG) G4cout << " knl={ ";
	for(kit=(it->knl).begin();kit!=(it->knl).end();kit++)
	  {
	    if(DEBUG) G4cout<<(*kit)<<", ";
	    (*kit) /= (*it).l; 
	  }
	if(DEBUG) G4cout << " }";
	
	if(DEBUG) G4cout << " ksl={ ";
	for(kit=(it->ksl).begin();kit!=(it->ksl).end();kit++)
	  {
	    if(DEBUG) G4cout<<(*kit)<<" ";
	    (*kit) /= (*it).l; 
	  }
	if(DEBUG) G4cout << " }" << G4endl;
	
	theBeamline.push_back(new BDSTMultipole( (*it).name,
						 (*it).l * m,
						 aper,
						 FeRad,
						 (*it).outR*m,
						 it->knl,
						 it->ksl,
						 (*it).material ) );

	added_comp=true;
      }
      
      if((*it).type==_ELEMENT ) {

	//
	// geometry
	//
        G4double aper = bpRad;
        if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;

/* Fix for element volume overlaps - do not set default outR!
	if( (*it).outR < aper/m)
	  {
	    G4cerr << (*it).name << ": outer radius smaller than aperture: "
		   << "aper= "<<aper/m<<"m outR= "<<(*it).outR<<"m"<<G4endl;
	    G4cerr << (*it).name << ": setting outer radius to default = "
		   << "aper+22*cm"<<G4endl;
	    (*it).outR = aper/m + 0.22;
	  }
*/
	if(DEBUG) G4cout << "---->adding Element,"
			 << " name= " << (*it).name
			 << " l= " << (*it).l << "m"
			 << " aper= " << aper/m << "m"
			 << " outR= " << (*it).outR << "m"
			 << G4endl;
	
	theBeamline.push_back(new BDSElement( (*it).name,
					      (*it).geometryFile,
					      (*it).bmapFile,
					      (*it).l * m,
					      aper,
					      (*it).outR*m ) );
      
	added_comp=true;
      }

      if((*it).type==_SOLENOID ) {

	//
	// geometry
	//
        G4double aper = bpRad;
        if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;

	FeRad = aper;

	if( (*it).outR < aper/m)
	  {
	    G4cerr << (*it).name << ": outer radius smaller than aperture: "
		   << "aper= "<<aper/m<<"m outR= "<<(*it).outR<<"m"<<G4endl;
	    G4cerr << (*it).name << ": setting outer radius to default = "
		   << "aper+22*cm"<<G4endl;
	    (*it).outR = aper/m + 0.22;
	  }

	//
	// magnetic field
	//
	// B = B/Brho * Brho = ks * Brho
	// brho is in Geant4 units, but ks is not -> multiply ks by m^-1
	G4double bField;
        if((*it).B != 0){
          bField = (*it).B * tesla;
          (*it).ks  = (bField/brho) / m;
        }
        else{
	  bField = ((*it).ks/m) * brho;
	  (*it).B = bField/tesla;
        }

	if(DEBUG) G4cout << "---->adding Solenoid,"
			 << " name= " << (*it).name
			 << " l= " << (*it).l << "m"
			 << " ks= " << (*it).ks << "m^-1"
			 << " brho= " << fabs(brho)/(tesla*m) << "T*m"
			 << " B= " << bField/tesla << "T"
			 << " aper= " << aper/m << "m"
			 << " outR= " << (*it).outR << "m"
			 << " FeRad= " << FeRad/m << "m"
			 << " material= " << (*it).material
			 << G4endl;
	
	theBeamline.push_back(new BDSSolenoid( (*it).name,
					       (*it).l * m,
					       aper,
					       FeRad,
					       bField,
					       (*it).outR*m,
					       (*it).material,
					       (*it).spec ) );
	
	added_comp=true;
      }
      
      if((*it).type==_ECOL ) {

	G4Material* theMaterial;
	if((*it).material != "")
	  theMaterial = theMaterials->GetMaterial( (*it).material );
	else
	  theMaterial = theMaterials->GetMaterial( "Graphite" );

	if(DEBUG) G4cout << "---->adding Ecol,"
			 << " name= " << (*it).name 
			 << " xaper= " << (*it).xsize <<"m"
			 << " yaper= " << (*it).ysize <<"m"
			 << " material= " << (*it).material
			 << G4endl;

	theBeamline.push_back(new BDSCollimator((*it).name,
						(*it).l * m,
						bpRad,
						(*it).xsize * m,
						(*it).ysize * m,
						_ECOL,
						theMaterial,
						(*it).outR*m) );
      
	added_comp=true;
      }

      if((*it).type==_RCOL ) {

	G4Material* theMaterial;
	if((*it).material != "")
	  theMaterial = theMaterials->GetMaterial( (*it).material );
	else
	  theMaterial = theMaterials->GetMaterial( "Graphite" );

	if(DEBUG) G4cout << "---->adding Rcol,"
			 << " name= " << (*it).name 
			 << " xaper= " << (*it).xsize <<"m"
			 << " yaper= " << (*it).ysize <<"m"
			 << " material= " << (*it).material
			 << G4endl;

	theBeamline.push_back(new BDSCollimator( (*it).name,
						 (*it).l * m,
						 bpRad,
						 (*it).xsize * m,
						 (*it).ysize * m,
						 _RCOL,
						 theMaterial,
						 (*it).outR*m) );
      
	added_comp=true;
      }

      if((*it).type==_LASER ) {
	if((*it).l == 0) (*it).l = 1e-8;
	
	if(DEBUG) G4cout << "---->adding Laser,"
			 << " name= "<< (*it).name
			 << " l=" << (*it).l/m<<"m"
			 << " lambda= " << (*it).waveLength/m << "m"
			 << " xSigma= " << (*it).xsize/m << "m"
			 << " ySigma= " << (*it).ysize/m << "m"
			 << " xdir= " << (*it).xdir
			 << " ydir= " << (*it).ydir
			 << " zdir= " << (*it).zdir
			 << G4endl;
	
	G4ThreeVector direction = 
	  G4ThreeVector((*it).xdir,(*it).ydir,(*it).zdir);
	G4ThreeVector position  = G4ThreeVector(0,0,0);
	
	theBeamline.push_back(new BDSLaserWire( (*it).name,
						(*it).l * m,
						(*it).waveLength * m,
						direction,
						position ) );
	
	added_comp=true;
      }

      if((*it).type==_TRANSFORM3D ) {
	
	if(DEBUG) G4cout << "---->adding Transform3d,"
			 << " name= " << (*it).name
			 << " xdir= " << (*it).xdir/m << "m"
			 << " ydir= " << (*it).ydir/m << "m"
			 << " zdir= " << (*it).zdir/m << "m"
			 << " phi= " << (*it).phi/rad << "rad"
			 << " theta= " << (*it).theta/rad << "rad"
			 << " psi= " << (*it).psi/rad << "rad"
			 << G4endl;
	
	theBeamline.push_back(new BDSTransform3D( (*it).name,
						  (*it).xdir *m,
						  (*it).ydir *m,
						  (*it).zdir *m,
						  (*it).phi *rad,
						  (*it).theta *rad,
						  (*it).psi *rad ) );
	
      
	added_comp=true;
      }

      if(added_comp)      //for BDSOutline
	{
	  list<BDSAcceleratorComponent*>::iterator curr = theBeamline.end();
	  curr--; //get last element added
      
	  (*curr)->SetK1((*it).k1);
	  (*curr)->SetK2((*it).k2);
	  (*curr)->SetK3((*it).k3);
	}
    }
  
  
  if (verbose || DEBUG) G4cout << "size of beamline element list: "<< beamline_list.size() << G4endl;
  
  
  //
  // construct the component list
  //

  if (verbose || DEBUG) G4cout << "now constructing geometry" << G4endl;
  
  list<BDSAcceleratorComponent*>::const_iterator iBeam;
  
  G4ThreeVector rtot = G4ThreeVector(0.,0.,0.);   // world dimension
  G4ThreeVector rlast = G4ThreeVector(0.,0.,0.);  // edge of last element coordinates
  G4ThreeVector rmin = G4ThreeVector(0.,0.,0.);
  G4ThreeVector rmax = G4ThreeVector(0.,0.,0.);

  G4ThreeVector localX = G4ThreeVector(1,0,0);  // local coordinate axis
  G4ThreeVector localY = G4ThreeVector(0,1,0);
  G4ThreeVector localZ = G4ThreeVector(0,0,1);

  G4double s_tot = 0; // position along the beamline

  // define geometry scope
  for(iBeam=theBeamline.begin();iBeam!=theBeamline.end();iBeam++)
    {

      if(DEBUG) G4cout << (*iBeam)->GetName() << "  "
		       << (*iBeam)->GetLength() << "  "
		       << (*iBeam)->GetAngle() << "  "
		       << G4endl;

      (*iBeam)->SetSPos(s_tot+(*iBeam)->GetArcLength()/2.0);

      // advance coordinates , but not for cylindrical sampler
      if(( (*iBeam)->GetType() != "sampler") || ( (*iBeam)->GetLength() <= samplerLength ) )
	{
	  s_tot+= (*iBeam)->GetArcLength();

	  G4double angle=(*iBeam)->GetAngle();
	  if(!angle && (*iBeam)->GetType()=="transform3d")
	    angle=(*iBeam)->GetPhi();
	  G4double theta=(*iBeam)->GetTheta();
	  G4double psi=(*iBeam)->GetPsi();

	  // define new coordinate system local frame	  
	  localX.rotate(psi,localZ);
	  localY.rotate(psi,localZ);
	  localZ.rotate(psi,localZ);

	  localX.rotate(angle,localY);
	  localY.rotate(angle,localY);
	  localZ.rotate(angle,localY);

	  localX.rotate(theta,localX);
	  localY.rotate(theta,localX);
	  localZ.rotate(theta,localX);

	  // advance the coordinate system translation
	  rtot += localZ * (*iBeam)->GetZLength();
	  if(DEBUG)
	    G4cout << (*iBeam)->GetType() << " " << rtot << G4endl;
	}

      if(rmax(0)<rtot(0)) rmax(0) = rtot(0);
      if(rmax(1)<rtot(1)) rmax(1) = rtot(1);
      if(rmax(2)<rtot(2)) rmax(2) = rtot(2);

      if(rmin(0)>rtot(0)) rmin(0) = rtot(0);
      if(rmin(1)>rtot(1)) rmin(1) = rtot(1);
      if(rmin(2)>rtot(2)) rmin(2) = rtot(2);
    }
    
  
  BDSGlobals->SetTotalS(s_tot);
  
  // -----------------------------------
  
  // determine the world size
  
  G4String LocalName="World";
  
  G4double WorldSizeX = 1 * ( (fabs(rmin(0)) + fabs(rmax(0)) ) + 5*BDSGlobals->GetComponentBoxSize());
  G4double WorldSizeY = 1 * ( (fabs(rmin(1)) + fabs(rmax(1)) ) + 5*BDSGlobals->GetComponentBoxSize());
  G4double WorldSizeZ = 1 * ( (fabs(rmin(2)) + fabs(rmax(2)) ) + 5*BDSGlobals->GetComponentBoxSize());
  
  //G4cout<<"world radius="<<WorldRadius/m<<" m"<<G4endl;
  if(verbose || DEBUG)
    {
      G4cout<<"minX="<<rmin(0)/m<<"m"<<" maxX="<<rmax(0)/m<<" m"<<G4endl;
      G4cout<<"minY="<<rmin(1)/m<<"m"<<" maxY="<<rmax(1)/m<<" m"<<G4endl;
      G4cout<<"minZ="<<rmin(2)/m<<"m"<<" maxZ="<<rmax(2)/m<<" m"<<G4endl;
      
      G4cout<<"box size="<<BDSGlobals->GetComponentBoxSize()/m<<" m"<<G4endl;
      G4cout<<"s_tot="<<s_tot/m<<" m"<<G4endl;
    }

  bdsOutput.zMax=s_tot;

  solidWorld = new G4Box("World", WorldSizeX, WorldSizeY, WorldSizeZ);
    
  logicWorld = new G4LogicalVolume(solidWorld,	       //its solid
				   theMaterials->GetMaterial("Vacuum"), //its material
				   "World");	       //its name
  
  logicWorld->SetVisAttributes (G4VisAttributes::Invisible);
  // set world volume visibility for debugging
  if (DEBUG) logicWorld->SetVisAttributes(new G4VisAttributes(true));

  // set default max step length (only for particles which have the
  // G4StepLimiter process enabled)
  G4UserLimits* WorldUserLimits =new G4UserLimits();
  WorldUserLimits->SetMaxAllowedStep(100*m);
  logicWorld->SetUserLimits(WorldUserLimits);


  G4cout<<"Charged Thresholdcut="<<BDSGlobals->GetThresholdCutCharged()/GeV<<" GeV"<<G4endl;
  G4cout<<"Photon Thresholdcut="<<BDSGlobals->GetThresholdCutPhotons()/GeV<<" GeV"<<G4endl;


  G4cout<<"Creating regions..."<<G4endl;
  
  G4Region* precisionRegion = new G4Region("precision");
   
  G4ProductionCuts* theProductionCuts = new G4ProductionCuts();
  
  if(BDSGlobals->GetProdCutPhotonsP()>0)
    theProductionCuts->SetProductionCut(BDSGlobals->GetProdCutPhotonsP(),"gamma");

  if(BDSGlobals->GetProdCutElectronsP()>0)
    theProductionCuts->SetProductionCut(BDSGlobals->GetProdCutElectronsP(),"e-");

  if(BDSGlobals->GetProdCutPositronsP()>0)
    theProductionCuts->SetProductionCut(BDSGlobals->GetProdCutPositronsP(),"e+");
  
  precisionRegion->SetProductionCuts(theProductionCuts);

  // world

  physiWorld = new G4PVPlacement(0,		// no rotation
  				 0,             // at (0,0,0)
                                 logicWorld,	// its logical volume
                                 LocalName,	// its name
                                 NULL,		// its mother  volume
                                 false,		// no boolean operation
                                 0);		// copy number


  // sensitive detectors

  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  G4bool use_graphics=true;
  G4ThreeVector TargetPos;

  // set default output formats:
  G4cout.precision(15);
  
  if(DEBUG) G4cout<<" total length="<<s_tot/m<<G4endl;
  
  // reset counters:
  for(iBeam=theBeamline.begin();iBeam!=theBeamline.end();iBeam++){

    // zero length components have no logical volumes
    if((*iBeam)->GetLength()!=0.)
      {
	G4String logVolName = (*iBeam)->GetMarkerLogicalVolume()->GetName();
	(*LogVolCount)[logVolName]=1;
      }
  }
  
  if (verbose || DEBUG) G4cout<<"starting placement procedure "<<G4endl;
  
  rtot = G4ThreeVector(0.,0.,0.);
  localX = G4ThreeVector(1.,0.,0.); 
  localY = G4ThreeVector(0.,1.,0.);
  localZ = G4ThreeVector(0.,0.,1.);
  
  G4RotationMatrix globalRotation;
  
  for(iBeam=theBeamline.begin();iBeam!=theBeamline.end();iBeam++)
    { 
      //(*iBeam)->SetZLower(rtot.z());
       
      G4double angle=(*iBeam)->GetAngle();
      G4double theta=(*iBeam)->GetTheta();
      G4double psi = (*iBeam)->GetPsi();
      G4double tilt = (*iBeam)->GetTilt();
      G4double phi = (*iBeam)->GetPhi();
      G4double length = (*iBeam)->GetZLength();

      if( (*iBeam)->GetType() == "transform3d")
	{

	  if(DEBUG) G4cout<<"transform3d : "<<phi<<" "<<theta<<" "<<psi<<G4endl;


	  rtot(0) += (*iBeam)->GetXOffset(); 
	  rtot(1) += (*iBeam)->GetYOffset(); 
	  rtot(2) += (*iBeam)->GetZOffset(); 

	  rlast(0) += (*iBeam)->GetXOffset();
	  rlast(1) += (*iBeam)->GetYOffset(); 
	  rlast(2) += (*iBeam)->GetZOffset(); 

	  globalRotation.rotate(psi,localZ);
	  localX.rotate(psi,localZ);
	  localY.rotate(psi,localZ);

	  globalRotation.rotate(phi,localY);
	  localX.rotate(phi,localY);
	  localZ.rotate(phi,localY);
	  
	  globalRotation.rotate(theta,localX);
	  localY.rotate(theta,localX);
	  localZ.rotate(theta,localX);
	  	  	  
	  continue;
	}
      
      // rotation matrix for component placement
      G4RotationMatrix *rotateComponent = new G4RotationMatrix;

      // tilted bends influence reference frame, otherwise just local tilt
      if((*iBeam)->GetType() == "sbend" || (*iBeam)->GetType() == "rbend" )
	{
	  globalRotation.rotate(tilt,localZ);
	  localX.rotate(tilt,localZ);
	  localY.rotate(tilt,localZ);
	}
      else 
	rotateComponent->rotateZ(tilt);
    
      // define center of bended elements from the previos coordinate frame
      G4ThreeVector zHalfAngle = localZ; 

      if((*iBeam)->GetType() == "sbend" || (*iBeam)->GetType() == "rbend")
	zHalfAngle.rotate(angle/2,localY);

      if(DEBUG)
	{
	  G4cout<<"zHalfAngle="<<zHalfAngle<<G4endl;
	  G4cout<<"localZ="<<localZ<<G4endl;
	  G4cout<<"localX="<<localX<<G4endl;
	  G4cout<<"localY="<<localY<<G4endl;
	  G4cout<<"rlast="<<rlast<<G4endl;
	}
      
      // target position
      TargetPos = rlast + zHalfAngle *  ( length/2 + BDSGlobals->GetLengthSafety()/2 ) ;

      if(DEBUG) G4cout<<"TargetPos="<<TargetPos<<G4endl;

      // advance the coordinates, but not for cylindrical samplers 
      if( ( ( (*iBeam)->GetType() != "sampler") || ( length <= samplerLength ) )  && ( (*iBeam)->GetType()!=_ELEMENT ))
	{
	  if(DEBUG) G4cout << (*iBeam)->GetType() << " "
			   << (*iBeam)->GetName() << " "
			   << G4endl;
	  rtot = rlast + zHalfAngle * ( length/2 + BDSGlobals->GetLengthSafety()/2 );
	  rlast = rtot + zHalfAngle * ( length/2 + BDSGlobals->GetLengthSafety()/2 );
	}

      // rotate to the previous reference frame
      rotateComponent->transform(globalRotation);

      rotateComponent->invert();

      // recompute global rotation
      // define new coordinate system local frame	  
 
      // bends transform the coordinate system
      if( (*iBeam)->GetType() == "sbend" || (*iBeam)->GetType() == "rbend")
	{
	  globalRotation.rotate(angle,localY);
	  localX.rotate(angle,localY);
	  localZ.rotate(angle,localY);
	  
	  
	  globalRotation.rotate(theta,localX);
	  localY.rotate(theta,localX);
	  localZ.rotate(theta,localX);
	  
	  // bend trapezoids defined along z-axis
	  rotateComponent->rotateY(-pi/2-angle/2);
	}


      //(*iBeam)->SetZUpper(rtot.z());
      
      // zero length components not placed (transform3d)
      if(length!=0.){
	
	G4LogicalVolume* LocalLogVol=(*iBeam)->GetMarkerLogicalVolume();
	
	G4String LogVolName=LocalLogVol->GetName();
	int nCopy=(*LogVolCount)[LogVolName]-1;
	(*LogVolCount)[LogVolName]++;


	// add the wolume to one of the regions
	if((*iBeam)->GetType() == _ELEMENT)
	  {
	    //G4cout<<"ENCOUNTERED ELEMENT : "<<_ELEMENT<<" ADDING TO PRECISION REg\n";
	    LocalLogVol->SetRegion(precisionRegion);
	    precisionRegion->AddRootLogicalVolume(LocalLogVol);
	  }

	
	// set up the sensitive volumes for energy counting:
	(*iBeam)->SetCopyNumber(nCopy);
	G4LogicalVolume* SensVol=(*iBeam)->GetSensitiveVolume();

	if(SensVol)
	  {
	    BDSEnergyCounterSD* ECounter=new BDSEnergyCounterSD(LogVolName);
	    (*iBeam)->SetBDSEnergyCounter(ECounter);
	    SensVol->SetSensitiveDetector(ECounter);
	    SDman->AddNewDetector(ECounter);
	    theECList->push_back(ECounter);
	  }

	vector<G4LogicalVolume*> MultipleSensVols = (*iBeam)->GetMultipleSensitiveVolumes();
	if((*iBeam)->GetType()!="sampler" && MultipleSensVols.size()>0)
	   {
	     for(G4int i=0; i<(G4int)MultipleSensVols.size(); i++)
	       {
		 BDSEnergyCounterSD* ECounter=
		   new BDSEnergyCounterSD(LogVolName+BDSGlobals->StringFromInt(i));
		 (*iBeam)->SetBDSEnergyCounter(ECounter);
		 MultipleSensVols.at(i)->SetSensitiveDetector(ECounter);
		 SDman->AddNewDetector(ECounter);
		 theECList->push_back(ECounter);	     
	       }
	   }
	
	if((*iBeam)->GetType()=="sampler") {
	  LocalName=(*iBeam)->GetName()+"_phys";
	  bdsOutput.SampName.push_back(LocalName + "_" + BDSGlobals->StringFromInt(nCopy+1));
	} else {
	  //it would be nice to set correctly names also for other elements...
	  //but need to count them!
	  LocalName=(*iBeam)->GetName()+"_phys";
	}

	/*
	//for torus sbend
	if((*iBeam)->GetType() == "sbend") {

	  G4double rho = length/fabs(angle);

	  G4RotationMatrix* Rot=new G4RotationMatrix();
	  Rot->rotateX(-pi/2 * rad);
	  //Rot->rotateZ(pi * rad);
	  //Rot->rotateZ(- ( pi/2 - fabs(angle)/2 ) * rad);
	  TargetPos -= zHalfAngle *  ( length/2 + BDSGlobals->GetLengthSafety()/2 ) ;
	  TargetPos+=G4ThreeVector(-rho,0,0);
	  //TargetPos=G4ThreeVector(0,0,rho);
	  //if(angle < 0)
	  //{
	  //  Rot->rotateZ(pi);
	  //  TargetPos=G4ThreeVector(rho,0,0);
	  //}
	  rotateComponent=Rot;
	}
	*/

	// Align Component - most cases does nothing. 
	// Currently only used for BDSElement	
	// For other elements stores global position and rotation,
	// needed for BDSOutline
	(*iBeam)->AlignComponent(//TargetPos,
				 rlast,
				 rotateComponent,
				 globalRotation,
				 rtot,
				 rlast,
				 localX,
				 localY,
				 localZ);


	G4PVPlacement* PhysiComponentPlace = 
	  new G4PVPlacement(
			    rotateComponent,  // its rotation
			    TargetPos,        // its position
			    LocalName,	      // its name
			    LocalLogVol,      // its logical volume
			    physiWorld,	      // its mother  volume
			    false,	      // no boolean operation
			    nCopy);	      // copy number
	
	(*iBeam)->PrepareField(PhysiComponentPlace);

	if(use_graphics)
	  {
	    (*iBeam)->GetVisAttributes()->SetVisibility(false);
	    //(*iBeam)->GetVisAttributes()->SetForceSolid(true);
	    (*iBeam)->GetMarkerLogicalVolume()->
	      SetVisAttributes((*iBeam)->GetVisAttributes());
	  }
	
      }
    }

  // construct tunnel
  for(it = beamline_list.begin();it!=beamline_list.end();it++)
    {
      
      if((*it).type==_TUNNEL ) {
	G4cout<<"BUILDING TUNNEL : "<<(*it).l<<"  "<<(*it).name<<G4endl;
	
	G4String gFormat="",  gFile="";
	G4String geometry = (*it).geometryFile;

	// get geometry format and file
	G4int pos = geometry.find(":");
	
	if(pos<0) { 
	  G4cerr<<"WARNING: invalid geometry reference format : "<<geometry<<endl;
	  gFormat="none";
	}
	
	else {
	  gFormat = geometry.substr(0,pos);
	  gFile = geometry.substr(pos+1,geometry.length() - pos); 
	}
	
	G4cout<<"placing components\n: geometry format - "<<gFormat<<G4endl<<
	  "file - "<<gFile<<G4endl;
	
	GGmadDriver *ggmad;
	
	if(gFormat=="gmad") {
	 
	  ggmad = new GGmadDriver(gFile);
	  ggmad->Construct(logicWorld);

	  
	} else  G4cerr<<"Tunnel won't be build! "<<endl;
      }
      
    }
  
  // free the parser list
  beamline_list.clear();

  // theProductionCuts->SetProductionCut(1.e-6*m,"gamma");
  // theProductionCuts->SetProductionCut(1.e-6*m,"e+");
  // theProductionCuts->SetProductionCut(1.e-6*m,"e-");
  // precisionRegion->SetProductionCuts(theProductionCuts);
  
  if(verbose || DEBUG) G4cout<<"end placement, size="<<theBeamline.size()<<G4endl;
  
  if(verbose || DEBUG) G4cout<<"Detector Construction done"<<G4endl; 

  if(DEBUG) G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  return physiWorld;
}

  
  
//=================================================================

void BDSDetectorConstruction::SetMagField(const G4double fieldValue)
{
 
  G4FieldManager* fieldMgr =
    G4TransportationManager::GetTransportationManager()->GetFieldManager();
  magField = new G4UniformMagField(G4ThreeVector(0.,fieldValue,0.));  
  fieldMgr->SetDetectorField(magField);
  fieldMgr->CreateChordFinder(magField);

}

//=================================================================
  
void BDSDetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructBDS(beamline_list));
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

  delete theMaterials;
}

//=================================================================
