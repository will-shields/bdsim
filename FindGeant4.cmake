# Taken from
# http://root.cern.ch/svn/root/trunk/cmake/modules/FindGEANT4.cmake
# with some additional changes

# - Try to find Geant4
# Once done this will define
#
#  Geant4_FOUND - system has Geant4
#  Geant4_INCLUDE_DIR - the Geant4 include directory
#  Geant4_LIBRARIES - The libraries needed to use Geant4
#

message(STATUS "Looking for Geant4...")

# include directory

if (NOT Geant4_INCLUDE_DIR)
  FIND_PATH(Geant4_INCLUDE_DIR NAMES Geant4 geant4)
endif (NOT Geant4_INCLUDE_DIR)

if (NOT Geant4_INCLUDE_DIR)
   message(STATUS "Geant4 include directory not found, trying default, please provide it with -DGeant4_INCLUDE_DIR=")
   if (APPLE)
       set(Geant4_INCLUDE_DIR /opt/local/include)
   else()
       set(Geant4_INCLUDE_DIR /usr/include)
   endif()
endif()

# library directory

if(Geant4_LIBRARY_DIR)
  FIND_LIBRARY(Geant4_LIBRARIES NAMES Geant4 PATH_SUFFIXES geant4 Geant4)
else(Geant4_LIBRARY_DIR)
   message(STATUS "Geant4 library directory not found, trying default, please provide it with -DGeant4_LIBRARY_DIR=")
   if (APPLE)
      set(Geant4_LIBRARY_DIR /opt/local/lib)
   else()
      set(Geant4_LIBRARY_DIR /usr/lib)
   endif()
endif(Geant4_LIBRARY_DIR)

if (Geant4_LIBRARY_DIR)
    # YIL simple array with all libraries:
    if(APPLE)
        file(GLOB Geant4_LIBRARIES ${Geant4_LIBRARY_DIR}/libG4*.dylib ${G4_SUPPORT})
    else()
        file(GLOB Geant4_LIBRARIES ${Geant4_LIBRARY_DIR}/libG4*.so)
    endif()
endif(Geant4_LIBRARY_DIR)

if (Geant4_INCLUDE_DIR AND Geant4_LIBRARIES)
   set(Geant4_FOUND TRUE)
endif (Geant4_INCLUDE_DIR AND Geant4_LIBRARIES)

message(STATUS "Geant4_INCLUDE_DIR: ${Geant4_INCLUDE_DIR}")
message(STATUS "Geant4_LIBRARY_DIR: ${Geant4_LIBRARY_DIR}")
#message(STATUS "Geant4_LIBRARIES: ${Geant4_LIBRARIES}")

if (Geant4_FOUND)
  if (NOT Geant4_FIND_QUIETLY)
    MESSAGE(STATUS "Looking for Geant4... - found ${Geant4_LIBRARY_DIR}")
#    message(STATUS "Found ${Geant4_LIBRARY_DIR}")
  endif (NOT Geant4_FIND_QUIETLY)
  SET(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${Geant4_LIBRARY_DIR})
else (Geant4_FOUND)
  if (Geant4_FIND_REQUIRED)
    message(FATAL_ERROR "Looking for Geant4... - Not found")
  endif (Geant4_FIND_REQUIRED)
endif (Geant4_FOUND)

