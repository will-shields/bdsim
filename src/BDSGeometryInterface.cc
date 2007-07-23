// Geometry Interface Class
// ========================
//
// A class of functions to output Geant4/Mokka/BDSIM parameters for elements
// - to include geometry, optics, fields, etc.

#include "BDSGlobalConstants.hh"
#include "BDSGeometryInterface.hh"
#include "BDSAcceleratorComponent.hh"

using namespace std;

typedef list<BDSAcceleratorComponent*> BDSBeamline;
extern BDSBeamline theBeamline;

BDSGeometryInterface::BDSGeometryInterface(G4String filename):
  itsFileName(filename)
{}

BDSGeometryInterface::~BDSGeometryInterface()
{}

void BDSGeometryInterface::Optics()
{
  ofstream optics;
  
  G4cout << "Generating Optics file to: " << itsFileName << " ..." << G4endl;

  optics.open(itsFileName);

  optics << setw(10) << "Name" 
	 << setw(10) << "Length[m]" 
	 << setw(10) << "S [m]" 
	 << setw(10) << "K0" 
	 << setw(10) << "K1" 
	 << setw(10) << "K2" 
	 << setw(10) << "K3"
	 << setw(10) << "TILT"
	 << setw(10) << "AperX [m]"
	 << setw(10) << "AperY [m]"
	 <<" Aper_Type"  << G4endl;

  list<BDSAcceleratorComponent*>::const_iterator iBeam;
  
  for(iBeam=theBeamline.begin();iBeam!=theBeamline.end();iBeam++)
    { 
      G4int aper_type; //1 = rect, 2 = circ, 3 = elispe
      if((*iBeam)->GetType() == "rcol" ) //RCOL
	aper_type=1;
      else if((*iBeam)->GetType() == "ecol") //ECOL
	{
	  if((*iBeam)->GetAperX()==(*iBeam)->GetAperY()) 
	    aper_type=2;
	  else aper_type=3;
	}
      else aper_type=2; // circular is default
      
      optics.setf(ios::fixed, ios::floatfield);
      optics.setf(ios::showpoint);
      
      optics.precision(8);
      
      optics << setw(10) << (*iBeam)->GetName() << " "
	     << (*iBeam)->GetLength()/m  << " "
	     << (*iBeam)->GetSPos()/m  << " "
	     << (*iBeam)->GetAngle()   << " "
	     << (*iBeam)->GetK1()   << " "
	     << (*iBeam)->GetK2()   << " "
	     << (*iBeam)->GetK3()   << " "
	     << (*iBeam)->GetTilt() << " "
	     << (*iBeam)->GetAperX()/m   << " "
	     << (*iBeam)->GetAperY()/m   << " "
	     << aper_type
	     << G4endl;
	}
      optics.close();

}


void BDSGeometryInterface::Survey()
{
  ofstream survey;

  G4cout << "Generating Survey: " << itsFileName << " ..." << G4endl;

  survey.open(itsFileName);
  survey << setw(10) << "Name" 
	     << setw(8) << "Length[m]" 
	     << setw(8) << "X [m]" 
	     << setw(8) << "Y [m]" 
	     << setw(8) << "Z [m]" 
	     << setw(8) << "Phi [rad]" 
	     << setw(8) << "Theta [rad]" 
	     << setw(8) << "Psi [rad]" 
	     << setw(8) << "AperX [m]"
	     << setw(8) << "AperY [m]"
	     << setw(8) <<"Aper_Type" 
	     << setw(8) << "K0" 
	     << setw(8) << "K1" 
	     << setw(8) << "K2" 
	     << setw(8) << "K3" << G4endl;
  
  list<BDSAcceleratorComponent*>::const_iterator iBeam;
  
  for(iBeam=theBeamline.begin();iBeam!=theBeamline.end();iBeam++)
    { 
      G4int aper_type; //1 = rect, 2 = circ, 3 = elispe
      if((*iBeam)->GetType() == 14 ) //RCOL
	aper_type=1;
      else if((*iBeam)->GetType() == 13) //ECOL
	if((*iBeam)->GetAperX()==(*iBeam)->GetAperY()) 
	  aper_type=2;
	else aper_type=3;
      else aper_type=1;
      
      G4double phi, theta, psi;
      if((*iBeam)->GetRotation())
	{
	  // sort out rounding errors where zz -> 1.000001, etc.
	  if(fabs((*iBeam)->GetRotation()->zz())>1)
	    {
	      G4ThreeVector newZ = (*iBeam)->GetRotation()->rowZ();
	      newZ.setZ(1.0);
	      (*iBeam)->GetRotation()->setRows((*iBeam)->GetRotation()->rowX(),
					       (*iBeam)->GetRotation()->rowY(),
					       newZ);
	    }
	  
	  phi = (*iBeam)->GetRotation()->getPhi();
	  theta = (*iBeam)->GetRotation()->getTheta();
	  psi = (*iBeam)->GetRotation()->getPsi();
	  
	}
      else
	phi = theta = psi = 0.0;
      
      survey.setf(ios::fixed, ios::floatfield);
      survey.setf(ios::showpoint);
      
      survey.precision(8);
      
      survey << setw(10) << (*iBeam)->GetName() << " "
	     << (*iBeam)->GetLength()/m  << " "
	     << (*iBeam)->GetPosition().x()/m  << " "
	     << (*iBeam)->GetPosition().y()/m  << " "
	     << (*iBeam)->GetPosition().z()/m  << " "
	     << phi/radian  << " "
	     << theta/radian  << " "
	     << psi/radian  << " "
	     << (*iBeam)->GetAperX()/m   << " "
	     << (*iBeam)->GetAperY()/m   << " "
	     << aper_type  << " "
	     << (*iBeam)->GetAngle()   << " "
	     << (*iBeam)->GetK1()   << " "
	     << (*iBeam)->GetK2()   << " "
	     << (*iBeam)->GetK3()
	     << G4endl;
    }
  survey.close();
  
}
