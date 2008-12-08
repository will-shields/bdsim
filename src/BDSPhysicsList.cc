//  
//   BDSIM, (C) 2001-2007 
//    
//   version 0.4 
//   last modified : 10 Sept 2007 by malton@pp.rhul.ac.uk
//  

//
//    Physics lists
//

const int DEBUG = 0;

#include "BDSGlobalConstants.hh" // must be first in include list
#include "BDSPhysicsList.hh"

#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ios.hh"
#include <iomanip>   

//#include "GeneralPhysics.hh"
//#include "MuonPhysics.hh"


#include "HadronPhysicsLHEP.hh"

//#include "IonPhysics.hh"


// physics processes

//
// EM
//

// gamma
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "BDSGammaConversionToMuons.hh"

// charged particles
#include "G4MultipleScattering.hh"
#include "G4Cerenkov.hh"

// e
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

// mu
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4MuonNucleusProcess.hh"

//ions
#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"


//
// Low EM
//

//gamma
#include "G4LowEnergyRayleigh.hh"
#include "G4LowEnergyPhotoElectric.hh"
#include "G4LowEnergyCompton.hh"
#include "G4LowEnergyGammaConversion.hh"

//e
#include "G4LowEnergyIonisation.hh"
#include "G4LowEnergyBremsstrahlung.hh"
#include "G4AnnihiToMuPair.hh"

//ions
#include "G4hLowEnergyIonisation.hh"


//#include "BDSLaserWirePhysics.hh"
#include "BDSLaserCompton.hh"
//#include "BDSPlanckScatterPhysics.hh"
#include "BDSSynchrotronRadiation.hh"
#include "BDSContinuousSR.hh"


//#include "BDSeBremPhysics.hh"
//#include "BDSGammaConversionPhysics.hh"
//#include "BDSLowEMPhysics.hh"

#include "G4StepLimiter.hh"


//
// Hadronic
//
#include "G4TheoFSGenerator.hh"
#include "G4GeneratorPrecompoundInterface.hh"
#include "G4QGSModel.hh"
#include "G4GammaParticipants.hh"
#include "G4QGSMFragmentation.hh"
#include "G4ExcitedStringDecay.hh"

#include "G4GammaNuclearReaction.hh"
#include "G4ElectroNuclearReaction.hh"
#include "G4PhotoNuclearProcess.hh"
#include "G4ElectronNuclearProcess.hh"
#include "G4PositronNuclearProcess.hh"


//
// particle definition
//

// Bosons
#include "G4ChargedGeantino.hh"
#include "G4Geantino.hh"
#include "G4Gamma.hh"
#include "G4OpticalPhoton.hh"

// leptons
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4NeutrinoMu.hh"
#include "G4AntiNeutrinoMu.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"

// Hadrons
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"

//ShortLived
#include "G4ShortLivedConstructor.hh"


extern G4bool verbose;

BDSPhysicsList::BDSPhysicsList():  G4VUserPhysicsList()
{
  // construct particles

  defaultCutValue = 0.7*mm;  

  SetVerboseLevel(1);
   
}

BDSPhysicsList::~BDSPhysicsList()
{
}

