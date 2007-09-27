/*
 *  parser.h
 *
 *    GMAD parser functions
 *    Ilya Agapov 2005-2006
 *    bdsim v.0.3
 */

#ifndef __PARSER_H
#define __PARSER_H

#include "sym_table.h"
#ifndef _WIN32
#include <unistd.h>
#endif
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <list>
#include <vector>

#include <iostream>

#include "gmad.h"

//using namespace std;

//double pow(double x, double y) {return exp( y * log(x));}

int yyerror(char *);

extern FILE* yyin;
extern int yylex();


const int MAX_EXPAND_ITERATIONS = 50;
const int MAX_MULTIPOLE_ORDER = 5;

const int _undefined = 0;

std::list<double> _tmparray;  // for reading of arrays
std::list<char*> _tmpstring;

// representation of arrays used in tokens
struct Array {
  std::vector<char*> symbols;
  double *data;
  int size;
};

const char *typestr(int type) {
  switch(type){
  case _MARKER : 
    return "marker";
  case _DRIFT :
    return "drift";
  case _RF :
   return "rf";
  case _SBEND : 
    return "sbend";
  case _RBEND :
    return "rbend";
  case _QUAD :
    return "quadrupole";
  case _SEXTUPOLE:
    return "sextupole";
  case _OCTUPOLE :
    return "octupole";
  case _MULT :
    return "multipole";
  case _SOLENOID : 
    return "solenoid";
  case _ECOL : 
    return "ecol";
  case _VKICK :
    return "vkick";
  case _HKICK :
    return "hkick";
  case _RCOL : 
    return "rcol";
  case _LINE :
    return "line";
  case _REV_LINE :
    return "rev_line";
  case _SAMPLER :
    return "sampler";
  case _CSAMPLER:
    return "csampler";
  case _GAS:
    return "gas";
  case _TUNNEL:
    return "tunnel";
  case _MATERIAL:
    return "material";
  case _ATOM:
    return "atom";
  case _LASER:
    return "laser";
  case _ELEMENT :
    return "element";
  case _TRANSFORM3D :
    return "transform3d";
  default:
    return "none";
  }
};
struct Parameters params;
struct Options options;
struct Element element;

void print(struct Parameters params)
{
  printf("printing parameters:\n");
  std::list<double>::iterator it;
  for(it = params.knl.begin();it!=params.knl.end();it++)
    printf(" %f ", (*it));
  printf("\n");
};

void flush(struct Element& e )
{
  e.l = 0;
  e.B = 0;
  e.k0 = 0;
  e.k1 = 0;
  e.k2 = 0;
  e.k3 = 0;
  e.angle = 0;
  e.tilt = 0;
  e.phi = 0;
  e.psi = 0;
  e.theta = 0;

  e.gradient = 0;

  e.hgap = 0;
  e.aper = 0;
  e.outR = 0;
  e.waveLength = 0;

  e.xdir = 0;
  e.ydir = 0;
  e.zdir = 0;

  e.name = NULL;
  e.type = _NONE;

  e.A = 0;
  e.Z = 0;
  e.density = 0;
  e.temper = 300;

  /*  
      e.knl = std::list<double>(0);
      e.ksl = std::list<double>(0);
      
      geometryFile
      bmapFile
      material;
  */

  //e.material = "";
  e.spec = "";
};

