// Geometry Interface Class
// ========================
//
// A class of functions to output Geant4/Mokka/BDSIM parameters for elements
// - to include geometry, optics, fields, etc.

#include "BDSGlobalConstants.hh"
#include "BDSGeometryInterface.hh"
#include "BDSAcceleratorComponent.hh"
#include "BDSBeamline.hh"

using namespace std;

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

  for(BDSBeamline::Instance()->first();!BDSBeamline::Instance()->isDone();BDSBeamline::Instance()->next())
    { 
      G4int aper_type; //1 = rect, 2 = circ, 3 = elispe
      if(BDSBeamline::Instance()->currentItem()->GetType() == "rcol" ) //RCOL
	aper_type=1;
      else if(BDSBeamline::Instance()->currentItem()->GetType() == "ecol") //ECOL
	{
	  if(BDSBeamline::Instance()->currentItem()->GetAperX()==BDSBeamline::Instance()->currentItem()->GetAperY()) 
	    aper_type=2;
	  else aper_type=3;
	}
      else aper_type=2; // circular is default
      
      optics.setf(ios::fixed, ios::floatfield);
      optics.setf(ios::showpoint);
      
      optics.precision(8);
      
      optics << setw(10) 
	     << BDSBeamline::Instance()->currentItem()->GetType() << " "
	     << BDSBeamline::Instance()->currentItem()->GetName() << " "
	     << BDSBeamline::Instance()->currentItem()->GetLength()/m  << " "
	     << BDSBeamline::Instance()->positionS()/m  << " "
	     << BDSBeamline::Instance()->currentItem()->GetAngle()   << " "
	     << BDSBeamline::Instance()->currentItem()->GetK1()   << " "
	     << BDSBeamline::Instance()->currentItem()->GetK2()   << " "
	     << BDSBeamline::Instance()->currentItem()->GetK3()   << " "
	     << BDSBeamline::Instance()->currentItem()->GetTilt() << " "
	     << BDSBeamline::Instance()->currentItem()->GetAperX()/m   << " "
	     << BDSBeamline::Instance()->currentItem()->GetAperY()/m   << " "
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
  
  G4double length(0.0);
  G4double arc_length(0.0);
  for(BDSBeamline::Instance()->first();!BDSBeamline::Instance()->isDone();BDSBeamline::Instance()->next())
    { 
      G4int aper_type; //1 = rect, 2 = circ, 3 = elispe
      if(BDSBeamline::Instance()->currentItem()->GetType() == 14 ) //RCOL
	aper_type=1;
      else if(BDSBeamline::Instance()->currentItem()->GetType() == 13) //ECOL
	if(BDSBeamline::Instance()->currentItem()->GetAperX()==BDSBeamline::Instance()->currentItem()->GetAperY()) 
	  aper_type=2;
	else aper_type=3;
      else aper_type=1;
      
      G4double phi, theta, psi;
      if(BDSBeamline::Instance()->currentItem()->GetRotation())
	{
	  // sort out rounding errors where zz -> 1.000001, etc.
	  if(fabs(BDSBeamline::Instance()->currentItem()->GetRotation()->zz())>1)
	    {
	      G4ThreeVector newZ = BDSBeamline::Instance()->currentItem()->GetRotation()->rowZ();
	      newZ.setZ(1.0);
	      BDSBeamline::Instance()->currentItem()->GetRotation()->setRows(BDSBeamline::Instance()->currentItem()->GetRotation()->rowX(),
					       BDSBeamline::Instance()->currentItem()->GetRotation()->rowY(),
					       newZ);
	    }
	  
	  phi = BDSBeamline::Instance()->currentItem()->GetRotation()->getPhi();
	  theta = BDSBeamline::Instance()->currentItem()->GetRotation()->getTheta();
	  psi = BDSBeamline::Instance()->currentItem()->GetRotation()->getPsi();
	  
	}
      else
	phi = theta = psi = 0.0;
      
      survey.setf(ios::fixed, ios::floatfield);
      survey.setf(ios::showpoint);
      
      survey.precision(7);
      
      survey << setw(10) << BDSBeamline::Instance()->currentItem()->GetType() << " "
	     << setw(10) << BDSBeamline::Instance()->currentItem()->GetName() << " "
	     << setw(10) << BDSBeamline::Instance()->currentItem()->GetLength()/m  << " "
	     << setw(10) << BDSBeamline::Instance()->currentItem()->GetArcLength()/m  << " "
	     << setw(10) << BDSBeamline::Instance()->currentItem()->GetPosition().x()/m  << " "
	     << setw(10) << BDSBeamline::Instance()->currentItem()->GetPosition().y()/m  << " "
	     << setw(10) << BDSBeamline::Instance()->currentItem()->GetPosition().z()/m  << " "
	     << setw(10) << phi/radian  << " "
	     << setw(10) << theta/radian  << " "
	     << setw(10) << psi/radian  << " "
	     << setw(10) << BDSBeamline::Instance()->currentItem()->GetAperX()/m   << " "
	     << setw(10) << BDSBeamline::Instance()->currentItem()->GetAperY()/m   << " "
	     << setw(10) << aper_type  << " "
	     << setw(10) << BDSBeamline::Instance()->currentItem()->GetAngle()   << " "
	     << setw(10) << BDSBeamline::Instance()->currentItem()->GetK1()   << " "
	     << setw(10) << BDSBeamline::Instance()->currentItem()->GetK2()   << " "
	     << setw(10) << BDSBeamline::Instance()->currentItem()->GetK3()   << " "
	     << G4endl;
      length+=BDSBeamline::Instance()->currentItem()->GetLength()/m;
      arc_length+=BDSBeamline::Instance()->currentItem()->GetArcLength()/m;
    }
  survey << "Total length = " << length << "m" << G4endl;
  survey << "Total arc length = " <<  arc_length << "m" << G4endl;
  survey.close();
}
