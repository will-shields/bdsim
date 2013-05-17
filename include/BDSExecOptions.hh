/* BDSIM code.    
   Author: Stewart T. Boogert , Royal Holloway, Univ. of London.
   Last modified 16.04.2013
   Copyright (c) 2002 by BDSIM authors.  ALL RIGHTS RESERVED. 
*/

#ifndef BDSExecOptions_h
#define BDSExecOptions_h

#include "G4Types.hh"
#include "BDSDebug.hh"
#include "BDSOutput.hh"

class BDSExecOptions
{
public: 

  static BDSExecOptions* Instance();
  ~BDSExecOptions();

  void Usage();
  void Parse(int arcg, char **argv);
  void Print(); 

  G4String GetInputFilename()                   {return inputFilename;}
  void     SetInputFilename(G4String s)         {inputFilename = s;}       
  G4String GetVisMacroFilename()                {return visMacroFilename;}
  void     SetVisMacroFilename(G4String s)      {visMacroFilename = s;}
  G4String GetOutputFilename()                  {return outputFilename;}
  void     SetOutputFilename(G4String s)        {outputFilename = s;}
  BDSOutputFormat GetBDSOutputFormat()          {return outputFormat;}
  void     SetBDSOutputFormat(BDSOutputFormat f){outputFormat = f;}
  G4String GetFillOutputFilename()              {return outputFilename;}
 
  G4int    GetGFlash()               {return gflash;}
  void     SetGFlash(G4bool b)       {gflash = b;}
  G4double GetGFlashEMax()           {return gflashemax;}
  void     SetGFlashEMax(G4double d) {gflashemax = d;}
  G4double GetGFlashEMin()           {return gflashemin;}  
  void     SetGFlashEMin(G4double d) {gflashemin = d;}

protected : 
  BDSExecOptions();
  static BDSExecOptions* _instance;
  
private :
  G4String inputFilename;
  G4String visMacroFilename;
  G4String outputFilename;
  BDSOutputFormat outputFormat;
  G4String outlineFilename;

  G4int    gflash;
  G4double gflashemax;
  G4double gflashemin;

  G4bool verbose;
  G4bool verboseEvent;
  G4bool verboseStep;
  G4bool batch; 
  G4bool outline;
  G4bool listMaterials;
  
  G4int verboseRunLevel;
  G4int verboseEventLevel;
  G4int verboseTrackLevel;
  G4int verboseSteppingLevel;
};

#endif