void copy_properties(std::list<struct Element>::iterator dest, std::list<struct Element>::iterator src)
{

  if(DEBUG) printf("%s %s \n",typestr((*dest).type),typestr((*src).type));

  (*dest).type = (*src).type;  
 
  (*dest).l = (*src).l;

  (*dest).angle = (*src).angle; 
  (*dest).xsize = (*src).xsize; 
  (*dest).ysize = (*src).ysize; 

  (*dest).xdir = (*src).xdir; 
  (*dest).ydir = (*src).ydir; 
  (*dest).zdir = (*src).zdir; 
  (*dest).phi = (*src).phi; 
  (*dest).theta = (*src).theta; 
  (*dest).psi = (*src).psi; 
  (*dest).waveLength = (*src).waveLength; 
 
  (*dest).aper = (*src).aper; 
  (*dest).outR = (*src).outR; 
  (*dest).tilt = (*src).tilt; 
  (*dest).B = (*src).B; 
  (*dest).k0 = (*src).k0; 
  (*dest).k1 = (*src).k1;  
  (*dest).k2 = (*src).k2; 
  (*dest).k3 = (*src).k3; 
  (*dest).knl = (*src).knl;
  (*dest).ksl = (*src).ksl;
  (*dest).hgap = (*src).hgap;

  (*dest).gradient = (*src).gradient; 

  (*dest).A = (*src).A;
  (*dest).Z = (*src).Z;
  (*dest).density = (*src).density;
  (*dest).temper = (*src).temper; 
  (*dest).components = (*src).components;
  (*dest).componentsWeights = (*src).componentsWeights;
  (*dest).componentsFractions = (*src).componentsFractions;
  (*dest).symbol = (*src).symbol;

  (*dest).geometryFile = (*src).geometryFile;

  (*dest).bmapFile = (*src).bmapFile;

  (*dest).material = (*src).material;

  (*dest).spec = (*src).spec;

  return;
}; 

void inherit_properties(struct Element e)
{
  // copy parameters into temporary buffer params from element e
  // parameters already set in params have priority and are not overridden
  
  if(!params.lset) { params.l = e.l; params.lset = 1; }
  if(!params.Bset) { params.B = e.B; params.Bset = 1; }
  if(!params.k0set) { params.k0 = e.k0; params.k0set = 1; }
  if(!params.k1set) { params.k1 = e.k1; params.k1set = 1; }
  if(!params.k2set) { params.k2 = e.k2; params.k2set = 1; }
  if(!params.k3set) { params.k3 = e.k3; params.k3set = 1; }
  if(!params.angleset) { params.angle = e.angle; params.angleset = 1; }
  if(!params.xsizeset) { params.xsize = e.xsize; params.xsizeset = 1; }
  if(!params.ysizeset) { params.ysize = e.ysize; params.ysizeset = 1; }
 
  if(!params.xdirset) { params.xdir = e.xdir; params.xdirset = 1; }
  if(!params.ydirset) { params.ydir = e.ydir; params.ydirset = 1; }
  if(!params.zdirset) { params.zdir = e.zdir; params.zdirset = 1; }
  if(!params.waveLength) { params.waveLength = e.waveLength; params.waveLengthset = 1; }
  if(!params.phiset) { params.phi = e.phi; params.phiset = 1; }
  if(!params.psiset) { params.psi = e.psi; params.psiset = 1; }
  if(!params.thetaset) { params.theta = e.theta; params.thetaset = 1; }
  if(!params.hgapset) { params.hgap = e.hgap; params.hgapset = 1; }

  if(!params.Aset) { params.A = e.A; params.Aset = 1; }
  if(!params.Zset) { params.Z = e.Z; params.Zset = 1; }
  if(!params.densityset) { params.density = e.density; params.densityset = 1; }
  if(!params.componentsset) 
    { params.components = e.components; params.componentsset = 1; }
  if(!params.componentsWeightsset) 
    { params.componentsWeights = e.componentsWeights; params.componentsWeightsset = 1; }
  if(!params.componentsFractionsset) 
    { params.componentsFractions = e.componentsFractions; params.componentsFractionsset = 1; }
  if(!params.symbolset) { strncpy(params.symbol, e.symbol.c_str(),64); params.symbolset = 1; }

  if(!params.aperset) { params.aper = e.aper; params.aperset = 1; }
  if(!params.outRset) { params.outR = e.outR; params.outRset = 1; }

  if(!params.gradientset) { params.gradient = e.gradient; params.gradientset = 1; }

  if(!params.tiltset) { params.tilt = e.tilt; params.tiltset = 1; }
  if(!params.knlset) { params.knl = e.knl; params.knlset = 1; }
  if(!params.kslset) { params.ksl = e.ksl; params.kslset = 1; }

  if(!params.specset) { strncpy(params.spec,e.spec.c_str(),1024); params.specset = 1; }
  if(!params.materialset) { strncpy(params.material,e.spec.c_str(),64); params.materialset = 1; }



};

void set_vector(std::list<double>& dst, struct Array *src)
{
  for(int i=0; i< src->size;i++){
    dst.push_back(src->data[i]);
    if(DEBUG) std::cout << src->data[i] << " ";
  }
  if(DEBUG) std::cout << std::endl;
  
};


