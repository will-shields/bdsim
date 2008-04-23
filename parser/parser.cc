/*
 *  interactive gmad
 *  Ilya Agapov 2005
 */


#include "gmad.h"
#include <cstdio>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
  cout<<endl;
  if(argc<2) return -1;
  gmad_parser(argv[1]);
}
