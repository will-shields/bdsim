#include "BDSBeamPipeInfo.hh"
#include "BDSDebug.hh"
#include "BDSGlobalConstants.hh" 
#include "BDSMagnetType.hh"
#include "BDSMagnetOuterInfo.hh"
#include "BDSQuadrupole.hh"

#include "globals.hh"

BDSQuadrupole::BDSQuadrupole(G4String            name,
			     G4double            length,
			     G4double            /*bGrad*/,
			     BDSBeamPipeInfo*    beamPipeInfo,
			     BDSMagnetOuterInfo* magnetOuterInfo):
  BDSMagnet(BDSMagnetType::quadrupole, name, length,
	    beamPipeInfo, magnetOuterInfo)
{;}

void BDSQuadrupole::Build() 
{
#ifdef BDSDEBUG
  G4cout << __METHOD_NAME__ << G4endl;
#endif
  BDSMagnet::Build();
  /*
  if(BDSGlobalConstants::Instance()->GetIncludeIronMagFields())
    {
      G4double polePos[4];
      G4double Bfield[3];
      
      //coordinate in GetFieldValue
      polePos[0]=-BDSGlobalConstants::Instance()->GetMagnetPoleRadius()*sin(CLHEP::pi/4);
      polePos[1]=BDSGlobalConstants::Instance()->GetMagnetPoleRadius()*cos(CLHEP::pi/4);
      polePos[2]=0.;
      polePos[3]=-999.;//flag to use polePos rather than local track
      
      itsMagField->GetFieldValue(polePos,Bfield);
      G4double BFldIron=
	sqrt(Bfield[0]*Bfield[0]+Bfield[1]*Bfield[1])*
	BDSGlobalConstants::Instance()->GetMagnetPoleSize()/
	(BDSGlobalConstants::Instance()->GetComponentBoxSize()/2-
	 BDSGlobalConstants::Instance()->GetMagnetPoleRadius());
      
      // Magnetic flux from a pole is divided in two directions
      BFldIron/=2.;
      
      BuildOuterFieldManager(4, BFldIron,CLHEP::pi/4);
      }*/
}