void set_vector(std::list<char*>& dst, struct Array *src)
{
  for(int i=0; i< src->size;i++){
    dst.push_back(src->symbols[i]);
    if(DEBUG) std::cout << src->symbols[i] << " ";
  }
  if(DEBUG) std::cout << std::endl;
};

void set_vector(std::list<int>& dst, struct Array *src)
{
  for(int i=0; i< src->size;i++){
    dst.push_back((int)(src->data[i]));
    if(DEBUG) std::cout << (int)(src->data[i]) << " ";
  }
  if(DEBUG) std::cout << std::endl;
};


// list of all encountered elements
std::list<struct Element> element_list;

// temporary list
std::list<struct Element> tmp_list;

std::list<struct Element> beamline_list;
std::list<struct Element> material_list;
std::list<struct Element> atom_list;

char* current_line = "";
char* current_start = "";
char* current_end = "";

struct symtab *symtab; 

extern struct symtab * symlook(char *s);

std::list<struct Element>::iterator element_lookup(char *name);
std::list<struct Element>::iterator element_lookup(char *name, std::list<struct Element>& el);
int write_table(struct Parameters params,char* name, int type, std::list<struct Element> *lst=NULL);
int expand_line(char *name, char *start, char *end);
void print(std::list<struct Element> l, int ident=0);

// *********************
// functions
// *********************


void help()
{
  printf("helping...\n");
}

void quit()
{
  printf("parsing complete...\n");
  exit(0);
}

