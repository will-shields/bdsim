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

  optics << setw(10) << "Type" 
         << setw(10) << "Name" 
	 << setw(10) << "Length[m]" 
	 << setw(10) << "S [m]" 
	 << setw(10) << "Angle[rad]" 
	 << setw(10) << "K1 [m^-2]" 
	 << setw(10) << "K2 [m^-3]" 
	 << setw(10) << "K3 [m^-4]"
	 << setw(10) << "TILT"
	 << setw(10) << "AperX [m]"
	 << setw(10) << "AperY [m]"
	 << setw(10) << "Aper_Type"
	 << G4endl;

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
      
      optics << setw(10) 
	     << (*iBeam)->GetType() << " "
	     << (*iBeam)->GetName() << " "
	     << (*iBeam)->GetLength()/m  << " "
	     << (*iBeam)->GetSPos()/m  << " "
	     << (*iBeam)->GetAngle()   << " "
	     << (*iBeam)->GetK1()   << " "
	     << (*iBeam)->GetK2()   << " "
	     << (*iBeam)->GetK3()   << " "
	     << (*iBeam)->GetTilt() << " "
	     << (*iBeam)->GetAperX()/m   << " "
	     << (*iBeam)->GetAperY()/m   << " "
	     << aper_type   << " "
	     << G4endl;
	}
      optics.close();

}


void BDSGeometryInterface::Survey()
{
  ofstream survey;

  G4cout << "Generating Survey: " << itsFileName << " ..." << G4endl;

  survey.open(itsFileName);
  survey << setw(10) << "Type" << " "
	 << setw(10) << "Name" << " "
	 << setw(10) << "Length[m]" << " "
	 << setw(10) << "Arc len[m]" << " "
	 << setw(10) << "X [m]" << " "
	 << setw(10) << "Y [m]" << " "
	 << setw(10) << "Z [m]" << " "
	 << setw(10) << "Phi [rad]" << " "
	 << setw(10) << "Theta [rad]"
	 << setw(10) << "Psi [rad]" << " "
	 << setw(10) << "AperX [m]" << " "
	 << setw(10) << "AperY [m]" << " "
	 << setw(10) << "Aper_Type" << " " 
	 << setw(10) << "Angle[rad]" << " "
	 << setw(10) << "K1 [m^-2]" << " " 
	 << setw(10) << "K2 [m^-3]" << " " 
	 << setw(10) << "K3 [m^-4]" << " "

	 << G4endl;
  
  list<BDSAcceleratorComponent*>::const_iterator iBeam;
  G4double length(0.0);
  G4double arc_length(0.0);
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
      
      survey.precision(7);
      
      survey << setw(10) << (*iBeam)->GetType() << " "
	     << setw(10) << (*iBeam)->GetName() << " "
	     << setw(10) << (*iBeam)->GetLength()/m  << " "
	     << setw(10) << (*iBeam)->GetArcLength()/m  << " "
	     << setw(10) << (*iBeam)->GetPosition().x()/m  << " "
	     << setw(10) << (*iBeam)->GetPosition().y()/m  << " "
	     << setw(10) << (*iBeam)->GetPosition().z()/m  << " "
	     << setw(10) << phi/radian  << " "
	     << setw(10) << theta/radian  << " "
	     << setw(10) << psi/radian  << " "
	     << setw(10) << (*iBeam)->GetAperX()/m   << " "
	     << setw(10) << (*iBeam)->GetAperY()/m   << " "
	     << setw(10) << aper_type  << " "
	     << setw(10) << (*iBeam)->GetAngle()   << " "
	     << setw(10) << (*iBeam)->GetK1()   << " "
	     << setw(10) << (*iBeam)->GetK2()   << " "
	     << setw(10) << (*iBeam)->GetK3()   << " "
	     << G4endl;
      length+=(*iBeam)->GetLength()/m;
      arc_length+=(*iBeam)->GetArcLength()/m;
    }
  survey << "Total length = " << length << "m" << G4endl;
  survey << "Total arc length = " <<  arc_length << "m" << G4endl;
  survey.close();
}
