#include "BDSGlobalConstants.hh"
#include "BDSBunch.hh"
#include <iostream>
#include "globals.hh"

const int DEBUG = 0;

using namespace std;

extern G4bool verbose;      // run options
extern G4bool verboseStep;
extern G4bool verboseEvent;
extern G4int verboseEventNumber;
extern G4bool isBatch;

extern G4int nptwiss;

BDSBunch::BDSBunch()
{
  sigmaX = 0;
  sigmaY = 0;
  sigmaT = 0;

  sigmaXp = 0;
  sigmaYp = 0;
  
  X0 = 0;
  Y0 = 0;
  Xp0 = 0;
  Yp0 = 0;
  
  betaX = 0;
  betaY = 0;
  alphaX = 0;
  alphaY = 0;
  emitX = 0;
  emitY = 0;

  partId = 0;

  ignoreLines = 0;

  distribType = -1;
  
  GaussGen=new RandGauss(*HepRandom::getTheEngine());
  FlatGen=new RandFlat(*HepRandom::getTheEngine());
  
}

BDSBunch::~BDSBunch()
{
  delete GaussGen;
  delete FlatGen;
}

// set options from gmad

G4double val;

void BDSBunch::SetOptions(struct Options& opt)
{
  map<const G4String, int, strCmp> distType;
  distType["gauss"]=_GAUSSIAN;
  distType["ring"]=_RING;
  distType["square"]=_SQUARE;
  distType["circle"]=_CIRCLE;
  distType["guineapig_bunch"]=_GUINEAPIG_BUNCH;
  distType["guineapig_pairs"]=_GUINEAPIG_PAIRS;
  distType["guineapig_slac"]=_GUINEAPIG_SLAC;
  distType["cain"]=_CAIN;
  distType["eshell"]=_ESHELL;
  distType["udef"]=_UDEF;

#define _skip(nvalues) for(G4int i=0;i<nvalues;i++) InputBunchFile>>val;

  // twiss parameters - set always if present
  SetBetaX(opt.betx);
  SetBetaY(opt.bety);
  SetAlphaX(opt.alfx);
  SetAlphaY(opt.alfy);
  SetEmitX(opt.emitx);
  SetEmitY(opt.emity);

  ignoreLines = opt.nlinesIgnore;
//  distribType = _GAUSSIAN; // default

  map<const G4String,int>::iterator iter;
  iter = distType.find(opt.distribType);
  if(iter!=distType.end()) 
    distribType = (*iter).second;
  switch(distribType){
    //  if(opt.distribType == "gauss")
  case _GAUSSIAN:
    {
      distribType = _GAUSSIAN; 
      SetSigmaX(opt.sigmaX); 
      SetSigmaY(opt.sigmaY);
      SetSigmaXp(opt.sigmaXp);
      SetSigmaYp(opt.sigmaYp);
      SetSigmaT(opt.sigmaT);
      energySpread = opt.sigmaE;
      break;
      //return;
    } 
    //  else if(opt.distribType == "ring")
  case _RING:
    {
      distribType = _RING;
      X0 = opt.X0;
      Y0 = opt.Y0;
      rMin = opt.Rmin;
      rMax = opt.Rmax;
      energySpread = opt.sigmaE;
      //return;
      break;
    } 
    //else if(opt.distribType == "eshell")
    
  case _ESHELL:
    {
      distribType = _ESHELL;
      shellx = opt.x;
      shelly = opt.y;
      shellxp = opt.xp;
      shellyp = opt.yp;
      energySpread = opt.sigmaE;
      //return;
      break;
    } 
    //else if(opt.distribType == "guineapig_bunch")
  case _GUINEAPIG_BUNCH:
    {
      distribType = _GUINEAPIG_BUNCH;
      inputfile = opt.distribFile;
      InputBunchFile.open(inputfile);
      if(!InputBunchFile.good()) 
	{ G4cerr<<"Cannot open bunch file "<<inputfile<<G4endl; exit(1); }
      if(DEBUG) 
	G4cout<<"GUINEAPIG_BUNCH: skipping "<<opt.nlinesIgnore<<"  lines"<<G4endl;
      _skip(opt.nlinesIgnore * 6);
      //return;
      break;
    } 
    //else if(opt.distribType == "guineapig_slac")
  case _GUINEAPIG_SLAC:
    {
      distribType = _GUINEAPIG_SLAC;
      inputfile = opt.distribFile;
      InputBunchFile.open(inputfile);
      if(!InputBunchFile.good())  
	{ G4cerr<<"Cannot open bunch file "<<inputfile<<G4endl; exit(1); }
      _skip(opt.nlinesIgnore * 6);
      //return;
      break;
    } 
  case _GUINEAPIG_PAIRS:
    //else if(opt.distribType == "guineapig_pairs")
    {
      distribType = _GUINEAPIG_PAIRS;
      inputfile = opt.distribFile;
      InputBunchFile.open(inputfile);
      if(!InputBunchFile.good()) 
	{ G4cerr<<"Cannot open bunch file "<<inputfile<<G4endl; exit(1); }
      _skip(opt.nlinesIgnore * 7);
      //return;
      break;
    }
  case _CAIN:
    //else if(opt.distribType == "cain")
    {
      distribType = _CAIN;
      inputfile = opt.distribFile;
      InputBunchFile.open(inputfile);
      if(!InputBunchFile.good()) 
	{ G4cerr<<"Cannot open bunch file "<<inputfile<<G4endl; exit(1); }
      _skip(opt.nlinesIgnore * 14);
      //return;
      break;
    } 
    //else //assuming the format is "field[unit]:field[unit]:..." - User Defined
  default:
    {
      G4cout<<"distrType -> "<<opt.distribType<<G4endl;
      distribType = _UDEF; 
      
      // construct the list of read attributes
      
      G4String unparsed_str = opt.distribType; 
      G4int pos = unparsed_str.find(":");
      
      struct Doublet sd;
      
      while(pos > 0)
	{
	  pos = unparsed_str.find(":");
	  G4String token = unparsed_str.substr(0,pos);
	  unparsed_str = unparsed_str.substr(pos+1);
	  //G4cout<<"token ->"<<token<<G4endl;
	  //G4cout<<"unparsed_str ->"<<unparsed_str<<G4endl;
	  //G4cout<<"pos ->"<<pos<<G4endl;
	  
	  // see if the token has a meeting
	  if( token.length() > 2) {
	    if(token.substr(0,1)=="E") {
	      //G4cout<<"E!"<<G4endl;
	      G4String rest = token.substr(1);
	      //G4cout<<"rest ->"<<rest<<G4endl;
	      G4int pos1 = rest.find("[");
	      G4int pos2 = rest.find("]");
	      if(pos1 < 0 || pos2 < 0) {
		G4cerr<<"unit format wrong!!!"<<G4endl;
	      } else {
		G4String fmt = rest.substr(pos1+1,pos2-1);
		//G4cout<<"fmt ->"<<fmt<<G4endl;
		sd.name = "E"; 
		if(fmt=="GeV") sd.unit=1;
		if(fmt=="MeV") sd.unit=1.e-3;
		if(fmt=="KeV") sd.unit=1.e-6;
		if(fmt=="eV") sd.unit=1.e-9;
		
		fields.push_back(sd);
	      }
	    }
	    if( (token.substr(0,1)=="x") && (token.substr(1,1)!="p") ) {
	      //G4cout<<"x!"<<G4endl;
	      //G4cout<<token.substr(0,1)<<G4endl;
	      //G4cout<<token.substr(1,2)<<G4endl;
	      G4String rest = token.substr(1);
	      //G4cout<<"rest ->"<<rest<<G4endl;
	      G4int pos1 = rest.find("[");
	      G4int pos2 = rest.find("]");
	      if(pos1 < 0 || pos2 < 0) {
		G4cerr<<"unit format wrong!!!"<<G4endl;
	      } else {
		G4String fmt = rest.substr(pos1+1,pos2-1);
		//G4cout<<"fmt ->"<<fmt<<G4endl;
		sd.name="x";
		
		if(fmt=="m") sd.unit=1;
		if(fmt=="cm") sd.unit=1.e-2;
		if(fmt=="mm") sd.unit=1.e-3;
		if(fmt=="mum") sd.unit=1.e-6;
		if(fmt=="nm") sd.unit=1.e-9;
		
		fields.push_back(sd);
		
	      }
	    }
	    if(token.substr(0,1)=="y" && token.substr(1,1)!="p" ) {
	      //G4cout<<"y!"<<G4endl;
	      G4String rest = token.substr(1);
	      //G4cout<<"rest ->"<<rest<<G4endl;
	      G4int pos1 = rest.find("[");
	      G4int pos2 = rest.find("]");
	      if(pos1 < 0 || pos2 < 0) {
		G4cerr<<"unit format wrong!!!"<<G4endl;
	      } else {
		G4String fmt = rest.substr(pos1+1,pos2-1);
		//G4cout<<"fmt ->"<<fmt<<G4endl;
		sd.name="y";
		
		if(fmt=="m") sd.unit=1;
		if(fmt=="cm") sd.unit=1.e-2;
		if(fmt=="mm") sd.unit=1.e-3;
		if(fmt=="mum") sd.unit=1.e-6;
		if(fmt=="nm") sd.unit=1.e-9;
		
		fields.push_back(sd);
	      }
	    }
	    if(token.substr(0,1)=="z" && token.substr(1,1)!="p" ) {
	      //G4cout<<"z!"<<G4endl;
	      G4String rest = token.substr(1);
	      //G4cout<<"rest ->"<<rest<<G4endl;
	      G4int pos1 = rest.find("[");
	      G4int pos2 = rest.find("]");
	      if(pos1 < 0 || pos2 < 0) {
		G4cerr<<"unit format wrong!!!"<<G4endl;
	      } else {
		G4String fmt = rest.substr(pos1+1,pos2-1);
		//G4cout<<"fmt ->"<<fmt<<G4endl;
		sd.name="z";

		if(fmt=="m") sd.unit=1;
		if(fmt=="cm") sd.unit=1.e-2;
		if(fmt=="mm") sd.unit=1.e-3;
		if(fmt=="mum") sd.unit=1.e-6;
		if(fmt=="nm") sd.unit=1.e-9;
		
		fields.push_back(sd);
	      }
	    }
	    if(token.substr(0,2)=="xp") {
	      //G4cout<<"xp!"<<G4endl;
	      G4String rest = token.substr(1);
	      //G4cout<<"rest ->"<<rest<<G4endl;
	      G4int pos1 = rest.find("[");
	      G4int pos2 = rest.find("]");
	      if(pos1 < 0 || pos2 < 0) {
		G4cerr<<"unit format wrong!!!"<<G4endl;
	      } else {
		G4String fmt = rest.substr(pos1+1,pos2-1);
		//G4cout<<"fmt ->"<<fmt<<G4endl;
		sd.name="xp";
		
		if(fmt=="rad") sd.unit=1;
		if(fmt=="mrad") sd.unit=1.e-3;
		
		
		fields.push_back(sd);
		
	      }
	    }
	    if(token.substr(0,2)=="yp") {
	      //G4cout<<"yp!"<<G4endl;
	      G4String rest = token.substr(1);
	      //G4cout<<"rest ->"<<rest<<G4endl;
	      G4int pos1 = rest.find("[");
	      G4int pos2 = rest.find("]");
	      if(pos1 < 0 || pos2 < 0) {
		G4cerr<<"unit format wrong!!!"<<G4endl;
	      } else {
		G4String fmt = rest.substr(pos1+1,pos2-1);
		//G4cout<<"fmt ->"<<fmt<<G4endl;
		sd.name="yp";
		
		if(fmt=="rad") sd.unit=1;
		if(fmt=="mrad") sd.unit=1.e-3;
		
		
		fields.push_back(sd);
	      }
	    }
	    if(token.substr(0,2)=="zp") {
	      //G4cout<<"E!"<<G4endl;
	      G4String rest = token.substr(1);
	      //G4cout<<"rest ->"<<rest<<G4endl;
	      G4int pos1 = rest.find("[");
	      G4int pos2 = rest.find("]");
	      if(pos1 < 0 || pos2 < 0) {
		G4cerr<<"unit format wrong!!!"<<G4endl;
	      } else {
		G4String fmt = rest.substr(pos1+1,pos2-1);
		//G4cout<<"fmt ->"<<fmt<<G4endl;
		sd.name="zp";
		
		if(fmt=="rad") sd.unit=1;
		if(fmt=="mrad") sd.unit=1.e-3;
		
		
		fields.push_back(sd);
	      }
	    }
	    if(token.substr(0,2)=="pt") {
	      //G4cout<<"pt!"<<G4endl;
	      G4String rest = token.substr(1);
	      //G4cout<<"rest ->"<<rest<<G4endl;
	      G4int pos1 = rest.find("[");
	      G4int pos2 = rest.find("]");
	      if(pos1 < 0 || pos2 < 0) {
		G4cerr<<"unit format wrong!!!"<<G4endl;
	      } else {
		G4String fmt = rest.substr(pos1+1,pos2-1);
		//G4cout<<"fmt ->"<<fmt<<G4endl;
		sd.name="pt";
		
		fields.push_back(sd);
	      }
	    }
	    
	    
	  }
	  else {
	    G4cerr << "Cannot determine bunch data format" << G4endl; exit(1);
	  }
	  
	}

      inputfile = opt.distribFile;
      InputBunchFile.open(inputfile);
      if(!InputBunchFile.good()) {
	G4cerr<<"Cannot open bunch file "<<inputfile<<G4endl; 
	G4cerr<<"Reverting to gaussian bunch type" << G4endl;
	distribType = _GAUSSIAN;
      }
    }
  }
  return;  
}