int write_table(struct Parameters params,char* name, int type, std::list<struct Element> *lst)
{
  if(DEBUG) printf("k1=%.10g ,k2=%.10g, type=%d, lset = %d\n",params.k1, params.k2, type,params.lset);
  
  struct Element e;
  flush(e);
  // common parameters for all elements
  e.name = name;
  e.lst = NULL;
  e.aper = params.aper;
  e.outR = params.outR;
  e.xsize = params.xsize;
  e.ysize = params.ysize;
  e.material = params.material;  
  
  //specific parameters
  switch(type) {
  case _MARKER :
    e.type= _MARKER;
    break;
  case _DRIFT:
    e.type = _DRIFT;
    e.l = params.l;
    break;

  case _RF:
    e.type = _RF;
    e.l = params.l;
    e.gradient = params.gradient;
    break;

  case _SBEND:
    e.type = _SBEND;
    e.l = params.l;
    e.B = params.B;
    e.angle = params.angle;
    e.hgap = params.hgap;
    e.k1 = params.k1;
    if(params.tiltset) e.tilt = params.tilt;
    break;

  case _RBEND:
    e.type = _RBEND;
    e.l = params.l;
    e.B = params.B;
    e.angle = params.angle;
    e.hgap = params.hgap;
    e.k1 = params.k1;
    if(params.tiltset) e.tilt = params.tilt;
    break;

  case _VKICK:
    e.type = _VKICK;
    e.l = params.l;
    e.B = params.B;
    e.angle = params.angle;
    if(params.tiltset) e.tilt = params.tilt;
    break;

  case _HKICK:
    e.type = _HKICK;
    e.l = params.l;
    e.B = params.B;
    e.angle = params.angle;
    if(params.tiltset) e.tilt = params.tilt;
    break;

  case _QUAD:

    e.type = _QUAD;
      
    if(params.lset) {
      e.l = params.l;
    }
    if(params.k0set) {
      if(VERBOSE)
	printf("Warning: k0 will not be set for element %s of type QUADRUPOLE\n",name);
    }
    if(params.k1set) {
      e.k1 = params.k1;
    }
    if(params.k2set) {
      if(VERBOSE)
	printf("Warning: k2 will not be set for element %s of type QUADRUPOLE\n",name);
      
    }
    if(params.tiltset) {
      e.tilt = params.tilt;
    }

    e.spec = std::string(params.spec); 
    
    break;

  case _SEXTUPOLE:

    e.type = _SEXTUPOLE;
      
    if(params.lset) {
      e.l = params.l;
    }
    if(params.k0set) {
      if(VERBOSE)
	printf("Warning: k0 will not be set for element %s of type SEXTUPOLE\n",name);
    }
    if(params.k2set) {
      e.k2 = params.k2;
    }
    if(params.k1set) {
      if(VERBOSE)
	printf("Warning: k1 will not be set for element %s of type SEXTUPOLE\n",name);
    }
    
    break;
  case _OCTUPOLE:

    e.type = _OCTUPOLE;
    
    if(params.lset) {
      e.l = params.l;
    }
    
    if(params.k3set) {
      e.k3 = params.k3;
    }
    
      
    break;

  case _MULT:
    
    e.type = _MULT;
    
    if(params.knlset)
      e.knl = params.knl;

    if(params.kslset)
      e.ksl = params.ksl;

    if(params.lset) {
      e.l = params.l;
    }
    if(params.k1set) {
      if(VERBOSE)
	printf("Warning: k1 will not be set for element %s of type MULTIPOLE\n",name);
    }
    if(params.k0set) {
      if(VERBOSE)
	printf("Warning: k0 will not be set for element %s of type MULTIPOLE\n",name);
    }
    if(params.k2set) {
      if(VERBOSE)
	printf("Warning: k2 will not be set for element %s of type MULTIPOLE\n",name);
    }
    
    break;

  case _ECOL:
    e.type = _ECOL;
    e.l = params.l;
    e.material = std::string(params.material);
    break;
  case _RCOL:
    e.type = _RCOL;
    e.l = params.l;
    e.material = std::string(params.material);
    break;

  case _LASER:
    e.type = _LASER;
    e.l = params.l;
    e.xdir = params.xdir;
    e.ydir = params.ydir;
    e.zdir = params.zdir;
    e.waveLength = params.waveLength;
    break;

  case _ELEMENT:
    e.type = _ELEMENT;
    e.l = params.l;
    e.geometryFile = std::string(params.geometry);
    //strcpy(e.geometryFile,params.geometry);
    e.bmapFile = std::string(params.bmap);
    break;

  case _LINE:
   
    e.lst = lst;
    e.type = _LINE;
    break;

  case _REV_LINE:

    e.lst = lst;
    e.type = _REV_LINE;
    break;

  case _SAMPLER:
    e.type = _SAMPLER;
    break;
    
  case _TRANSFORM3D:
    e.type = _TRANSFORM3D;
    e.xdir = params.xdir;
    e.ydir = params.ydir;
    e.zdir = params.zdir;
    e.theta = params.theta;
    e.phi = params.phi;
    e.psi = params.psi;
    break;

  case _MATERIAL:
    e.type = _MATERIAL;
    e.A = params.A;
    e.Z = params.Z;
    e.density = params.density;
    e.temper = params.temper;
    e.components = params.components;
    e.componentsWeights = params.componentsWeights;
    e.componentsFractions = params.componentsFractions;
    material_list.push_back(e);
    return 0;

  case _ATOM:
    e.type = _ATOM;
    e.A = params.A;
    e.Z = params.Z;
    e.symbol = params.symbol;
    atom_list.push_back(e);
    return 0;

  case _TUNNEL:
    e.type = _TUNNEL;
    e.l = -1;
    e.geometryFile = std::string(params.geometry);
    break;

  default:
    break;  
  }

  element_list.push_back(e);

  return 0;

}

