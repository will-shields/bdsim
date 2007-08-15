//  
//   BDSIM, (C) 2001-2006 
//   
//   version 0.3
//  
//
//
//   Geometry construction
//
//
//   History
//
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
#include "BDSSectorBend.hh"
#include "BDSRBend.hh"
#include "BDSQuadrupole.hh"
#include "BDSDrift.hh"
#include "BDSRfCavity.hh"
#include "BDSSkewSextupole.hh"
#include "BDSSextupole.hh"
#include "BDSOctupole.hh"
#include "BDSDecapole.hh"
#include "BDSTMultipole.hh"
#include "BDSSampler.hh"

#include "BDSLaserWire.hh"
#include "BDSLWCalorimeter.hh"

#include "BDSMuSpoiler.hh"
#include "BDSTransform3D.hh"
#include "BDSElement.hh"
#include "BDSSamplerCylinder.hh"
#include "BDSComponentOffset.hh"
#include "BDSCollimator.hh"
#include "BDSDump.hh"

// output interface
#include "BDSOutput.hh"


//#include "BDSMultipoleOuterMagField.hh"
#include "G4MagneticField.hh"

// GMAD interface
#include "parser/gmad.h"
#include "ggmad.hh"

using namespace std;

//extern void GetMaterial(G4Material *&theMaterial, G4String material); //from ggmad

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
G4ThreeVector ZeroVec(0,0,0);

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

  aMaterial=theMaterials->GetMaterial("Graphite"); // default collimator material

  if(DEBUG) G4cout<<"-->starting BDS construction \n"<<G4endl;

  return ConstructBDS(beamline_list);

}