// get initial bunch distribution parameters in Gaussian case 
G4double BDSBunch::GetSigmaT() 
{
  return sigmaT;
} 

G4double BDSBunch::GetSigmaX()
{

  return sigmaX;
}
 
G4double BDSBunch::GetSigmaY()
{
  return sigmaY;
} 

G4double BDSBunch::GetSigmaXp()
{
  return sigmaXp;
}

G4double BDSBunch::GetSigmaYp()
{
  return sigmaYp;
}

// generate primary coordinates

G4double BDSBunch::GetNextX()
{
  return sigmaX * GaussGen->shoot() * m;
} 

G4double BDSBunch::GetNextY()
{
  return sigmaY * GaussGen->shoot() * m;
}

G4double BDSBunch::GetNextZ()
{
  return 0;
}



G4double BDSBunch::GetNextXp()
{
  return sigmaXp * GaussGen->shoot();
}

G4double BDSBunch::GetNextYp()
{
  return sigmaYp * GaussGen->shoot();
}

G4double BDSBunch::GetNextT()
{
  return - sigmaT* (1.-2.*GaussGen->shoot());
}


void BDSBunch::GetNextParticle(G4double& x0,G4double& y0,G4double& z0,
			       G4double& xp,G4double& yp,G4double& zp,
			       G4double& t, G4double& E)
{

  //G4cout<<"Twiss: "<<betaX<<" "<<betaY<<" "<<alphaX<<" "<<alphaY<<" "<<emitX<<" "<<emitY<<G4endl;

  if(verboseStep) G4cout<<"distribution type: "<<distribType<<G4endl;

  double r, phi;
  // Rescal must be at the top of GetNextParticle
  if(BDSGlobals->DoTwiss() && partId<nptwiss)
    {
      // temp numbers - to be replaced by parsed parameters

      
      G4double sigx = sqrt(betaX*emitX);
      G4double sigxp= sqrt(emitX / betaX);
      
      G4double sigy = sqrt(betaY*emitY);
      G4double sigyp= sqrt(emitY / betaY);

      G4double pi = 2.*asin(1.);

      partId++;
      E = BDSGlobals->GetBeamKineticEnergy();
      zp = 1;
      t=0;
      z0=0;
      G4double phase_factor = 1 / ( (nptwiss/2.) - 1.0 );
      if(partId<=nptwiss/2) //primary - xx' ellipse
	{
	  x0 = sigx * cos(partId* 2 * pi*phase_factor);
	  xp = -sigxp * ( alphaX * cos(partId * 2 * pi*phase_factor )
			  + sin(partId * 2 * pi*phase_factor ) );
	  y0 = 0;
	  yp = 0;
	}
      else if(partId<=nptwiss) //primary - yy' ellipse
	{
	  x0 = 0;
	  xp = 0;
	  y0 = sigy * cos( (partId-nptwiss/2)*2*pi*phase_factor);
	  yp = -sigyp * ( alphaY * cos( (partId-nptwiss/2) * 2 * pi*phase_factor)
			  + sin( (partId-nptwiss/2) * 2 * pi*phase_factor) );
	}
      //tmp - check units of above equations!!
      x0*=m;
      y0*=m;
      xp*=radian;
      yp*=radian;
      return;
    }

  switch(distribType){
  case _GAUSSIAN:
    {
      x0 = sigmaX * GaussGen->shoot() * m;
      y0 = sigmaY * GaussGen->shoot() * m;
      z0 = 0;
      xp = sigmaXp * GaussGen->shoot();
      yp = sigmaYp * GaussGen->shoot();
      zp=sqrt(1.-xp*xp -yp*yp);  
      t = - sigmaT* (1.-2.*GaussGen->shoot());
      E = BDSGlobals->GetBeamKineticEnergy() * (1 + energySpread * GaussGen->shoot());
      break;
    }
  case _RING:
    {
      if(DEBUG) G4cout<<"RING: rMin="<<rMin<<" rMax="<<rMax<<G4endl;
      
      r = ( rMin + (rMax - rMin) *  rand() / RAND_MAX ) * m;
      phi = 2 * pi * rand() / RAND_MAX;
      
      x0 = r * sin(phi);
      y0 = r * cos(phi);
      z0 = 0;
      xp = 0;
      yp = 0;
      zp=sqrt(1.-xp*xp -yp*yp);  
      t = 0;
      E = BDSGlobals->GetBeamKineticEnergy()
	* (1 + energySpread/2. * (1. -2. * FlatGen->shoot()));
      break;
    }
  case _ESHELL:
    {// generate elliptical shell - first generate on S1 and then transform into ellipse
      
      if(DEBUG) G4cout<<"SHELL: x="<<shellx<<" xp="<<shellxp<<G4endl;
      
      phi = 2 * pi * rand() / RAND_MAX;
      
      x0 = sin(phi) * shellx;
      xp = cos(phi) * shellxp;
      
      phi = 2 * pi * rand() / RAND_MAX;
      
      y0 = sin(phi) * shelly;
      yp = cos(phi) * shellyp;
      
      z0 = 0;
      
      zp=sqrt(1.-xp*xp -yp*yp);  
      t = 0;
      E = BDSGlobals->GetBeamKineticEnergy()
	* (1 + energySpread/2. * (1. -2. * FlatGen->shoot()));
      break;
    }
  case _GUINEAPIG_BUNCH:
    {
      #define  _READ(value) InputBunchFile>>value
      if(_READ(E))
	{
	  _READ(x0);
	  _READ(y0);
	  _READ(z0);
	  _READ(xp);
	  _READ(yp);
	  
	  E*=GeV;
	  x0*= micrometer;
	  y0*= micrometer;
	  z0*= micrometer;
	  xp*=1.e-6*radian;
	  yp*=1.e-6*radian;
	  zp=sqrt(1.-xp*xp -yp*yp);  
	  t=-z0/c_light;
	  // use the Kinetic energy:
	  E-=BDSGlobals->GetParticleDefinition()->GetPDGMass();
	}
      break;
    }
  case _GUINEAPIG_SLAC:
    {
      #define  _READ(value) InputBunchFile>>value
      if(_READ(E))
	{
	  _READ(xp);
	  _READ(yp);
	  _READ(z0);
	  _READ(x0);
	  _READ(y0);
	  
	  E*=GeV;
	  x0*= nanometer;
	  y0*= nanometer;
	  z0*= micrometer;
	  xp*=radian;
	  yp*=radian;
	  zp=sqrt(1.-xp*xp -yp*yp);  
	  t=-z0/c_light;
	  // use the Kinetic energy:
	  E-=BDSGlobals->GetParticleDefinition()->GetPDGMass();
	}
      else{
	InputBunchFile.clear();
	InputBunchFile.seekg(0);
	_skip(ignoreLines * 6);
	GetNextParticle(x0,y0,z0,xp,yp,zp,t,E);
	
	/*   _READ(E);
	     _READ(xp);
	     _READ(yp);
	     _READ(z0);
	     _READ(x0);
	     _READ(y0);
	     
	     E*=GeV;
	     x0*= nanometer;
	     y0*= nanometer;
	     z0*= micrometer;
	     xp*=radian;
	     yp*=radian;
	     zp=sqrt(1.-xp*xp -yp*yp);
	     t=-z0/c_light;
	     // use the Kinetic energy:
	     E-=BDSGlobals->GetParticleDefinition()->GetPDGMass();
	*/
      }
      break;
    }
  case _GUINEAPIG_PAIRS:
    {
      #define  _READ(value) InputBunchFile>>value
      if(_READ(E))
	{
	  _READ(xp);
	  _READ(yp);
	  _READ(zp);
	  _READ(x0);
	  _READ(y0);
	  _READ(z0);
	  if(E>0) BDSGlobals->SetParticleDefinition(G4ParticleTable::
						    GetParticleTable()
						    ->FindParticle("e-"));
	  if(E<0) BDSGlobals->SetParticleDefinition(G4ParticleTable::
						    GetParticleTable()
						    ->FindParticle("e+"));
	  E=fabs(E)*GeV;
	  x0*= nanometer;
	  y0*= nanometer;
	  z0*= nanometer;
	  xp*=radian;
	  yp*=radian;
	  zp*=radian;
	  // Using the sign of the pair file zp
	  // but calculating zp more accurately
	  if(zp<0) zp = -sqrt(1-(xp*xp+yp*yp));
	  else zp = sqrt(1-(xp*xp+yp*yp));
	  t=-z0/c_light;
	  // use the Kinetic energy:
	  E-=BDSGlobals->GetParticleDefinition()->GetPDGMass();
	}
      break;
    }
  case _CAIN:
    {// Note that for the CAIN input the following variables are read in but NOT used by BDSIM:
      //     generation, weight, spin_x, spin_y, spin_z
      
      G4int type;
      G4int gen;
      G4int pos;
      G4double weight;
      G4double part_mass;
      G4double px,py,pz;
      G4double sx;
      G4double sy;
      G4double sz;
      G4String dbl_var;
      G4String rep = 'E';      
      #define  _READ(value) InputBunchFile>>value
      
      if(_READ(type))
	{
	  
	  _READ(gen); // Read in but not currently used
	  
	  _READ(dbl_var);
	  if(dbl_var.contains('D'))
	    {
	      pos = dbl_var.first('D');
	      weight = atof( dbl_var.replace(pos,1,rep.data(),1) );
	    }
	  else weight = atof(dbl_var);
	  // weight read in but not currently used
	  
	  _READ(dbl_var);
	  if(dbl_var.contains('D'))
	    {
	      pos = dbl_var.first('D');
	      t = atof( dbl_var.replace(pos,1,rep.data(),1) );
	    }
	  else t = atof(dbl_var);
	  
	  _READ(dbl_var);
	  if(dbl_var.contains('D'))
	    {
	      pos = dbl_var.first('D');
	      x0 = atof( dbl_var.replace(pos,1,rep.data(),1) );
	    }
	  else x0 = atof(dbl_var);
	  
	  _READ(dbl_var);
	  if(dbl_var.contains('D'))
	    {
	      pos = dbl_var.first('D');
	      y0 = atof( dbl_var.replace(pos,1,rep.data(),1) );
	    }
	  else y0 = atof(dbl_var);
	  
	  _READ(dbl_var);
	  if(dbl_var.contains('D'))
	    {
	      pos = dbl_var.first('D');
	      z0 = atof( dbl_var.replace(pos,1,rep.data(),1) );
	    }
	  else z0 = atof(dbl_var);
	  
	  _READ(dbl_var);
	  if(dbl_var.contains('D'))
	    {
	      pos = dbl_var.first('D');
	      E = atof( dbl_var.replace(pos,1,rep.data(),1) );
	    }
	  else E = atof(dbl_var);
	  
	  _READ(dbl_var);
	  if(dbl_var.contains('D'))
	    {
	      pos = dbl_var.first('D');
	      px = atof( dbl_var.replace(pos,1,rep.data(),1) );
	    }
	  else px = atof(dbl_var);
	  
	  _READ(dbl_var);
	  if(dbl_var.contains('D'))
	    {
	      pos = dbl_var.first('D');
	      py = atof( dbl_var.replace(pos,1,rep.data(),1) );
	    }
	  else py = atof(dbl_var);
	  
	  _READ(dbl_var);
	  if(dbl_var.contains('D'))
	    {
	      pos = dbl_var.first('D');
	      pz = atof( dbl_var.replace(pos,1,rep.data(),1) );
	    }
	  else pz = atof(dbl_var);
	  
	  // Spin Components read in - but not currently used
	  _READ(sx);
	  _READ(sy);
	  _READ(sz);
	  
	  if(type==1) 
	    BDSGlobals->SetParticleDefinition(G4ParticleTable::
					      GetParticleTable()
					      ->FindParticle("gamma"));
	  else if(type==2) 
	    BDSGlobals->SetParticleDefinition(G4ParticleTable::
					      GetParticleTable()
					      ->FindParticle("e-"));
	  
	  else if(type==3) 
	    BDSGlobals->SetParticleDefinition(G4ParticleTable::
					      GetParticleTable()
					      ->FindParticle("e+"));
	  
	  t*= m/c_light;
	  x0*= m;
	  y0*= m;
	  z0*= m;
	  E*=eV;
	  px*=eV/c_light;
	  py*=eV/c_light;
	  pz*=eV/c_light;
	  
	  part_mass = BDSGlobals->GetParticleDefinition()->GetPDGMass();
	  // use the Kinetic energy:
	  E-=part_mass;
	  
	  // calculate the momentum direction needed for BDSPrimaryGenerator
	  xp = px*c_light / sqrt(E*E + 2*E*part_mass);
	  yp = py*c_light / sqrt(E*E + 2*E*part_mass);
	  zp = pz*c_light / sqrt(E*E + 2*E*part_mass);
	  /*	  
		  G4cout << "Bunch input was: " << G4endl;
		  G4cout << type << "\t"
		  << gen << "\t"
		  << weight << "\t"
		  << t/m << "\t"
		  << x0/m << "\t"
		  << y0/m << "\t"
		  << z0/m << "\t"
		  << E/eV << "\t"
		  << px/(eV/c_light) << "\t"
		  << py/(eV/c_light) << "\t"
		  << pz/(eV/c_light) << "\t"
		  << sx << "\t"
		  << sy << "\t"
		  << sz << "\t"
		  << xp << "\t"
		  << yp << "\t"
		  << zp << "\t"
		  << G4endl << G4endl;
	  */	  
	}
      break;
    }
  case _UDEF:
    {//G4cout<<"distrType=UDEF"<<G4endl;
      
      E = x0 = y0 = z0 = xp = yp = zp = 0;
      
      #define  _READ(value) InputBunchFile>>value
      
      G4int type;
      
      list<struct Doublet>::iterator it;
      
      for(it=fields.begin();it!=fields.end();it++)
	{
	  //G4cout<<it->name<<"  ->  "<<it->unit<<G4endl;
	  if(it->name=="E") { _READ(E); E *= ( GeV * it->unit ); }
	  if(it->name=="x") { _READ(x0); x0 *= ( m * it->unit ); }
	  if(it->name=="y") { _READ(y0); y0 *= ( m * it->unit ); }
	  if(it->name=="z") { _READ(z0); z0 *= ( m * it->unit ); }
	  if(it->name=="xp") { _READ(xp); xp *= ( radian * it->unit ); }
	  if(it->name=="yp") { _READ(yp); yp *= ( radian * it->unit ); }
	  if(it->name=="zp") { _READ(zp); zp *= ( radian * it->unit ); }
	  if(it->name=="pt") {
	    _READ(type);
	    if(InputBunchFile.good())
	      BDSGlobals->SetParticleDefinition(G4ParticleTable::
						GetParticleTable()
						->FindParticle(type));
	  }
	  
	  
	  zp=sqrt(1.-xp*xp -yp*yp);  
	  t=-z0/c_light;
	  // use the Kinetic energy:
	  E-=BDSGlobals->GetParticleDefinition()->GetPDGMass();
	  
	}
      break;
    }
  default:
    {
      G4Exception("BDSBunch: Unknown distribution file type!");
    }
  }
}