int expand_line(char *name, char *start, char* end)
{
  std::list<struct Element>::iterator it;
  
  struct Element e;
  
  it = element_lookup(name);
  
//  if( (it!=NULL) && ((*it).type == _LINE || (*it).type == _REV_LINE) ) 
  if((*it).type == _LINE || (*it).type == _REV_LINE ) 

    {
      
      // delete the previous beamline
      
      beamline_list.clear();
      
      // expand the desired beamline
      
      e.type = (*it).type;
      e.name = name;
      e.l = 0;
      e.lst = NULL;
      
      beamline_list.push_back(e);
      
      if(VERBOSE) printf("expanding line %s, range = %s/%s\n",name,start,end);
      
      if(!(*it).lst) return 0; //list empty

      
      // first expand the whole range 
      std::list<struct Element>::iterator sit = (*it).lst->begin();
      std::list<struct Element>::iterator eit = (*it).lst->end();

      // copy the list into the resulting list
      switch((*it).type){
	case _LINE:
          beamline_list.insert(beamline_list.end(),(*it).lst->begin(),(*it).lst->end());
          break;
	case _REV_LINE:
          beamline_list.insert(beamline_list.end(),(*it).lst->rbegin(),(*it).lst->rend());
	  break;
	default:
          beamline_list.insert(beamline_list.end(),(*it).lst->begin(),(*it).lst->end());
	}
      bool is_expanded = false;
      
      // insert material entries.
      // TODO:::


      // parse starting from the second element until the list is expanded
      int iteration = 0;
      while(!is_expanded)
        {
	  is_expanded = true;
	  for(it = ++beamline_list.begin();it!=beamline_list.end();it++ )
	    {
	      if(DEBUG) printf("%s , %s \n",(*it).name,typestr((*it).type));
	      
	      if((*it).type == _LINE || (*it).type == _REV_LINE)  // list - expand further	  
		{
		  is_expanded = false;
		  // lookup the line in main list
		  std::list<struct Element>::iterator tmpit = element_lookup((*it).name);
		  
		  if( (tmpit != NULL) && ( (*tmpit).lst != NULL) ) { // sublist found and not empty
		    
		    if(DEBUG)
		      printf("inserting sequence for %s - %s ...",(*it).name,(*tmpit).name);
		    if((*it).type == _LINE)
		    beamline_list.insert(it,(*tmpit).lst->begin(),(*tmpit).lst->end());
		    else if((*it).type == _REV_LINE)
		    beamline_list.insert(it,(*tmpit).lst->rbegin(),(*tmpit).lst->rend());
		    if(DEBUG) printf("inserted\n");
		    
		    // delete the list pointer
		    beamline_list.erase(it--);
		    
		  } else if ( tmpit != NULL ) // entry points to a scalar element type -
		    //transfer properties from the main list
		    { 
		      if(DEBUG) printf("keeping element...%s\n",(*it).name);
		      copy_properties(it,tmpit);
		      if(DEBUG) printf("done\n");

		    } else  // element of undefined type - neglecting
		      {
			if(VERBOSE)
			  printf("Warning : Expanding line %s : element %s has not been \
                               defined , skipping \n",name,(*it).name);
			beamline_list.erase(it--);
		      }
		  
		} else  // element - keep as it is 
		  {
		    // do nothing
		  }
	      
	    }
	  iteration++;
	  if( iteration > MAX_EXPAND_ITERATIONS )
	    {
	      printf("Error : Line expansion of '%s' seems to loop, \
                     \n possible recursive line definition,quitting \n",name);
	      exit(0);
	    }
	  
	}// while
      
      
      // leave only the desired range
      //
      // rule - from first occurence of 'start' till first 'end' coming after 'start'


      if( (start!=NULL)) // determine  the start element
	{
	  sit = element_lookup(start,beamline_list);
	  
	  if(sit==NULL)
	    {
	      sit = beamline_list.begin();
	    }
	  
	  if(!strcmp(start,"#s")) sit = beamline_list.begin(); 
	  
	  beamline_list.erase(beamline_list.begin(),sit);

	}

      if( (end!=NULL)) // determine  the end element
	{
	  eit = element_lookup(end,beamline_list);
	  
	  if(eit==NULL)
	    {
	      eit = beamline_list.end();
	    }
	  
	  if(!strcmp(end,"#e")) eit = beamline_list.end();	  


	  beamline_list.erase(++eit,beamline_list.end());
	}


      // insert the tunnel if present

      it = element_lookup("tunnel");
      if(it!=NULL)
	beamline_list.push_back(*it);
      
      return 0;
    }
  
  
  printf("line '%s' not found",name);
  return 1;
  
}

std::list<struct Element>::iterator element_lookup(char *name)
{
   std::list<struct Element>::iterator it;

   for(it=element_list.begin();it!=element_list.end();it++)
     {
       if(!strcmp((*it).name,name) )
	 return it;
     }
   return NULL;
}

std::list<struct Element>::iterator element_lookup(char *name,std::list<struct Element>& el)
{
   std::list<struct Element>::iterator it;

   for(it=el.begin();it!=el.end();it++)
     {
       if(!strcmp((*it).name,name) )
	 return it;
     }
   return NULL;
}


