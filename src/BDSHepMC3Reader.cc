/* 
Beam Delivery Simulation (BDSIM) Copyright (C) Royal Holloway, 
University of London 2001 - 2019.

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
#ifdef USE_HEPMC3

#include "BDSBunch.hh"
#include "BDSDebug.hh"
#include "BDSEventGeneratorFileType.hh"
#include "BDSException.hh"
#include "BDSHepMC3Reader.hh"
#include "BDSParticleCoords.hh"
#include "BDSParticleCoordsFull.hh"
#include "BDSParticleCoordsFullGlobal.hh"
#include "BDSPhysicalConstants.hh"
#include "BDSPrimaryVertexInformation.hh"
#include "BDSPrimaryVertexInformationV.hh"
#include "BDSUtilities.hh"

#include "G4Event.hh"
#include "G4LorentzVector.hh"
#include "G4PhysicalConstants.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4TransportationManager.hh"

#include "HepMC3/Attribute.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Reader.h"
#include "HepMC3/ReaderAscii.h"
#include "HepMC3/ReaderAsciiHepMC2.h"
#include "HepMC3/ReaderHEPEVT.h"
#include "HepMC3/ReaderLHEF.h"
#ifdef HEPMC3_ROOTIO
#include "HepMC3/ReaderRoot.h"
#include "HepMC3/ReaderRootTree.h"
#endif

#include "CLHEP/Units/SystemOfUnits.h"

#include "globals.hh"

#include <utility>

BDSHepMC3Reader::BDSHepMC3Reader():
  hepmcEvent(nullptr),
  reader(nullptr)
{;}

BDSHepMC3Reader::BDSHepMC3Reader(const G4String& distrType,
				 const G4String& fileNameIn,
				 BDSBunch*       bunchIn):
  hepmcEvent(nullptr),
  reader(nullptr),
  bunch(bunchIn)
{
  std::pair<G4String, G4String> ba = BDS::SplitOnColon(distrType); // before:after
  BDSEventGeneratorFileType fileType = BDS::DetermineEventGeneratorFileType(ba.second);

  switch (fileType.underlying())
    {
    case BDSEventGeneratorFileType::hepmc2:
      {reader = new HepMC3::ReaderAsciiHepMC2(fileNameIn); break;}
    case BDSEventGeneratorFileType::hepmc3:
      {reader = new HepMC3::ReaderAscii(fileNameIn); break;}
    case BDSEventGeneratorFileType::hpe:
      {reader = new HepMC3::ReaderHEPEVT(fileNameIn); break;}
#ifdef HEPMC3_ROOTIO
    case BDSEventGeneratorFileType::root:
      {reader = new HepMC3::ReaderRoot(fileNameIn); break;}
    case BDSEventGeneratorFileType::treeroot:
      {reader = new HepMC3::ReaderRootTree(fileNameIn); break;}
#else
    case BDSEventGeneratorFileType::root:
    case BDSEventGeneratorFileType::treeroot:
      {
	throw BDSException(__METHOD_NAME__, "HEPMC3 installation not compiled with ROOT support so can't load root file.");
	break;
      }
#endif
    case BDSEventGeneratorFileType::lhef:
      {reader = new HepMC3::ReaderLHEF(fileNameIn); break;}
    default:
      {break;}
    }
}

BDSHepMC3Reader::~BDSHepMC3Reader()
{
  delete hepmcEvent;
  delete reader;
}

void BDSHepMC3Reader::GeneratePrimaryVertex(G4Event* anEvent)
{
  if (!reader)
    {throw BDSException(__METHOD_NAME__, "no file reader available");}
  
  delete hepmcEvent;
  hepmcEvent = new HepMC3::GenEvent();

  bool readEventOK = reader->read_event(*hepmcEvent);
  if (!readEventOK)
    {anEvent->SetEventAborted(); return;}
  
  HepMC2G4(hepmcEvent, anEvent);
}

void BDSHepMC3Reader::HepMC2G4(const HepMC3::GenEvent* hepmcevt,
			       G4Event* g4event)
{
  BDSParticleCoordsFull centralCoords = bunch->GetNextParticleLocal();
  G4PrimaryVertex* g4vtx = new G4PrimaryVertex(centralCoords.x,
					       centralCoords.y,
					       centralCoords.z,
					       centralCoords.T);

  double overallWeight = hepmcevt->weight();
  std::vector<BDSPrimaryVertexInformation> vertexInfos;
  for (const auto& particlePtr : hepmcevt->particles())
    {
      const HepMC3::GenParticle* particle = particlePtr.get();
      if (!particle->children().empty())
	{continue;} // this particle is not at the end of the tree - ignore
      
      int pdgcode = particle->pdg_id();
      HepMC3::FourVector fv = particle->momentum();
      G4LorentzVector p(fv.px(), fv.py(), fv.pz(), fv.e());
      G4double px = p.x() * CLHEP::GeV;
      G4double py = p.y() * CLHEP::GeV;
      G4double pz = p.z() * CLHEP::GeV;
      G4ThreeVector unitMomentum(px,py,pz);
      unitMomentum = unitMomentum.unit();
      
      G4PrimaryParticle* g4prim = new G4PrimaryParticle(pdgcode, px, py, pz);

      BDSParticleCoordsFull local(centralCoords.x,
				  centralCoords.y,
				  centralCoords.z,
				  unitMomentum.x(),
				  unitMomentum.y(),
				  unitMomentum.z(),
				  centralCoords.T,
				  centralCoords.s,
				  g4prim->GetTotalEnergy(),
				  overallWeight);
      
      BDSParticleCoordsFullGlobal fullCoords = bunch->ApplyTransform(local);
      G4double brho     = 0;
      G4double charge   = g4prim->GetCharge();
      G4double momentum = g4prim->GetTotalMomentum();
      if (BDS::IsFinite(charge)) // else leave as 0
	{
	  brho = momentum / CLHEP::GeV / BDS::cOverGeV / charge;
	  brho *= CLHEP::tesla*CLHEP::m; // rigidity (in Geant4 units)
	}
      BDSPrimaryVertexInformation vertexInfo(fullCoords,
					     g4prim->GetCharge(),
					     brho,
					     g4prim->GetMass());
      vertexInfos.emplace_back(vertexInfo);

      // update momentum in case of a beam line transform
      g4prim->SetMomentumDirection(G4ThreeVector(fullCoords.global.xp,
						 fullCoords.global.yp,
						 fullCoords.global.zp));
      
      g4vtx->SetPrimary(g4prim);
    }

  g4vtx->SetUserInformation(new BDSPrimaryVertexInformationV(vertexInfos));
  
  g4event->AddPrimaryVertex(g4vtx);
}

G4bool BDSHepMC3Reader::CheckVertexInsideWorld(const G4ThreeVector& pos) const
{
  G4Navigator* navigator= G4TransportationManager::GetTransportationManager()
                                                 -> GetNavigatorForTracking();

  G4VPhysicalVolume* world= navigator-> GetWorldVolume();
  G4VSolid* solid = world->GetLogicalVolume()->GetSolid();
  EInside qinside = solid->Inside(pos);

  if( qinside != kInside) return false;
  else return true;
}

#endif
