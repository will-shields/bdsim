//  
//   BDSIM, (C) 2001-2008 
//   
//   version 0.5-dev
//  
//
//
//   Main code
//
//
//   History
//     17 Jul 2007 by Malton v.0.4
//     26 Jan 2007 by Agapov v.0.3
//     28 Mar 2006 by Agapov v.0.2
//     
//

#include "BDSDebug.hh" 
#include "BDSExecOptions.hh"     // executable command line options 
#include "BDSGlobalConstants.hh" // global parameters

#include "G4UImanager.hh"        // G4 session managers
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4GeometryManager.hh"

#include "Randomize.hh"

#ifdef G4VIS_USE
#include "BDSVisManager.hh"
#endif


#include <cstdlib>      // standard headers 
#include <cstdio>
#include <ctime>
#include <unistd.h>
#include <getopt.h>


#include "BDSDetectorConstruction.hh"   // Geant4 includes
#include "BDSPhysicsList.hh"
#include "QGSP_BERT.hh"
#include "QGSP_BERT_HP.hh"
#include "BDSPrimaryGeneratorAction.hh"
#include "BDSRunAction.hh"
#include "BDSEventAction.hh"
#include "BDSSteppingAction.hh"
#include "BDSStackingAction.hh"
#include "BDSUserTrackingAction.hh"
#include "BDSRunManager.hh"
#include "G4EventManager.hh"
#include "G4TrackingManager.hh"
#include "G4SteppingManager.hh"
#include "G4GeometrySampler.hh"
#include "G4ImportanceAlgorithm.hh"
#include "G4GeometryTolerance.hh"

#include "BDSGeometryInterface.hh"

#include "BDSOutput.hh" 
#include "BDSBunch.hh"
#include "BDSMaterials.hh"

#include "parser/gmad.h"  // GMAD parser


//=======================================================

// Print program usage
static void usage()
{
  G4cout<<"bdsim : version 0.6.0"<<G4endl;
  G4cout<<"        (C) 2001-2013 Royal Holloway University London"<<G4endl;
  G4cout<<"        http://www.ph.rhul.ac.uk/twiki/bin/view/PP/JAI/BdSim"<<G4endl;
  G4cout<<G4endl;

  G4cout<<"Usage: bdsim [options]"<<G4endl;
  G4cout<<"Options:"<<G4endl;
  G4cout<<"--file=<filename>     : specify the lattice file "<<G4endl
	<<"--output=<fmt>        : output format (root|ascii), default ascii"<<G4endl
	<<"--outfile=<file>      : output file name. Will be appended with _N"<<G4endl
        <<"                        where N = 0, 1, 2, 3... etc."<<G4endl
	<<"--vis_mac=<file>      : file with the visualization macro script, default vis.mac"<<G4endl
	<<"--gflash=N            : whether or not to turn on gFlash fast shower parameterisation. Default 0."<<G4endl
	<<"--gflashemax=N            : maximum energy for gflash shower parameterisation in GeV. Default 10000."<<G4endl
	<<"--gflashemin=N            : minimum energy for gflash shower parameterisation in GeV. Default 0.1."<<G4endl
	<<"--help                : display this message"<<G4endl
	<<"--verbose             : display general parameters before run"<<G4endl
    	<<"--verbose_event       : display information for every event "<<G4endl
    	<<"--verbose_step        : display tracking information after each step"<<G4endl
	<<"--verbose_event_num=N : display tracking information for event number N"<<G4endl
	<<"--batch               : batch mode - no graphics"<<G4endl
	<<"--outline=<file>      : print geometry info to <file>"<<G4endl
	<<"--outline_type=<fmt>  : type of outline format"<<G4endl
	<<"                        where fmt = optics | survey"<<G4endl
	<<"--materials           : list materials included in bdsim by default"<<G4endl;
}

