/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/
#include "BDSGlobalConstants.hh"

#include "BDSEnergyCounterSD.hh"
#include "BDSEnergyCounterHit.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

#include "G4Navigator.hh"
#include "G4AffineTransform.hh"

#include "G4RunManager.hh"
#include "G4Version.hh"
#include<string>

extern G4int event_number;
extern G4bool verbose;

BDSEnergyCounterSD::BDSEnergyCounterSD(G4String name)
:G4VSensitiveDetector(name)
{
  collectionName.insert("EC_"+name);
  #define NMAXCOPY 5
  HitID = new G4int[NMAXCOPY];
}

BDSEnergyCounterSD::~BDSEnergyCounterSD()
{delete [] HitID;}

void BDSEnergyCounterSD::Initialize(G4HCofThisEvent*)
{
  BDSEnergyCounterCollection = new BDSEnergyCounterHitsCollection
    (SensitiveDetectorName,collectionName[0]); 
  for(G4int i=0; i<NMAXCOPY;i++)HitID[i]=-1;
}

G4bool BDSEnergyCounterSD::ProcessHits(G4Step*aStep,G4TouchableHistory*)
{ 
  if(BDSGlobals->GetStopTracks())
  #if G4VERSION_NUMBER > 940
    enrg = (aStep->GetTrack()->GetTotalEnergy() - aStep->GetTotalEnergyDeposit()); // Why subtract the energy deposit of the step? Why not add?
#else
  enrg = (aStep->GetTrack()->GetTotalEnergy() - aStep->GetDeltaEnergy()); // Why subtract the energy deposit of the step? Why not add?
#endif
  else
    enrg = aStep->GetTotalEnergyDeposit();
#ifdef DEBUG
  G4cout << "BDSEnergyCounterSD> enrg = " << enrg << G4endl;
#endif
  if (enrg==0.) return false;      
  

  G4int nCopy=aStep->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo();
#ifdef DEBUG
  if(nCopy>0){
    G4cout << "BDSEnergyCounterSD::ProcessHits> nCopy = " << nCopy << G4endl;
  }
#endif
  if(nCopy>NMAXCOPY-1)
    {
      G4cerr<<" BDSEnergyCounterSD: nCopy too large: nCopy="<<nCopy<<
	"NMAXCOPY="<<NMAXCOPY<<" Volume="<<
	aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()<<G4endl;
      G4Exception("Killing program in BDSEnergyCounterSD::ProcessHits");
    }

  // Get Translation and Rotation of Sampler Volume w.r.t the World Volume
  // as described in Geant4 FAQ's: http://geant4.cern.ch/support/faq.shtml
  G4AffineTransform tf=(aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform());
  G4ThreeVector pos = aStep->GetTrack()->GetPosition();
  G4ThreeVector momDir= aStep->GetTrack()->GetMomentumDirection();

  G4ThreeVector LocalPosition = tf.TransformPoint(pos);
  G4ThreeVector LocalDirection = tf.TransformAxis(momDir);

  zpos=0.5*(aStep->GetPreStepPoint()->GetPosition().z()
	    + aStep->GetPostStepPoint()->GetPosition().z());

  xpos=aStep->GetTrack()->GetPosition().x();//LocalPosition.x();
  ypos=aStep->GetTrack()->GetPosition().y();//LocalPosition.y();
  //  zpos=LocalPosition.z();

  
  if(verbose && BDSGlobals->GetStopTracks()) G4cout << "BDSEnergyCounterSD: Current Volume: " << aStep->GetTrack()->GetVolume()->GetName() <<"\tEvent: " << event_number << "\tEnergy: " << enrg/GeV << "GeV\tPosition: " << zpos/m <<"m"<< G4endl;

  if (HitID[nCopy]==-1)
    { 
      BDSEnergyCounterHit* ECHit= 
	new BDSEnergyCounterHit(nCopy,enrg,zpos*enrg,aStep->GetTrack()->GetDefinition()->GetPDGEncoding(),aStep->GetTrack()->GetParentID(), aStep->GetTrack()->GetVolume()->GetName(), aStep->GetTrack()->GetWeight(), xpos, ypos);
      HitID[nCopy]= BDSEnergyCounterCollection->insert(ECHit)-1;
    }
  else
    {
      (*BDSEnergyCounterCollection)[HitID[nCopy]]-> AddEnergy(enrg);
      (*BDSEnergyCounterCollection)[HitID[nCopy]]-> 
	AddEnergyWeightedPosition(enrg*zpos);
    }
  
    
  if(BDSGlobals->GetStopTracks())
    aStep->GetTrack()->SetTrackStatus(fStopAndKill);

  
  return true;
}

void BDSEnergyCounterSD::EndOfEvent(G4HCofThisEvent*HCE)
{
  G4int HCID = GetCollectionID(0); 
  HCE->AddHitsCollection( HCID, BDSEnergyCounterCollection );
}

void BDSEnergyCounterSD::clear()
{} 

void BDSEnergyCounterSD::DrawAll()
{} 

void BDSEnergyCounterSD::PrintAll()
{} 