// insert a sampler into beamline_list
void add_sampler(char *name, char *before, int before_count)
{
  if(DEBUG) std::cout<<"inserting sampler before "<<before<<"["<<before_count<<"]"<<std::endl;

  std::list<struct Element>::iterator it;

  int element_count = 1;  // count from 1 like in goddam FORTRAN -- for range parsing
  struct Element e;
  e.type = _SAMPLER;
  e.name = name;
  e.lst = NULL;

  for(it = beamline_list.begin();it != beamline_list.end(); ++it)
    {
      if(DEBUG) std::cout<<"-->"<<(*it).name<<std::endl;

      if( !strcmp((*it).name, before)) 
	{

	  if( before_count == element_count)
	    {
	      beamline_list.insert(it,e);
	      return;
	    }

	  element_count++;
	}

    }

  std::cout<<"current beamline doesn't contain element "<<before<<" with number "<<before_count<<std::endl;

}

// insert a cylindrical sampler into beamline_list
void add_csampler(char *name, char *before, int before_count, double length, double rad)
{
  if(DEBUG) std::cout<<"inserting sampler before "<<before<<"["<<before_count<<"]"<<std::endl;

  std::list<struct Element>::iterator it;

  int element_count = 1;  // count from 1 like in goddam FORTRAN -- for range parsing
  struct Element e;
  e.type = _CSAMPLER;
  e.l = length;
  e.r = rad;
  e.name = name;
  e.lst = NULL;

  for(it = beamline_list.begin();it != beamline_list.end(); ++it)
    {
      if(DEBUG) std::cout<<"-->"<<(*it).name<<std::endl;

      if( !strcmp((*it).name, before)) 
	{

	  if( before_count == element_count)
	    {
	      beamline_list.insert(it,e);
	      return;
	    }

	  element_count++;
	}

    }

  std::cout<<"current beamline doesn't contain element "<<before<<" with number "<<before_count<<std::endl;

}

// insert a beam dumper into beamline_list
void add_dump(char *name, char *before, int before_count)
{
  if(DEBUG) std::cout<<"inserting dump before "<<before<<"["<<before_count<<"]"<<std::endl;

  std::list<struct Element>::iterator it;

  int element_count = 1;  // count from 1 like in goddam FORTRAN -- for range parsing
  struct Element e;
  e.type = _DUMP;
  e.name = name;
  e.lst = NULL;

  for(it = beamline_list.begin();it != beamline_list.end(); ++it)
    {
      if(DEBUG) std::cout<<"-->"<<(*it).name<<std::endl;

      if( !strcmp((*it).name, before))
        {

          if( before_count == element_count)
            {
              beamline_list.insert(it,e);
              return;
            }


          element_count++;
        }

    }

  std::cout<<"current beamline doesn't contain element "<<before<<" with number "<<before_count<<std::endl;

}

// insert beam gas                                             
void add_gas(char *name, const char *before, int before_count, const char *material)
{
  printf("gas %s will be inserted into %s number %d\n",material,before,before_count);
  struct Element e;
  e.type = _GAS;
  e.name = name;
  e.lst = NULL;
  element_list.insert(beamline_list.end(),e);
 
}


void print(std::list<struct Element> l, int ident)
{

  if(VERBOSE) if(ident == 0) printf("using line %s\n",current_line);

  std::list<struct Element>::iterator it;
  std::list<double>::iterator it2;

  for(it=l.begin();it!=l.end();it++)
    {
      for(int i=0;i<ident;i++)
	printf("--");

      printf("->%s : %s",(*it).name,typestr((*it).type));

      switch((*it).type) {
      case _DRIFT:
      case _SBEND:
      case _RBEND:
      case _QUAD:
      case _SEXTUPOLE:
	printf(", l=%.10g, k0=%.10g, k1=%.10g, k2=%.10g,angle=%.10g,tilt=%.10g ",
	       (*it).l,(*it).k0,(*it).k1,(*it).k2, (*it).angle,(*it).tilt);
	break;
      case _MULT:

	printf(" , knl={");
	for(it2=(*it).knl.begin();it2!=(*it).knl.end();it2++)
	  printf("%.10g, ",(*it2));
	printf("},  ksl={");
	for(it2=(*it).ksl.begin();it2!=(*it).ksl.end();it2++)
	  printf("%.10g, ",(*it2));
	printf("}");
	break;

      case _ELEMENT:
	printf("\ngeometry file : %s\n",(*it).geometryFile.c_str());
	printf("B map file : %s\n",(*it).bmapFile.c_str());
	//printf("E map driver : %s\n",(*it).geometryFile);
	//printf("E map file : %s\n",(*it).geometryFile);
	break;

      case _CSAMPLER:
	printf(" length=%.10g, radius=%.10g",(*it).l, (*it).r);
	break;

      case _TRANSFORM3D:
	printf(" xdir=%.10g, ydir=%.10g, zdir=%.10g,  phi=%.10g, theta=%.10g,psi=%.10g",
	       (*it).xdir, (*it).ydir, (*it).zdir, (*it).phi, (*it).theta, (*it).psi);
	break;
      case _MATERIAL:
	printf(" A=%.10g, Z=%.10g, density=%.10g,  temper=%.10g",
	       (*it).A, (*it).Z, (*it).density, (*it).temper);
	break;
      default:
	break;
      }

      printf("\n");

      if((*it).lst != NULL)
	{
	  print(*(*it).lst,++ident);
	  ident--;
	}
    }
}

