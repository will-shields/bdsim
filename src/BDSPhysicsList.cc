//  
//   BDSIM, (C) 2001-2007 
//    
//   version 0.4 
//   last modified : 10 Sept 2007 by malton@pp.rhul.ac.uk
//  

//
//    Physics lists
//

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
#include "HadronPhysicsQGSP_BERT.hh"
#include "HadronPhysicsQGSP_BERT_HP.hh"
#include "HadronPhysicsFTFP_BERT.hh"
#include "G4Decay.hh"
#include "G4eeToHadrons.hh"

#include "HadronPhysicsLHEP.hh"
#include "G4EmStandardPhysics.hh"

//#include "IonPhysics.hh"


// physics processes

//
// EM
//

// gamma
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4GammaConversionToMuons.hh"
#include "G4PhotoElectricEffect.hh"
#include "BDSXSBias.hh" //added by L.D. 16/11/09
#include "BDSGammaConversion_LPB.hh" //added by M.D. Salt, R.B. Appleby, 15/10/09

// charged particles
#if G4VERSION>8 && G4MINORVERSION>1
#include "G4eMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"
#include "G4hMultipleScattering.hh"
#else
#include "G4MultipleScattering.hh"
#endif
#include "G4Cerenkov.hh"

// e
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "BDSeBremsstrahlung_LPB.hh" //added by M.D. Salt, R.B.Appleby,15/10/09

// mu
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4MuonNucleusProcess.hh"

//ions
#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"


//Decay of unstable particles
#include "G4Decay.hh"

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

#include "BDSLaserCompton.hh"
#include "BDSSynchrotronRadiation.hh"
#include "BDSContinuousSR.hh"
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

//Planck scattering
#include "BDSPlanckScatterBuilder.hh"

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

  //defaultCutValue = 0.7*mm;  
  defaultCutValue = BDSGlobals->GetDefaultRangeCut()*m;  

  SetVerboseLevel(1);
   
}

BDSPhysicsList::~BDSPhysicsList()
{
}