G4VPhysicalVolume* BDSDetectorConstruction::ConstructBDS(list<struct Element>& beamline_list)
{
  
  // set default output formats:
  G4cout.precision(10);
  
  
  // convert the parsed element list to list of BDS elemenst
  
  list<struct Element>::iterator it;
  
  for(it = atom_list.begin();it!=atom_list.end();it++)
  {
    theMaterials->AddElement((*it).name,(*it).symbol,(*it).Z,(*it).A);
  }

  for(it = material_list.begin();it!=material_list.end();it++)
  {
    if((*it).Z != 0)
      theMaterials->AddMaterial((*it).name,(*it).Z,(*it).A,(*it).density);

    if((*it).components.size() != 0){

      if((*it).componentsWeights.size()==(*it).components.size())
	theMaterials->AddMaterial((*it).name,(*it).density,kStateSolid,(*it).temper,
		1*atmosphere,(*it).components,(*it).componentsWeights);

      else if((*it).componentsWeights.size()==(*it).components.size()) 
        theMaterials->AddMaterial((*it).name,(*it).density,kStateSolid,(*it).temper,
                1*atmosphere,(*it).components,(*it).componentsFractions);

      else {G4Exception("Badly defined material - number of components is not equal to number of weights or mass fractions!"); exit(1);}
    }
    else {G4Exception("Badly defined material - need more information!"); exit(1);}
  }

  //BDSBeamline theBeamline;
  
  // set global magnetic field first
  SetMagField(0.0); // necessary to set a global field; so chose zero
  
  
  // radius of curvature (!!!!! valid for electrons only???!!!!)
  G4double P0 = BDSGlobals->GetBeamTotalEnergy();
  
  // magnetic rigidity
  G4double brho=
    sqrt(pow(P0,2)- pow(electron_mass_c2,2))/(0.299792458 * (GeV/(tesla*m)));

  G4double bField;

  // beampipe radius
  G4double bpRad=BDSGlobals->GetBeampipeRadius();
  // ???
  G4double FeRad = bpRad;
  
  // quadrupole field gradient
  G4double bPrime;
  // sectupole field coef.
  G4double bDoublePrime;  // for sextupole field
  G4double bTriplePrime;  // for octupole field
  
  // stuff for rescaling due to synchrotron radiation, IGNORING
  G4double synch_factor = 1;
  G4double samplerLength = 1.E-11 * m;

  G4Material* theMaterial;

  if(DEBUG) G4cout<<"bpRad , m : "<<bpRad / m <<G4endl;
  G4bool added_comp = false;
  for(it = beamline_list.begin();it!=beamline_list.end();it++)
    {
      added_comp = false;
     
      if(DEBUG) G4cout<<(*it).name<<"    "<<typestr((*it).type)<<"  "<<(*it).l<<G4endl;


         
      if((*it).type==_SAMPLER ) {
	if(DEBUG) G4cout<<"---->adding Sampler "<<G4String( (*it).name )<<G4endl;
	
	theBeamline.push_back(new BDSSampler( G4String( (*it).name ) , samplerLength ) );
	bdsOutput.nSamplers++;
      
      added_comp=true;
      }

      if((*it).type==_CSAMPLER ) {
	if(DEBUG) G4cout<<"---->adding CSampler "<<G4String( (*it).name )<<G4endl;
	
	if( (*it).l < 1.E-4 * m ) (*it).l = 1.0 * m; // replace short cylinders with 1 meter cylinders

	theBeamline.push_back(new BDSSamplerCylinder( G4String( (*it).name ) , (*it).l * m , (*it).r * m) );
	bdsOutput.nSamplers++;
      
      added_comp=true;
      }

      if((*it).type==_DUMP ) {
//	G4Exception("DUMP command not yet supported. Please remove this command from your input file.");
        if(DEBUG) G4cout<<"---->adding DUMP "<<G4String( (*it).name )<<G4endl;

        theBeamline.push_back(new BDSDump( G4String( (*it).name ) , samplerLength ) );
        //bdsOutput.nSamplers++;

      added_comp=true;
      }

      if((*it).type==_DRIFT ) {
	G4double aper = bpRad;
	if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;

	if(DEBUG) G4cout<<"---->adding Drift "<<G4String( (*it).name )<<" l="<<(*it).l/m<<
		    " aper="<<aper/m<<G4endl;
	if((*it).l > 0) // skip zero-length drift-defined elements
	  theBeamline.push_back(new BDSDrift(G4String((*it).name),(*it).l * m,aper));
      
      added_comp=true;
      }

       if((*it).type==_RF ) {
	G4double aper = bpRad;
	if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;

	if(DEBUG) G4cout<<"---->adding RF "<<G4String( (*it).name )<<" l="<<(*it).l/m<<
	  " aper="<<aper/m<<"grad="<<(*it).gradient / megavolt<<G4endl;
	if((*it).l > 0) // skip zero-length elements
	  theBeamline.push_back(new BDSRfCavity(G4String((*it).name),(*it).l * m,aper,
						(*it).gradient,(*it).material));
      
      added_comp=true;
      }
      
      if((*it).type==_SBEND ) {

	G4double aper = bpRad;
	if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;
	FeRad = aper;

	(*it).angle*=-1;

	// get the particle charge
        double charge = BDSGlobals->GetParticleDefinition()->GetPDGCharge();
	
	if((*it).B != 0){
	  bField = (*it).B * tesla;
	  (*it).angle  = -bField * (*it).l / ( brho * charge ) / tesla;
	}
	else{
	  bField = -charge * brho * (*it).angle / (*it).l * tesla;
	}
	bPrime = -charge * brho * (*it).k1 * tesla / m;

	if( (*it).outR < (*it).aper/m)
	  {
	      G4cerr << (*it).name << " has outer radius smaller than aperture!"<<G4endl;
	      G4cerr << "aper= "<<(*it).aper/m<<"m outR= "<<(*it).outR<<"m"<<G4endl;
	      G4cerr << "Setting to default - 22*cm"<<G4endl;
	      (*it).outR = aper/m + 0.22;
	  }


	if(DEBUG) G4cout<<"---->adding Sbend "<<G4String( (*it).name )<<"  l= "<<(*it).l<<
	  " angle="<<(*it).angle<<" tilt="<<(*it).tilt<< " bpRad="<<aper << " FeRad="<<FeRad<<G4endl;
	
	if( fabs((*it).angle) < 1.e-7 * rad ) {
	  theBeamline.push_back(new BDSDrift(G4String((*it).name),(*it).l * m,aper));
	} 
	else {
	  theBeamline.push_back(new BDSSectorBend((*it).name,(*it).l * m,aper,FeRad,bField,
			(*it).angle,(*it).outR*m,(*it).tilt,bPrime,(*it).material));
	}
      
      added_comp=true;
      }

      if((*it).type==_RBEND ) {

        G4double aper = bpRad;
        if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;
        FeRad = aper;

        (*it).angle*=-1;

        // get the particle charge
        double charge = BDSGlobals->GetParticleDefinition()->GetPDGCharge();
        
        if((*it).B != 0){
          bField = (*it).B * tesla;
          (*it).angle  = -bField * (*it).l / ( brho * charge ) / tesla;
        }
        else{
          bField = -charge * brho * (*it).angle / (*it).l * tesla;
        }
        bPrime = -charge * brho * (*it).k1 * tesla / m;

        if( (*it).outR < (*it).aper/m)
          {
              G4cerr << (*it).name << " has outer radius smaller than aperture!"<<G4endl;
              G4cerr << "aper= "<<(*it).aper/m<<"m outR= "<<(*it).outR<<"m"<<G4endl;
              G4cerr << "Setting to default - 22*cm"<<G4endl;
              (*it).outR = aper/m + 0.22;
         }


        if(DEBUG) G4cout<<"---->adding Rbend "<<G4String( (*it).name )<<"  l= "<<(*it).l<<
          " angle="<<(*it).angle<<" tilt="<<(*it).tilt<< " bpRad="<<aper << " FeRad="<<FeRad<<G4endl;

        if( fabs((*it).angle) < 1.e-7 * rad ) {
          theBeamline.push_back(new BDSDrift(G4String((*it).name),(*it).l * m,aper));
        }
        else {
          theBeamline.push_back(new BDSRBend((*it).name,(*it).l * m,aper,FeRad,bField,
                         (*it).angle,(*it).outR*m,(*it).tilt,bPrime,(*it).material));
        }

      added_comp=true;
      }

      if((*it).type==_HKICK ) {

	G4double aper = bpRad;
        if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;
	FeRad = aper;

	(*it).angle*=-1;

	// get the particle charge
        double charge = BDSGlobals->GetParticleDefinition()->GetPDGCharge();		

        if((*it).B != 0){
          bField = (*it).B * tesla;
          (*it).angle  = -bField * (*it).l / ( brho * charge ) / tesla;
        }
        else{
          bField = -charge * brho * (*it).angle / (*it).l * tesla;
        }
	bPrime = -charge * brho * (*it).k1 * tesla / m * synch_factor;
	
	if( (*it).outR < aper/m)
	  {
	    G4cerr << (*it).name << " has outer radius smaller than aperture!"<<G4endl;
	    G4cerr << "aper= "<<aper/m<<"m outR= "<<(*it).outR<<"m"<<G4endl;
	    G4cerr << "Setting to default - 22*cm"<<G4endl;
            (*it).outR = aper/m + 0.22;
	  }
	
	if(DEBUG) G4cout<<"---->adding hkick "<<G4String( (*it).name )<<"  l= "<<(*it).l<<
		    " angle="<<(*it).angle<<" tilt="<<(*it).tilt<<G4endl;
	
	if( fabs((*it).angle) < 1.e-7 * rad ) {
	  theBeamline.push_back(new BDSDrift(G4String((*it).name),(*it).l * m,bpRad));
	} 
	else {
	  theBeamline.push_back(new BDSRBend( (*it).name,(*it).l * m,aper,FeRad,bField,
					      (*it).angle,(*it).outR*m,(*it).tilt,
						bPrime, (*it).material));
	}
      
      added_comp=true;
      }

      if((*it).type==_VKICK ) {

        G4double aper = bpRad;
        if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;
	FeRad = aper;

	(*it).angle*=-1;

	// get the particle charge
        double charge = BDSGlobals->GetParticleDefinition()->GetPDGCharge();		

        if((*it).B != 0){
          bField = (*it).B * tesla;
          (*it).angle  = -bField * (*it).l / ( brho * charge ) / tesla;
        }
        else{
          bField = -charge * brho * (*it).angle / (*it).l * tesla;
        }
	bPrime = -charge * brho * (*it).k1 * tesla / m * synch_factor;

	if( (*it).outR < aper/m)
	  {
	    G4cerr << (*it).name << " has outer radius smaller than aperture!"<<G4endl;
	    G4cerr << "aper= "<<aper/m<<"m outR= "<<(*it).outR<<"m"<<G4endl;
	    G4cerr << "Setting to default - 22*cm"<<G4endl;
            (*it).outR = aper/m + 0.22;
	  }

	if(DEBUG) G4cout<<"---->adding vkick "<<G4String( (*it).name )<<"  l= "<<(*it).l<<
		    " angle="<<(*it).angle<<" tilt="<<(*it).tilt<<G4endl;
	
	if( fabs((*it).angle) < 1.e-7 * rad ) {
	  theBeamline.push_back(new BDSDrift(G4String((*it).name),(*it).l * m,bpRad));
	} 
	else {
	  theBeamline.push_back(new BDSRBend( (*it).name,(*it).l * m,aper,FeRad,bField,
					      (*it).angle,(*it).outR * m,pi/2,
						bPrime,(*it).material));
	}
      
      added_comp=true;
      }


      if((*it).type==_QUAD ) {

	// get the particle charge
        double charge = BDSGlobals->GetParticleDefinition()->GetPDGCharge();	
	
	//bPrime = brho * (*it).k1 / (*it).l * tesla  * synch_factor;
	bPrime = -charge * brho * (*it).k1 * tesla / m;

	G4double aper = bpRad;
	if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;
	FeRad = aper;
	
	if( (*it).outR < aper/m)
	  {
	    G4cerr << (*it).name << " has outer radius smaller than aperture!"<<G4endl;
	    G4cerr << "aper= "<<aper/m<<"mm outR= "<<(*it).outR<<"mm"<<G4endl;
	    G4cerr << "Setting to default - 22*cm"<<G4endl;
	    (*it).outR = aper/m + 0.22;
	  }
	
	if(DEBUG) { G4cout<<"---->adding Quad, "<<G4String( (*it).name )<<
	    " k1 ="<<(*it).k1<<" b' ="<<bPrime<<" brho = "<<brho<< " aper="<<
	    aper/m<<" outR="<<(*it).outR<<" spec="<<(*it).spec<<G4endl; }
	
	theBeamline.push_back(
			      new BDSQuadrupole(G4String((*it).name),
						(*it).l * m, aper,FeRad, bPrime, 
						(*it).tilt,(*it).outR * m, 
						(*it).material,(*it).spec));
	
	added_comp=true;
      }
      
      if((*it).type==_SEXTUPOLE ) {

	// get the particle charge
        double charge = BDSGlobals->GetParticleDefinition()->GetPDGCharge();	

	//bDoublePrime = brho * (*it).k2 / (*it).l * tesla / (m*m) * synch_factor;
	bDoublePrime = -charge * brho * (*it).k2 * tesla / (m*m) * synch_factor;

	G4double aper = bpRad;
        if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;
	FeRad = aper;

	if( (*it).outR < aper/m)
	  {
	    G4cerr << (*it).name << " has outer radius smaller than aperture!"<<G4endl;
	    G4cerr << "aper= "<<aper/m<<"m outR= "<<(*it).outR<<"m"<<G4endl;
	    G4cerr << "Setting to default - 22*cm"<<G4endl;
	    (*it).outR = aper/m + 0.22;
	  }
	
	if(DEBUG) { G4cout<<"---->adding Sextupole, "<<G4String( (*it).name )<<
	    " k1 ="<<(*it).k2<<" b'' ="<<bDoublePrime<<" brho = "<<brho<<" aper="<<aper/m<<G4endl;}
	theBeamline.push_back(new BDSSextupole(G4String((*it).name),(*it).l * m,aper,
			FeRad,bDoublePrime,(*it).tilt,(*it).outR * m, (*it).material));
      
      added_comp=true;
      }

      if((*it).type==_OCTUPOLE ) {

	double charge = BDSGlobals->GetParticleDefinition()->GetPDGCharge();	

	//bDoublePrime = brho * (*it).k2 / (*it).l * tesla / (m*m) * synch_factor;
	//bTriplePrime = brho * (*it).k3 / (*it).l * tesla / (m*m*m) * synch_factor;
	bDoublePrime = -charge * brho * (*it).k2 * tesla / (m*m) * synch_factor;
	bTriplePrime = -charge * brho * (*it).k3 * tesla / (m*m*m) * synch_factor;

	G4double aper = bpRad;
	if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;
	FeRad = aper;

	if( (*it).outR < aper/m)
	  {
	    G4cerr << (*it).name << " has outer radius smaller than aperture!"<<G4endl;
	    G4cerr << "aper= "<<aper/m<<"m outR= "<<(*it).outR<<"m"<<G4endl;
	    G4cerr << "Setting to default - 22*cm"<<G4endl;
            (*it).outR = aper/m + 0.22;
	  }

	if(DEBUG) { G4cout<<"---->adding octupole, "<<G4String( (*it).name )<<
		      " k3 ="<<(*it).k3<<" b''' ="<<bTriplePrime<<" brho = "<<brho<<" aper="<<aper/m<<G4endl;}
	theBeamline.push_back(new BDSOctupole(G4String((*it).name),(*it).l * m,aper,
			FeRad,bTriplePrime,(*it).tilt,(*it).outR * m,(*it).material));
      
      added_comp=true;
      }
      if((*it).type==_MULT ) {
	
	G4double aper = bpRad;
	if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;
	FeRad = aper;

        if( (*it).outR < aper/m)
          {
            G4cerr << (*it).name << " has outer radius smaller than aperture!"<<G4endl;
            G4cerr << "aper= "<<aper/m<<"m outR= "<<(*it).outR<<"m"<<G4endl;
            G4cerr << "Setting to default - 22*cm"<<G4endl;
            (*it).outR = aper/m + 0.22;
          }

	if(DEBUG) { G4cout<<"---->adding multipole, "<<G4String( (*it).name )<<" aper="<<aper/m<<G4endl;}
	
	
	if(DEBUG) G4cout<<"knl=";
	list<double>::iterator kit;
	for(kit=(it->knl).begin();kit!=(it->knl).end();kit++)
	  {
	    if(DEBUG) G4cout<<(*kit)<<" ";
	    (*kit) /= (*it).l; 
	  }
	if(DEBUG) G4cout<<G4endl;
	
	if(DEBUG) G4cout<<"ksl=";
	
	for(kit=(it->ksl).begin();kit!=(it->ksl).end();kit++)
	  {
	    if(DEBUG) G4cout<<(*kit)<<" ";
	    (*kit) /= (*it).l; 
	  }
	if(DEBUG) G4cout<<G4endl;
	
	theBeamline.push_back(new BDSTMultipole(G4String((*it).name),(*it).l * m,aper,
			FeRad, (*it).outR*m, it->knl,it->ksl,(*it).material));
      }
      
      
      if((*it).type==_ELEMENT ) {
	
	if(DEBUG) { G4cout<<"---->adding Element, "<<G4String( (*it).name )<<
	    " l ="<<(*it).l<<G4endl;}
	
        G4double aper = bpRad;
        if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;

	if( (*it).outR < aper/m)
	  {
	    G4cerr << (*it).name << " has outer radius smaller than aperture!"<<G4endl;
	    G4cerr << "aper= "<<aper/m<<"m outR= "<<(*it).outR<<"m"<<G4endl;
	    G4cerr << "Setting to default - 22*cm"<<G4endl;
	    (*it).outR = aper/m + 0.22;
	  }
	
	theBeamline.push_back(new BDSElement( G4String((*it).name) , G4String((*it).geometryFile), 
					      G4String((*it).bmapFile), (*it).l * m, aper, (*it).outR * m) );
      
      added_comp=true;
      }

      if((*it).type==_SOLENOID ) {
	
	if(DEBUG) { G4cout<<"---->adding Solenoid, "<<G4String( (*it).name )<<
		      " l ="<<(*it).l<<G4endl;}
	
        G4double aper = bpRad;
        if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;
	
	//theBeamline.push_back(new BDSElement( G4String((*it).name) , G4String((*it).geometryFile), 
	//			      G4String((*it).bmapFile), (*it).l * m, aper ) );
      
      added_comp=true;
      }
      
      if((*it).type==_ECOL ) {
	theMaterial = aMaterial;
	if((*it).material != "")
	  theMaterial = theMaterials->GetMaterial( (*it).material );

        G4double aper = bpRad;
        if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;
	if( (*it).outR < aper/m)
	  {
	    G4cerr << (*it).name << " has outer radius smaller than aperture!"<<G4endl;
	    G4cerr << "aper= "<<aper/m<<"m outR= "<<(*it).outR<<"m"<<G4endl;
	    G4cerr << "Setting to default - 22*cm"<<G4endl;
	    (*it).outR = aper/m + 0.22;
	  }
	
	if(DEBUG) { G4cout<<"---->adding Ecol, "<<G4String( (*it).name )<<G4endl<<
		      "xaper="<<(*it).xsize<<"m, yaper="<<(*it).ysize<<
		      "m, material="<<(*it).material<<G4endl;}

	
	if(DEBUG) G4cout<<"retrieved material :"<<aMaterial->GetName()<<G4endl;

	theBeamline.push_back(new BDSCollimator(G4String((*it).name),(*it).l * m,bpRad,
						(*it).xsize * m,(*it).ysize * m,_ECOL,theMaterial, (*it).outR*m) );
      
      added_comp=true;
      }
      if((*it).type==_RCOL ) {
	theMaterial = aMaterial;
	if((*it).material != "")
	  theMaterial = theMaterials->GetMaterial( (*it).material );

        G4double aper = bpRad;
        if( (*it).aper > 1.e-10*m ) aper = (*it).aper * m;
	if( (*it).outR < aper/m)
	  {
	    G4cerr << (*it).name << " has outer radius smaller than aperture!"<<G4endl;
	    G4cerr << "aper= "<<aper/m<<"m outR= "<<(*it).outR<<"m"<<G4endl;
	    G4cerr << "Setting to default - 22*cm"<<G4endl;
	    (*it).outR = aper/m + 0.22;
	  }


	if(DEBUG) { G4cout<<"---->adding Rcol, "<<G4String( (*it).name )<<G4endl<<
		      "xaper="<<(*it).xsize<<"m, yaper="<<(*it).ysize<<
		      "m, material="<<(*it).material<<G4endl;}

	if(DEBUG) G4cout<<"retrieved material :"<<aMaterial->GetName()<<G4endl;

	theBeamline.push_back(new BDSCollimator(G4String((*it).name),(*it).l * m,bpRad,
						(*it).xsize * m,(*it).ysize * m,_RCOL,theMaterial, (*it).outR*m) );
      
      added_comp=true;
      }
      if((*it).type==_LASER ) {
	if((*it).l == 0) (*it).l = 1e-8;
	
	if(DEBUG) { G4cout<<"---->adding Laser, "<<G4String( (*it).name )<<G4endl<<
		      "l="<<(*it).l/m<<"m, lambda="<<(*it).waveLength / m<<
		      "m, xSigma="<<(*it).xsize/m<<" ySigma="<<(*it).ysize<<
		      "m, xdir="<<(*it).xdir/m<<" ydir="<<(*it).ydir<<"m, zdir="<<(*it).zdir/m<<
		      G4endl; }
	
	//G4ThreeVector direction = G4ThreeVector(1,0,0);
	G4ThreeVector direction = 
	  G4ThreeVector((*it).xdir,(*it).ydir,(*it).zdir);
	G4ThreeVector position  = G4ThreeVector(0,0,0);
	
	theBeamline.push_back(new BDSLaserWire(G4String((*it).name),(*it).l * m,(*it).waveLength * m,
					       direction, position) );
	
	
	
      added_comp=true;
      }
      if((*it).type==_TRANSFORM3D ) {
	
	if(DEBUG) { G4cout<<"---->adding Transform3d, "<<G4String( (*it).name )<<G4endl<<
		      " xdir="<<(*it).xdir/m<<"m ydir="<<(*it).ydir<<"m, zdir="<<(*it).zdir/m<<
		      "m, phi="<<(*it).phi/rad<<" theta="<<(*it).theta/rad<<" psi="<<(*it).psi/rad<<
		      G4endl; }
	
	theBeamline.push_back(new BDSTransform3D(G4String((*it).name),(*it).xdir *m,(*it).ydir *m,
						 (*it).zdir *m, (*it).phi * rad, (*it).theta*rad,(*it).psi*rad ));
	
      
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
  
  
  if(DEBUG) G4cout<<"size of parser list: "<< beamline_list.size() << G4endl;;

  
  
  
  // construct the component list
  
  if(DEBUG) G4cout<<"now constructing geometry"<<G4endl;
  
  list<BDSAcceleratorComponent*>::const_iterator iBeam;
  
  
  G4ThreeVector rtot = G4ThreeVector(0.,0.,0.);  //world dimension
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

      if(DEBUG) G4cout<<(*iBeam)->GetName()<<"  "<<(*iBeam)->GetLength()<<
		  "  "<<(*iBeam)->GetAngle()<<G4endl;

      (*iBeam)->SetSPos(s_tot+(*iBeam)->GetLength()/2);
      // advance coordinates , but not for cylindrical sampler
      if(( (*iBeam)->GetType() != "sampler") || ( (*iBeam)->GetLength() <= samplerLength ) )
	{
	  s_tot+= (*iBeam)->GetLength();

	  rtot += localZ * (*iBeam)->GetLength()/2;

	  G4double angle=(*iBeam)->GetAngle();
	  G4double theta=(*iBeam)->GetTheta();

	  // define new coordinate system local frame	  
	  localX.rotate(angle,localY);
	  localY.rotate(angle,localY);
	  localZ.rotate(angle,localY);

	  localX.rotate(theta,localX);
	  localY.rotate(theta,localX);
	  localZ.rotate(theta,localX);

	  // advance the coordinate system translation
	  rtot+= localZ *  (*iBeam)->GetLength();

	}

      if(rmax(0)>rtot(0)) rmax(0) = rtot(0);
      if(rmax(1)>rtot(1)) rmax(1) = rtot(1);
      if(rmax(2)>rtot(2)) rmax(2) = rtot(2);

      if(rmax(0)<rtot(0)) rmin(0) = rtot(0);
      if(rmin(1)<rtot(1)) rmin(1) = rtot(1);
      if(rmin(2)<rtot(2)) rmin(2) = rtot(2);
    }
    
  
  BDSGlobals->SetTotalS(s_tot);
  
  // -----------------------------------
  
  // determine the world size
  
  G4String LocalName="World";
  
  G4double WorldSizeX = 1 * ( (fabs(rmin(0)) + fabs(rmax(0)) ) + 5*BDSGlobals->GetComponentBoxSize());
  G4double WorldSizeY = 1 * ( (fabs(rmin(1)) + fabs(rmax(1)) ) + 5*BDSGlobals->GetComponentBoxSize());
  G4double WorldSizeZ = 1 * ( (fabs(rmin(2)) + fabs(rmax(2)) ) + 5*BDSGlobals->GetComponentBoxSize());
  
  //G4cout<<"world radius="<<WorldRadius/m<<" m"<<G4endl;
  if(DEBUG)
    {
      G4cout<<"minX="<<rmin(0)/m<<"m"<<" maxX="<<rmax(0)/m<<" m"<<G4endl;
      G4cout<<"minY="<<rmin(1)/m<<"m"<<" maxY="<<rmax(1)/m<<" m"<<G4endl;
      G4cout<<"minZ="<<rmin(2)/m<<"m"<<" maxZ="<<rmax(2)/m<<" m"<<G4endl;
      
      G4cout<<"box size="<<BDSGlobals->GetComponentBoxSize()/m<<" m"<<G4endl;
      G4cout<<"s_tot="<<s_tot/m<<" m"<<G4endl;
    }

  bdsOutput.zMax=s_tot;

  solidWorld = new G4Box("World",WorldSizeX,WorldSizeY,
			   WorldSizeZ);
    
  logicWorld = new G4LogicalVolume(solidWorld,	       //its solid
				   // theMaterials->GetMaterial("Air"), //its material
				   theMaterials->GetMaterial("Vacuum"), //its material
				   "World");	       //its name
  
  logicWorld->SetVisAttributes (G4VisAttributes::Invisible);
  // set world volume visibility for debugging
  if(DEBUG) logicWorld->SetVisAttributes(new G4VisAttributes(true));
  
  G4UserLimits* WorldUserLimits =new G4UserLimits();
  WorldUserLimits->SetMaxAllowedStep(1000*m);



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

  physiWorld = new G4PVPlacement(0,			//no rotation
  				 ZeroVec,       	//at (0,0,0)
                                 LocalName,		//its name
                                 logicWorld,		//its logical volume
                                 NULL,			//its mother  volume
                                 false,			//no boolean operation
                                 0);			//copy number


  // sensitive detectors

  G4SDManager* SDman = G4SDManager::GetSDMpointer();

   G4bool use_graphics=true;
  G4double s_local=-s_tot/2.;
  G4ThreeVector TargetPos;

  // set default output formats:
  G4cout.precision(15);
  
  if(DEBUG) G4cout<<" total length="<<s_tot/m<< " s_local="<<s_local/m<<G4endl;
  
  // reset counters:
  for(iBeam=theBeamline.begin();iBeam!=theBeamline.end();iBeam++){

    // zero length components have no logical volumes
    if((*iBeam)->GetLength()!=0.)
      {
	G4String logVolName = (*iBeam)->GetMarkerLogicalVolume()->GetName();
	(*LogVolCount)[logVolName]=1;
      }
  }
  
  if(DEBUG) G4cout<<"staring placement procedure "<<G4endl;
  
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
      
                   
      G4RotationMatrix *rotateComponent = new G4RotationMatrix; // for component placement


      //tilted bends influence reference frame, otherwise just local tilt
      if((*iBeam)->GetType() == "sbend")
	{
	  globalRotation.rotate(tilt,localZ);
	  localX.rotate(tilt,localZ);
	  localY.rotate(tilt,localZ);
	}
      else 
	rotateComponent->rotateZ(tilt);
    
      // define center of bended elements from the previos coordinate frame	  
         
      G4ThreeVector zHalfAngle = localZ; 

      if((*iBeam)->GetType() == "sbend")
	zHalfAngle.rotate(angle/2,localY);

      if(DEBUG)
	{
	  G4cout<<"zHalfNAgle="<<zHalfAngle<<G4endl;
	  G4cout<<"localZ="<<localZ<<G4endl;
	  G4cout<<"localX="<<localX<<G4endl;
	  G4cout<<"localY="<<localY<<G4endl;
	  G4cout<<"rlast="<<rlast<<G4endl;
	}
      
      // target position
      TargetPos = rlast + zHalfAngle *  ( (*iBeam)->GetLength()/2 + BDSGlobals->GetLengthSafety()/2 ) ;

      if(DEBUG) G4cout<<"TargetPos="<<TargetPos<<G4endl;

      // advance the coordinates, but not for cylindrical samplers 
      if( ( ( (*iBeam)->GetType() != "sampler") || ( (*iBeam)->GetLength() <= samplerLength ) )  && ( (*iBeam)->GetType()!=_ELEMENT ))
	{
	  if(DEBUG) G4cout << (*iBeam)->GetType() << " " << (*iBeam)->GetName() << G4endl;
	  rtot = rlast + zHalfAngle * ( (*iBeam)->GetLength()/2 + BDSGlobals->GetLengthSafety()/2 );
	  rlast = rtot + zHalfAngle * ( (*iBeam)->GetLength()/2 + BDSGlobals->GetLengthSafety()/2 );
	      
	}

      // rotate to the previous reference frame
      rotateComponent->transform(globalRotation);

      rotateComponent->invert();

      // recompute global rotation
      // define new coordinate system local frame	  
 
      // sbends transform the coordinate system, but not rbends
      if((*iBeam)->GetType() == "sbend")
	{
	  globalRotation.rotate(angle,localY);
	  localX.rotate(angle,localY);
	  localZ.rotate(angle,localY);
	  
	  
	  globalRotation.rotate(theta,localX);
	  localY.rotate(theta,localX);
	  localZ.rotate(theta,localX);
	  
	  // sbend trapezoids defined along z-axis
	  rotateComponent->rotateY(-pi/2-angle/2);

	}


      //(*iBeam)->SetZUpper(rtot.z());
      
      // zero lenght components not placed (transform3d)
      if((*iBeam)->GetLength()!=0.){
	
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
	
//	if(LogVolName=="sampler") {
	if((*iBeam)->GetType()=="sampler") {
	  LocalName=(*iBeam)->GetName()+"_phys";
	  bdsOutput.SampName.push_back(LocalName + "_" + BDSGlobals->StringFromInt(nCopy+1));
	}

	// Align Component - most cases does nothing. 
	// Currently only used for BDSElement	

	(*iBeam)->AlignComponent(TargetPos,
				 rotateComponent,
				 globalRotation,
				 rtot,
				 rlast,
				 localX,
				 localY,
				 localZ);


	G4PVPlacement* PhysiComponentPlace = 
	  new G4PVPlacement(
			    rotateComponent,   //  rotation
			    TargetPos,         // its position
			    LocalName,	      // its name
			    LocalLogVol,	      // its logical volume
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
//   theProductionCuts->SetProductionCut(1.e-6*m,"e+");
//   theProductionCuts->SetProductionCut(1.e-6*m,"e-");
  
//   precisionRegion->SetProductionCuts(theProductionCuts);
  
  if(DEBUG) G4cout<<"end placement, size="<<theBeamline.size()<<G4endl;
  
  if(verbose) G4cout<<"Detector Construction done"<<G4endl; 

  return physiWorld;
  
  
}

  
  
//=================================================================

void BDSDetectorConstruction::SetMagField(const G4double fieldValue)
{
 
  G4FieldManager* fieldMgr = G4TransportationManager::GetTransportationManager()->
	GetFieldManager();
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
  //if(BDSGlobals) delete BDSGlobals;

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
