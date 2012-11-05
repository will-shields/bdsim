#include <cstdlib>
#include <iostream>
#include "getEnv.h"

std::string getEnv(const char* env)
{
  const char* tmp = std::getenv(env);
  if(tmp==NULL){
    std::cout << "getEnv - no environment variable \"" << env << "\" is set" << std::endl;
    return (const char*)"";
  } else {
    std::string stmp = (std::string)tmp+ "/";
    std::cout << "getEnv - environment variable \"" << stmp << "\" is set" << std::endl;
    return stmp;
  }
}
