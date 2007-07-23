/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 

   Modified 22.03.05 by J.C.Carter, Royal Holloway, Univ. of London.
   Removed StringFromInt function - using BDSGlobals version
   Added/Changed Sampler code for Plane Sampler or Cylinder Sampler (GABs Code)
*/



const int DEBUG = 0;

//======================================================
//======================================================
#include "BDSGlobalConstants.hh" // must be first in include list

#include "BDSEventAction.hh"

#include <ctime> 
#include <vector>
#include <algorithm>

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4StackManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"

#include "G4FieldManager.hh"
#include "G4ChordFinder.hh"

#include "BDSSampler.hh"
#include "BDSSamplerHit.hh"
#include "BDSEnergyCounterHit.hh"

#include "BDSLWCalorimeter.hh"
#include "BDSLWCalorimeterHit.hh"

#include "BDSSynchrotronRadiation.hh"

#include "BDSAcceleratorComponent.hh"

#include "BDSOutput.hh"

#include "BDSRunManager.hh"

typedef list<BDSAcceleratorComponent*>  BDSBeamline;
BDSBeamline theBeamline;

typedef std::vector<G4int> MuonTrackVector;
MuonTrackVector* theMuonTrackVector;

typedef std::map<G4String,int> LogVolCountMap;
extern LogVolCountMap* LogVolCount;

typedef list<BDSEnergyCounterSD*>  ECList;
extern ECList* theECList;

extern G4double BDSeBremFireDist;
extern G4double BDSeBremZMin,BDSeBremZMax;

// link to G4eBremsstrahlung.cc and G4GammaConversion.cc for muon studies
extern G4bool BDSeBremsFiredThisEvent;
extern G4double BDSeBremFireDist;

G4double htot;
G4int event_number;
G4bool FireLaserCompton;


extern BDSOutput bdsOutput;
extern G4String outputFilename;
extern G4bool isBatch;


extern G4bool verbose;      // run options
extern G4bool verboseStep;
extern G4bool verboseEvent;
extern G4int verboseEventNumber;
extern G4bool isBatch;

extern int nptwiss;


//======================================================

BDSEventAction::BDSEventAction()
:SamplerCollID_plane(-1),SamplerCollID_cylin(-1),
LWCalorimeterCollID(-1),drawFlag("all")
{ 
  if(isBatch) printModulo=1000;
  else printModulo=1;
  
  itsOutputFileNumber=1;
  
  itsRecordSize=1024;
    
  LastComp=NULL;
}

//======================================================

BDSEventAction::~BDSEventAction()
{
}

//======================================================

void BDSEventAction::BeginOfEventAction(const G4Event* evt)
{ 
  event_number = evt->GetEventID();
  htot=0.;

  
  if(BDSGlobals->GetStoreMuonTrajectories())
    theMuonTrackVector=new MuonTrackVector();
  
  
   if(BDSGlobals->DoTwiss())
    {
      if(event_number==0) {
	if(!BDSGlobals->GetSynchRescale())G4cout << "\n---> Calculating Twiss Parameters"<<G4endl;
	if(BDSGlobals->GetSynchRescale()) G4cout<<"\n---> Calculating Twiss Parameters and Rescaling magnets" <<G4endl;
      }
    }
  else
    {
      if ((event_number+1)%printModulo ==0)
	{
	  G4cout << "\n---> Begin of event: " << event_number ;
	  
// 	  if(BDSGlobals->GetUseTimer())
// 	    { 
// 	      BDSGlobals->GetTimer()->Stop();
// 	      G4cout<<" Time: "<<*BDSGlobals->GetTimer();
// 	      BDSGlobals->GetTimer()->Start();
// 	    }
	  G4cout << G4endl;
	}
    }

  if(verboseEvent) G4cout<<"Begin of event: "<<event_number<<G4endl ;

  
  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  //G4cout << bdsOutput.GetPlaneSamplerNumber() << " < PlaneSamplers" << G4endl;
  //if( bdsOutput.GetPlaneSamplerNumber() > 0)
  {   
    //if (SamplerCollID_plane==-1)
      SamplerCollID_plane = SDman->GetCollectionID("Sampler_plane");
  }
  
//if( bdsOutput.GetCylinderSamplerNumber() > 0 )
  {   
    //if (SamplerCollID_cylin==-1)
    //SamplerCollID_cylin = SDman->GetCollectionID("Sampler_cylinder");
  }
  
  //if( bdsOutput.GetLWCalorimeterNumber() > 0 )
  {
    //if (LWCalorimeterCollID==-1) 
    //LWCalorimeterCollID = SDman->GetCollectionID("LWCalorimeterCollection");
  }
  FireLaserCompton=true;
}

//======================================================

