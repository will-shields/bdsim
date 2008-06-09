//  
//   BDSIM, (C) 2001-2006 
//    
//   version 0.2 
//   last modified : 28 Mar 2006 by agapov@pp.rhul.ac.uk
//  



//
//    Stacking action - taken when secondaries created
//



#include "BDSGlobalConstants.hh"
#include "BDSStackingAction.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ios.hh"

const int DEBUG  = 0;
const int DEBUG2 = 0;

BDSStackingAction::BDSStackingAction()
{ 
}

BDSStackingAction::~BDSStackingAction()
{
}

G4ClassificationOfNewTrack 
BDSStackingAction::ClassifyNewTrack(const G4Track * aTrack)
{
  //G4ClassificationOfNewTrack classification = fWaiting;
  G4ClassificationOfNewTrack classification = fUrgent;
 
  G4String pName=aTrack->GetDefinition()->GetParticleName();


  if(DEBUG) {
    G4cout<<"StackingAction: ClassifyNewtrack "<<aTrack->GetTrackID()<<
      " "<<aTrack->GetDefinition()->GetParticleName()<<G4endl;

    G4StackManager* SM = G4EventManager::GetEventManager()->GetStackManager();

    G4cout<<"N total tracks : "<<SM->GetNTotalTrack() << G4endl;
    G4cout<<"N waiting tracks : "<<SM->GetNWaitingTrack() << G4endl;
    G4cout<<"N urgent tracks : "<<SM->GetNUrgentTrack() << G4endl;
    G4cout<<"N postponed tracks : "<<SM->GetNPostponedTrack() << G4endl;
    G4cout<<"Events to process : "<<
      G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed()<<G4endl;
     G4cout<<"Number of event : "<<
       G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEvent()<<G4endl;

    
  }

  if(BDSGlobals->DoTwiss())
    {
            if((aTrack->GetParentID() <= 0) &&
	       (aTrack->GetTrackStatus()==fPostponeToNextEvent) )
	classification = fPostpone;
    }

  if(BDSGlobals->GetStopTracks()) // if tracks killed after interaction
    {
      
      // kill secondary electrons
      
      if( (aTrack->GetParentID() > 0) && 
	  (aTrack->GetDefinition() == G4Electron::ElectronDefinition() ) )
	{
	  
	  
	  classification = fKill;
	  //classification = fUrgent;

	  // if we are in the twiss module - aperture hit is suspicious
	  if( BDSGlobals->DoTwiss() ) 
	    G4cout<<"WARNING : Electron "<<aTrack->GetParentID()<<" outside of aperture, twiss results will be incorrect"<<
	      G4endl;;

	}
      
      // kill secondary photons
      
      if( (aTrack->GetParentID() > 0) && 
	  (aTrack->GetDefinition() == G4Gamma::GammaDefinition() ) )
	{
	  classification = fKill;
	}
      
      // kill secondary positrons
      
      if( (aTrack->GetParentID() > 0) && 
	  (aTrack->GetDefinition() == G4Positron::PositronDefinition() ) )
	{
	  classification = fKill;

	  // if we are in the twiss module - aperture hit is suspicious
	  if( BDSGlobals->DoTwiss() ) 
	    G4cout<<"WARNING : Positron outside of aperture, twiss results will be incorrect"<<
	      G4endl;
	}

      // kill secondary protons/antiprotons
      
      if( (aTrack->GetParentID() > 0) && 
	  ( (aTrack->GetDefinition() == G4Proton::ProtonDefinition() ) ||
	    (aTrack->GetDefinition() == G4AntiProton::AntiProtonDefinition()) ) )
	{
	  classification = fKill;
	  
	  // if we are in the twiss module - aperture hit is suspicious
	  if( BDSGlobals->DoTwiss() ) 
	    G4cout<<"WARNING : Proton outside of aperture, twiss results will be incorrect"<<
	      G4endl;
	}
      
    }

   if(BDSGlobals->getWaitingForDump()) // if waiting for placet synchronization
     {
       // when waiting to synchronize with placet - put on postponed stack
       if( aTrack->GetTrackStatus()==fPostponeToNextEvent )
       classification = fPostpone;
       //BDSGlobals->setWaitingForDump(false); // next event ok
     }
  
    if(BDSGlobals->getDumping()) // in the process of dumping
     {
	if(DEBUG){
          G4cout<<"reclassifying track "<<aTrack->GetTrackID()<<G4endl;

          G4cout<<"r= "<<aTrack->GetPosition()<<G4endl;
	}

	G4AffineTransform tf = BDSGlobals->GetDumpTransform().Inverse();
//	const G4RotationMatrix Rot=tf.NetRotation();
//	const G4ThreeVector Trans=-tf.NetTranslation();

        G4ThreeVector initialPos=aTrack->GetPosition();
        G4ThreeVector momDir=aTrack->GetMomentumDirection();
	G4double refTime = (BDSGlobals->referenceQueue.front() 
				- aTrack->GetGlobalTime())/2;

	G4ThreeVector transformedPos = initialPos + momDir*c_light*refTime;
	//G4cout << "RefTime = " << refTime << " " << BDSGlobals->referenceQueue.front()
	//	<< " " << aTrack->GetGlobalTime() << G4endl;
	//pos.setZ(aTrack->GetGlobalTime()*c_light);


//        G4ThreeVector LocalPosition=pos+Trans;
//        G4ThreeVector LocalDirection=Rot*momDir;
        G4ThreeVector LocalPosition=tf.TransformPoint(transformedPos);
        G4ThreeVector LocalDirection=tf.TransformAxis(momDir);

	if(DEBUG){
	  G4cout << "Stacking: Pos = " << transformedPos << G4endl;
	  G4cout << "LocalPos: Pos = " << LocalPosition << G4endl;
	  G4cout << "Stacking: mom = " << momDir << G4endl;
	  G4cout << "LocalDir: mom = " << LocalDirection << G4endl;
	}

        G4double x=LocalPosition.x()/micrometer;
        G4double y=LocalPosition.y()/micrometer;
	G4double z=LocalPosition.z()/micrometer;
        G4double xPrime=LocalDirection.x()/(1e-6*radian);
        G4double yPrime=LocalDirection.y()/(1e-6*radian);
	G4double t=aTrack->GetGlobalTime();

        BDSGlobals->fileDump.precision(15);
        // TODO : dump the file
	//        BDSGlobals->fileDump << aTrack->GetTotalEnergy()/GeV << "\t"
	//<< x << "\t" << y << "\t" << z << "\t"
	//<< xPrime << "\t" << yPrime << "\t" << t <<"\n"; // SPM
	if(DEBUG2) printf("Out: %.15f %.15f %.15f %.15f %.15f %.15f %f\n",
		aTrack->GetTotalEnergy()/GeV,x,y,z,xPrime,yPrime,t);
        tmpParticle outputParticle, transformedParticle;
        if(aTrack->GetDefinition()->GetPDGEncoding()==-11)
	  outputParticle.E=-(aTrack->GetTotalEnergy());
        else outputParticle.E=aTrack->GetTotalEnergy();
        outputParticle.xp=momDir.x();
        outputParticle.yp=momDir.y();
        outputParticle.x=initialPos.x();
        outputParticle.y=initialPos.y();
        outputParticle.z=initialPos.z();
        outputParticle.t=t;
        outputParticle.trackID=aTrack->GetTrackID();
        outputParticle.parentID=aTrack->GetParentID();

	transformedParticle.x=x;
	transformedParticle.y=y;
	transformedParticle.z=z;
	transformedParticle.xp=xPrime;
	transformedParticle.yp=yPrime;
	transformedParticle.t=t;
	transformedParticle.E=aTrack->GetTotalEnergy()/GeV;

        BDSGlobals->outputQueue.push_back(outputParticle);
        BDSGlobals->transformedQueue.push_back(transformedParticle);
        classification = fPostpone;
     }

     if(BDSGlobals->getReading()){
       classification = fWaiting_1;
     }

  return classification;
}


void BDSStackingAction::NewStage()
{
  // urgent stack empty, looking into the waiting stack
 
  if(DEBUG) G4cout<<"StackingAction: New stage"<<G4endl;

  //stackManager->ReClassify();
  
  return;
 
}
    
void BDSStackingAction::PrepareNewEvent()
{ 
}


