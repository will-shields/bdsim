#ifndef BDSDebug_h
#define BDSDebug_h

#include <stddef.h>
#include <string>


inline std::string methodName(const std::string& prettyFunction)
{
  size_t colons = prettyFunction.find("::");
  size_t begin = prettyFunction.substr(0,colons).rfind(" ") + 1;
  size_t end = prettyFunction.rfind("(") - begin;

  return prettyFunction.substr(begin,end) +"> ";
}

#define __METHOD_NAME__ methodName(__PRETTY_FUNCTION__)

#define __BDS_DEBUG_FNC__(message) 
#ifdef DEBUG 
G4cout << __FUNCTION__ << ">" << message << G4endl; 
#endif

#ifdef DEBUG
#define __BDS_DEBUG_MET__(message) G4cout << __METHOD_NAME__ << ">" << message << G4endl; 
#else 
#define __BDS_DEBUG_MET__(message) ;
#endif

#endif