void BDSPhysicsList::ConstructProcess()
{
  // register physics processes here
  AddTransportation();

  // standard e+/e-/gamma electromagnetic interactions
  if(BDSGlobals->GetPhysListName() == "em_standard") 
    {
      ConstructEM();
      if(BDSGlobals->GetSynchRadOn()) ConstructSR();
      return;
    }

  if(BDSGlobals->GetPhysListName() == "merlin") 
    {
      ConstructMerlin();
      if(BDSGlobals->GetSynchRadOn()) ConstructSR();
      return;
    }

  // low energy em processes
  if(BDSGlobals->GetPhysListName() == "em_low") 
    {
      ConstructEM_Low_Energy();
      if(BDSGlobals->GetSynchRadOn()) ConstructSR();
      return;
    }

  // standard electromagnetic + muon
  if(BDSGlobals->GetPhysListName() == "em_muon") 
    {
      ConstructMuon();
      return;
    }

  // standard hadronic - photo-nuclear etc.
  if(BDSGlobals->GetPhysListName() == "hadronic_standard") 
    {
      ConstructEM();
      ConstructHadronic();
      return;
    }


  // physics list for laser wire - standard em stuff +
  // weighted compton scattering from laser wire
  if(BDSGlobals->GetPhysListName() == "lw") 
    {
      ConstructEM();
      ConstructLaserWire();
      return;
    }


  //default - standard (only transportation)
  if(BDSGlobals->GetPhysListName() != "standard")
    {
      G4cerr<<"WARNING : Unknown physics list "<<BDSGlobals->GetPhysListName()<<
	"  using transportation only (standard) "<<G4endl;
    } 

  // stuff needed to limitsteps
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    pmanager->AddProcess(new G4StepLimiter,-1,-1,1);
  }
}

void BDSPhysicsList::ConstructParticle()
{

  // pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();
  
  // gamma
  G4Gamma::GammaDefinition();
  
  // optical photon
  G4OpticalPhoton::OpticalPhotonDefinition();
  
  // leptons
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();
  
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();  
  
  // mesons
  G4MesonConstructor mConstructor;
  mConstructor.ConstructParticle();
  
  // barions
  G4BaryonConstructor bConstructor;
  bConstructor.ConstructParticle();
  
  // ions
  G4IonConstructor iConstructor;
  iConstructor.ConstructParticle();
  
  //  Construct  resonaces and quarks
  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();

  //theMiscLHEP=new G4MiscLHEPBuilder;
  //theStoppingHadron=new G4StoppingHadronBuilder;



  // set primary particle definition and kinetic beam parameters other than total energy


  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  BDSGlobals->SetParticleDefinition(particleTable->
					FindParticle(BDSGlobals->tmpParticleName) );
  
  if(!BDSGlobals->GetParticleDefinition()) 
    {
      G4Exception("Particle not found, quitting!");
      exit(1);
    }

  // set kinetic beam parameters other than total energy

  BDSGlobals->SetBeamMomentum( sqrt(pow(BDSGlobals->GetBeamTotalEnergy(),2)-
			       pow(BDSGlobals->GetParticleDefinition()->GetPDGMass(),2)) );
  
  BDSGlobals->SetBeamKineticEnergy(BDSGlobals->GetBeamTotalEnergy() - 
				   BDSGlobals->GetParticleDefinition()->GetPDGMass() );

  G4cout<<"Beam properties:"<<G4endl;
  G4cout<<"     Particle : "<<BDSGlobals->tmpParticleName<<G4endl;
  G4cout<<"     Mass : "<<BDSGlobals->GetParticleDefinition()->GetPDGMass()/GeV<< " GeV"<<G4endl;
  G4cout<<"     Charge : "<<BDSGlobals->GetParticleDefinition()->GetPDGCharge()<< " e"<<G4endl;
  G4cout<<"     Total Energy : "<< BDSGlobals->GetBeamTotalEnergy()/GeV<<" GeV"<<G4endl;
  G4cout<<"     Kinetic Energy : "<< BDSGlobals->GetBeamKineticEnergy()/GeV<<" GeV"<<G4endl;
  G4cout<<"     Momentum : "<< BDSGlobals->GetBeamMomentum()/GeV<<" GeV"<<G4endl;
}


#include "G4Region.hh"
#include "G4ProductionCuts.hh"
void BDSPhysicsList::SetCuts()
{
  if (verbose){
    G4cout << "BDSPhysicsList:: setting cuts\n";
    
  }
  
  SetCutsWithDefault();   

  if(BDSGlobals->GetProdCutPhotons()>0)
    SetCutValue(BDSGlobals->GetProdCutPhotons(),"gamma");
  
  if(BDSGlobals->GetProdCutElectrons()>0)
    SetCutValue(BDSGlobals->GetProdCutElectrons(),"e-");
  
  if(BDSGlobals->GetProdCutPositrons()>0)
    SetCutValue(BDSGlobals->GetProdCutPositrons(),"e+");


  if(verbose)
    DumpCutValuesTable(); 
  
}