void print(struct Options opt)
{
  std::cout<<"Options : "<<std::endl;
  std::cout<<"particle : "<<opt.particleName<<std::endl;
  std::cout<<"energy : "<<opt.beamEnergy<<std::endl;
  std::cout<<"n particles : "<<opt.numberOfParticles<<std::endl;
  std::cout<<"n macroparticles : "<<opt.numberToGenerate<<std::endl;
  std::cout<<"sigmaX           : "<<opt.sigmaX<<std::endl;
  std::cout<<"interactions on           : "<<opt.turnOnInteractions<<std::endl;
}


void set_value(std::string name, double value )
{
  if(name == "energy" ) options.beamEnergy = value;
  if(name == "nparticles" ) options.numberOfParticles = (int)value;
  if(name == "ngenerate" ) options.numberToGenerate = (int)value;
  if(name == "nperfile" ) options.numberOfEventsPerNtuple = (int)value;
  if(name == "eventNumberOffset" ) options.eventNumberOffset = (int)value;
  if(name == "beampipeRadius" ) options.beampipeRadius = value;

  if(name == "boxSize" ) {options.componentBoxSize = value;}

  if(name == "tunnelRadius" ) options.tunnelRadius = value;
  if(name == "beampipeThickness" ) options.beampipeThickness = value;

  if(name == "sigmaX" ) options.sigmaX = value;
  if(name == "sigmaY" ) options.sigmaY = value;
  if(name == "sigmaXp" ) options.sigmaXp = value;
  if(name == "sigmaYp" ) options.sigmaYp = value;
  if(name == "sigmaT" ) options.sigmaT = value;
  if(name == "sigmaE" ) options.sigmaE = value;

  if(name == "x" ) options.x = value;
  if(name == "y" ) options.y = value;
  if(name == "xp" ) options.xp = value;
  if(name == "yp" ) options.yp = value;

  if(name == "X0" ) options.X0 = value;
  if(name == "Y0" ) options.X0 = value;
  if(name == "Rmin" ) options.Rmin = value;
  if(name == "Rmax" ) options.Rmax = value;

  if(name == "deltaChord") options.deltaChord = value;
  if(name == "deltaIntersection") options.deltaIntersection = value;
  if(name == "chordStepMinimum") options.chordStepMinimum = value;

  if(name == "lengthSafety") options.lengthSafety = value;

  if(name == "turnInteractions") 
    {
      if(value == 0) options.turnOnInteractions = false;
      else options.turnOnInteractions = true;
    }

  if(name == "thresholdCutCharged" ) options.thresholdCutCharged = value;
  if(name == "thresholdCutPhotons" ) options.thresholdCutPhotons = value;
  if(name == "useEMHadronic" ) options.useEMHadronic = (int) value;

  if(name == "storeTrajectory") options.storeTrajectory = (int) value; 
  if(name == "storeMuonTrajectory") options.storeMuonTrajectories = (int) value; 
  if(name == "storeNeutronTrajectory") options.storeNeutronTrajectories = (int) value; 

  if(name == "stopTracks") options.stopTracks = (int) value; 

  if(name == "randomSeed") options.randomSeed = (int) value;

  if(name == "nlinesIgnore") options.nlinesIgnore = (int) value;

  if(name == "synchRadOn") options.synchRadOn = (int) value;
  if(name == "srMeanFreeFactor") options.synchMeanFreeFactor = (int) value;
  if(name == "srRescale") options.synchRescale = (int) value;
  if(name == "srLowX") options.synchLowX = value;
  if(name == "srLowGamE") options.synchLowGamE = value;
  if(name == "srMultiplicity") options.synchPhotonMultiplicity = (int) value;
  if(name == "srTrackPhotons") options.synchTrackPhotons = (int) value;

  if(name == "minimumEpsilonStep" ) options.minimumEpsilonStep = value;
  if(name == "maximumEpsilonStep" ) options.maximumEpsilonStep = value;
  if(name == "deltaOneStep" ) options.deltaOneStep = value;

  if(name == "prodCutPhotons" ) options.prodCutPhotons = value;
  if(name == "prodCutPhotonsP" ) options.prodCutPhotonsP = value;

  if(name == "prodCutElectrons" ) options.prodCutElectrons = value;
  if(name == "prodCutElectronsP" ) options.prodCutElectronsP = value;

  if(name == "prodCutPositrons" ) options.prodCutPositrons = value;
  if(name == "prodCutPositronsP" ) options.prodCutPositronsP = value;

  // twiss parameters

  if(name == "betx" ) options.betx = value;
  if(name == "bety" ) options.bety = value;
  if(name == "alfx" ) options.alfx = value;
  if(name == "alfy" ) options.alfy = value;
  if(name == "emitx" ) options.emitx = value;
  if(name == "emity" ) options.emity = value;

  if(name == "doTwiss" ) options.doTwiss = (int) value;


}


