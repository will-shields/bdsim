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

#include<string>

extern G4int event_number;
extern G4bool verbose;

BDSEnergyCounterSD::BDSEnergyCounterSD(G4String name)
:G4VSensitiveDetector(name)
{
  collectionName.insert("EC_"+name);
  nMaxCopy=150;
  HitID = new G4int[150];
}

BDSEnergyCounterSD::~BDSEnergyCounterSD()
{delete [] HitID;}

void BDSEnergyCounterSD::Initialize(G4HCofThisEvent*)
{
  BDSEnergyCounterCollection = new BDSEnergyCounterHitsCollection
    (SensitiveDetectorName,collectionName[0]); 
  for(G4int i=0; i<nMaxCopy;i++)HitID[i]=-1;
}

G4bool BDSEnergyCounterSD::ProcessHits(G4Step*aStep,G4TouchableHistory*)
{ 
  G4double edep = aStep->GetTotalEnergyDeposit();
  G4int partID = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();
  G4int parentID = aStep->GetTrack()->GetParentID();
  G4double enrg;

  if(BDSGlobals->GetStopTracks())
    enrg = (aStep->GetTrack()->GetTotalEnergy() - aStep->GetDeltaEnergy());
  else
    enrg = edep;

  if (enrg==0.) return false;      
  

  G4int nCopy=aStep->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo();
  if(nCopy>nMaxCopy-1)
    {
      G4cout<<" BDSEnergyCounterSD: nCopy too large: nCopy="<<nCopy<<
	"nMaxCopy="<<nMaxCopy<<" Volume="<<
	aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()<<G4endl;
      G4Exception("Killing program in BDSEnergyCounterSD::ProcessHits");
    }

  G4double zpos=0.5*(aStep->GetPreStepPoint()->GetPosition().z()
		     + aStep->GetPostStepPoint()->GetPosition().z());

  
  if(verbose && BDSGlobals->GetStopTracks()) G4cout << "BDSEnergyCounterSD: Current Volume: " << aStep->GetTrack()->GetVolume()->GetName() <<"\tEvent: " << event_number << "\tEnergy: " << enrg/GeV << "GeV\tPosition: " << zpos/m <<"m"<< G4endl;

  if (HitID[nCopy]==-1)
    { 
      BDSEnergyCounterHit* ECHit= 
	new BDSEnergyCounterHit(nCopy,enrg,zpos*enrg,partID,parentID);
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

