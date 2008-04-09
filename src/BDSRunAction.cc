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
#include "BDSDump.hh"

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

const int DEBUG = 0;

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
      //G4cout<<"end of event : "<<event_number<<G4endl;
      G4cout<<"last event reached! dumping"<<G4endl;
      
      G4StackManager* SM = G4EventManager::GetEventManager()->GetStackManager();
      
      BDSGlobals->setWaitingForDump(false);
      BDSGlobals->setDumping(true);
      
      BDSGlobals->fileDump.open(BDSGlobals->GetFifo()); //SPM
      if(!BDSGlobals->fileDump.good()){
	G4Exception("BDSGlobals->GetFifo(): fifo not found. Quitting.");
	exit(1);
      }
      BDSGlobals->outputQueue.clear();
      BDSGlobals->fileDump << "# nparticles = " << SM->GetNPostponedTrack() << "\n";
      SM->TransferStackedTracks(fPostpone, fUrgent);// so that they can be reclassified
      SM->ReClassify();
      BDSGlobals->fileDump.close(); // SPM
      BDSGlobals->setDumping(false);
      BDSGlobals->setReading(true);
      
      // read in the stuff from placet
      
      int token;
      G4double x,y,z,t,xp,yp,zp,E;
      x = y = z = xp = yp = zp = t = E = 0;
      BDSGlobals->holdingQueue.clear();
      
      G4AffineTransform tf = BDSGlobals->GetDumpTransform();
      G4ThreeVector pos;
      G4ThreeVector momDir;
      G4ThreeVector LocalPosition;
      G4ThreeVector LocalDirection;
      
      FILE* fifo = fopen(BDSGlobals->GetFifo(),"r");

      if(fifo != NULL){
        fscanf(fifo,"# nparticles = %i",&token);
	G4cout << "# nparticles read from fifo = " << token << G4endl;
	for(int i=0; i< token;i++){
	  fscanf(fifo,"%lf %lf %lf %lf %lf %lf",&E,&x,&y,&z,&xp,&yp);
	  if(DEBUG) printf("%f %f %f %f %f %f\n",E,x,y,z,xp,yp);
	  
	  xp *= 1e-6*radian;
	  yp *= 1e-6*radian;
	  zp = sqrt(1-xp*xp-yp*yp)*radian;

	  pos = G4ThreeVector(x,y,z)*micrometer;
	  momDir = G4ThreeVector(xp,yp,zp);
	  
	  LocalPosition = tf.TransformPoint(pos);
	  LocalDirection = tf.TransformAxis(momDir);

	  LocalPosition -= LocalDirection*c_light
				*BDSGlobals->referenceQueue.front();
//	  t = -z/c_light;
	  LocalPosition += LocalDirection.unit()*1e-4*micrometer; // temp fix for recirculation in dump volume

	  if(DEBUG){	  	  
            G4cout << "Stacking: Pos = " << pos << G4endl;
            G4cout << "LocalPos: Pos = " << LocalPosition << G4endl;
            G4cout << "Stacking: mom = " << momDir << G4endl;
            G4cout << "LocalDir: mom = " << LocalDirection << G4endl;
	  }
	  tmpParticle holdingParticle;
	  holdingParticle.E = E*GeV - BDSGlobals->GetParticleDefinition()->GetPDGMass();
//	  holdingParticle.t = t;
	  holdingParticle.xp = LocalDirection.x();
	  holdingParticle.yp = LocalDirection.y();
	  holdingParticle.zp = LocalDirection.z();
	  
	  holdingParticle.x = LocalPosition.x();
	  holdingParticle.y = LocalPosition.y();
	  holdingParticle.z = LocalPosition.z();
	  
	  BDSGlobals->holdingQueue.push_back(holdingParticle);
	  if(DEBUG) G4cout << "Read particle number " << i << G4endl;
        }
        sleep(1);
	fclose(fifo);
        BDSGlobals->setReading(false);
        BDSGlobals->setReadFromStack(false);
	BDSGlobals->referenceQueue.pop_front();
	BDSGlobals->referenceQueue.pop_front();

	if(DEBUG) G4cout << "Number read in = " << BDSGlobals->holdingQueue.size() << G4endl;
      }
      else{
	G4Exception("Read from fifo failed: bad file name\n");
	exit(1);
      }
      BDSDump::nUsedDumps++;
    }
}
//==========================================================