void BDSPhysicsList::ConstructProcess()
{
  
  //Apply the following in all cases - transportation and step limiter
  AddTransportation();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager *pmanager = particle->GetProcessManager();
    pmanager->AddProcess(new G4StepLimiter,-1,-1,1);
  }
  //===========================================
  //Some options
  //-------------------------------------------
  //Build planck scattering if option is set
  if(BDSGlobals->GetDoPlanckScattering()){
    BDSPlanckScatterBuilder* psbuild = new BDSPlanckScatterBuilder();
    psbuild->Build();
  }
  //A flag to switch on hadronic lead particle biasing
  if (BDSGlobals->GetUseHadLPB() ){
    setenv("SwitchLeadBiasOn","1",1); 
  }
  //Synchrotron radiation
  if(BDSGlobals->GetSynchRadOn()) ConstructSR();
  //Particle decay
  if(BDSGlobals->GetDecayOn()) ConstructDecay();
  //============================================


  if (BDSGlobals->GetPhysListName() != "standard"){ // register physics processes here
    
    // standard e+/e-/gamma electromagnetic interactions
    if(BDSGlobals->GetPhysListName() == "em_standard") 
      {
        ConstructEM();
        return;
      }
    
    if(BDSGlobals->GetPhysListName() == "merlin") 
      {
        ConstructMerlin();
        return;
      }
    
    // low energy em processes
    if(BDSGlobals->GetPhysListName() == "em_low") 
      {
        ConstructEM_Low_Energy();
        return;
      }
    
    // standard electromagnetic + muon
    if(BDSGlobals->GetPhysListName() == "em_muon") 
      {
        ConstructEM();
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
    
    // standard electromagnetic + muon + hadronic
    if(BDSGlobals->GetPhysListName() == "hadronic_muon") 
      {
        ConstructEM();
        ConstructMuon();
        ConstructHadronic();
        return;
      }
    
    if(BDSGlobals->GetPhysListName() == "hadronic_QGSP_BERT") {
      ConstructEM();
      G4VPhysicsConstructor* hadPhysList = new HadronPhysicsQGSP_BERT("hadron");
      hadPhysList -> ConstructProcess();
      return;
    }
    
    if(BDSGlobals->GetPhysListName() == "hadronic_QGSP_BERT_muon") {
      ConstructEM();
      ConstructMuon();
      G4VPhysicsConstructor* hadPhysList = new HadronPhysicsQGSP_BERT("hadron");
      hadPhysList -> ConstructProcess();
      return;
    }
    
    if(BDSGlobals->GetPhysListName() == "hadronic_FTFP_BERT"){
      ConstructEM();
      HadronPhysicsFTFP_BERT *myHadPhysList = new HadronPhysicsFTFP_BERT;
      myHadPhysList->ConstructProcess();
      return;
    }
    
    if(BDSGlobals->GetPhysListName() == "hadronic_QGSP_BERT_HP_muon"){
      ConstructEM();
      ConstructMuon();
      HadronPhysicsQGSP_BERT_HP *myHadPhysList = new HadronPhysicsQGSP_BERT_HP;
      myHadPhysList->ConstructProcess();
      return;
    }
    
    if(BDSGlobals->GetPhysListName() == "hadronic_FTFP_BERT_muon"){
      ConstructEM();
      ConstructMuon();
      HadronPhysicsFTFP_BERT *myHadPhysList = new HadronPhysicsFTFP_BERT;
      myHadPhysList->ConstructProcess();
      return;
    }
    // physics list for laser wire - standard em stuff +
    // weighted compton scattering from laser wire
    if(BDSGlobals->GetPhysListName() == "lw") {
      ConstructEM();
      ConstructLaserWire();
      return;
    } 
    //default - standard (only transportation)
    G4cerr<<"WARNING : Unknown physics list "<<BDSGlobals->GetPhysListName()<<
      "  using transportation only (standard) "<<G4endl;
    return;
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

    // set primary particle definition and kinetic beam parameters other than total energy
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  BDSGlobals->SetParticleDefinition(particleTable->
                                    FindParticle(BDSGlobals->GetParticleName()) );  
  
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
  G4cout<<"     Particle : "<<BDSGlobals->GetParticleDefinition()->GetParticleName()<<G4endl;
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

    pmanager->AddProcess(new G4StepLimiter,-1,-1,1);

    if (particleName == "gamma") {
      // gamma         
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
      pmanager->AddDiscreteProcess(new G4ComptonScattering);
      if (BDSGlobals->GetUseEMLPB()){ //added by M.D. Salt, R.B. Appleby, 15/10/09
        G4GammaConversion* gammaconversion = new G4GammaConversion();
        GammaConversion_LPB* gammaconversion_lpb = new GammaConversion_LPB();
        gammaconversion_lpb->RegisterProcess(gammaconversion);
        pmanager->AddDiscreteProcess(gammaconversion_lpb);
      } else {
        pmanager->AddDiscreteProcess(new G4GammaConversion);
      }
      
    } else if (particleName == "e-") {
      //electron
#if G4VERSION>8 && G4MINORVERSION>1
      pmanager->AddProcess(new G4eMultipleScattering,-1, 1,1);
#else
      pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
#endif
      pmanager->AddProcess(new G4eIonisation,       -1, 2,2);
      if(BDSGlobals->GetUseEMLPB()){ //added by M.D. Salt, R.B. Appleby, 15/10/09
        G4eBremsstrahlung* ebremsstrahlung = new G4eBremsstrahlung();
        eBremsstrahlung_LPB* ebremsstrahlung_lpb = new eBremsstrahlung_LPB();
        ebremsstrahlung_lpb->RegisterProcess(ebremsstrahlung);
        pmanager->AddProcess(ebremsstrahlung_lpb,     -1,-1,3);
      } else {
        pmanager->AddProcess(new G4eBremsstrahlung,   -1, 3,3);     
      }
            
      if(BDSGlobals->GetTurnOnCerenkov()){
#if G4VERSION > 8 && G4MINORVERSION > 0
        G4Cerenkov* theCerenkovProcess = new G4Cerenkov;
        pmanager->AddProcess(theCerenkovProcess);
        pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
#else
        pmanager->AddProcess(new G4Cerenkov,          -1, 5,-1);
#endif
      }
      
    } else if (particleName == "e+") {
      //positron
#if G4VERSION>8 && G4MINORVERSION>1
      pmanager->AddProcess(new G4eMultipleScattering,-1, 1,1);
      #else
      pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
#endif
      pmanager->AddProcess(new G4eIonisation,       -1, 2,2);
      if (BDSGlobals->GetUseEMLPB()){
        G4eBremsstrahlung* ebremsstrahlung = new G4eBremsstrahlung();
        eBremsstrahlung_LPB* ebremsstrahlung_lpb = new eBremsstrahlung_LPB();
        ebremsstrahlung_lpb->RegisterProcess(ebremsstrahlung);
        pmanager->AddProcess(ebremsstrahlung_lpb,     -1,-1,3);
      } else {
        pmanager->AddProcess(new G4eBremsstrahlung,   -1, 3,3);
      }
      pmanager->AddProcess(new G4eplusAnnihilation,  0,-1,4);
      if(BDSGlobals->GetTurnOnCerenkov()){      
#if G4VERSION > 8 && G4MINORVERSION > 0
        G4Cerenkov* theCerenkovProcess = new G4Cerenkov;
        pmanager->AddProcess(theCerenkovProcess);
        pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
#else
        pmanager->AddProcess(new G4Cerenkov,          -1, 5,-1);
#endif 
      }
    } else if ((!particle->IsShortLived()) &&
	       (particle->GetPDGCharge() != 0.0) && 
	       (particle->GetParticleName() != "chargedgeantino")) {
      //all others charged particles except geantino
#if G4VERSION>8 && G4MINORVERSION>1
      pmanager->AddProcess(new G4hMultipleScattering,-1, 1,1);
#else
      pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
#endif
      pmanager->AddProcess(new G4hIonisation,       -1, 2,2);
           if(BDSGlobals->GetTurnOnCerenkov()){
#if  G4VERSION > 8 &&  G4MINORVERSION > 0
        G4Cerenkov* theCerenkovProcess = new G4Cerenkov;
        pmanager->AddProcess(theCerenkovProcess);
        pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
#else
        pmanager->AddProcess(new G4Cerenkov,          -1, 3,-1);
#endif
      }
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
    
    pmanager->AddProcess(new G4StepLimiter,-1,-1,1);

    if (particleName == "gamma") {
      // gamma         
      G4GammaConversionToMuons* gammaconversiontomuons = new G4GammaConversionToMuons();
      BDSXSBias* gammaconversiontomuon_xsbias = new BDSXSBias();
      gammaconversiontomuons->SetCrossSecFactor(BDSGlobals->GetGammaToMuFe());
      gammaconversiontomuon_xsbias->RegisterProcess(gammaconversiontomuons);
      gammaconversiontomuon_xsbias->SetEnhanceFactor(BDSGlobals->GetGammaToMuFe());
      pmanager->AddDiscreteProcess(gammaconversiontomuon_xsbias);
#ifdef DEBUG
      G4cout << "BDSPhysicsList> GammaRoMuFe = " << BDSGlobals->GetGammaToMuFe() << G4endl;
#endif
    } else if (particleName == "e+") {
      //positron
      //===========ee to hadrons in development================
      //      G4eeToHadrons* eetohadrons = new G4eeToHadrons();
      // BDSXSBias* eetohadrons_xsbias = new BDSXSBias();
      //G4cout << "eeToHadronsXSBias = " << BDSGlobals->GetEeToHadronsFe() << G4endl;
      //eetohadrons->SetCrossSecFactor(BDSGlobals->GetEeToHadronsFe());
      //eetohadrons_xsbias->RegisterProcess(eetohadrons);
      //eetohadrons_xsbias->SetEnhanceFactor(BDSGlobals->GetEeToHadronsFe());
      //pmanager->AddDiscreteProcess(eetohadrons_xsbias);
      //pmanager->AddDiscreteProcess(eetohadrons_xsbias);
      //-------------------------------------------------------
      G4AnnihiToMuPair* annihitomupair = new G4AnnihiToMuPair();
      BDSXSBias* annihitomupair_xsbias = new BDSXSBias();
      annihitomupair->SetCrossSecFactor(BDSGlobals->GetAnnihiToMuFe());
      annihitomupair_xsbias->RegisterProcess(annihitomupair);
      annihitomupair_xsbias->SetEnhanceFactor(BDSGlobals->GetAnnihiToMuFe());
      pmanager->AddDiscreteProcess(annihitomupair_xsbias); 
#ifdef DEBUG
      G4cout << "BDSPhysicsList> AnnihiToMuFe = " << BDSGlobals->GetAnnihiToMuFe() << G4endl;
#endif    
    } else if( particleName == "mu+" || 
               particleName == "mu-"    ) {
      //muon  
#if  G4VERSION>8  && G4MINORVERSION>1
      pmanager->AddProcess(new G4MuMultipleScattering,-1, 1,1);
#else 
      pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
#endif
      pmanager->AddProcess(new G4MuIonisation,      -1, 2,2);
      pmanager->AddProcess(new G4MuBremsstrahlung,  -1, 3,3);
      pmanager->AddProcess(new G4MuPairProduction,  -1, 4,4);
      if(BDSGlobals->GetTurnOnCerenkov()){
#if  G4VERSION > 8 &&  G4MINORVERSION > 0
        G4Cerenkov* theCerenkovProcess = new G4Cerenkov;
        pmanager->AddProcess(theCerenkovProcess);
        pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
#else
        pmanager->AddProcess(new G4Cerenkov,          -1, 5,-1);
#endif
        pmanager->AddDiscreteProcess(new G4MuonNucleusProcess);     
      }
    }
  }
}
   

void BDSPhysicsList::ConstructDecay()
{
  theParticleIterator->reset();
  G4Decay* theDecayProcess = new G4Decay();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    pmanager->AddProcess(new G4StepLimiter,-1,-1,1);
    
    if (theDecayProcess->IsApplicable(*particle)) { 
      pmanager -> AddProcess(theDecayProcess);
      pmanager -> SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager -> SetProcessOrdering(theDecayProcess, idxAtRest);
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
    
    if (particleName == "e-") {
      //electron
#if G4VERSION>8 && G4MINORVERSION>1
      pmanager->AddProcess(new G4eMultipleScattering,-1, 1,1);
#else 
      pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
#endif
      pmanager->AddProcess(new G4eIonisation,       -1, 2,2);
      pmanager->AddProcess(new G4eBremsstrahlung,   -1, 3,3);      
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

    pmanager->AddProcess(new G4StepLimiter,-1,-1,1);
     
    if (particleName == "gamma") {
     
      pmanager->AddDiscreteProcess(new G4LowEnergyRayleigh());
      pmanager->AddDiscreteProcess(new G4LowEnergyPhotoElectric);
      pmanager->AddDiscreteProcess(new G4LowEnergyCompton);
      pmanager->AddDiscreteProcess(new G4LowEnergyGammaConversion);
      
    } else if (particleName == "e-") {
      #if G4VERSION>8 && G4MINORVERSION>1
        pmanager->AddProcess(new G4eMultipleScattering,-1, 1,1);
      #else
        pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
      #endif
      pmanager->AddProcess(new G4LowEnergyIonisation,        -1, 2,2);
      pmanager->AddProcess(new G4LowEnergyBremsstrahlung,    -1, 3,3);
	    
    } else if (particleName == "e+") {
      #if G4VERSION>8 && G4MINORVERSION>1
        pmanager->AddProcess(new G4eMultipleScattering,-1, 1,1);
      #else 
        pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
      #endif
      pmanager->AddProcess(new G4eIonisation,        -1, 2,2);
      pmanager->AddProcess(new G4eBremsstrahlung,    -1, 3,3);
      pmanager->AddProcess(new G4eplusAnnihilation,   0,-1,4);
      
    } else if( particleName == "mu+" || 
               particleName == "mu-"    ) {
      #if G4VERSION>8 && G4MINORVERSION>1
        pmanager->AddProcess(new G4MuMultipleScattering,-1, 1,1);
      #else 
        pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
      #endif
      pmanager->AddProcess(new G4MuIonisation,      -1, 2,2);
      pmanager->AddProcess(new G4MuBremsstrahlung,  -1, 3,3);
      pmanager->AddProcess(new G4MuPairProduction,  -1, 4,4);       

    } else if (particleName == "GenericIon") {
      #if G4VERSION>8 && G4MINORVERSION>1
        pmanager->AddProcess(new G4hMultipleScattering,-1, 1,1);
      #else 
        pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
      #endif
      pmanager->AddProcess(new G4hLowEnergyIonisation,       -1,2,2);
      //      pmanager->AddProcess(new G4ionIonisation,      -1, 2,2);
      // it dose not work here
    } else if ((!particle->IsShortLived()) &&
	       (particle->GetPDGCharge() != 0.0) && 
	       (particle->GetParticleName() != "chargedgeantino")) {

      #if G4VERSION>8 && G4MINORVERSION>1
        pmanager->AddProcess(new G4hMultipleScattering,-1, 1,1);
      #else 
        pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
      #endif
      pmanager->AddProcess(new G4hLowEnergyIonisation,       -1,2,2);
    }
  }
}

void BDSPhysicsList::ConstructLaserWire()
{
  G4cout << "Constructing laser-wire" << G4endl;

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

  G4NeutronBuilder* theNeutrons=new G4NeutronBuilder;
  G4LHEPNeutronBuilder * theLHEPNeutron;
  theNeutrons->RegisterMe(theLHEPNeutron=new G4LHEPNeutronBuilder);

  G4ProtonBuilder * thePro;
  G4LHEPProtonBuilder * theLHEPPro;

  thePro=new G4ProtonBuilder;
  thePro->RegisterMe(theLHEPPro=new G4LHEPProtonBuilder);

  G4PiKBuilder * thePiK;
  G4LHEPPiKBuilder * theLHEPPiK;

  thePiK=new G4PiKBuilder;
  thePiK->RegisterMe(theLHEPPiK=new G4LHEPPiKBuilder);

  theNeutrons->Build();
  thePro->Build();
  thePiK->Build();

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
  // G4SynchrotronRadiation* srProcess = new G4SynchrotronRadiation;

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
      //      pmanager->SetProcessOrderingToLast(contSR,idxPostStep);
      //      pmanager->SetProcessActivation(idx, false);
    }
    
  }
  return; 
}
