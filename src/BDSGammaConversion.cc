/** BDSIM, v0.1   

Last modified 15.11.2005 by Ilya Agapov

**/

#include "BDSGammaConversion.hh"
#include "G4UnitsTable.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
// constructor

#if CLHEP_VERSION < 1
  using namespace CLHEP;
#endif

BDSGammaConversion::BDSGammaConversion(const G4String& processName)
  : G4VDiscreteProcess (processName),            // initialization
    theCrossSectionTable(NULL),
    theMeanFreePathTable(NULL),  
    LowestEnergyLimit (2*electron_mass_c2),
    HighestEnergyLimit(100*GeV),
    NumbBinTable(100),
    fminimalEnergy(1*eV)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
// destructor
 
BDSGammaConversion::~BDSGammaConversion()
{
   if (theCrossSectionTable) {
      theCrossSectionTable->clearAndDestroy();
      delete theCrossSectionTable;
   }

   if (theMeanFreePathTable) {
      theMeanFreePathTable->clearAndDestroy();
      delete theMeanFreePathTable;
   }
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void BDSGammaConversion::SetPhysicsTableBining(
                                  G4double lowE, G4double highE, G4int nBins)
{
  LowestEnergyLimit = lowE; HighestEnergyLimit = highE; NumbBinTable = nBins;
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void BDSGammaConversion::BuildPhysicsTable(const G4ParticleDefinition&)
// Build cross section and mean free path tables
{
   G4double LowEdgeEnergy, Value;
   G4PhysicsLogVector* ptrVector;

// Build cross section per atom tables for the e+e- pair creation

   if (theCrossSectionTable) {
         theCrossSectionTable->clearAndDestroy(); delete theCrossSectionTable;}

   theCrossSectionTable = new G4PhysicsTable(G4Element::GetNumberOfElements());
   const G4ElementTable* theElementTable = G4Element::GetElementTable();
   G4double AtomicNumber;
   size_t J;

   for ( J=0 ; J < G4Element::GetNumberOfElements(); J++ )  
       { 
        //create physics vector then fill it ....
        ptrVector = new G4PhysicsLogVector(LowestEnergyLimit,HighestEnergyLimit,
                                           NumbBinTable );
        AtomicNumber = (*theElementTable)[J]->GetZ();
 
        for ( G4int i = 0 ; i < NumbBinTable ; i++ )      
           {
             LowEdgeEnergy = ptrVector->GetLowEdgeEnergy( i ) ;
             Value = ComputeCrossSectionPerAtom( LowEdgeEnergy, AtomicNumber);  
             ptrVector->PutValue( i , Value ) ;
           }

        theCrossSectionTable->insertAt( J , ptrVector ) ;

      }

// Build mean free path table for the e+e- pair creation

   if (theMeanFreePathTable) 
     { theMeanFreePathTable->clearAndDestroy(); delete theMeanFreePathTable;}

   theMeanFreePathTable= new G4PhysicsTable(G4Material::GetNumberOfMaterials());
   const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();
   G4Material* material;

   for ( J=0 ; J < G4Material::GetNumberOfMaterials(); J++ )
       { 
        //create physics vector then fill it ....
        ptrVector = new G4PhysicsLogVector(LowestEnergyLimit,HighestEnergyLimit,
                                           NumbBinTable);
        material = (*theMaterialTable)[J];
 
        for ( G4int i = 0 ; i < NumbBinTable ; i++ )      
           {
             LowEdgeEnergy = ptrVector->GetLowEdgeEnergy( i ) ;
             Value = ComputeMeanFreePath( LowEdgeEnergy, material);  
             ptrVector->PutValue( i , Value ) ;
           }

        theMeanFreePathTable->insertAt( J , ptrVector ) ;

      }
   
   PrintInfoDefinition();        
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double BDSGammaConversion::ComputeCrossSectionPerAtom
                              (G4double GammaEnergy, G4double AtomicNumber)
 
// Calculates the microscopic cross section in GEANT4 internal units.
// A parametrized formula from L. Urban is used to estimate
// the total cross section.
// It gives a good description of the data from 1.5 MeV to 100 GeV.
// below 1.5 MeV: sigma=sigma(1.5MeV)*(GammaEnergy-2electronmass)
//                                   *(GammaEnergy-2electronmass) 

{
 G4double GammaEnergyLimit = 1.5*MeV;
 G4double CrossSection = 0.0 ;
 if ( AtomicNumber < 1. ) return CrossSection;
 if ( GammaEnergy < 2*electron_mass_c2 ) return CrossSection;
 
 static const G4double
    a0= 8.7842e+2*microbarn, a1=-1.9625e+3*microbarn, a2= 1.2949e+3*microbarn,
    a3=-2.0028e+2*microbarn, a4= 1.2575e+1*microbarn, a5=-2.8333e-1*microbarn;

 static const G4double
    b0=-1.0342e+1*microbarn, b1= 1.7692e+1*microbarn, b2=-8.2381   *microbarn,
    b3= 1.3063   *microbarn, b4=-9.0815e-2*microbarn, b5= 2.3586e-3*microbarn;

 static const G4double
    c0=-4.5263e+2*microbarn, c1= 1.1161e+3*microbarn, c2=-8.6749e+2*microbarn,
    c3= 2.1773e+2*microbarn, c4=-2.0467e+1*microbarn, c5= 6.5372e-1*microbarn;

 G4double GammaEnergySave = GammaEnergy ;
 if (GammaEnergy < GammaEnergyLimit) GammaEnergy = GammaEnergyLimit ;

 G4double X=log(GammaEnergy/electron_mass_c2),X2=X*X, X3=X2*X, X4=X3*X, X5=X4*X;

 G4double F1 = a0 + a1*X + a2*X2 + a3*X3 + a4*X4 + a5*X5,
          F2 = b0 + b1*X + b2*X2 + b3*X3 + b4*X4 + b5*X5,
          F3 = c0 + c1*X + c2*X2 + c3*X3 + c4*X4 + c5*X5;     

 CrossSection = (AtomicNumber+1.)*
                (F1*AtomicNumber + F2*AtomicNumber*AtomicNumber + F3);

 if (GammaEnergySave < GammaEnergyLimit)
   {
     X = (GammaEnergySave - 2.*electron_mass_c2)
        /(GammaEnergyLimit- 2.*electron_mass_c2);
     CrossSection *= X*X;
   }

 if (CrossSection < 0.) CrossSection = 0.;

 return CrossSection;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4VParticleChange* BDSGammaConversion::PostStepDoIt(const G4Track& aTrack,
                                                  const G4Step&  aStep)
//
// The secondaries e+e- energies are sampled using the Bethe - Heitler
// cross sections with Coulomb correction.
// A modified version of the random number techniques of Butcher & Messel
// is used (Nuc Phys 20(1960),15).
//
// GEANT4 internal units.
//
// Note 1 : Effects due to the breakdown of the Born approximation at
//          low energy are ignored.
// Note 2 : The differential cross section implicitly takes account of 
//          pair creation in both nuclear and atomic electron fields.
//          However triplet prodution is not generated.
 
{

   fParticleChange.Initialize(aTrack);
   G4Material* aMaterial = aTrack.GetMaterial();

   const G4DynamicParticle* aDynamicGamma = aTrack.GetDynamicParticle();
   G4double GammaEnergy = aDynamicGamma->GetKineticEnergy();
   G4ParticleMomentum GammaDirection = aDynamicGamma->GetMomentumDirection();


   // gab: Get muon pair weight:
   muonWeight=0.;
   G4double MuonMFP;
   if(BDSGlobals->GetUseMuonShowers())
     {
       MuonMFP=ComputeMuonMeanFreePath(GammaEnergy, aMaterial);
       muonWeight=MeanFreePath/MuonMFP;
       if(muonWeight<1.e-300)muonWeight=0.;
       // gab: Weight according to the relative cross-section
       fParticleChange.SetMuonWeight(muonWeight);       
     }

   G4double epsil ;
   G4double epsil0 = electron_mass_c2/GammaEnergy ;

  // do it fast if GammaEnergy < 2. MeV
   const G4double Egsmall=2.*MeV; 

   // select randomly one element constituing the material  
   G4Element* anElement = SelectRandomAtom(aDynamicGamma, aMaterial);

   if (GammaEnergy<Egsmall) { epsil = epsil0 + (0.5-epsil0)*G4UniformRand(); }

   else
   {  // now comes the case with GammaEnergy >= 2. MeV


   // Extract Coulomb factor for this Element
   G4double FZ = 8.*(anElement->GetIonisation()->GetlogZ3());
   if (GammaEnergy > 50.*MeV) FZ += 8.*(anElement->GetfCoulomb());

   // limits of the screening variable
   G4double screenfac = 136.*epsil0/(anElement->GetIonisation()->GetZ3());
   G4double screenmax = exp ((42.24 - FZ)/8.368) - 0.952 ;
   G4double screenmin = std::min(4.*screenfac,screenmax);

   // limits of the energy sampling
   G4double epsil1 = 0.5 - 0.5*sqrt(1. - screenmin/screenmax) ;
   G4double epsilmin = std::max(epsil0,epsil1) , epsilrange = 0.5 - epsilmin;

   //
   // sample the energy rate of the created electron (or positron) 
   //
   //G4double epsil, screenvar, greject ;
   G4double  screenvar, greject ;

   G4double F10 = ScreenFunction1(screenmin) - FZ;
   G4double F20 = ScreenFunction2(screenmin) - FZ;
   G4double NormF1 = std::max(F10*epsilrange*epsilrange,0.); 
   G4double NormF2 = std::max(1.5*F20,0.);

   do {
        if ( NormF1/(NormF1+NormF2) > G4UniformRand() )
             { epsil = 0.5 - epsilrange*pow(G4UniformRand(), 0.333333);
               screenvar = screenfac/(epsil*(1-epsil));
               greject = (ScreenFunction1(screenvar) - FZ)/F10;
             } 
        else { epsil = epsilmin + epsilrange*G4UniformRand();
               screenvar = screenfac/(epsil*(1-epsil));
               greject = (ScreenFunction2(screenvar) - FZ)/F20;
             }

   } while( greject < G4UniformRand() );

   }   //  end of epsil sampling
   
   //
   // fixe charges randomly
   //

   G4double ElectTotEnergy, PositTotEnergy;
   if (RandBit::shootBit())
     {
       ElectTotEnergy = (1.-epsil)*GammaEnergy;
       PositTotEnergy = epsil*GammaEnergy;
     }
   else
     {
       PositTotEnergy = (1.-epsil)*GammaEnergy;
       ElectTotEnergy = epsil*GammaEnergy;
     }

   //
   // scattered electron (positron) angles. ( Z - axis along the parent photon)
   //
   //  universal distribution suggested by L. Urban 
   // (Geant3 manual (1993) Phys211),
   //  derived from Tsai distribution (Rev Mod Phys 49,421(1977))

   G4double u;
   const G4double a1 = 0.625 , a2 = 3.*a1 , d = 27. ;

   if (9./(9.+d) >G4UniformRand()) 
     u= - log(G4UniformRand()*G4UniformRand())/a1;
   else                            
     u= - log(G4UniformRand()*G4UniformRand())/a2;

   G4double TetEl = u*electron_mass_c2/ElectTotEnergy;
   G4double TetPo = u*electron_mass_c2/PositTotEnergy;
   G4double Phi  = twopi * G4UniformRand();
   G4double dxEl= sin(TetEl)*cos(Phi),
     dyEl= sin(TetEl)*sin(Phi),dzEl=cos(TetEl);
   G4double dxPo=-sin(TetPo)*cos(Phi),
     dyPo=-sin(TetPo)*sin(Phi),dzPo=cos(TetPo);
   
   //
   // kinematic of the created pair
   //
   // the electron and positron are assumed to have a symetric
   // angular distribution with respect to the Z axis along the parent photon.

   if(muonWeight)
     fParticleChange.SetNumberOfSecondaries(4); 
   else
     fParticleChange.SetNumberOfSecondaries(2); 

   G4double ElectKineEnergy = std::max(0.,ElectTotEnergy - electron_mass_c2);
   G4double localEnergyDeposit = 0.;

   if (ElectKineEnergy > fminimalEnergy)
     {
       G4ThreeVector ElectDirection (dxEl, dyEl, dzEl);
       ElectDirection.rotateUz(GammaDirection);   
 
       // create G4DynamicParticle object for the particle1  
       G4DynamicParticle* aParticle1= new G4DynamicParticle(
                        G4Electron::Electron(),ElectDirection,ElectKineEnergy);
       fParticleChange.AddSecondary(aParticle1); 
     }
   else
     { localEnergyDeposit += ElectKineEnergy;}   

   // the e+ is always created (even with Ekine=0) for further annihilation.

   G4double PositKineEnergy = std::max(0.,PositTotEnergy - electron_mass_c2);
   if (PositKineEnergy < fminimalEnergy)
     { localEnergyDeposit += PositKineEnergy; PositKineEnergy = 0.;}

   G4ThreeVector PositDirection (dxPo, dyPo, dzPo);
   PositDirection.rotateUz(GammaDirection);   
 
   // create G4DynamicParticle object for the particle2 
   G4DynamicParticle* aParticle2= new G4DynamicParticle(
                      G4Positron::Positron(),PositDirection,PositKineEnergy);
   fParticleChange.AddSecondary(aParticle2); 

#if G4VERSION > 6
   fParticleChange.ProposeLocalEnergyDeposit(localEnergyDeposit);
#else
   fParticleChange.SetLocalEnergyDeposit(localEnergyDeposit);
#endif

   // --------------------------
   // gab add muons (doesn't conserve energy, but good for stats)
   // Code adopted from G4GammaConversionToMuons
   static const G4double Mmuon=G4MuonPlus::MuonPlus()->GetPDGMass();
   static const G4double Mele=electron_mass_c2;
   static const G4double sqrte=sqrt(exp(1.));
   static G4double Zlast=0,B,Dn,Zthird,Winfty,A027,C1Num2,C2Term2;

   if(muonWeight>0)
     {       
       if(BDSGlobals->GetUseMuonShowers())
	 {
	   G4double Z=anElement->GetZ();
	   G4double A = anElement->GetA()/(g/mole);
	   if(Zlast!=Z) // the element has changed
	     { Zlast=Z;
	     if(Z==1) // special case of Hydrogen
	       { B=202.4;
	       Dn=1.49;
	       }
	     else
	       { B=183.;
	       Dn=1.54*pow(A,0.27);
	       }
	     Zthird=pow(Z,-1./3.); // Z**(-1/3)
	     Winfty=B*Zthird*Mmuon/(Dn*Mele);
	     A027=pow(A,0.27);
	     G4double C1Num=0.35*A027;
	     C1Num2=C1Num*C1Num;
	     C2Term2=Mele/(183.*Zthird*Mmuon);
	     }
	   
	   G4double GammaMuonInv=Mmuon/GammaEnergy;
	   G4double sqrtx=sqrt(.25-GammaMuonInv);
	   G4double xmax=.5+sqrtx;
	   G4double xmin=.5-sqrtx;
	   
	   // generate xPlus according to the differential cross section by rejection
	   G4double Ds2=(Dn*sqrte-2.);
	   G4double sBZ=sqrte*B*Zthird/Mele;
	   G4double LogWmaxInv=1./log(Winfty*(1.+2.*Ds2*GammaMuonInv)
				      /(1.+2.*sBZ*Mmuon*GammaMuonInv));
	   G4double xPlus,xMinus,xPM,result,W;

	   // gab fix an anomalous case:
	   G4int ntry=0;
	   do
	     { xPlus=xmin+G4UniformRand()*(xmax-xmin);
	     xMinus=1.-xPlus;
	     xPM=xPlus*xMinus;
	     G4double del=Mmuon*Mmuon/(2.*GammaEnergy*xPM);
	     W=Winfty*(1.+Ds2*del/Mmuon)/(1.+sBZ*del);
	     if(W<1.) W=1.; // to avoid negative cross section at xmin
	     G4double xxp=1.-4./3.*xPM; // the main xPlus dependence
	     result=xxp*log(W)*LogWmaxInv;
	     if(result>1.)
	       { G4cout << "error in dSigxPlusGen, result=" << result << " is >1" << '\n';
	       exit(10);
	       }
	     // gab fix
	     ntry++;
	     }
	   //	   while (G4UniformRand() > result);
	   while ((G4UniformRand() > result)&&ntry<10);

	   // now generate the angular variables via the auxilary 
	   //variables t,psi,rho
	   G4double t;
	   G4double psi;
	   G4double rho;
	   
	   G4double thetaPlus,thetaMinus,phiHalf; // final angular variables
	   
	   do      // t, psi, rho generation start  (while angle < pi)
	     {
	       //generate t by the rejection method
	       G4double C1=C1Num2* GammaMuonInv/xPM;
	       G4double f1_max=(1.-xPM) / (1.+C1);
	       G4double f1; // the probability density
	       do
		 { t=G4UniformRand();
		 f1=(1.-2.*xPM+4.*xPM*t*(1.-t)) / (1.+C1/(t*t));
		 if(f1<0 | f1> f1_max) // should never happend
		   { G4cout << "outside allowed range f1=" << f1 << G4endl;
		   exit(1);
		   }
		 }
	       while ( G4UniformRand()*f1_max > f1);

	       // generate psi by the rejection method
	       G4double f2_max=1.-2.*xPM*(1.-4.*t*(1.-t));
	       
	       // long version
	       G4double f2;
	       do
		 { psi=2.*pi*G4UniformRand();
		 f2=1.-2.*xPM+4.*xPM*t*(1.-t)*(1.+cos(2.*psi));
		 if(f2<0 | f2> f2_max) // should never happend
		   { G4cout << "outside allowed range f2=" << f2 << G4endl;
		   exit(1);
		   }
		 }
	       while ( G4UniformRand()*f2_max > f2);

	       // generate rho by direct transformation
	       G4double C2Term1=GammaMuonInv/(2.*xPM*t);
	       G4double C2=4./sqrt(xPM)*pow(C2Term1*C2Term1+C2Term2*C2Term2,2);
	       G4double rhomax=1.9/A027*(1./t-1.);
	       G4double beta=log( (C2+pow(rhomax,4))/C2 );
	       rho=pow(C2 *( exp(beta*G4UniformRand())-1. ) ,0.25);
	       
	       //now get from t and psi the kinematical variables
	       G4double u=sqrt(1./t-1.);
	       G4double xiHalf=0.5*rho*cos(psi);
	       phiHalf=0.5*rho/u*sin(psi);
	       
	       thetaPlus =GammaMuonInv*(u+xiHalf)/xPlus;
	       thetaMinus=GammaMuonInv*(u-xiHalf)/xMinus;

	     } while ( fabs(thetaPlus)>pi | fabs(thetaMinus) >pi);

	   // now construct the vectors
	   // azimuthal symmetry, take phi0 at random between 0 and 2 pi
	   G4double phi0=2.*pi*G4UniformRand(); 
	   G4double EPlus=xPlus*GammaEnergy;
	   G4double EMinus=xMinus*GammaEnergy;
	   
	   // mu+ mu- directions for gamma in z-direction
	   G4ThreeVector 
	     MuPlusDirection  ( sin(thetaPlus) *cos(phi0+phiHalf),
				sin(thetaPlus)  *sin(phi0+phiHalf), 
				cos(thetaPlus) );
	   G4ThreeVector MuMinusDirection (-sin(thetaMinus)*cos(phi0-phiHalf),
					   -sin(thetaMinus) *sin(phi0-phiHalf), 
					   cos(thetaMinus) );
	   // rotate to actual gamma direction
	   MuPlusDirection.rotateUz(GammaDirection);
	   MuMinusDirection.rotateUz(GammaDirection);
	   
	   // create G4DynamicParticle object for the particle3
	   G4DynamicParticle* aParticle3= 
	     new G4DynamicParticle(G4MuonPlus::MuonPlus(),
				   MuPlusDirection,EPlus-Mmuon);
	   fParticleChange.AddSecondary(aParticle3);
	   	   
	   // create G4DynamicParticle object for the particle4
	   G4DynamicParticle* aParticle4= 
	     new G4DynamicParticle(G4MuonMinus::MuonMinus(),
				   MuMinusDirection,EMinus-Mmuon);
	   fParticleChange.AddSecondary(aParticle4);	   
	 }
     }
   // --------------------------


   //
   // Kill the incident photon 
   //
   fParticleChange.SetMomentumChange( 0., 0., 0. );
   fParticleChange.SetEnergyChange( 0. ); 

#if G4VERSION > 6   
   //fParticleChange.ProposeMomentumChange( 0., 0., 0. );
   //fParticleChange.ProposeEnergyChange( 0. ); 
   fParticleChange.ProposeTrackStatus( fStopAndKill );
#else
   //fParticleChange.SetMomentumChange( 0., 0., 0. );
   //fParticleChange.SetEnergyChange( 0. ); 
   fParticleChange.SetStatusChange( fStopAndKill );
#endif


   //  Reset NbOfInteractionLengthLeft and return fParticleChange
   // return G4VDiscreteProcess::PostStepDoIt( aTrack, aStep );
   return &fParticleChange;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Element* BDSGammaConversion::SelectRandomAtom(
                                         const G4DynamicParticle* aDynamicGamma,
                                               G4Material* aMaterial)
{
  // select randomly 1 element within the material 

  const G4int NumberOfElements            = aMaterial->GetNumberOfElements();
  const G4ElementVector* theElementVector = aMaterial->GetElementVector();
  if (NumberOfElements == 1) return (*theElementVector)[0];

  const G4double* NbOfAtomsPerVolume = aMaterial->GetVecNbOfAtomsPerVolume();

  G4double PartialSumSigma = 0. ;
  G4double rval = G4UniformRand()/MeanFreePath;
 
  for ( G4int i=0 ; i < NumberOfElements ; i++ )
      { PartialSumSigma += NbOfAtomsPerVolume[i] *
                  GetCrossSectionPerAtom(aDynamicGamma, (*theElementVector)[i]);
        if (rval <= PartialSumSigma) return ((*theElementVector)[i]);
      }
  G4cout << " WARNING !!! - The Material '"<< aMaterial->GetName()
       << "' has no elements, NULL pointer returned." << G4endl;
  return NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool BDSGammaConversion::StorePhysicsTable(const G4ParticleDefinition* particle,
				              const G4String& directory, 
				              G4bool          ascii)
{
  G4String filename;

  // store cross section table
  filename = GetPhysicsTableFileName(particle,directory,"CrossSection",ascii);
  if ( !theCrossSectionTable->StorePhysicsTable(filename, ascii) ){
    G4cout << " FAIL theCrossSectionTable->StorePhysicsTable in " << filename
           << G4endl;
    return false;
  }

  // store mean free path table
  filename = GetPhysicsTableFileName(particle,directory,"MeanFreePath",ascii);
  if ( !theMeanFreePathTable->StorePhysicsTable(filename, ascii) ){
    G4cout << " FAIL theMeanFreePathTable->StorePhysicsTable in " << filename
           << G4endl;
    return false;
  }
  
  G4cout << GetProcessName() << " for " << particle->GetParticleName()
         << ": Success to store the PhysicsTables in "  
         << directory << G4endl;
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool BDSGammaConversion::RetrievePhysicsTable(const G4ParticleDefinition* particle,
					         const G4String& directory, 
				                 G4bool          ascii)
{
  // delete theCrossSectionTable and theMeanFreePathTable
  if (theCrossSectionTable != 0) {
    theCrossSectionTable->clearAndDestroy();
    delete theCrossSectionTable;
  }
  if (theMeanFreePathTable != 0) {
    theMeanFreePathTable->clearAndDestroy();
    delete theMeanFreePathTable;
  }

  G4String filename;

  // retreive cross section table
  filename = GetPhysicsTableFileName(particle,directory,"CrossSection",ascii);
  theCrossSectionTable = new G4PhysicsTable(G4Element::GetNumberOfElements());
  if ( !theCrossSectionTable->RetrievePhysicsTable(filename, ascii) ){
    G4cout << " FAIL theCrossSectionTable->RetrievePhysicsTable in " << filename
           << G4endl;  
    return false;
  }

  // retreive mean free path table
  filename = GetPhysicsTableFileName(particle,directory,"MeanFreePath",ascii);
  theMeanFreePathTable = new G4PhysicsTable(G4Material::GetNumberOfMaterials());
  if ( !theMeanFreePathTable->RetrievePhysicsTable(filename, ascii) ){
    G4cout << " FAIL theMeanFreePathTable->RetrievePhysicsTable in " << filename
           << G4endl;  
    return false;\
  }
  
  G4cout << GetProcessName() << " for " << particle->GetParticleName()
         << ": Success to retrieve the PhysicsTables from "
         << directory << G4endl;
  return true;
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BDSGammaConversion::PrintInfoDefinition()
{
  G4String comments = "Total cross sections from a parametrisation. ";
           comments += "Good description from 1.5 MeV to 100 GeV for all Z. \n";
           comments += "        e+e- energies according Bethe-Heitler";
                     
  G4cout << G4endl << GetProcessName() << ":  " << comments
         << "\n        PhysicsTables from " 
	           << G4BestUnit(LowestEnergyLimit, "Energy")
         << " to " << G4BestUnit(HighestEnergyLimit,"Energy") 
         << " in " << NumbBinTable << " bins. \n";
}         


G4double BDSGammaConversion::ComputeMuonCrossSectionPerAtom(
                         G4double Egam, G4double Z, G4double A)
// Calculates the microscopic cross section in GEANT4 internal units.
// Total cross section parametrisation from H.Burkhardt
// It gives a good description at any energy (from 0 to 10**21 eV)
{ static const G4double Mmuon=G4MuonPlus::MuonPlus()->GetPDGMass();
  static const G4double Mele=electron_mass_c2;
  static const G4double GammaEnergyLimit=4* Mmuon;
  static const G4double Rc=elm_coupling/Mmuon; // classical particle radius
  static const G4double sqrte=sqrt(exp(1.));
  static const G4double PowSat=-0.88;

  static G4double CrossSection = 0.0 ;

  if ( A < 1. ) return 0;
  if ( Egam < GammaEnergyLimit ) return 0 ; // below threshold return 0

  static G4double EgamLast=0,Zlast=0,PowThres,Ecor,B,Dn,Zthird,Winfty,WMedAppr,
      Wsatur,sigfac;
  
  if(Zlast==Z && Egam==EgamLast) return CrossSection; // already calculated
  EgamLast=Egam;
  
  if(Zlast!=Z) // new element
  { Zlast=Z;
    if(Z==1) // special case of Hydrogen
    { B=202.4;
      Dn=1.49;
    }
    else
    { B=183.;
      Dn=1.54*pow(A,0.27);
    }
    Zthird=pow(Z,-1./3.); // Z**(-1/3)
    Winfty=B*Zthird*Mmuon/(Dn*Mele);
    WMedAppr=1./(4.*Dn*sqrte*Mmuon);
    Wsatur=Winfty/WMedAppr;
    sigfac=4.*fine_structure_const*Z*Z*Rc*Rc;
    PowThres=1.479+0.00799*Dn;
    Ecor=-18.+4347./(B*Zthird);
  }
  G4double CorFuc=1.+.04*log(1.+Ecor/Egam);
  G4double Eg=pow(1.-4.*Mmuon/Egam,PowThres)*pow( pow(Wsatur,PowSat)+
              pow(Egam,PowSat),1./PowSat); // threshold and saturation
  CrossSection=7./9.*sigfac*log(1.+WMedAppr*CorFuc*Eg);
  //  CrossSection*=CrossSecFactor; // increase the CrossSection by  (by default 1)
  return CrossSection;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