BDSOutput*    bdsOutput;         // output interface
BDSBunch      bdsBunch;          // bunch information 
BDSSamplerSD* BDSSamplerSensDet; // sampler???

G4String outputFilename="output";  //receives a .txt or .root in BDSOutput
G4bool verbose = false;  // run options
G4bool verboseStep = false;
G4bool verboseEvent = false;
G4int verboseEventNumber = -1;
G4int gflash = 0;
G4double gflashemax = 10000;
G4double gflashemin = 0.1;
G4bool isBatch = false;



G4int nptwiss = 200; // number of particles for twiss parameters matching (by tracking) and reference bunch for wakefields

//=======================================================

int main(int argc,char** argv) {

  /* Executable command line options reader object */
  BDSExecOptions *bdsOptions = BDSExecOptions::Instance();
  bdsOptions->Parse(argc,argv);
  bdsOptions->Usage();
  bdsOptions->Print();

  BDSOutputFormat outputFormat=_ASCII;
  char *fifoName=NULL;  //receives a .txt or .root in BDSOutput
  G4String outlinefile="BDSOutline.dat";  
  G4String outlineType="";
  G4String inputFilename= "optics.gmad"; // input file with gmad lattice description
  G4String visMacroFile="vis.mac"; // visualization macro file
  G4bool outline = false;

  G4int verboseRunLevel = 0;
  G4int verboseEventLevel = 0;
  G4int verboseTrackingLevel = 0;
  G4int verboseSteppingLevel = 0;


#ifdef DEBUG
  G4cout << __METHOD_NAME__ << " DEBUG mode is on." << G4endl;
#endif  

  //
  // Parse the command line options 
  //
  
  static struct option LongOptions[] = {
    { "help" , 0, 0, 0 },
    { "outline", 1, 0, 0 },
    { "outline_type", 1, 0, 0 },
    { "verbose", 0, 0, 0 },
    { "verbose_step", 0, 0, 0 },
    { "verbose_event", 0, 0, 0 },
    { "verbose_event_num", 1, 0, 0 },
    { "verbose_G4run", 1, 0, 0 },
    { "verbose_G4event", 1, 0, 0 },
    { "verbose_G4tracking", 1, 0, 0 },
    { "verbose_G4stepping", 1, 0, 0 },
    { "file", 1, 0, 0 },
    { "vis_mac", 1, 0, 0 },
    { "gflash", 1, 0, 0 },
    { "gflashemax", 1, 0, 0 },
    { "gflashemin", 1, 0, 0 },
    { "output", 1, 0, 0 },
    { "outfile", 1, 0, 0 },
    { "fifo", 1, 0, 0 },
    { "batch", 0, 0, 0 },
    { "materials", 0, 0, 0 },
    { 0, 0, 0, 0 }
  };
  
  int OptionIndex = 0;
  int c;
//   int ThisOptionId;
 
  for(;;)
    {
      
//       ThisOptionId = optind ? optind : 1;
      OptionIndex = 0;
      
      c = getopt_long(argc, argv, "Vv",
		      LongOptions, &OptionIndex );
      
      if ( c == -1 ) // end of options list
  	break;
      
      switch (c) {
      case 0:
	if( !strcmp(LongOptions[OptionIndex].name , "help") )
	  {
	    usage();
	    return 1;
	  }
	if( !strcmp(LongOptions[OptionIndex].name , "batch") )
	  {
	    isBatch = true;
	  }
	if( !strcmp(LongOptions[OptionIndex].name , "verbose") )
	  {
	    verbose = true; 
	  }
	if( !strcmp(LongOptions[OptionIndex].name , "verbose_step") )
	  {
	    verboseStep = true; 
	    // we shouldn't have verbose steps without verbose events etc.
	    verboseEvent = true;
	  }
	if( !strcmp(LongOptions[OptionIndex].name , "verbose_event") )
	  {
	    verboseEvent = true; 
	  }
	if( !strcmp(LongOptions[OptionIndex].name , "verbose_event_num") )
	  {
	    if(optarg)
	      verboseEventNumber = atoi(optarg); 
	  }
	if( !strcmp(LongOptions[OptionIndex].name , "verbose_G4run") )
	  {
	    if(optarg)
	      verboseRunLevel = atoi(optarg);
	  }
	if( !strcmp(LongOptions[OptionIndex].name , "verbose_G4event") )
	  {
	    if(optarg)
	      verboseEventLevel = atoi(optarg);
	  }
	if( !strcmp(LongOptions[OptionIndex].name , "verbose_G4tracking") )
	  {
	    if(optarg)
	      verboseTrackingLevel = atoi(optarg);
	  }
	if( !strcmp(LongOptions[OptionIndex].name , "verbose_G4stepping") )
	  {
	    if(optarg)
	      verboseSteppingLevel = atoi(optarg);
	  }
	if( !strcmp(LongOptions[OptionIndex].name , "output") )
	  {
	    if(optarg) {
	      if(!strcmp(optarg,"ascii") || !strcmp(optarg,"ASCII")) outputFormat=_ASCII;
	      else if (!strcmp(optarg,"root") || !strcmp(optarg,"ROOT")) outputFormat=_ROOT;
	      else {
		G4cerr<<"unknown output format "<<optarg<<G4endl;
		exit(1);
	      }
#ifndef USE_ROOT
	      if (outputFormat == _ROOT) {
		G4cerr << "ERROR outputFormat root, but BDSIM not configured with ROOT support!" << G4endl;
		G4cerr << "Use ascii instead, or recompile with ROOT!" << G4endl;
		exit(1);
	      }
#endif
	    }
	  }
	if( !strcmp(LongOptions[OptionIndex].name , "outfile") )
	  {
	    if(optarg) {
	      outputFilename=optarg;
	    }
	  }
	if( !strcmp(LongOptions[OptionIndex].name , "fifo") )
	  {
	    if(optarg) {
	      fifoName=optarg;
	    }
	  }
	if( !strcmp(LongOptions[OptionIndex].name , "outline") )
	  {
	    if(optarg) outlinefile = optarg; 
	    outline=true;
	  }
	if( !strcmp(LongOptions[OptionIndex].name , "outline_type") )
	  {
	    if(optarg) outlineType = optarg; 
	    outline=true;  // can't have outline type without turning on outline!
	  }
	if( !strcmp(LongOptions[OptionIndex].name , "file") )
	  {
	    if(optarg) {
	      inputFilename=optarg;
	    }
	    else {
	      G4cout<<"please specify the lattice filename"<<G4endl;
	    }
	  }
	if( !strcmp(LongOptions[OptionIndex].name , "vis_mac") )
	  {
	    if(optarg) {
	      visMacroFile=optarg;
	    }
	    else {
	      G4cout<<"please specify the visualization macro file"<<G4endl;
	    }
	  }
	if( !strcmp(LongOptions[OptionIndex].name , "gflash") )
	  {
	    if(optarg)
	      gflash = atoi(optarg); 
	  }
	if( !strcmp(LongOptions[OptionIndex].name , "gflashemax") )
	  {
	    if(optarg)
	      gflashemax = atof(optarg); 
	  }
	if( !strcmp(LongOptions[OptionIndex].name , "gflashemin") )
	  {
	    if(optarg)
	      gflashemin = atof(optarg); 
	  }
	if( !strcmp(LongOptions[OptionIndex].name, "materials") )
	  {
	    BDSMaterials::ListMaterials();
	    return 1;
	  }
	break;
      // case -1:
      // 	break;
      default:
	break;
      }
      
    }


  //
  // parse lattice file
  //

  G4cout<<"Using input file: "<<inputFilename<<G4endl;

  if( gmad_parser(inputFilename) == -1)
    {
      G4cout<<"can't open input file "<<inputFilename<<G4endl;
      exit(1);
    }


  //
  // pass the run control and beam options read from the lattice
  // file via the gmad parser to the BDSGlobalConstants and 
  // to the BDSBunch instances
  //

#ifdef DEBUG
  G4cout << "Setting global constants." << G4endl;
#endif  

  //  BDSGlobals = new BDSGlobalConstants(options);
  if (fifoName) {
    //    BDSGlobals->SetFifo(fifoName);
    BDSGlobalConstants::Instance()->SetFifo(fifoName);
  }

#ifdef DEBUG
  G4cout << "Setting bunch options." << G4endl;
#endif  

  bdsBunch.SetOptions(options);

  //
  // set default output formats:
  //
#ifdef DEBUG
  G4cout << "Setting up output." << G4endl;
#endif  
  bdsOutput = new BDSOutput();
  bdsOutput->SetFormat(outputFormat);
  G4cout.precision(10);

  //
  // initialize random number generator
  //

  // choose the Random engine
#ifdef DEBUG
  G4cout << "Initialising random number generator." << G4endl;
#endif  
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

  long seed;

  // get the seed from options if positive, else
  // user time as a seed

  if(BDSGlobalConstants::Instance()->GetRandomSeed()>=0)
    seed = BDSGlobalConstants::Instance()->GetRandomSeed();
  else
    seed = time(NULL);
  
  // set the seed
  CLHEP::HepRandom::setTheSeed(seed);

#ifdef DEBUG
  G4cout<<"Seed from BDSGlobalConstants="<<BDSGlobalConstants::Instance()->GetRandomSeed()<<G4endl;
#endif

  G4cout<<"Random number generator's seed="<<CLHEP::HepRandom::getTheSeed()<<G4endl;


  //
  // construct mandatory run manager (the G4 kernel) and
  // set mandatory initialization classes
  //

#ifdef DEBUG 
  G4cout<<"constructing run manager"<<G4endl;
#endif
  BDSRunManager * runManager = new BDSRunManager;
  // runManager->SetNumberOfAdditionalWaitingStacks(1);


  //For geometry sampling, phys list must be initialized before detector.
#ifdef DEBUG 
  G4cout<<"constructing phys list"<<G4endl;
#endif

  BDSPhysicsList* PhysList=new BDSPhysicsList;
  runManager->SetUserInitialization(PhysList);
  
#ifdef DEBUG 
  G4cout<<"user init phys list"<<G4endl;
#endif

  // Set the geometry tolerance
  static G4GeometryTolerance* theGeometryTolerance = G4GeometryTolerance::GetInstance();
  G4cout << "main> default geometry tolerances: surface " << theGeometryTolerance->GetSurfaceTolerance() << " " 
	 << theGeometryTolerance->GetAngularTolerance() << " " 
	 << theGeometryTolerance->GetRadialTolerance() << " " <<G4endl;
  G4double worldMaximumExtent=1000*m;
  // This sets the tolerances for the geometry (1e-11 times this value)
  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldMaximumExtent); 
  G4cout << "main> geometry toleranceswith worldMaximumExtent=" << worldMaximumExtent/m << "m: surface: " 
	 << theGeometryTolerance->GetSurfaceTolerance() << " angular: " << theGeometryTolerance->GetAngularTolerance() 
	 << " radial: " << theGeometryTolerance->GetRadialTolerance() << " " <<G4endl;
  
  
