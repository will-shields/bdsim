/*
 * GMAD interface 
 * I. Agapov 2005-2006
 * bdsim v.0.3
 *
 * modification history:
*/

#ifndef _GMAD_H
#define _GMAD_H

#define _RESERVED 1

#include <iostream>
#include <stdio.h>
#include <list>
#include <string>

using namespace std;

// types of elements

const int _NONE = -1;
const int _MARKER = 1;
const int _DRIFT = 2;
const int _RF = 3;
const int _SBEND = 4; 
const int _QUAD  = 5;
const int _SEXTUPOLE  = 6;
const int _OCTUPOLE = 7;
const int _MULT  = 8;
const int _SOLENOID = 9;
const int _ELEMENT = 10;
const int _LINE = 11;
const int _COLLIMATOR = 12;
const int _ECOL = 13;
const int _RCOL = 14;
const int _LASER=15;
const int _MATERIAL=16;

const int _VKICK=31;
const int _HKICK=32;

const int _SAMPLER = 41;
const int _CSAMPLER = 42;
const int _GAS = 51;
const int _TUNNEL = 52;

const int _TRANSFORM3D = 61;

const char *typestr(int type);


// geometry and field formats

const int _GMAD = 0;
const int _MOKKA = 1;

const int _GAUSSIAN = 0;
const int _RING = 1;
const int _SQUARE = 2;
const int _CIRCLE = 3; 
const int _ESHELL = 31;
const int _UDEF = 32;

const int _GUINEAPIG_BUNCH = 4;
const int _GUINEAPIG_PAIRS = 5;
const int _GUINEAPIG_SLAC = 6;
const int _CAIN = 7;


// options passed with option and beam command

struct Options {

  string physicsList;


  // beam parameters
  string particleName;
  string distribType;
  string distribFile;

  int numberOfParticles;

  int nlinesIgnore; // ignore first lines in the input bunch file


  double sigmaX;
  double sigmaY;
  double sigmaT;
  double sigmaXp;
  double sigmaYp;
  double beamEnergy;
  double sigmaE;

  double x, xp, y, yp; // for the elliptic shell distribution

  double betx, bety, alfx, alfy, emitx, emity; // initial twiss parameters
  int doTwiss;

  // for element specification
  double X0;
  double Y0;
  double Rmin;
  double Rmax;

  double xsize, ysize;

  int numberToGenerate;
  int numberOfEventsPerNtuple;
  unsigned long int eventNumberOffset;
  int backgroundScaleFactor;

  // default geometry parameters

  double componentBoxSize;
  double tunnelRadius;
  double beampipeRadius;
  double beampipeThickness;

  string pipeMaterial;

  double thresholdCutCharged;
  double thresholdCutPhotons;
  
  double prodCutPhotons;
  double prodCutPhotonsP;
  double prodCutElectrons;
  double prodCutElectronsP;
  double prodCutPositrons;
  double prodCutPositronsP;


  double trackWeightFactor;
  double deltaChord;
  double chordStepMinimum;
  double deltaIntersection;
  double minimumEpsilonStep;
  double maximumEpsilonStep;
  double deltaOneStep;
  bool turnOnInteractions;
  int useLowEMPhysics;
  int synchRadOn;
  int synchRescale;
  int synchTrackPhotons;
  double synchLowX;
  double synchLowGamE;
  int synchPhotonMultiplicity;
  int synchMeanFreeFactor;
  int planckOn;
  int eBremOn;
  double lengthSafety;
  long int randomSeed;

  int verboseStep;
  int verboseEventNumber;

  int useTimer;
  int useEMHadronic;
  int useMuonPairProduction;
  double muonProductionScaleFactor;
  double hadronInelasticScaleFactor;
  int storeMuonTrajectories;
  int storeNeutronTrajectories;
  int storeTrajectory;
  int stopTracks;
  int useMuonShowers;
  double muonLowestGeneratedEnergy;


};

// type of beamline list entries

struct Element {
  short type;
  char * name;

  
  double l,k0,k1,k2,k3,angle,aper,tilt,xsize,ysize,r, outR;
  double xdir, ydir, zdir, waveLength; // for laser wire and 3d transforms

  double gradient; // for rf cavities

  double phi, theta, psi; // for 3d transforms

  list<double> knl;
  list<double> ksl;

   // material properties

  double A; 
  double Z; 
  double density; 
  double temper; 

  string geometryFile;
  string bmapFile;
  string material;

  string spec;  // arbitrary specification to pass to beamline builder
  
  // in case the element is a list itself (line)
  list <Element> *lst;

  
};


// parameters - used in the parser

struct Parameters {

  // length, multipole coefficients

  double l;  int lset;    // length
  double k0; int k0set;   // dipole 
  double k1; int k1set;   // quadrupole
  double k2; int k2set;   // sextupole
  double k3; int k3set;   // octupole

  list<double> knl;           // multipole expansion coefficients
  list<double> ksl;           // skew multipole expansion
  
  int knlset; int kslset;

  
  // placement, geometrical sizes etc.

  double r; int rset; //radius, i.e cylindrical sampler
  
  double angle; int angleset;   // bending angle
  double aper; int aperset;   // aperture (circular)
  double phi, theta, psi; // for 3d transforms
  int phiset, thetaset, psiset;

  double gradient; int gradientset;

  double outR; int outRset; // outer radius of magnets

  double xsize, ysize; int xsizeset, ysizeset; // aperture (or laser spotsize for laser)
  double xdir, ydir, zdir, waveLength; int xdirset, ydirset, zdirset, waveLengthset;

  double tilt; int tiltset;   // tilt

  // twiss parameters
  

  // for external geometry and field definition files
  char geometry[64]; int geomset;
  char bmap[64]; int bmapset;
  char emap[64];
  char material[64]; int materialset;


  // string to pass a custom type specification
  char spec[1024]; int specset;

  // material properties

  double A; int Aset;
  double Z; int Zset;
  double density; int densityset;
  double temper; int temperset;
  
  // reset the parameters to defaults
  void flush() {

    l=0; lset = 0;
    r = 0; rset = 0;

    A = 0; Aset = 0;
    Z = 0; Zset = 0;
    density = 0; densityset = 0;
    temper = 0; temperset = 0;

    angle = 0; angleset = 0;
    xsize = 0; xsizeset = 0;
    ysize = 0; ysizeset = 0;
    
    xdir = 0; xdirset = 0;
    ydir = 0; ydirset = 0;
    zdir = 0; zdirset = 0; 
    waveLength = 0; waveLengthset = 0; 

    phi = 0; phiset = 0;
    theta = 0; thetaset = 0;
    psi = 0; psiset = 0;

    aper = 0; aperset = 0;
    outR = 0; outRset = 0;
    tilt = 0; tiltset = 0;

    k0 = 0; k0set = 0;
    k1 = 0; k1set = 0;
    k2 = 0; k2set = 0;
    k3 = 0; k3set = 0;
    
    knlset = 0; kslset=0;


    knl.erase(knl.begin(),knl.end());
    ksl.erase(ksl.begin(),ksl.end());

    gradient = 0; gradientset = 0;
    
  
    strcpy(geometry,"");  geomset = 0;

    strcpy(bmap,""); bmapset = 0;


    strcpy(material,""); materialset = 0;
    strcpy(spec,""); specset = 0;
  }

  
};


extern list<Element> beamline_list;
extern Options options;

// parse the input file and construct beamline_list and options 
int gmad_parser(FILE *f);

int gmad_parser(string name);


#endif
