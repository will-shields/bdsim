/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 25.12.2003
   Copyright (c) 2003 by G.A.Blair.  ALL RIGHTS RESERVED. 
 
   Modified 22.03.05 by J.C.Carter, Royal Holloway, Univ. of London.
   Added code for GABs BeamGasPlug
   Changed Killer to kill just electrons/positrons if called KILLERE

   Modified 10.05.05 IA
   Removed Killer element, added cuts per element check
   Can have killer functionality if cuts set to 0

*/


//====================================================
//  Class description here ...
//
//====================================================

#include "BDSGlobalConstants.hh" // must be first in include list

#include "BDSSteppingAction.hh"

#include <iostream>
#include "G4Track.hh"

#include"G4TransportationManager.hh"
#include"G4FieldManager.hh"

#include"G4Navigator.hh"
#include"G4StepPoint.hh"
#include "G4RotationMatrix.hh"
#include "G4AffineTransform.hh"

#include "BDSMaterials.hh"
#include "G4Material.hh"
#include "G4VSensitiveDetector.hh"
#include "Randomize.hh"

#include "G4UserLimits.hh"

#include "BDSNeutronTrackInfo.hh"

#include "G4VUserTrackInformation.hh"

#include "G4VProcess.hh"

#include "G4MagneticField.hh"
#include "G4EventManager.hh"
#include "G4StackManager.hh"
#include "G4ChordFinder.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4Region.hh"
#include "BDSAcceleratorComponent.hh"

#include "BDSQuadStepper.hh"
#include "BDSSextStepper.hh"
#include "BDSOctStepper.hh"
#include "myQuadStepper.hh"

extern BDSMaterials* theMaterials;

typedef list<BDSAcceleratorComponent*> BDSBeamline;
extern BDSBeamline theBeamline;

extern G4double BDS_Threshold_Energy;
extern G4double BDSLocalRadiusOfCurvature;

extern G4int event_number;

extern G4double initial_x,initial_xp,initial_y,initial_yp,initial_z,initial_E;

extern G4bool verbose;      // run options
extern G4bool verboseStep;
extern G4bool verboseEvent;
extern G4int verboseEventNumber;
extern G4bool isBatch;

extern G4int nptwiss;

const int DEBUG = 0;

//static G4LogicalVolume* LastLogVol;
//====================================================

BDSSteppingAction::BDSSteppingAction()
{ 
  //  itsZposTolerance=1.e-11*m;
  itsZposTolerance=1.e-4*m;
  itsPosKick=1.e-11*m;
  itsNmax=10000;
  postponedEnergy=0;
}

//====================================================

BDSSteppingAction::~BDSSteppingAction()
{ }


//====================================================

extern G4double htot;

