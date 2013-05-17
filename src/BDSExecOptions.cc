#include "BDSExecOptions.hh"

BDSExecOptions* BDSExecOptions::_instance=0;

BDSExecOptions* BDSExecOptions::Instance(){
  if(_instance==0) {
    _instance = new BDSExecOptions();
  }
  return _instance;
}

BDSExecOptions::BDSExecOptions() {  
  inputFilename       = "optics.mad";
  visMacroFilename    = "vis.mac";
  outputFilename      = "output";
  outputFormat = _ASCII;
  outlineFilename     = "outline.dat";

  gflash      = 0;
  gflashemax  = 10000;
  gflashemin  = 0.1;

  verbose       = false;
  verboseEvent  = false;
  verboseStep   = false;
  batch         = false; 
  outline       = false;
  listMaterials = false;
  
  verboseRunLevel      = 0;
  verboseEventLevel    = 0;
  verboseTrackLevel    = 0;
  verboseSteppingLevel = 0;
}

void BDSExecOptions::Parse(int argc, char **argv) {
  
}

void BDSExecOptions::Usage() {
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
	<<"--gflashemax=N        : maximum energy for gflash shower parameterisation in GeV. Default 10000."<<G4endl
	<<"--gflashemin=N        : minimum energy for gflash shower parameterisation in GeV. Default 0.1."<<G4endl
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
void BDSExecOptions::Print() {
  G4cout << __METHOD_NAME__ << G4endl;
  G4cout << __METHOD_NAME__ << inputFilename       << G4endl;
  G4cout << __METHOD_NAME__ << visMacroFilename    << G4endl;
  G4cout << __METHOD_NAME__ << outputFilename      << G4endl;
  G4cout << __METHOD_NAME__ << outputFormat        << G4endl;
  G4cout << __METHOD_NAME__ << outlineFilename     << G4endl;
  G4cout << __METHOD_NAME__ << gflash              << G4endl;
  G4cout << __METHOD_NAME__ << gflashemin          << G4endl;  
  G4cout << __METHOD_NAME__ << gflashemax          << G4endl;
  G4cout << __METHOD_NAME__ << verbose             << G4endl;
  G4cout << __METHOD_NAME__ << verboseEvent        << G4endl;  
  G4cout << __METHOD_NAME__ << verboseStep         << G4endl;  
  G4cout << __METHOD_NAME__ << batch               << G4endl;
  G4cout << __METHOD_NAME__ << outline             << G4endl;
  G4cout << __METHOD_NAME__ << listMaterials       << G4endl;
  G4cout << __METHOD_NAME__ << verboseRunLevel     << G4endl;  
  G4cout << __METHOD_NAME__ << verboseEventLevel   << G4endl;
  G4cout << __METHOD_NAME__ << verboseTrackLevel   << G4endl;  
  G4cout << __METHOD_NAME__ << verboseSteppingLevel<< G4endl;

  return;
}