#ifdef DEBUG 
  G4cout<<"constructing detector"<<G4endl;
#endif
  BDSDetectorConstruction* detector = new BDSDetectorConstruction;
 
#ifdef DEBUG 
  G4cout<<"user init detector"<<G4endl;
#endif
  runManager->SetUserInitialization(detector);


  //
  // Set user action classes
  //
#ifdef DEBUG 
  G4cout<<"main: user action - runaction"<<G4endl;
#endif
  runManager->SetUserAction(new BDSRunAction);

#ifdef DEBUG 
  G4cout<<"main: user action - eventaction"<<G4endl;
#endif
  runManager->SetUserAction(new BDSEventAction());

#ifdef DEBUG 
  G4cout<<"main: user action - steppingaction"<<G4endl;
#endif
  runManager->SetUserAction(new BDSSteppingAction);

#ifdef DEBUG 
  G4cout<<"main: user action - trackingaction"<<G4endl;
#endif
  runManager->SetUserAction(new BDSUserTrackingAction);

#ifdef DEBUG 
  G4cout<<"main: user action - stackingaction"<<G4endl;
#endif
  runManager->SetUserAction(new BDSStackingAction);

#ifdef DEBUG 
  G4cout<<"main: user action - detector"<<G4endl;
#endif
  runManager->SetUserAction(new BDSPrimaryGeneratorAction(detector));

  

  //
  // Initialize G4 kernel
  //
