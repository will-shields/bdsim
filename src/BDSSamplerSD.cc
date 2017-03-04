#include "BDSGlobalConstants.hh" 
#include "BDSDebug.hh"
#include "BDSParticle.hh"
#include "BDSRunManager.hh"
#include "BDSSamplerRegistry.hh"
#include "BDSSamplerSD.hh"
#include "BDSSamplerHit.hh"
#include "BDSTrajectory.hh"

#include "globals.hh" // geant4 types / globals
#include "G4AffineTransform.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4ThreeVector.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"

#include <vector>

BDSSamplerSD::BDSSamplerSD(G4String name):
  G4VSensitiveDetector("sampler/" + name),
  itsHCID(-1),
  SamplerCollection(nullptr),
  itsCollectionName(name),
  registry(nullptr),
  globals(nullptr)
{
  collectionName.insert(name);
}

BDSSamplerSD::~BDSSamplerSD()
{;}

void BDSSamplerSD::Initialize(G4HCofThisEvent* HCE)
{
  // Create Sampler hits collection
  SamplerCollection = new BDSSamplerHitsCollection(GetName(),itsCollectionName);

  // Record id for use in EventAction to save time - slow string lookup by collection name
  if (itsHCID < 0)
    {itsHCID = G4SDManager::GetSDMpointer()->GetCollectionID(SamplerCollection);}
  HCE->AddHitsCollection(itsHCID,SamplerCollection);

  registry = BDSSamplerRegistry::Instance(); // cache pointer to registry
  globals  = BDSGlobalConstants::Instance(); // cache pointer to globals
}

G4bool BDSSamplerSD::ProcessHits(G4Step* aStep, G4TouchableHistory* /*readOutTH*/)
{
  // Do not store hit if the particle pre step point is not on the boundary
  G4StepPoint* postStepPoint = aStep->GetPostStepPoint();
  if(postStepPoint->GetStepStatus() != fGeomBoundary)
    {
#ifdef BDSDEBUG
      G4cout << __METHOD_NAME__ << "not storing as not on geometry boundary" << G4endl;
#endif
      return false; // this step was not stored
    }
  
  G4Track* track    = aStep->GetTrack();
  G4int TrackID     = track->GetTrackID();           // unique ID of track
  G4int ParentID    = track->GetParentID();          // unique ID of track's mother
  G4double t        = track->GetGlobalTime();        // time since beginning of event
  G4double energy   = track->GetTotalEnergy();       // total track energy
  G4int turnstaken  = globals->TurnsTaken();         // turn Number
  G4ThreeVector pos = track->GetPosition();          // current particle position (global)
  G4ThreeVector mom = track->GetMomentumDirection(); // current particle direction (global)
  G4double weight   = track->GetWeight();            // weighting
  
  // The copy number of physical volume is the sampler ID in BDSIM scheme.
  // track->GetVolume gives the volume in the mass world. pre/postStepPoint->->GetVolume()
  // give the ones in the parallel sampler world this SD is attached to. If the post step
  // point is on a boundary, it belongs to the next volume - ie not the one of interest
  // so always use the pre step point for volume identification.
  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  G4int samplerID   = preStepPoint->GetTouchable()->GetVolume()->GetCopyNo();
  
#ifdef BDSDEBUG
  G4cout << __METHOD_NAME__ << "Sampler ID: " << samplerID << G4endl;
#endif

  //Initialize variables for the local position and direction
  G4ThreeVector localPosition;
  G4ThreeVector localDirection;
  
  // Get coordinate transform and prepare local coordinates
  G4Transform3D localToGlobal = registry->GetTransformInverse(samplerID);
  if (localToGlobal == G4Transform3D::Identity) // no transform was provided - look it up
    {
#ifdef BDSDEBUG
      G4cout << __METHOD_NAME__ << "Getting transform dynamically from geometry." << G4endl;
#endif
      // Transform not provided so look up geometry and get the transform. OK in mass world
      // but error prone in parallel worlds for very thin volumes.
      // NOTE: we're looking up mass world here!
      G4AffineTransform tf = preStepPoint->GetTouchableHandle()->GetHistory()->GetTopTransform();
      localPosition  = tf.TransformPoint(pos);
      localDirection = tf.TransformAxis(mom);
    }
  else
    {
      //If the local to global transform3d is defined...
      // Cast 3 vector to 'point' to transform position (required to be explicit for * operator)
      localPosition  = localToGlobal * (HepGeom::Point3D<G4double>)pos;
      // Now, if the sampler is infinitely thin, the local z should be 0, but it's finite.
      // Account for this by purposively setting local z to be 0.
      localPosition.setZ(0.0);
      // Cast 3 vector to 3 vector to transform vector (required to be explicit for * operator)
      localDirection = localToGlobal * (HepGeom::Vector3D<G4double>)mom;
    }

  BDSParticle local(localPosition,localDirection,energy,t);
  
#ifdef BDSDEBUG
  G4cout << __METHOD_NAME__ << "Local coordinates: " << local << G4endl;
#endif

  const BDSSamplerInfo& info = registry->GetInfo(samplerID);
  G4String samplerName = info.Name();
  G4double s           = info.SPosition();
  G4int beamlineIndex  = info.BeamlineIndex();
  
  G4int nEvent = BDSRunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  nEvent += globals->EventNumberOffset();
  G4int    PDGtype     = track->GetDefinition()->GetPDGEncoding();
  G4String pName       = track->GetDefinition()->GetParticleName();

  // more advanced / detailed information
  G4ThreeVector vtx               = track->GetVertexPosition();
  G4ThreeVector dir               = track->GetVertexMomentumDirection();
  G4ThreeVector posLastScatter    = G4ThreeVector(); //bdsTraj->GetPositionOfLastScatter(track);
  G4ThreeVector momDirLastScatter = G4ThreeVector(); // bdsTraj->GetMomDirAtLastScatter(track);
  G4double timeLastScatter        = 0.0;             // bdsTraj->GetTimeAtLastScatter(track);
  G4double energyLastScatter      = 0.0;             // bdsTraj->GetEnergyAtLastScatter(track);
  G4double vertexEnergy           = 0.0;             // track->GetVertexKineticEnergy() + track->GetParticleDefinition()->GetPDGMass();
  G4double vertexTime             = 0.0;             // bdsTraj->GetTimeAtVertex(track);

  // store production/scatter point
  BDSParticle lastScatter(posLastScatter,momDirLastScatter,energyLastScatter,timeLastScatter);
  BDSParticle production(vtx,dir,vertexEnergy,vertexTime); //production point
  BDSParticle global(pos,mom,energy,t); // global point
 
  // process that creating the particle
  G4String process = "";
  if(track->GetCreatorProcess()) 
    {process = track->GetCreatorProcess()->GetProcessName();}
  
  BDSSamplerHit* smpHit = new BDSSamplerHit(samplerName,
                                            samplerID,
                                            globals->GetInitialPoint(),
                                            production,
                                            lastScatter,
                                            local,
                                            global,
                                            s,
                                            weight,
                                            PDGtype,
                                            nEvent,
                                            ParentID,
                                            TrackID,
                                            turnstaken,
                                            process,
                                            beamlineIndex);
  
#ifdef BDSDEBUG
  G4cout << __METHOD_NAME__ << *smpHit;
#endif
  SamplerCollection->insert(smpHit);

  return true;    //The hit was stored
}