// particular physics process constructors

void BDSPhysicsList::ConstructEM()
{
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    
    if (particleName == "gamma") {
      // gamma         
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
      pmanager->AddDiscreteProcess(new G4ComptonScattering);
      pmanager->AddDiscreteProcess(new G4GammaConversion);
      //pmanager->AddDiscreteProcess(new G4GammaConversionToMuons);
      
    } else if (particleName == "e-") {
      //electron
      pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
      pmanager->AddProcess(new G4eIonisation,       -1, 2,2);
      pmanager->AddProcess(new G4eBremsstrahlung,   -1, 3,3);     
      pmanager->AddProcess(new G4StepLimiter,       -1,-1,4);  

#if G4VERSION > 8  && G4MINORVERSION > 0
      G4Cerenkov* theCerenkovProcess = new G4Cerenkov;
      pmanager->AddProcess(theCerenkovProcess);
      pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
#else
      pmanager->AddProcess(new G4Cerenkov,          -1, 5,-1);
#endif

    } else if (particleName == "e+") {
      //positron
      pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
      pmanager->AddProcess(new G4eIonisation,       -1, 2,2);
      pmanager->AddProcess(new G4eBremsstrahlung,   -1, 3,3);
      pmanager->AddProcess(new G4eplusAnnihilation,  0,-1,4);

#if G4VERSION > 8  && G4MINORVERSION > 0
      G4Cerenkov* theCerenkovProcess = new G4Cerenkov;
      pmanager->AddProcess(theCerenkovProcess);
      pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
#else
      pmanager->AddProcess(new G4Cerenkov,          -1, 5,-1);
#endif
      
    } else if( particleName == "mu+" || 
               particleName == "mu-"    ) {
      //muon  
      //pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
      //pmanager->AddProcess(new G4MuIonisation,      -1, 2,2);
      //pmanager->AddProcess(new G4MuBremsstrahlung,  -1, 3,3);
      //pmanager->AddProcess(new G4MuPairProduction,  -1, 4,4);     
      //pmanager->AddDiscreteProcess(new G4MuonNucleusProcess);     
      
    } else if ((!particle->IsShortLived()) &&
	       (particle->GetPDGCharge() != 0.0) && 
	       (particle->GetParticleName() != "chargedgeantino")) {
      //all others charged particles except geantino
      pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
      pmanager->AddProcess(new G4hIonisation,       -1, 2,2);
#if G4VERSION > 8  && G4MINORVERSION > 0
      G4Cerenkov* theCerenkovProcess = new G4Cerenkov;
      pmanager->AddProcess(theCerenkovProcess);
      pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
#else
      pmanager->AddProcess(new G4Cerenkov,          -1, 3,-1);
#endif
      //step limit
      //pmanager->AddProcess(new G4StepLimiter,       -1,-1,4);         
      ///pmanager->AddProcess(new G4UserSpecialCuts,   -1,-1,5);  
    }
  }
}

