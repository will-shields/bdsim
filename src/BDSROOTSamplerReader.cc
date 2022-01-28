/* 
Beam Delivery Simulation (BDSIM) Copyright (C) Royal Holloway, 
University of London 2001 - 2022.

This file is part of BDSIM.

BDSIM is free software: you can redistribute it and/or modify 
it under the terms of the GNU General Public License as published 
by the Free Software Foundation version 3 of the License.

BDSIM is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with BDSIM.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "BDSBunchEventGenerator.hh"
#include "BDSDebug.hh"
#include "BDSException.hh"
#include "BDSOutputLoaderSampler.hh"
#include "BDSROOTSamplerReader.hh"
#include "BDSParticleCoords.hh"
#include "BDSParticleCoordsFull.hh"
#include "BDSParticleCoordsFullGlobal.hh"
#include "BDSPhysicalConstants.hh"
#include "BDSPrimaryVertexInformation.hh"
#include "BDSPrimaryVertexInformationV.hh"
#include "BDSUtilities.hh"

#include "G4Event.hh"
#include "G4LorentzVector.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4RunManager.hh"
#include "G4TransportationManager.hh"
#include "G4VSolid.hh"

#include "CLHEP/Units/SystemOfUnits.h"

#include "globals.hh"

#include <utility>

BDSROOTSamplerReader::BDSROOTSamplerReader(const G4String& distrType,
				 const G4String& fileNameIn,
				 BDSBunchEventGenerator* bunchIn,
                                           G4bool removeUnstableWithoutDecayIn,
				 G4bool warnAboutSkippedParticlesIn):
  currentFileEventIndex(0),
  nEventsInFile(0),
  reader(nullptr),
  fileName(fileNameIn),
  bunch(bunchIn),
  removeUnstableWithoutDecay(removeUnstableWithoutDecayIn),
  warnAboutSkippedParticles(warnAboutSkippedParticlesIn),
  worldSolid(nullptr)
{
  std::pair<G4String, G4String> ba = BDS::SplitOnColon(distrType); // before:after
  samplerName = ba.second;
  G4cout << __METHOD_NAME__ << "reading sampler named: \"" << samplerName << "\"" << G4endl;
  referenceBeamMomentumOffset = bunch->ReferenceBeamMomentumOffset();
  reader = new BDSOutputLoaderSampler(fileName, samplerName);
  nEventsInFile = reader->NEventsInFile();
}

BDSROOTSamplerReader::~BDSROOTSamplerReader()
{
  delete reader;
}

void BDSROOTSamplerReader::GeneratePrimaryVertex(G4Event* anEvent)
{
  if (!reader)
    {throw BDSException(__METHOD_NAME__, "no file reader available");}
  
  G4int nParticles = 0;
  G4PrimaryVertex* vertex = nullptr;
  while (nParticles < 1)
    {
      if (currentFileEventIndex > nEventsInFile)
	{
	  G4cout << __METHOD_NAME__ << "End of file reached. Return to beginning of file for next event." << G4endl;
	  currentFileEventIndex = 0;
	}
      vertex = ReadSingleEvent(currentFileEventIndex);
      nParticles = vertex->GetNumberOfParticle();
      currentFileEventIndex++;
    }
  anEvent->AddPrimaryVertex(vertex);
}

void BDSROOTSamplerReader::RecreateAdvanceToEvent(G4int eventOffset)
{
  G4cout << "BDSROOTSamplerLoader::RecreateAdvanceToEvent> Advancing file to event: " << eventOffset << G4endl;
  for (G4int i = 0; i < eventOffset; i++)
    {
      G4Event* event = new G4Event();
      GeneratePrimaryVertex(event);
      delete event;
    }
}

void BDSROOTSamplerReader::ReadPrimaryParticlesFloat(G4long index)
{
  particles.clear();
  const auto sampler = reader->SamplerDataFloat(index);
  
  int n = sampler->n;
  for (int i = 0; i < n; i++)
    {
      G4int pdgID = (G4int)sampler->partID[i];
      G4double xp = (G4double)sampler->xp[i];
      G4double yp = (G4double)sampler->yp[i];
      G4double zp = (G4double)sampler->zp[i];
      G4double p  = (G4double)sampler->p[i];
      G4ThreeVector momentum = G4ThreeVector(xp,yp,zp) * p * CLHEP::GeV;
      auto g4prim = new G4PrimaryParticle(pdgID, momentum.x(), momentum.y(), momentum.z());
      particles.push_back(g4prim);
    }
}

void BDSROOTSamplerReader::ReadPrimaryParticlesDouble(G4long index)
{
  particles.clear();
  const auto sampler = reader->SamplerDataDouble(index);
  
  int n = sampler->n;
  for (int i = 0; i < n; i++)
    {
      G4int pdgID = (G4int)sampler->partID[i];
      G4double xp = (G4double)sampler->xp[i];
      G4double yp = (G4double)sampler->yp[i];
      G4double zp = (G4double)sampler->zp[i];
      G4double p  = (G4double)sampler->p[i];
      G4ThreeVector momentum = G4ThreeVector(xp,yp,zp) * p;
      auto g4prim = new G4PrimaryParticle(pdgID, momentum.x(), momentum.y(), momentum.z());
      particles.push_back(g4prim);
    }
}

G4PrimaryVertex* BDSROOTSamplerReader::ReadSingleEvent(G4long index)
{
  if (reader->DoublePrecision())
    {ReadPrimaryParticlesDouble(index);}
  else
    {ReadPrimaryParticlesFloat(index);}
  
  BDSParticleCoordsFull centralCoords = bunch->GetNextParticleLocal();
  // do the transform for the reference particle (if any transform) and use that
  BDSParticleCoordsFullGlobal centralCoordsGlobal = bunch->ApplyTransform(centralCoords);
  G4PrimaryVertex* g4vtx = new G4PrimaryVertex(centralCoordsGlobal.global.x,
					       centralCoordsGlobal.global.y,
					       centralCoordsGlobal.global.z,
					       centralCoordsGlobal.global.T);
  
  double overallWeight = 1.0;
  
  std::vector<BDSPrimaryVertexInformation> vertexInfos;
  G4int nParticlesSkipped = 0;
  
  for (const auto& particle : particles)
    {
      // if the particle definition isn't found from the pdgcode in the construction
      // of G4PrimaryParticle, it means the mass, charge, etc will be wrong - don't
      // stack this particle into the vertex.
      // technically shouldn't happen as bdsim produced this output... but safety first
      const G4ParticleDefinition* pd = particle->GetParticleDefinition();
      G4bool deleteIt = !pd;
      if (pd && removeUnstableWithoutDecay)
        {deleteIt = !(pd->GetPDGStable()) && !pd->GetDecayTable();}
      
      if (deleteIt)
        {
#ifdef BDSDEBUG
          G4cout << __METHOD_NAME__ << "skipping particle with PDG ID: " << pdgcode << G4endl;
#endif
          nParticlesSkipped++;
          continue;
        }
  
      G4ThreeVector unitMomentum = particle->GetMomentumDirection();
      unitMomentum.transform(referenceBeamMomentumOffset);
      G4double rp = unitMomentum.perp();
      
      BDSParticleCoordsFull local(centralCoords.x,
				  centralCoords.y,
				  centralCoords.z,
				  unitMomentum.x(),
				  unitMomentum.y(),
				  unitMomentum.z(),
				  centralCoords.T,
				  centralCoords.s,
				  particle->GetTotalEnergy(),
				  overallWeight);

      if (!bunch->AcceptParticle(local, rp, particle->GetKineticEnergy(), particle->GetPDGcode()))
	{
	  nParticlesSkipped++;
	  continue;
	}
      
      BDSParticleCoordsFullGlobal fullCoords = bunch->ApplyTransform(local);
      // ensure it's in the world - not done in primary generator action for event generators
      if (!VertexInsideWorld(fullCoords.global.Position()))
	{
	  nParticlesSkipped++;
	  continue;
	}  
      
      G4double brho     = 0;
      G4double charge   = particle->GetCharge();
      G4double momentum = particle->GetTotalMomentum();
      if (BDS::IsFinite(charge)) // else leave as 0
	{
	  brho = momentum / CLHEP::GeV / BDS::cOverGeV / charge;
	  brho *= CLHEP::tesla*CLHEP::m; // rigidity (in Geant4 units)
	}
      BDSPrimaryVertexInformation vertexInfo(fullCoords,
					     particle->GetTotalMomentum(),
					     particle->GetCharge(),
					     brho,
					     particle->GetMass(),
					     particle->GetPDGcode());
      vertexInfos.emplace_back(vertexInfo);

      // update momentum in case of a beam line transform
      auto prim = new G4PrimaryParticle(*particle);
      prim->SetMomentumDirection(G4ThreeVector(fullCoords.global.xp,
					       fullCoords.global.yp,
					       fullCoords.global.zp));
      g4vtx->SetPrimary(prim);
    }
  
  if (nParticlesSkipped > 0 && warnAboutSkippedParticles)
    {G4cout << __METHOD_NAME__ << nParticlesSkipped << " particles skipped" << G4endl;}
  g4vtx->SetUserInformation(new BDSPrimaryVertexInformationV(vertexInfos));
  
  // clear initially loaded ones
  for (auto p : particles)
    {delete p;}
  return g4vtx;
}

G4bool BDSROOTSamplerReader::VertexInsideWorld(const G4ThreeVector& pos) const
{
  if (!worldSolid)
    {// cache the world solid
      G4Navigator* navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
      G4VPhysicalVolume* world = navigator->GetWorldVolume();
      worldSolid = world->GetLogicalVolume()->GetSolid();
    }
  EInside qinside = worldSolid->Inside(pos);
  return qinside == kInside;
}
