#include <string>
#include <iostream>
#include "getEnv.h"

const char* getEnv(const char* env)
{
  std::cout << "getEnv() - getting env ##" << env << "##" << std::endl;
  const char* tmp = std::getenv(env);
  if(tmp==NULL){
    std::cout << "getEnv - no environment variable \"" << env << "\" is set" << std::endl;
    return (const char*)"";
  } else {
    std::string stmp = (std::string)tmp+ "/";
    std::cout << "getEnv - environment variable \"" << stmp << "\" is set" << std::endl;
    return stmp.c_str();
  }
}