void BDSPhysicsList::ConstructMuon()
{
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    
    if (particleName == "gamma") {
      // gamma         
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
      pmanager->AddDiscreteProcess(new G4ComptonScattering);
      pmanager->AddDiscreteProcess(new G4GammaConversion);
      pmanager->AddDiscreteProcess(new BDSGammaConversionToMuons);
      
    } else if (particleName == "e-") {
      //electron
      pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
      pmanager->AddProcess(new G4eIonisation,       -1, 2,2);
      pmanager->AddProcess(new G4eBremsstrahlung,   -1, 3,3);     
      //pmanager->AddProcess(new G4StepLimiter,   -1, 1,5);
#if G4VERSION > 8  && G4MINORVERSION > 0
      G4Cerenkov* theCerenkovProcess = new G4Cerenkov;
      pmanager->AddProcess(theCerenkovProcess);
      pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
#else
      pmanager->AddProcess(new G4Cerenkov,          -1, 4,-1);
#endif

      
    } else if (particleName == "e+") {
      //positron
      pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
      pmanager->AddProcess(new G4eIonisation,       -1, 2,2);
      pmanager->AddProcess(new G4eBremsstrahlung,   -1, 3,3);
      pmanager->AddProcess(new G4eplusAnnihilation,  0,-1,4);
#if G4VERSION > 8  && G4MINORVERSION > 0
      G4Cerenkov* theCerenkovProcess = new G4Cerenkov;
      pmanager->AddProcess(theCerenkovProcess);
      pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
#else
      pmanager->AddProcess(new G4Cerenkov,          -1, 5,-1);
#endif
      
    } else if( particleName == "mu+" || 
               particleName == "mu-"    ) {
      //muon  
      pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
      pmanager->AddProcess(new G4MuIonisation,      -1, 2,2);
      pmanager->AddProcess(new G4MuBremsstrahlung,  -1, 3,3);
      pmanager->AddProcess(new G4MuPairProduction,  -1, 4,4);
#if G4VERSION > 8  && G4MINORVERSION > 0
      G4Cerenkov* theCerenkovProcess = new G4Cerenkov;
      pmanager->AddProcess(theCerenkovProcess);
      pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
#else
      pmanager->AddProcess(new G4Cerenkov,          -1, 5,-1);
#endif
      pmanager->AddDiscreteProcess(new G4MuonNucleusProcess);     
      
    } else if ((!particle->IsShortLived()) &&
	       (particle->GetPDGCharge() != 0.0) && 
	       (particle->GetParticleName() != "chargedgeantino")) {
      //all others charged particles except geantino
      pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
      pmanager->AddProcess(new G4hIonisation,       -1, 2,2);
#if G4VERSION > 8  && G4MINORVERSION > 0
      G4Cerenkov* theCerenkovProcess = new G4Cerenkov;
      pmanager->AddProcess(theCerenkovProcess);
      pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
#else
      pmanager->AddProcess(new G4Cerenkov,          -1, 3,-1);
#endif
      //step limit
      //pmanager->AddProcess(new G4StepLimiter,       -1,-1,4);         
      ///pmanager->AddProcess(new G4UserSpecialCuts,   -1,-1,5);  
    }
  }
}


void BDSPhysicsList::ConstructMerlin()
{
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    
    if (particleName == "gamma") {
      // gamma         
      //pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
      //pmanager->AddDiscreteProcess(new G4ComptonScattering);
      //pmanager->AddDiscreteProcess(new G4GammaConversion);
      
    } else if (particleName == "e-") {
      //electron
      pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
      pmanager->AddProcess(new G4eIonisation,       -1, 2,2);
      pmanager->AddProcess(new G4eBremsstrahlung,   -1, 3,3);      
      
    } else if (particleName == "e+") {
      //positron
      //pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
      //pmanager->AddProcess(new G4eIonisation,       -1, 2,2);
      //pmanager->AddProcess(new G4eBremsstrahlung,   -1, 3,3);
      //pmanager->AddProcess(new G4eplusAnnihilation,  0,-1,4);
      
    } else if( particleName == "mu+" || 
               particleName == "mu-"    ) {
      //muon  
      //pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
      //pmanager->AddProcess(new G4MuIonisation,      -1, 2,2);
      //pmanager->AddProcess(new G4MuBremsstrahlung,  -1, 3,3);
      //pmanager->AddProcess(new G4MuPairProduction,  -1, 4,4);       
      
    } else if ((!particle->IsShortLived()) &&
	       (particle->GetPDGCharge() != 0.0) && 
	       (particle->GetParticleName() != "chargedgeantino")) {
      //all others charged particles except geantino
      //jcc pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
      //jcc pmanager->AddProcess(new G4hIonisation,       -1, 2,2);
      //step limit
      //pmanager->AddProcess(new G4StepLimiter,       -1,-1,3);         
      ///pmanager->AddProcess(new G4UserSpecialCuts,   -1,-1,4);  
    }
  }
}


