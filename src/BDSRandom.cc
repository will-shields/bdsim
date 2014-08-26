#include "BDSRandom.hh"
#include "BDSExecOptions.hh"
#include "BDSGlobalConstants.hh"
#include "BDSDebug.hh"

#include "globals.hh" //G4 cout etc

#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/JamesRandom.h"

#include <ctime>

void BDS::CreateRandomNumberGenerator()
{
  // choose the Random engine
#ifdef BDSDEBUG
  G4cout << __FUNCTION__ << "> Initialising random number generator." << G4endl;
#endif  
  CLHEP::HepRandom::setTheEngine(new CLHEP::HepJamesRandom);
  G4cout << __FUNCTION__ << " " << CLHEP::HepRandom::getTheEngine() << G4endl;
}

void BDS::SetSeed()
{
#ifdef BDSDEBUG
  G4cout << __FUNCTION__ << "> set the seed" << G4endl;
  G4cout << __FUNCTION__ << "> Seed from BDSGlobalConstants = " 
	 << BDSGlobalConstants::Instance()->GetRandomSeed() << G4endl
         << __FUNCTION__ << "> Seed from BDSExecOptions = "
         << BDSExecOptions::Instance()->GetSeed() << G4endl
         << __FUNCTION__ << "> seed set in exec options : "
	 << BDSExecOptions::Instance()->SetSeed() << G4endl;
#endif
  // get seed from options if set
  // override with seed from execoptions if specifed
  
  // if seed positive set it, else use the time
  long seed = 0;
  if(BDSGlobalConstants::Instance()->GetRandomSeed()<=0)
    seed = time(NULL);
  else
    seed = BDSGlobalConstants::Instance()->GetRandomSeed();
  
  // if the seed was set by command line (exec) option - override the general option
  if(BDSExecOptions::Instance()->SetSeed())
    {seed = BDSExecOptions::Instance()->GetSeed();}

#ifdef BDSDEBUG
  G4cout << __FUNCTION__ << "> selected seed = " << seed << G4endl;
#endif

  CLHEP::HepRandom::setTheSeed(seed);

  // feedback - get the seed from the generator itself (ensures set correctly)
  G4cout << __FUNCTION__ << "> Random number generator's seed = "
	 << CLHEP::HepRandom::getTheSeed() << G4endl;
#ifdef BDSDEBUG
  BDS::PrintFullSeedState();
#endif
}

void BDS::PrintFullSeedState()
{
  G4cout << __FUNCTION__ << "> Random number generator's state: " << G4endl << G4endl;
  CLHEP::HepRandom::saveFullState(G4cout);
  G4cout << G4endl;
}

void BDS::WriteSeedState()
{
  // get the full seed state and write it to a file
  // Print generator full state to output 

  G4String seedstatefilename = BDSExecOptions::Instance()->GetOutputFilename() + ".seedstate.txt";
  std::ofstream ofseedstate (seedstatefilename.c_str());
  if (ofseedstate.is_open())
    {CLHEP::HepRandom::saveFullState(ofseedstate);}
  else
    {
      G4cout << __FUNCTION__ << "> cannot open file : " << seedstatefilename << G4endl;
      exit(1);
    }
}

void BDS::LoadSeedState(G4String inSeedFilename)
{
#ifdef BDSDEBUG
  G4cout << __FUNCTION__ << "> loading file: " << inSeedFilename << G4endl;
#endif
  std::ifstream ifseedstate;
  ifseedstate.open(inSeedFilename);
  if (ifseedstate.is_open())
    {CLHEP::HepRandom::restoreFullState(ifseedstate);}
  else
    {
      G4cout << __FUNCTION__ << "> cannot open file : " << inSeedFilename << G4endl;
      exit(1);
    }
  ifseedstate.close();
#ifdef BDSDEBUG
  BDS::PrintFullSeedState();
#endif
}