void BDSSteppingAction::UserSteppingAction(const G4Step* ThisStep)
{ 
  // G4cout<<"user stepping action called"<<G4endl;

  G4Track* ThisTrack=ThisStep->GetTrack();
  
  //G4int TrackID=ThisTrack->GetTrackID();
  
  G4String pName=ThisTrack->GetDefinition()->GetParticleName();

  if(ThisTrack->GetProperTime() > 1e-4*second)
    {
      G4cout << "WARNING: ProperTime > 1.e-4 second!" << G4endl;
      G4cout<<" Killing the particle"<<G4endl;
      ThisTrack->SetTrackStatus(fStopAndKill);
    }
  
  // Check for a problem at the boundaries, where an infinite loop can
  // apparently sometimes occur
  

  // leave Geant4 to take care of it 

 //  if(TrackID!=itsLastTrackID)
//     {
//       itsLastTrackID=TrackID;
//       itsNtry=0;
//     }
//   else 
//     { 
//       if(fabs(ThisTrack->GetPosition().z()-itsLastZpos)<itsZposTolerance)
// 	{
// 	  itsNtry++;
// 	  if(itsNtry>itsNmax)
// 	    {
// 	      G4cout<<" Killing the particle"<<G4endl;
// 	      ThisTrack->SetTrackStatus(fStopAndKill);
// 	      /* ThisTrack->
// 		SetPosition(
// 			    ThisTrack->GetPosition()+
// 			    ThisTrack->GetMomentumDirection()*itsPosKick
// 			    ); */
// 	      //	      itsNtry=0;
// 	    }
// 	}
//       else 	
// 	itsNtry=0;
//     }


//  itsLastZpos=ThisTrack->GetPosition().z();
  

  // check that there actually is a next volume as it may be the end of the optics line
  if(BDSGlobals->DoTwiss() && ThisTrack->GetNextVolume() && ThisTrack->GetParentID() <= 0) 
    {

      if(DEBUG){
	G4cout <<" ***"<< ThisTrack->GetVolume()->GetName()<<G4endl;
	G4cout <<" +++"<< ThisStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() << G4endl;      
	G4cout <<" ---"<< ThisStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() << G4endl;
	G4cout << "StepLength = " << ThisStep->GetStepLength() << G4endl;
	G4cout << "Track energy = " << ThisTrack->GetTotalEnergy() << G4endl;
      }
      
      G4int curr_delim = ThisTrack->GetVolume()->GetName().find("_");
      G4String curr_gmad_name = ThisTrack->GetVolume()->GetName().substr(0,curr_delim);
      G4int delim = ThisTrack->GetNextVolume()->GetName().find("_");
      G4String gmad_name = ThisTrack->GetNextVolume()->GetName().substr(0,delim);
      if (curr_gmad_name != gmad_name && gmad_name!="World")
      //      if(ThisTrack->GetVolume()->GetName().contains("samp"))
	{ 
	  G4cout << curr_gmad_name << " " << gmad_name << G4endl;
	  G4ThreeVector pos = ThisTrack->GetPosition();
	  postponedEnergy+=ThisTrack->GetTotalEnergy();
	  	  
	  G4StackManager* SM = G4EventManager::GetEventManager()->GetStackManager();
	  if(DEBUG) G4cout << "Postponing track " << SM->GetNPostponedTrack() << G4endl;
	  if(SM->GetNPostponedTrack()!= nptwiss-1 ) { 
	    // postpone track and save its coordinates for twiss fit
	    ThisTrack->SetTrackStatus(fPostponeToNextEvent);

// 	    G4ThreeVector pos=ThisTrack->GetPosition();
// 	    G4ThreeVector momDir=ThisTrack->GetMomentumDirection();

	    // Get Translation and Rotation of Sampler Volume w.r.t the World Volume
	 //    G4AffineTransform tf(ThisTrack->GetTouchable()->GetHistory()->GetTopTransform().Inverse());
// 	    const G4RotationMatrix Rot=tf.NetRotation();
// 	    const G4ThreeVector Trans=-tf.NetTranslation();
	    
// 	    G4ThreeVector LocalPosition=pos+Trans; 
// 	    G4ThreeVector LocalDirection=Rot*momDir; 


	    //subtract the energy end position chnge due to SR when rescaling

	    //G4cout<<"changing track coordinates according to sr"<<G4endl;


	  //   // get the field for the local element (assuming 
// 	    // it doesn't chnage much when the particle travles through it


// 	    G4FieldManager* TheFieldManager= ThisTrack->GetVolume()->GetLogicalVolume()->GetFieldManager();

// 	    G4double dE = 0*GeV;
// 	    G4double dx = 0*mm;

// 	    if(TheFieldManager)
// 	      {
// 		const G4Field* pField = TheFieldManager->GetDetectorField() ;
// 		G4ThreeVector  globPosition = track.GetPosition() ;
// 		G4double  globPosVec[3], FieldValueVec[3] ;
// 		globPosVec[0] = globPosition.x() ;
// 		globPosVec[1] = globPosition.y() ;
// 		globPosVec[2] = globPosition.z() ;
		
// 		if(pField)
// 		  pField->GetFieldValue( globPosVec, FieldValueVec ) ; 
		
// 		G4double Blocal=
// 		  Blocal=sqrt(FieldValueVec[0]*FieldValueVec[0] + 
// 			      FieldValueVec[1]*FieldValueVec[1] +
// 			      FieldValueVec[2]*FieldValueVec[2] );

// 		G4cout<<"Blocal = "<<Blocal/tesla<<" tesla"<<G4endl;

// 		G4ThreeVector InitMag=track.GetMomentum();
		
// 		// transform to the local coordinate frame
		
// 		G4AffineTransform tf(track.GetTouchable()->GetHistory()->GetTopTransform().Inverse());
// 		const G4RotationMatrix Rot=tf.NetRotation();
// 		const G4ThreeVector Trans=-tf.NetTranslation();
// 		InitMag=Rot*InitMag; 
		
// 		G4double Rlocal=(InitMag.z()/GeV)/(0.299792458 * Blocal/tesla) *m;

// 		G4cout<<"Rlocal = "<<Rlocal/m<<" m"<<G4endl;

// 		dE = - 

// 	      };


	  
// 	    LocalPosition += G4ThreeVector(dx,0,0);
// 	    pos = LocalPosition - Trans;

// 	    ThisTrack->SetPosition(pos);
// 	    ThisTrack->SetKineticEnergy( ThisTrack->GetKineticEnergy() + dE );

	  //   G4cout<<"postponed track: "<<SM->GetNPostponedTrack()<<G4endl;
// 	    G4cout<<"r: "<<LocalPosition<<G4endl;
// 	    G4cout<<"rp: "<<LocalDirection<<G4endl;
	    //x.push_back(
	  }

	  // all tracks arrived - do rescaling
	  if(SM->GetNPostponedTrack()== nptwiss-1)
	    {
	      SM->TransferStackedTracks(fPostpone, fUrgent);
	      if(verbose) G4cout << "\nMean Energy: " << (postponedEnergy/nptwiss)/GeV << G4endl;

	      list<BDSAcceleratorComponent*>::const_iterator iBeam;
	      G4String type="";	      
	      for(iBeam=theBeamline.begin();iBeam!=theBeamline.end();iBeam++)
		{ 
		  if( (*iBeam)->GetName() == gmad_name)
		    {
		      type = (*iBeam)->GetType();
		      if(verbose) G4cout << "Next Element is: " << (*iBeam)->GetName() << G4endl;
		      if(verbose) G4cout << "Element Type: " << type << G4endl;
		      G4double old_P0 = BDSGlobals->GetBeamTotalEnergy();
		      G4double old_brho = 
			sqrt(pow(old_P0,2)- pow(electron_mass_c2,2))/(0.299792458 * (GeV/(tesla*m)));
		      G4double new_P0 = postponedEnergy/nptwiss;
		      G4double new_brho = 
			sqrt(pow(new_P0,2)- pow(electron_mass_c2,2))/(0.299792458 * (GeV/(tesla*m)));
		      
		      if(BDSGlobals->GetSynchRescale()) 
			{
			  (*iBeam)->SynchRescale(new_brho/old_brho);
			  if(verbose) G4cout << "Rescaling " << (*iBeam)->GetName() << "by: " << new_brho/old_brho << G4endl;
			  G4cout << "*";
			  G4cout.flush();
			}
		      break;
		    }
		}
	      postponedEnergy=0;
	    }
	  return;
	}
    }

  
  // ------------  output in case of verbose step ---------------------


  if(verboseStep && (!BDSGlobals->GetSynchRescale()) )
    {

	int ID=ThisTrack->GetTrackID();


	G4cout.precision(10);
	G4cout<<"This volume="<< ThisTrack->GetVolume()->GetName()<<G4endl;
	
	G4LogicalVolume* LogVol=ThisTrack->GetVolume()->GetLogicalVolume();
	G4cout<<"This log volume="<<LogVol->GetName() <<G4endl;
	
	G4cout<<"ID="<<ID<<" part="<<
	  ThisTrack->GetDefinition()->GetParticleName()<<
	  "Energy="<<ThisTrack->GetTotalEnergy()/GeV<<
	  " mom Px="
	      <<ThisTrack->GetMomentum()[0]/GeV<<
	  " Py="<<ThisTrack->GetMomentum()[1]/GeV<<
	  " Pz="<<ThisTrack->GetMomentum()[2]/GeV<<" vol="<<
	  ThisTrack->GetVolume()->GetName()<<G4endl;
	
	G4cout<<" Global Position="<<ThisTrack->GetPosition()<<G4endl;
	
	
	if(ThisTrack->GetMaterial()->GetName() !="LCVacuum")
	  G4cout<<"material="<<ThisTrack->GetMaterial()->GetName()<<G4endl;
	//	G4Exception(" wrong material");
	

	  G4VProcess* proc=(G4VProcess*)(ThisStep->GetPostStepPoint()->
	    GetProcessDefinedStep() );

       	  if(proc)G4cout<<" post-step process="<<proc->GetProcessName()<<G4endl<<G4endl;


	  proc=(G4VProcess*)(ThisStep->GetPreStepPoint()->
	    GetProcessDefinedStep() );

       	  if(proc)G4cout<<" pre-step process="<<proc->GetProcessName()<<G4endl<<G4endl;

	

      }


  // -------------  kill tracks according to cuts -------------------
  

  // this cuts apply to default region

  //G4LogicalVolume* LogVol=ThisTrack->GetVolume()->GetLogicalVolume();
  //G4Region* reg = LogVol->GetRegion();

  //G4cout<<"I am in "<<reg->GetName()<<G4endl;

  //if(reg->GetName() != "precision")
    {
      if(pName=="gamma")
	{
	  G4double photonCut = BDSGlobals->GetThresholdCutPhotons();
	  
	  if(ThisTrack->GetTotalEnergy()<photonCut)
	    {
	     
	      ThisTrack->SetTrackStatus(fStopAndKill);
	     
	    }
	}
      
      if(pName=="e-"||pName=="e+")
	{
	  if(ThisTrack->GetTotalEnergy()<BDSGlobals->GetThresholdCutCharged())
	    {
	
	      ThisTrack->SetTrackStatus(fStopAndKill);
	
	    }
	}
      
    }
  
   
        
}

//====================================================