void BDSPhysicsList::ConstructEM_Low_Energy()
{

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
     
    if (particleName == "gamma") {
     
      pmanager->AddDiscreteProcess(new G4LowEnergyRayleigh());
      pmanager->AddDiscreteProcess(new G4LowEnergyPhotoElectric);
      pmanager->AddDiscreteProcess(new G4LowEnergyCompton);
      pmanager->AddDiscreteProcess(new G4LowEnergyGammaConversion);
      
    } else if (particleName == "e-") {
  
      pmanager->AddProcess(new G4MultipleScattering, -1, 1,1);
      pmanager->AddProcess(new G4LowEnergyIonisation,        -1, 2,2);
      pmanager->AddProcess(new G4LowEnergyBremsstrahlung,    -1, 3,3);
	    
    } else if (particleName == "e+") {

      pmanager->AddProcess(new G4MultipleScattering, -1, 1,1);
      pmanager->AddProcess(new G4eIonisation,        -1, 2,2);
      pmanager->AddProcess(new G4eBremsstrahlung,    -1, 3,3);
      pmanager->AddProcess(new G4eplusAnnihilation,   0,-1,4);
      
    } else if( particleName == "mu+" || 
               particleName == "mu-"    ) {

      pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
      pmanager->AddProcess(new G4MuIonisation,      -1, 2,2);
      pmanager->AddProcess(new G4MuBremsstrahlung,  -1, 3,3);
      pmanager->AddProcess(new G4MuPairProduction,  -1, 4,4);       

    } else if (particleName == "GenericIon") {

      pmanager->AddProcess(new G4MultipleScattering, -1, 1,1);
      pmanager->AddProcess(new G4hLowEnergyIonisation,       -1,2,2);
      //      pmanager->AddProcess(new G4ionIonisation,      -1, 2,2);
      // it dose not work here
    } else if ((!particle->IsShortLived()) &&
	       (particle->GetPDGCharge() != 0.0) && 
	       (particle->GetParticleName() != "chargedgeantino")) {

      pmanager->AddProcess(new G4MultipleScattering,-1,1,1);
      pmanager->AddProcess(new G4hLowEnergyIonisation,       -1,2,2);
    }
  }
}


void BDSPhysicsList::ConstructLaserWire()
{
  
  theParticleIterator->reset();

  BDSLaserCompton* lwProcess = new BDSLaserCompton;

  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    
    if (particleName == "e-") {
      pmanager->AddProcess(lwProcess);
      pmanager->SetProcessOrderingToLast(lwProcess,idxPostStep);
    }

    if (particleName == "e+") {
      pmanager->AddProcess(lwProcess);
      pmanager->SetProcessOrderingToLast(lwProcess,idxPostStep);
    }

  }

}