#ifdef DEBUG 
  G4cout<<"init kernel"<<G4endl;
#endif
  runManager->Initialize();

  //Create a geometric importance sampling store
  //  if(BDSGlobalConstants::Instance()->GetGeometryBias()){
  //    G4VIStore *aIstore = 0;
  //    aIstore = detector->CreateImportanceStore();
  //    G4GeometrySampler mgs(detector->GetWorldVolume(),"neutron");
  //    mgs.SetParallel(false);
  //    G4ImportanceAlgorithm* importanceAlgorithm = new G4ImportanceAlgorithm();
  //    mgs.PrepareImportanceSampling(aIstore, 0);//,0);
  //    mgs.Configure();
  //  }


  //
  // Set verbosity levels
  //
  runManager->SetVerboseLevel(verboseRunLevel);
  G4EventManager::GetEventManager()->SetVerboseLevel(verboseEventLevel);
  G4EventManager::GetEventManager()->GetTrackingManager()->SetVerboseLevel(verboseTrackingLevel);
  G4EventManager::GetEventManager()->GetTrackingManager()->GetSteppingManager()->SetVerboseLevel(verboseSteppingLevel);

  //
  // Close the geometry
  //
  try {
    G4bool bCloseGeometry = G4GeometryManager::GetInstance()->CloseGeometry(true,true);
    if(!bCloseGeometry) throw "bdsim.cc: error - geometry not closed.";
  }
  catch (char* strng) {
    G4cerr << "Exception raised: " << strng << G4endl;
    return 1;
  }

  bdsOutput->Init(0); // activate the output - setting the first filename to 
                     // be appended with _0

  //
  // Write survey file
  //

  if(outline) {
#ifdef DEBUG 
    G4cout<<"contructing geometry interface"<<G4endl;
#endif
    BDSGeometryInterface* BDSGI = new BDSGeometryInterface(outlinefile);

#ifdef DEBUG 
    G4cout<<"writing survey file"<<G4endl;
#endif
    if(outlineType=="survey") BDSGI->Survey();
    if(outlineType=="optics") BDSGI->Optics();

#ifdef DEBUG 
    G4cout<<"deleting geometry interface"<<G4endl;
#endif
    delete BDSGI;
  }

  //
  // Track nptwiss particles for beta functions 
  // and SR Rescaling. SR rescaling is adjusting the magnet fields according to
  // k-values considering the beam energy loss due to SR
  //
  if(BDSGlobalConstants::Instance()->DoTwiss())
    {

      G4cout<<"do twiss"<<G4endl;
      
      // disable SR process if present - analytical formulae used in rescaling
      G4ProcessManager *pManager = G4Electron::Electron()->GetProcessManager(); 	 
      G4ProcessVector *procVec=pManager->GetProcessList(); 	 
      G4int nProc=pManager->GetProcessListLength(); 	 
      
      
      for(G4int iProc=0;iProc<nProc;iProc++) 	 
	{ 	 
	  G4String pName=(*procVec)[iProc]->GetProcessName(); 	 
	  if(pName=="BDSSynchRad")  	 
	    { 	 
	      G4cout<<"Disabling SR"<<G4endl;
	      pManager->SetProcessActivation(iProc, false);

	    } 	 

	  if(pName=="contSR")  	 
	    { 	 
	      G4cout<<"Enabling constSR"<<G4endl;
	      pManager->SetProcessActivation(iProc, true);
	      
	    } 	 
	}

      // do not need secondaries whatsoever
      BDSGlobalConstants::Instance()->SetStopTracks(true);

      runManager->BeamOn(nptwiss);

      // Clear Stack
      G4EventManager::GetEventManager()->GetStackManager()->ClearPostponeStack();
      
      // turn  SR back on
      BDSGlobalConstants::Instance()->SetSynchTrackPhotons(options.synchTrackPhotons);

      //restore the stoptracks flag
      BDSGlobalConstants::Instance()->SetStopTracks(options.stopTracks);

      for(G4int iProc=0;iProc<nProc;iProc++) 	 
	{ 	 
	  G4String pName=(*procVec)[iProc]->GetProcessName(); 	 
	  if(pName=="BDSSynchRad")  	 
	    { 	 
	      G4cout<<"Enabling SR"<<G4endl;
	      pManager->SetProcessActivation(iProc, true);
	      
	    } 	 

	  if(pName=="contSR")  	 
	    { 	 
	      G4cout<<"Disabling constSR"<<G4endl;
	      pManager->SetProcessActivation(iProc, false);
	      
	    } 	 

	}

      G4cout<<"done"<<G4endl;
    
    }
  
  // now turn off SR Rescaling 
  BDSGlobalConstants::Instance()->SetDoTwiss(false);
  BDSGlobalConstants::Instance()->SetSynchRescale(false);


  //
  // Start the simulation
  // If not running in batch:
  //   1) start interactive session
  //   2) if visualisation requested, initialise visual manager
  //   3) execute visualisation macro (defined with option --vis_mac)
  //   4) wait for user input
  // else 
  //   generate and track the particles of the bunch as 
  //   defined by the user in the gmad input file
  //

  G4UIsession* session=0;
  G4VisManager* visManager=0;

 
  if(!isBatch)   // Interactive mode
    {
#ifdef G4UI_USE_TCSH
      session = new G4UIterminal(new G4UItcsh);
#else
      session = new G4UIterminal();
#endif    

#ifdef G4VIS_USE
#ifdef DEBUG 
      G4cout<<"Initializing Visualisation Manager"<<G4endl;
#endif
      visManager = new BDSVisManager;
      visManager->Initialize();
#endif
  
      // get the pointer to the User Interface manager 
      G4UImanager* UI = G4UImanager::GetUIpointer();  
      
      UI->ApplyCommand("/control/execute " + visMacroFile);    
      session->SessionStart();
      delete session;

#ifdef G4VIS_USE
#ifdef DEBUG 
      G4cout<<"Visualisation Manager deleting..."<<G4endl;
#endif
      delete visManager;
#endif
    }
  else           // Batch mode
    { 
      runManager->BeamOn(BDSGlobalConstants::Instance()->GetNumberToGenerate());
    }

  //
  // job termination
  //
  G4GeometryManager::GetInstance()->OpenGeometry();

#ifdef DEBUG 
  G4cout<<"BDSOutput deleting..."<<G4endl;
#endif
  delete bdsOutput;

  #ifdef DEBUG 
  G4cout<<"BDSGlobalConstants::Instance() deleting..."<<G4endl;
#endif
  delete BDSGlobalConstants::Instance();
  
#ifdef DEBUG 
  G4cout<<"BDSRunManager deleting..."<<G4endl;
#endif
  delete runManager;
 
  G4cout << "End of Run, Thank you for using BDSIM!" << G4endl;

  return 0;
}
