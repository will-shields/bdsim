   
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

#include "G4Version.hh"

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
#if G4VERSION_NUMBER > 819
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
      G4cout << "BDSPhysicsList: Using hadronic_FTFP_BERT_muon" << G4endl;
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

      if(0){
	G4GammaConversion* gammaconversion = new G4GammaConversion();
	gammaconversion->SetLambdaFactor(1/1.0e-20);
	BDSXSBias* gammaconversion_xsbias = new BDSXSBias();
	gammaconversion_xsbias->RegisterProcess(gammaconversion);
	gammaconversion_xsbias->SetEnhanceFactor(1e-20);
	pmanager->AddDiscreteProcess(gammaconversion_xsbias);
	
      } else if (BDSGlobals->GetUseEMLPB()){ //added by M.D. Salt, R.B. Appleby, 15/10/09
	  G4GammaConversion* gammaconversion = new G4GammaConversion();
	  GammaConversion_LPB* gammaconversion_lpb = new GammaConversion_LPB();
	  gammaconversion_lpb->RegisterProcess(gammaconversion);
	  pmanager->AddDiscreteProcess(gammaconversion_lpb);
      } else {
	pmanager->AddDiscreteProcess(new G4GammaConversion);
      }
    
      
    } else if (particleName == "e-") {
      //electron
#if G4VERSION_NUMBER>919
      pmanager->AddProcess(new G4eMultipleScattering,-1, 1,1);
#else
      pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
#endif
      pmanager->AddProcess(new G4eIonisation,       -1, 2,2);
      if(0){
	G4eBremsstrahlung* ebremsstrahlung = new G4eBremsstrahlung();
	ebremsstrahlung->SetLambdaFactor(1/1.0e-20);
	BDSXSBias* ebremsstrahlung_xsbias = new BDSXSBias();
	ebremsstrahlung_xsbias->RegisterProcess(ebremsstrahlung);
	ebremsstrahlung_xsbias->SetEnhanceFactor(1e-20);
	pmanager->AddDiscreteProcess(ebremsstrahlung_xsbias);     
      }	else if(BDSGlobals->GetUseEMLPB()){ //added by M.D. Salt, R.B. Appleby, 15/10/09
	  
        G4eBremsstrahlung* ebremsstrahlung = new G4eBremsstrahlung();
        eBremsstrahlung_LPB* ebremsstrahlung_lpb = new eBremsstrahlung_LPB();
        ebremsstrahlung_lpb->RegisterProcess(ebremsstrahlung);
        pmanager->AddProcess(ebremsstrahlung_lpb,     -1,-1,3);
      } else {
        pmanager->AddProcess(new G4eBremsstrahlung,   -1, 3,3);     
      }
            
      if(BDSGlobals->GetTurnOnCerenkov()){
#if G4VERSION_NUMBER > 909
        G4Cerenkov* theCerenkovProcess = new G4Cerenkov;
        pmanager->AddProcess(theCerenkovProcess);
        pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
#else
        pmanager->AddProcess(new G4Cerenkov,          -1, 5,-1);
#endif
      }
      
    } else if (particleName == "e+") {
      //positron
#if G4VERSION_NUMBER>919
      pmanager->AddProcess(new G4eMultipleScattering,-1, 1,1);
      #else
      pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
#endif
      pmanager->AddProcess(new G4eIonisation,       -1, 2,2);
      if(0){
	G4eBremsstrahlung* ebremsstrahlung = new G4eBremsstrahlung();
	ebremsstrahlung->SetLambdaFactor(1/1.0e-20);
	BDSXSBias* ebremsstrahlung_xsbias = new BDSXSBias();
	ebremsstrahlung_xsbias->RegisterProcess(ebremsstrahlung);
	ebremsstrahlung_xsbias->SetEnhanceFactor(1e-20);
	pmanager->AddDiscreteProcess(ebremsstrahlung_xsbias);      
      } else if (BDSGlobals->GetUseEMLPB()){
	G4eBremsstrahlung* ebremsstrahlung = new G4eBremsstrahlung();
	eBremsstrahlung_LPB* ebremsstrahlung_lpb = new eBremsstrahlung_LPB();
        ebremsstrahlung_lpb->RegisterProcess(ebremsstrahlung);
        pmanager->AddProcess(ebremsstrahlung_lpb,     -1,-1,3);
      } else {
        pmanager->AddProcess(new G4eBremsstrahlung,   -1, 3,3);
      }
      pmanager->AddProcess(new G4eplusAnnihilation,  0,-1,4);
      if(BDSGlobals->GetTurnOnCerenkov()){      
#if G4VERSION_NUMBER > 909
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
#if G4VERSION_NUMBER>919
      pmanager->AddProcess(new G4hMultipleScattering,-1, 1,1);
#else
      pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
#endif
      pmanager->AddProcess(new G4hIonisation,       -1, 2,2);
           if(BDSGlobals->GetTurnOnCerenkov()){
#if  G4VERSION_NUMBER > 909
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
      G4cout << "BDSPhysicsList> GammaToMuFe = " << BDSGlobals->GetGammaToMuFe() << G4endl;
#endif
    } else if (particleName == "e+") {
      //positron
      //===========ee to hadrons in development================
      G4eeToHadrons* eetohadrons = new G4eeToHadrons();
      // BDSXSBias* eetohadrons_xsbias = new BDSXSBias();
      // G4cout << "eeToHadronsXSBias = " << BDSGlobals->GetEeToHadronsFe() << G4endl;
      eetohadrons->SetCrossSecFactor(BDSGlobals->GetEeToHadronsFe());
      //eetohadrons_xsbias->RegisterProcess(eetohadrons);
      //eetohadrons_xsbias->SetEnhanceFactor(BDSGlobals->GetEeToHadronsFe());
      //pmanager->AddDiscreteProcess(eetohadrons_xsbias);
      pmanager->AddDiscreteProcess(eetohadrons);
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
#if  G4VERSION_NUMBER>919
      pmanager->AddProcess(new G4MuMultipleScattering,-1, 1,1);
#else 
      pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
#endif
      pmanager->AddProcess(new G4MuIonisation,      -1, 2,2);
      pmanager->AddProcess(new G4MuBremsstrahlung,  -1, 3,3);
      pmanager->AddProcess(new G4MuPairProduction,  -1, 4,4);
      if(BDSGlobals->GetTurnOnCerenkov()){
#if  G4VERSION_NUMBER > 909
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
#if G4VERSION_NUMBER>919
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
      #if G4VERSION_NUMBER>919
        pmanager->AddProcess(new G4eMultipleScattering,-1, 1,1);
      #else
        pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
      #endif
      pmanager->AddProcess(new G4LowEnergyIonisation,        -1, 2,2);
      pmanager->AddProcess(new G4LowEnergyBremsstrahlung,    -1, 3,3);
	    
    } else if (particleName == "e+") {
      #if G4VERSION_NUMBER>919
        pmanager->AddProcess(new G4eMultipleScattering,-1, 1,1);
      #else 
        pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
      #endif
      pmanager->AddProcess(new G4eIonisation,        -1, 2,2);
      pmanager->AddProcess(new G4eBremsstrahlung,    -1, 3,3);
      pmanager->AddProcess(new G4eplusAnnihilation,   0,-1,4);
      
    } else if( particleName == "mu+" || 
               particleName == "mu-"    ) {
      #if G4VERSION_NUMBER>919
        pmanager->AddProcess(new G4MuMultipleScattering,-1, 1,1);
      #else 
        pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
      #endif
      pmanager->AddProcess(new G4MuIonisation,      -1, 2,2);
      pmanager->AddProcess(new G4MuBremsstrahlung,  -1, 3,3);
      pmanager->AddProcess(new G4MuPairProduction,  -1, 4,4);       

    } else if (particleName == "GenericIon") {
      #if G4VERSION_NUMBER>919
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

      #if G4VERSION_NUMBER>919
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


// Hadron Processes

#include "G4HadronElasticProcess.hh"
#include "G4HadronFissionProcess.hh"
#include "G4HadronCaptureProcess.hh"

#include "G4PionPlusInelasticProcess.hh"
#include "G4PionMinusInelasticProcess.hh"
#include "G4KaonPlusInelasticProcess.hh"
#include "G4KaonZeroSInelasticProcess.hh"
#include "G4KaonZeroLInelasticProcess.hh"
#include "G4KaonMinusInelasticProcess.hh"
#include "G4ProtonInelasticProcess.hh"
#include "G4AntiProtonInelasticProcess.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4AntiNeutronInelasticProcess.hh"
#include "G4LambdaInelasticProcess.hh"
#include "G4AntiLambdaInelasticProcess.hh"
#include "G4SigmaPlusInelasticProcess.hh"
#include "G4SigmaMinusInelasticProcess.hh"
#include "G4AntiSigmaPlusInelasticProcess.hh"
#include "G4AntiSigmaMinusInelasticProcess.hh"
#include "G4XiZeroInelasticProcess.hh"
#include "G4XiMinusInelasticProcess.hh"
#include "G4AntiXiZeroInelasticProcess.hh"
#include "G4AntiXiMinusInelasticProcess.hh"
#include "G4DeuteronInelasticProcess.hh"
#include "G4TritonInelasticProcess.hh"
#include "G4AlphaInelasticProcess.hh"
#include "G4OmegaMinusInelasticProcess.hh"
#include "G4AntiOmegaMinusInelasticProcess.hh"

// Low-energy Models

#include "G4LElastic.hh"
#include "G4LFission.hh"
#include "G4LCapture.hh"

#include "G4LEPionPlusInelastic.hh"
#include "G4LEPionMinusInelastic.hh"
#include "G4LEKaonPlusInelastic.hh"
#include "G4LEKaonZeroSInelastic.hh"
#include "G4LEKaonZeroLInelastic.hh"
#include "G4LEKaonMinusInelastic.hh"
#include "G4LEProtonInelastic.hh"
#include "G4LEAntiProtonInelastic.hh"
#include "G4LENeutronInelastic.hh"
#include "G4LEAntiNeutronInelastic.hh"
#include "G4LELambdaInelastic.hh"
#include "G4LEAntiLambdaInelastic.hh"
#include "G4LESigmaPlusInelastic.hh"
#include "G4LESigmaMinusInelastic.hh"
#include "G4LEAntiSigmaPlusInelastic.hh"
#include "G4LEAntiSigmaMinusInelastic.hh"
#include "G4LEXiZeroInelastic.hh"
#include "G4LEXiMinusInelastic.hh"
#include "G4LEAntiXiZeroInelastic.hh"
#include "G4LEAntiXiMinusInelastic.hh"
#include "G4LEDeuteronInelastic.hh"
#include "G4LETritonInelastic.hh"
#include "G4LEAlphaInelastic.hh"
#include "G4LEOmegaMinusInelastic.hh"
#include "G4LEAntiOmegaMinusInelastic.hh"

// -- generator models
#include "G4TheoFSGenerator.hh"
#include "G4ExcitationHandler.hh"
#include "G4Evaporation.hh"
#include "G4CompetitiveFission.hh"
#include "G4FermiBreakUp.hh"
#include "G4StatMF.hh"
#include "G4GeneratorPrecompoundInterface.hh"
#include "G4Fancy3DNucleus.hh"
#include "G4LEProtonInelastic.hh"
#include "G4StringModel.hh"
#include "G4PreCompoundModel.hh"
#include "G4FTFModel.hh"
#include "G4QGSMFragmentation.hh"
#include "G4ExcitedStringDecay.hh"

//
// ConstructHad()
//
// Makes discrete physics processes for the hadrons, at present limited
// to those particles with GHEISHA interactions (INTRC > 0).
// The processes are: Elastic scattering, Inelastic scattering,
// Fission (for neutron only), and Capture (neutron).
//
// F.W.Jones  06-JUL-1998
//

void BDSPhysicsList::ConstructHad()
{
    // this will be the model class for high energies
    G4TheoFSGenerator * theTheoModel = new G4TheoFSGenerator;
       
    // all models for treatment of thermal nucleus 
    G4Evaporation * theEvaporation = new G4Evaporation;
    G4FermiBreakUp * theFermiBreakUp = new G4FermiBreakUp;
    G4StatMF * theMF = new G4StatMF;

    // Evaporation logic
    G4ExcitationHandler * theHandler = new G4ExcitationHandler;
        theHandler->SetEvaporation(theEvaporation);
        theHandler->SetFermiModel(theFermiBreakUp);
        theHandler->SetMultiFragmentation(theMF);
        theHandler->SetMaxAandZForFermiBreakUp(12, 6);
        theHandler->SetMinEForMultiFrag(3*MeV);
	
    // Pre equilibrium stage 
    G4PreCompoundModel * thePreEquilib = new G4PreCompoundModel(theHandler);

    
    // a no-cascade generator-precompound interaface
    G4GeneratorPrecompoundInterface * theCascade = new G4GeneratorPrecompoundInterface;
            theCascade->SetDeExcitation(thePreEquilib);  
	
    // here come the high energy parts
    // the string model; still not quite according to design - Explicite use of the forseen interfaces 
    // will be tested and documented in this program by beta-02 at latest.
    G4VPartonStringModel * theStringModel;
    theStringModel = new G4FTFModel;
    theTheoModel->SetTransport(theCascade);
    theTheoModel->SetHighEnergyGenerator(theStringModel);
    theTheoModel->SetMinEnergy(19*GeV);
    theTheoModel->SetMaxEnergy(100*TeV);

      G4VLongitudinalStringDecay * theFragmentation = new G4QGSMFragmentation;
      G4ExcitedStringDecay * theStringDecay = new G4ExcitedStringDecay(theFragmentation);
      theStringModel->SetFragmentationModel(theStringDecay);

// done with the generator model (most of the above is also available as default)
   G4HadronElasticProcess* theElasticProcess = 
                                    new G4HadronElasticProcess;
   G4LElastic* theElasticModel = new G4LElastic;
   theElasticProcess->RegisterMe(theElasticModel);
   G4HadronElasticProcess* theElasticProcess1 = 
                                    new G4HadronElasticProcess;
   theParticleIterator->reset();
   while ((*theParticleIterator)()) {
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();
      G4String particleName = particle->GetParticleName();
     
      if (particleName == "pi+") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4PionPlusInelasticProcess* theInelasticProcess = 
                                new G4PionPlusInelasticProcess("inelastic");
         G4LEPionPlusInelastic* theInelasticModel = 
                                new G4LEPionPlusInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "pi-") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4PionMinusInelasticProcess* theInelasticProcess = 
                                new G4PionMinusInelasticProcess("inelastic");
         G4LEPionMinusInelastic* theInelasticModel = 
                                new G4LEPionMinusInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "kaon+") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4KaonPlusInelasticProcess* theInelasticProcess = 
                                  new G4KaonPlusInelasticProcess("inelastic");
         G4LEKaonPlusInelastic* theInelasticModel = new G4LEKaonPlusInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "kaon0S") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4KaonZeroSInelasticProcess* theInelasticProcess = 
                             new G4KaonZeroSInelasticProcess("inelastic");
         G4LEKaonZeroSInelastic* theInelasticModel = 
                             new G4LEKaonZeroSInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "kaon0L") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4KaonZeroLInelasticProcess* theInelasticProcess = 
                             new G4KaonZeroLInelasticProcess("inelastic");
         G4LEKaonZeroLInelastic* theInelasticModel = 
                             new G4LEKaonZeroLInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "kaon-") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4KaonMinusInelasticProcess* theInelasticProcess = 
                                 new G4KaonMinusInelasticProcess("inelastic");
         G4LEKaonMinusInelastic* theInelasticModel = 
                                 new G4LEKaonMinusInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "proton") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4ProtonInelasticProcess* theInelasticProcess = 
                                    new G4ProtonInelasticProcess("inelastic");
         G4LEProtonInelastic* theInelasticModel = new G4LEProtonInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "anti_proton") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4AntiProtonInelasticProcess* theInelasticProcess = 
                                new G4AntiProtonInelasticProcess("inelastic");
         G4LEAntiProtonInelastic* theInelasticModel = 
                                new G4LEAntiProtonInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "neutron") {
         
          // elastic scattering
         G4LElastic* theElasticModel1 = new G4LElastic;
         theElasticProcess1->RegisterMe(theElasticModel1);
         pmanager->AddDiscreteProcess(theElasticProcess1);
          // inelastic scattering
         G4NeutronInelasticProcess* theInelasticProcess = 
                                    new G4NeutronInelasticProcess("inelastic");
         G4LENeutronInelastic* theInelasticModel = new G4LENeutronInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
          // fission
         G4HadronFissionProcess* theFissionProcess =
                                    new G4HadronFissionProcess;
         G4LFission* theFissionModel = new G4LFission;
         theFissionProcess->RegisterMe(theFissionModel);
         pmanager->AddDiscreteProcess(theFissionProcess);
         // capture
         G4HadronCaptureProcess* theCaptureProcess =
                                    new G4HadronCaptureProcess;
         G4LCapture* theCaptureModel = new G4LCapture;
         theCaptureProcess->RegisterMe(theCaptureModel);
         pmanager->AddDiscreteProcess(theCaptureProcess);
      }  
      else if (particleName == "anti_neutron") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4AntiNeutronInelasticProcess* theInelasticProcess = 
                               new G4AntiNeutronInelasticProcess("inelastic");
         G4LEAntiNeutronInelastic* theInelasticModel = 
                               new G4LEAntiNeutronInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
	 theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "lambda") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4LambdaInelasticProcess* theInelasticProcess = 
                                    new G4LambdaInelasticProcess("inelastic");
         G4LELambdaInelastic* theInelasticModel = new G4LELambdaInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "anti_lambda") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4AntiLambdaInelasticProcess* theInelasticProcess = 
                                new G4AntiLambdaInelasticProcess("inelastic");
         G4LEAntiLambdaInelastic* theInelasticModel = 
                                new G4LEAntiLambdaInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "sigma+") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4SigmaPlusInelasticProcess* theInelasticProcess = 
                                 new G4SigmaPlusInelasticProcess("inelastic");
         G4LESigmaPlusInelastic* theInelasticModel = 
                                 new G4LESigmaPlusInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "sigma-") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4SigmaMinusInelasticProcess* theInelasticProcess = 
                                 new G4SigmaMinusInelasticProcess("inelastic");
         G4LESigmaMinusInelastic* theInelasticModel = 
                                 new G4LESigmaMinusInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "anti_sigma+") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4AntiSigmaPlusInelasticProcess* theInelasticProcess = 
                             new G4AntiSigmaPlusInelasticProcess("inelastic");
         G4LEAntiSigmaPlusInelastic* theInelasticModel = 
                                 new G4LEAntiSigmaPlusInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "anti_sigma-") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4AntiSigmaMinusInelasticProcess* theInelasticProcess = 
                            new G4AntiSigmaMinusInelasticProcess("inelastic");
         G4LEAntiSigmaMinusInelastic* theInelasticModel = 
                                 new G4LEAntiSigmaMinusInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "xi0") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4XiZeroInelasticProcess* theInelasticProcess = 
                            new G4XiZeroInelasticProcess("inelastic");
         G4LEXiZeroInelastic* theInelasticModel = 
                                 new G4LEXiZeroInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "xi-") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4XiMinusInelasticProcess* theInelasticProcess = 
                            new G4XiMinusInelasticProcess("inelastic");
         G4LEXiMinusInelastic* theInelasticModel = 
                                 new G4LEXiMinusInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "anti_xi0") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4AntiXiZeroInelasticProcess* theInelasticProcess = 
                            new G4AntiXiZeroInelasticProcess("inelastic");
         G4LEAntiXiZeroInelastic* theInelasticModel = 
                                 new G4LEAntiXiZeroInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "anti_xi-") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4AntiXiMinusInelasticProcess* theInelasticProcess = 
                            new G4AntiXiMinusInelasticProcess("inelastic");
         G4LEAntiXiMinusInelastic* theInelasticModel = 
                                 new G4LEAntiXiMinusInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "deuteron") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4DeuteronInelasticProcess* theInelasticProcess = 
                            new G4DeuteronInelasticProcess("inelastic");
         G4LEDeuteronInelastic* theInelasticModel = 
                                 new G4LEDeuteronInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "triton") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4TritonInelasticProcess* theInelasticProcess = 
                            new G4TritonInelasticProcess("inelastic");
         G4LETritonInelastic* theInelasticModel = 
                                 new G4LETritonInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "alpha") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4AlphaInelasticProcess* theInelasticProcess = 
                            new G4AlphaInelasticProcess("inelastic");
         G4LEAlphaInelastic* theInelasticModel = 
                                 new G4LEAlphaInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "omega-") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4OmegaMinusInelasticProcess* theInelasticProcess = 
                            new G4OmegaMinusInelasticProcess("inelastic");
         G4LEOmegaMinusInelastic* theInelasticModel = 
                                 new G4LEOmegaMinusInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
      }
      else if (particleName == "anti_omega-") {
         pmanager->AddDiscreteProcess(theElasticProcess);
         G4AntiOmegaMinusInelasticProcess* theInelasticProcess = 
                            new G4AntiOmegaMinusInelasticProcess("inelastic");
         G4LEAntiOmegaMinusInelastic* theInelasticModel = 
                                 new G4LEAntiOmegaMinusInelastic;
         theInelasticProcess->RegisterMe(theInelasticModel);
         theInelasticProcess->RegisterMe(theTheoModel);
         pmanager->AddDiscreteProcess(theInelasticProcess);
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


#include "G4ParallelWorldScoringProcess.hh"
void BDSPhysicsList::AddScoringProcess(){

  G4int npw = paraWorldName.size();
  for ( G4int i = 0; i < npw; i++){
    G4ParallelWorldScoringProcess* theParallelWorldScoringProcess
      = new G4ParallelWorldScoringProcess("ParaWorldScoringProc");
    theParallelWorldScoringProcess->SetParallelWorld(paraWorldName[i]);

    theParticleIterator->reset();
    while( (*theParticleIterator)() ){
      G4ParticleDefinition* particle = theParticleIterator->value();
      if ( !particle->IsShortLived() ){
	G4ProcessManager* pmanager = particle->GetProcessManager();
	pmanager->AddProcess(theParallelWorldScoringProcess);
	pmanager->SetProcessOrderingToLast(theParallelWorldScoringProcess,idxAtRest);
	pmanager->SetProcessOrdering(theParallelWorldScoringProcess,idxAlongStep,1);
	pmanager->SetProcessOrderingToLast(theParallelWorldScoringProcess,idxPostStep);
      }
    }
  }

}

