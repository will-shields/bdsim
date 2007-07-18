/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/

//==========================================================
//==========================================================
#include "BDSGlobalConstants.hh" // must be first in include list

#include "BDSRunAction.hh"
#include "BDSRunManager.hh"

#include "G4Run.hh"
#include "G4StateManager.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"

#include "BDSAcceleratorComponent.hh"
#include <list>
#include <fstream>
#include <string>

typedef list<BDSAcceleratorComponent*>  myBeamline;
extern myBeamline theBeamline;

//==========================================================

BDSRunAction::BDSRunAction()
{
}

//==========================================================

BDSRunAction::~BDSRunAction()
{}

//==========================================================

void BDSRunAction::BeginOfRunAction(const G4Run* aRun)
{
  //  BDSGlobals->GetTimer()->Start();
 
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

 //  if(BDSGlobals->GetUseTimer())
//     {
//       //BDSGlobals->GetTimer()->Start();
//     }

  if (G4VVisManager::GetConcreteInstance())
    {
      G4UImanager* UI = G4UImanager::GetUIpointer(); 
      UI->ApplyCommand("/vis/scene/notifyHandlers");
    } 


}

//==========================================================

void BDSRunAction::EndOfRunAction(const G4Run* aRun)
{

  //  if (G4VVisManager::GetConcreteInstance()) {
  //   G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");

 //  if(BDSGlobals->GetUseTimer())
//     {
//       BDSGlobals->GetTimer()->Stop();
//       G4cout<<"RUN Elapsed time="<<*BDSGlobals->GetTimer()<<G4endl;
//     }


  /*  
  if(BDSGlobals->GetSynchRadOn())
    {
    ofstream synch_output("synch_factors_new.dat");
      myBeamline::const_iterator iBeam;
      for(iBeam=theBeamline.begin();iBeam!=theBeamline.end();iBeam++)
	{
	  synch_output<<(*iBeam)->GetSynchEnergyLoss()/ 
	    G4double(aRun->GetNumberOfEvent())/GeV<<G4endl;
	}
    }
  */
  if(BDSGlobals->getWaitingForDump()) // synchronization with placet
    {
//      G4cout<<"end of event : "<<event_number<<G4endl;


//      G4int nevent = BDSRunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();

  //    if(event_number == nevent - 1) {
        G4cout<<"last event reached! dumping"<<G4endl;

        G4StackManager* SM = G4EventManager::GetEventManager()->GetStackManager();

        BDSGlobals->setWaitingForDump(false);
        BDSGlobals->setDumping(true);

        BDSGlobals->fileDump.open(BDSGlobals->GetFifo()); //SPM
        if(!BDSGlobals->fileDump.good()){
          G4cerr << "BDSGlobals->GetFifo(): " << BDSGlobals->GetFifo() <<
                " not found. Quitting." << G4endl;
          exit(1);
        }
        BDSGlobals->fileDump << "# nparticles = " << SM->GetNPostponedTrack() << "\n";
        SM->TransferStackedTracks(fPostpone, fUrgent);// so that they can be reclassified
        SM->ReClassify();
        BDSGlobals->fileDump.close(); // SPM
        BDSGlobals->setDumping(false);
        BDSGlobals->setReading(true);
        //

        // TODO: read in the stuff from placet

        //
        BDSGlobals->fileRead.open(BDSGlobals->GetFifo());
        if(!BDSGlobals->fileDump.good()){
          G4cerr << "BDSGlobals->GetFifo(): " << BDSGlobals->GetFifo() <<
                " not found. Quitting." << G4endl;
          exit(1);
        }
        char token[255];
        BDSGlobals->fileRead.getline(token,255);
        G4cout << token << G4endl;

//        G4Track* newTrack;
//      G4VTrajectory* newTrajectory;
//        G4DynamicParticle* newParticle;
        double x,y,z,t,xp,yp,zp,E;
        x = y = z = xp = yp = zp = t = E = 0;
	BDSGlobals->holdingVector.clear();
        for(int i=0; i< SM->GetNPostponedTrack();i++){
          BDSGlobals->fileRead >> E >> x >> y >> z >> xp >> yp;
          zp = sqrt(1-xp*xp-yp*yp);
          t==0 ? t = -z/c_light : z = -t*c_light;
/*          newParticle = new G4DynamicParticle(BDSGlobals->GetParticleDefinition(),
                G4ThreeVector(xp,yp,zp),
                E-BDSGlobals->GetParticleDefinition()->GetPDGMass());
          newParticle->SetProperTime(t);
          newTrack = new G4Track(newParticle,t,G4ThreeVector(x,y,z));
//        newTrajectory = new G4Trajectory(newTrack);
          SM->PushOneTrack(newTrack);
*/
	  tmpParticle holdingParticle;
	  holdingParticle.E = E;
	  holdingParticle.x = x;
	  holdingParticle.y = y;
	  holdingParticle.z = z;
	  holdingParticle.t = t;
	  holdingParticle.xp = xp;
	  holdingParticle.yp = yp;
	  holdingParticle.zp = zp;
	  BDSGlobals->holdingVector.push_back(holdingParticle);
          G4cout << "Read particle number " << i << G4endl;
        }
        sleep(1);
        //SM->ReClassify();
        BDSGlobals->fileRead.close();

        BDSGlobals->setReading(false);
        BDSGlobals->setReadFromStack(false);
      }

//    }

}

//==========================================================
