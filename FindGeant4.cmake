# Taken from
# http://root.cern.ch/svn/root/trunk/cmake/modules/FindGEANT4.cmake
# with some additional changes

# - Try to find Geant4
# Once done this will define
#
#  Geant4_FOUND - system has Geant4
#  Geant4_INCLUDE_DIR - the Geant4 include directory
#  Geant4_LIBRARIES - The libraries needed to use Geant4
#  Geant4_DEFINITIONS - Compiler switches required for using Geant4
#

if (Geant4_INCLUDE_DIR AND Geant4_LIBRARY_DIR)
  SET (Geant4_INCLUDE_DIR Geant4_INCLUDE_DIR-NOTFOUND)
  SET (Geant4_LIB_DIR Geant4_LIB_DIR-NOTFOUND)
  SET (Geant4_PLISTS_LIB_DIR Geant4_PLISTS_LIB_DIR-NOTFOUND)
  SET (Geant4_DIR Geant4_DIR-NOTFOUND)
endif (Geant4_INCLUDE_DIR AND Geant4_LIBRARY_DIR)

MESSAGE(STATUS "Looking for Geant4...")

#FIND_PATH(Geant4_DIR NAMES env.sh PATHS
#  ${SIMPATH}/transport/geant4
#  ${SIMPATH}/transport/geant4/source
#  NO_DEFAULT_PATH
#)

# commented out for now - JS
#FIND_PATH(Geant4_INCLUDE_DIR NAMES G4Event.hh PATHS
#  ${SIMPATH}/transport/geant4/include
#  NO_DEFAULT_PATH
#)

# SET(Geant4_INCLUDE_DIR
# ${SIMPATH}/transport/geant4/include
# ${SIMPATH}/transport/geant4/source/interfaces/common/include 
# ${SIMPATH}/transport/geant4/physics_lists/hadronic/Packaging/include   
# ${SIMPATH}/transport/geant4/physics_lists/hadronic/QGSP/include
# )

#FIND_PATH(Geant4_LIB_DIR NAMES libG4baryons.so libG4baryons.dylib PATHS
#  ${SIMPATH}/transport/geant4/lib/Linux-g++
#  ${SIMPATH}/transport/geant4/lib/Linux-icc
#  ${SIMPATH}/transport/geant4/lib
#  NO_DEFAULT_PATH
#)

IF (Geant4_LIB_DIR)
  SET(Geant4_LIBRARY_DIR ${Geant4_LIB_DIR})
ENDIF (Geant4_LIB_DIR)

if (Geant4_INCLUDE_DIR AND Geant4_LIBRARY_DIR)
   set(Geant4_FOUND TRUE)
endif (Geant4_INCLUDE_DIR AND Geant4_LIBRARY_DIR)

if (Geant4_LIBRARY_DIR)
    # YIL simple array with all libraries:
    if(APPLE)
        file(GLOB Geant4_LIBRARIES ${Geant4_LIBRARY_DIR}/libG4*.dylib ${G4_SUPPORT)
    else()
        file(GLOB Geant4_LIBRARIES ${Geant4_LIBRARY_DIR}/libG4*.so)
    endif()
endif()

#message("Geant4_LIBRARY_DIR: ${Geant4_LIBRARY_DIR}")
#message("Geant4_LIBRARIES: ${Geant4_LIBRARIES}")

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