void BDSEventAction::EndOfEventAction(const G4Event* evt)
{
  if(BDSGlobals->GetSynchRescale())
    {
      if(event_number==nptwiss-1)
	{
	  G4cout << "\n---> Done" <<G4endl;
	  G4EventManager::GetEventManager()->GetStackManager()->clear();
	}
    }

  if(DEBUG) G4cout<<"BDSEventAction : end of event action"<<G4endl;
 
  if(verboseEvent || verboseEventNumber == event_number)
    G4cout<<"processing end of event"<<G4endl;

  G4SDManager * SDman = G4SDManager::GetSDMpointer();

  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  
  BDSSamplerHitsCollection*  SampHC=NULL;
  //BDSLWCalorimeterHitsCollection* LWCalHC=NULL;
  BDSEnergyCounterHitsCollection* BDSEnergyCounter_HC=NULL;

  
  if(DEBUG) G4cout<<"processing planar hits collection"<<G4endl;
  // are there any planar samplers?
  if(SamplerCollID_plane>=0)
    SampHC = (BDSSamplerHitsCollection*)(HCE->GetHC(SamplerCollID_plane));
  
  // if so, record the hits for each sampler 

  if(SampHC)  bdsOutput.WriteHits(SampHC);

	
  // are there any cylindrical samplers?
  // if so, record the hits

  SampHC=NULL;

  if(DEBUG) G4cout<<"processing cylinder hits collection"<<G4endl;

  if(SamplerCollID_cylin>=0)
    SampHC = (BDSSamplerHitsCollection*)(HCE->GetHC(SamplerCollID_cylin));


  if (SampHC) bdsOutput.WriteHits(SampHC);
  

  // are there any Laser wire calorimeters?
  // TODO : check it !!! at present not writing LW stuff
  // remember to uncomment LWCalHC above if using this

//   if(LWCalorimeterCollID>=0) 
//     LWCalHC=(BDSLWCalorimeterHitsCollection*)(HCE->GetHC(LWCalorimeterCollID));


  // create energy loss histogram
  
  for(iEC=theECList->begin();iEC!=theECList->end();iEC++)
    {
      G4String name=(*iEC)->GetCollectionName(0);
      
      G4int BDSEnergyCounter_ID= SDman->GetCollectionID(name);
      
      if(BDSEnergyCounter_ID>=0)
	{
	  BDSEnergyCounter_HC=
	    (BDSEnergyCounterHitsCollection*)(HCE->GetHC(BDSEnergyCounter_ID));
	
	  if(BDSEnergyCounter_HC) 
	    bdsOutput.WriteEnergyLoss(BDSEnergyCounter_HC);
	}
    }


  // f 0 events per ntuples - set max allowed events per ntuples  

  int evntsPerNtuple = BDSGlobals->GetNumberOfEventsPerNtuple();

  if(evntsPerNtuple>0)
    if ((event_number+1)% evntsPerNtuple == 0)
      {
	if(DEBUG) G4cout<<"writing to file "<<G4endl;
	// notify the output about the event end
	// this can be used for splitting output files etc.
	bdsOutput.Commit(itsOutputFileNumber++);
	if(DEBUG) G4cout<<"done"<<G4endl;
      }


  // Save interesting trajectories:
  G4TrajectoryContainer* TrajCont=evt->GetTrajectoryContainer();


  if(!TrajCont) return;

  
  TrajectoryVector* TrajVec=TrajCont->GetVector();


  if(BDSGlobals->GetStoreTrajectory()&& TrajVec) // store trajectories of primaries
    {
      bdsOutput.WriteTrajectory(TrajVec);
    }


  if(BDSGlobals->GetStoreMuonTrajectories()&& TrajVec)
    {

      if(DEBUG) G4cout<<"PROCESSING MUON TRAJECTORY VECTOR"<<G4endl;

      bdsOutput.WriteTrajectory(TrajVec);
   
    }

  if(BDSGlobals->GetStoreNeutronTrajectories()&& TrajVec)
    {
      
      if(DEBUG) G4cout<<"PROCESSING NEUTRON TRAJECTORY VECTOR"<<G4endl;
      
      bdsOutput.WriteTrajectory(TrajVec);
      
    }
  

  // needed to draw trajectories and hits:
  if(!isBatch) evt->Draw();
  
  // clear out the remaining trajectories
  TrajectoryVector::iterator iT;
  if(TrajVec)
    {
      for(iT=TrajVec->begin();iT<TrajVec->end();iT++)
	{
	  G4Trajectory* Traj=(G4Trajectory*)(*iT);
	  delete Traj;
	  TrajVec->erase(iT);
	  iT--;
	}
    }
  
  if(DEBUG) G4cout<<"End of event ended"<<G4endl;
}

//======================================================
