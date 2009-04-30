/*
 * GMAD interface
 *
 */

#include "gmad.h"
#include "sym_table.h"

#include <cmath>

using namespace std;

extern struct Parameters params;
extern struct symtab *symtab;
extern int yyparse();

extern FILE *yyin;
extern char* yyfilename;

extern int add_func(char *name, double (*func)(double));
extern int add_var(char *name, double val,int is_rserved = 0);

// aux. parser lists - to clear
extern list<struct Element> element_list;
extern list<struct Element> tmp_list;

void init()
{

  symtab = new struct symtab[NSYMS];

  // embedded arithmetical functions
  add_func("sqrt",sqrt);
  add_func("cos",cos);
  add_func("sin",sin);
  add_func("exp",exp);
  add_func("log",log); 
  add_func("tan",tan);
  add_func("asin",asin); 
  add_func("acos",acos);
  add_func("abs",fabs);
	add_func("atan",atan);

  add_var("pi",3.14159265358979,_RESERVED);

  add_var("TeV",1e+3,_RESERVED);
  add_var("GeV",1.0 ,_RESERVED);
  add_var("MeV",1e-3,_RESERVED);
  add_var("KeV",1e-6,_RESERVED);
  add_var("eV" ,1e-9,_RESERVED);

  add_var("MV",1.0,_RESERVED);

  add_var("Tesla",1.0,_RESERVED);

  add_var("m"  ,1.0 ,_RESERVED);
  add_var("cm" ,1e-2,_RESERVED);
  add_var("mm" ,1e-3,_RESERVED);
  add_var("um",1e-6,_RESERVED);
  add_var("nm" ,1e-9,_RESERVED);

  add_var("s"  ,1.0  ,_RESERVED);
  add_var("ms" ,1.e-3,_RESERVED);
  add_var("us",1.e-6,_RESERVED);
  add_var("ns" ,1.e-9,_RESERVED);

  add_var("rad" ,1.0  ,_RESERVED);
  add_var("mrad",1.e-3,_RESERVED);


  add_var("clight",2.99792458e+8,_RESERVED);

  params.flush();

  // Default Values for Options (the rest are set to 0)

  options.synchPhotonMultiplicity = 1;
  options.synchMeanFreeFactor = 1;

  
}

int gmad_parser(FILE *f)
{

  init();

  yyin=f; 
  while(!feof(yyin))
    {
      yyparse();
    }

  // clear temporary stuff

  cout<<"clearing..."<<endl;
  element_list.clear();
  tmp_list.clear();
  
    
  return 0;
};

int gmad_parser(string name)
{
  FILE *f = fopen(name.c_str(),"r");

  if(f==NULL) return -1;

  init();
  
  yyin=f; 
  yyfilename = new char[32];
  strncpy(yyfilename,name.c_str(),32);

  while(!feof(yyin))
    {
      yyparse();
    }

  // clear temporary stuff

  element_list.clear();
  tmp_list.clear();
  return 0;
};