void set_value(std::string name, std::string value )
{
  if(name == "fifo") options.fifo = value;
  if(name == "particle") options.particleName = value;
  if(name == "distrType" ) options.distribType = value;
  if(name == "distrFile" ) options.distribFile = value;  
  if(name == "physicsList" ) options.physicsList = value; 
  if(name == "pipeMaterial" ) options.pipeMaterial = value;
}

double property_lookup(char *element_name, char *property_name)
{
   std::list<struct Element>::iterator it = element_lookup(element_name);

   if(it == NULL) return 0;
   
   if(!strcmp(property_name,"l")) return (*it).l;
   if(!strcmp(property_name,"B")) return (*it).B;
   if(!strcmp(property_name,"k0")) return (*it).k0;
   if(!strcmp(property_name,"k1")) return (*it).k1;
   if(!strcmp(property_name,"k2")) return (*it).k2;
   if(!strcmp(property_name,"k3")) return (*it).k3;
   if(!strcmp(property_name,"aper")) return (*it).aper;
   if(!strcmp(property_name,"outR")) return (*it).outR;
   if(!strcmp(property_name,"xsize")) return (*it).xsize;
   if(!strcmp(property_name,"ysize")) return (*it).ysize;
   if(!strcmp(property_name,"xdir")) return (*it).xdir;
   if(!strcmp(property_name,"ydir")) return (*it).ydir;
   if(!strcmp(property_name,"zdir")) return (*it).zdir;
   if(!strcmp(property_name,"phi")) return (*it).phi;
   if(!strcmp(property_name,"psi")) return (*it).psi;
   if(!strcmp(property_name,"theta")) return (*it).theta;
   if(!strcmp(property_name,"waveLength")) return (*it).waveLength;
   if(!strcmp(property_name,"tilt")) return (*it).tilt;
   if(!strcmp(property_name,"gradient")) return (*it).gradient;
   if(!strcmp(property_name,"hgap")) return (*it).hgap;

   if(!strcmp(property_name,"A")) return (*it).A;
   if(!strcmp(property_name,"Z")) return (*it).Z;
   if(!strcmp(property_name,"density")) return (*it).density;
   if(!strcmp(property_name,"temper")) return (*it).temper;

   return 0;
}

// ******************************************************
// parser functions
// ******************************************************


int add_func(char *name, double (*func)(double))
{
  struct symtab *sp=symlook(name);
  sp->funcptr=func;
  return 0;
}

int add_var(char *name, double value, int is_reserved = 0)
{
  struct symtab *sp=symlook(name);
  sp->value=value;
  sp->is_reserved = is_reserved;
  return 0;
}

#endif