void BDSPhysicsList::ConstructHadronic()
{

 //  G4NeutronBuilder* theNeutrons=new G4NeutronBuilder;
//   G4LHEPNeutronBuilder * theLHEPNeutron;
//   theNeutrons->RegisterMe(theLHEPNeutron=new G4LHEPNeutronBuilder);

//   G4ProtonBuilder * thePro;
//   G4LHEPProtonBuilder * theLHEPPro;

//   thePro=new G4ProtonBuilder;
//   thePro->RegisterMe(theLHEPPro=new G4LHEPProtonBuilder);

//   G4PiKBuilder * thePiK;
//   G4LHEPPiKBuilder * theLHEPPiK;

//   thePiK=new G4PiKBuilder;
//   thePiK->RegisterMe(theLHEPPiK=new G4LHEPPiKBuilder);

//   theNeutrons->Build();
//   thePro->Build();
//   thePiK->Build();


  // Photonuclear processes

  G4PhotoNuclearProcess * thePhotoNuclearProcess;
  G4ElectronNuclearProcess * theElectronNuclearProcess;
  G4PositronNuclearProcess * thePositronNuclearProcess;
  G4ElectroNuclearReaction * theElectroReaction;
  G4GammaNuclearReaction * theGammaReaction;  
  
  G4TheoFSGenerator * theModel;
  G4GeneratorPrecompoundInterface * theCascade;
  G4QGSModel< G4GammaParticipants > * theStringModel;
  G4QGSMFragmentation * theFragmentation;
  G4ExcitedStringDecay * theStringDecay;



  thePhotoNuclearProcess = new G4PhotoNuclearProcess;
  theGammaReaction = new G4GammaNuclearReaction;
  theElectronNuclearProcess = new G4ElectronNuclearProcess;
  thePositronNuclearProcess = new G4PositronNuclearProcess;
  theElectroReaction = new G4ElectroNuclearReaction;

  theModel = new G4TheoFSGenerator;
  
  theStringModel = new G4QGSModel< G4GammaParticipants >;
  theStringDecay = new G4ExcitedStringDecay(theFragmentation=new G4QGSMFragmentation);
  theStringModel->SetFragmentationModel(theStringDecay);
  
  theCascade = new G4GeneratorPrecompoundInterface;
  
  theModel->SetTransport(theCascade);
  theModel->SetHighEnergyGenerator(theStringModel);


  G4ProcessManager * aProcMan = 0;
  
  aProcMan = G4Gamma::Gamma()->GetProcessManager();
  theGammaReaction->SetMaxEnergy(3.5*GeV);
  thePhotoNuclearProcess->RegisterMe(theGammaReaction);
  theModel->SetMinEnergy(3.*GeV);
  theModel->SetMaxEnergy(100*TeV);
  thePhotoNuclearProcess->RegisterMe(theModel);
  aProcMan->AddDiscreteProcess(thePhotoNuclearProcess);

  aProcMan = G4Electron::Electron()->GetProcessManager();
  theElectronNuclearProcess->RegisterMe(theElectroReaction);
  aProcMan->AddDiscreteProcess(theElectronNuclearProcess);
  
  aProcMan = G4Positron::Positron()->GetProcessManager();
  thePositronNuclearProcess->RegisterMe(theElectroReaction);
  aProcMan->AddDiscreteProcess(thePositronNuclearProcess);

}

void BDSPhysicsList::ConstructSR()
{

  // BDSIM's version of Synchrotron Radiation
  BDSSynchrotronRadiation* srProcess = new BDSSynchrotronRadiation;
  
  BDSContinuousSR *contSR = new BDSContinuousSR(); // contin. energy loss process

  // G4's version of Synchrotron Radiation - not used because does not have
  // Multiplicity or MeanFreeFactor capability
  //G4SynchrotronRadiation* srProcess = new G4SynchrotronRadiation;

  theParticleIterator->reset();

  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    
    if (particleName == "e-") {
      pmanager->AddProcess(srProcess);
      pmanager->SetProcessOrderingToLast(srProcess,idxPostStep);

      G4int idx = pmanager->AddProcess(contSR);
      pmanager->SetProcessOrderingToLast(contSR,idxPostStep);
      pmanager->SetProcessActivation(idx, false);
    }
    
    if (particleName == "e+") {
      pmanager->AddProcess(srProcess);
      pmanager->SetProcessOrderingToLast(srProcess,idxPostStep);

      //G4int idx = pmanager->AddProcess(contSR);
      //pmanager->SetProcessOrderingToLast(contSR,idxPostStep);
      //pmanager->SetProcessActivation(idx, false);
    }
    
  }
  return; 
}
