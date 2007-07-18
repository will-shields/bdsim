/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 28.12.2004
   Copyright (c) 2004 by G.A.Blair.  ALL RIGHTS RESERVED. 

   Modified 22.03.05 by J.C.Carter, Royal Holloway, Univ. of London.
   Added GABs SynchGen code
*/


const int DEBUG = 0;

//==================================================================
//==================================================================
#include "BDSGlobalConstants.hh" // must be first in include list
#include "BDSPrimaryGeneratorAction.hh"

#include "BDSDetectorConstruction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4EventManager.hh"
#include "G4StackManager.hh"
#include "G4Track.hh"
#include "G4Trajectory.hh"

#include "Randomize.hh"

//#include "BDSExtract.hh"
#include "BDSBunch.hh"

#include<iostream>

extern BDSBunch theBunch;


//===================================================
// Keep initial point in phase space for diagnostics
G4double initial_x,initial_xp,initial_y,initial_yp,initial_z,initial_E;

BDSPrimaryGeneratorAction::BDSPrimaryGeneratorAction(
                                               BDSDetectorConstruction* BDSDC)
:BDSDetector(BDSDC)
{

 
  particleGun  = new G4ParticleGun(1); // 1-particle gun

 //  particleGun->SetParticleDefinition(BDSGlobals->
//                                       GetParticleDefinition());

  particleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->
				     FindParticle("e-"));

  
  if(BDSGlobals->GetUseSynchPrimaryGen()) // synchrotron radiation generator
    {
      itsBDSSynchrotronRadiation=new BDSSynchrotronRadiation("tmpSynRad");
      G4double R=BDSGlobals->GetSynchPrimaryLength()/
	BDSGlobals->GetSynchPrimaryAngle();   
      itsSynchCritEng=3./2.*hbarc/pow(electron_mass_c2,3)*
	pow(BDSGlobals->GetBeamKineticEnergy(),3)/R;
      
      G4cout<<" BDSPrimaryGeneratorAction:  Critical Energy="<<
	itsSynchCritEng/keV<<" keV"<<G4endl;
      
      particleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->
		     FindParticle("gamma"));
    }

  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  particleGun->
    SetParticlePosition(G4ThreeVector(0.*cm,0.*cm,0.*cm));
  if(DEBUG) G4cout<<"Primary generator: particle energy"<<BDSGlobals->GetBeamKineticEnergy()<<G4endl;
  particleGun->SetParticleEnergy(BDSGlobals->GetBeamKineticEnergy());

}

//===================================================

BDSPrimaryGeneratorAction::~BDSPrimaryGeneratorAction()
{
  delete particleGun;
  //delete GaussGen;
  //delete FlatGen;
}
//===================================================

void BDSPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of event

  G4double x0, y0, z0, xp, yp, zp, t, E;

    G4cout << BDSGlobals->holdingVector.size() << " particles waiting to fire\n";

  if(!BDSGlobals->getReadFromStack()){
    theBunch.GetNextParticle(x0,y0,z0,xp,yp,zp,t,E); // get next starting point
  }
  else if(BDSGlobals->holdingVector.size()!=0){

    tmpParticle holdingParticle = (BDSGlobals->holdingVector)[BDSGlobals->holdingVector.size()-1];
    x0 = holdingParticle.x;
    y0 = holdingParticle.y;
    z0 = holdingParticle.z;
    xp = holdingParticle.xp;
    yp = holdingParticle.yp;
    zp = holdingParticle.zp;
    t = holdingParticle.t;
    E = holdingParticle.E;
    printf("TrackID %i: %f %f %f %f %f %f %f %f\n",
	(int)BDSGlobals->holdingVector.size(),x0,y0,z0,xp,yp,zp,t,E);
    BDSGlobals->holdingVector.pop_back();
  }
  else G4Exception("No new particles to fire...\n");

  G4ThreeVector PartMomDir=G4ThreeVector(xp,yp,zp);
 
  particleGun->SetParticleDefinition(BDSGlobals->
                                      GetParticleDefinition());
  particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0 ) );
  particleGun->SetParticleEnergy(E);
  particleGun->SetParticleMomentumDirection(PartMomDir);
  particleGun->SetParticleTime(t);

  particleGun->GeneratePrimaryVertex(anEvent);


  if(DEBUG )
    {
      G4cout<<" BDSPrimaryGeneratorAction: mom="<<PartMomDir
	    <<" Energy="<<E/GeV<<"GeV"<<G4endl;
      G4cout<<" BDSPrimaryGeneratorAction: Pos="<<
	particleGun->GetParticlePosition()/m<<"m"<<G4endl;
    }

  // save initial values outside scope for entry into the samplers:
  initial_x=x0;
  initial_xp=xp;
  initial_y=y0;
  initial_yp=yp;
  initial_z=-t*c_light;
  // total energy is used elsewhere:
  initial_E=E+BDSGlobals->GetParticleDefinition()->GetPDGMass();

}


//===================================================