G4double BDSBunch::GetEmitX()
{
  return emitX;
}

G4double BDSBunch::GetEmitY()
{
  return emitY;
}

G4double BDSBunch::GetAlphaX()
{
  return alphaX;
}

G4double BDSBunch::GetAlphaY()
{
  return alphaY;
}

G4double BDSBunch::GetBetaX()
{
  return betaX;
}

G4double BDSBunch::GetBetaY()
{
  return betaY;
}



// set initial bunch distribution parameters in Gaussian case 
void BDSBunch::SetSigmaT(double val) 
{
  sigmaT= val;
} 

void BDSBunch::SetSigmaX(double val)
{ 
  sigmaX = val;
}
 
void BDSBunch::SetSigmaY(double val)
{
  sigmaY = val;
} 

void BDSBunch::SetSigmaXp(double val)
{
  sigmaXp = val;
}

void BDSBunch::SetSigmaYp(double val)
{
  sigmaYp = val;
}

void BDSBunch::SetX0(double val)
{
  X0 = val;
} 

void BDSBunch::SetY0(double val)
{
  Y0 = val;
}

void BDSBunch::SetXp0(double val)
{
  Xp0 = val;
}

void BDSBunch::SetYp0(double val)
{
  Yp0 = val;
}

void BDSBunch::SetEmitX(double val)
{
  emitX = val;
}

void BDSBunch::SetEmitY(double val)
{
  emitY = val;
}

void BDSBunch::SetAlphaX(double val)
{
  alphaX = val;
}

void BDSBunch::SetAlphaY(double val)
{
  alphaY = val;
}

void BDSBunch::SetBetaX(double val)
{
  betaX = val;
}

void BDSBunch::SetBetaY(double val)
{
  betaY = val;
}
