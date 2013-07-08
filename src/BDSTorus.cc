#include "BDSTorus.hh"
#inlcude "G4PhysicalConstants.hh"
#include <iostream>

using namespace std;

#define DEBUG 1

BDSTorus::~BDSTorus(){
}

BDSTorus::BDSTorus(const G4String& name,
		   G4double r,
		   G4double length,
		   G4double angle):G4Torus(name,0,r,std::abs(length/angle),0,std::abs(angle)){
  cout << "BDSTorus::BDSTorus() - building torus, name, r, length, angle = " << name << " " << r << " " << length << " " << angle << " " << endl;
}

