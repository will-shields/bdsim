cmake_minimum_required(VERSION 2.6.4)

project(BDSIM) 

# project version
set( PROJECT_MAJOR_VERSION 0 )
set( PROJECT_MINOR_VERSION 6 )
set( PROJECT_PATCH_LEVEL 0 )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

# CLHEP

set(CLHEP_VERSION 9)
find_package(CLHEP REQUIRED)
add_definitions("-DCLHEP_VERSION=${CLHEP_VERSION}")
include_directories(${CLHEP_INCLUDE_DIR})

# ROOT

if(USE_ROOT)
find_package(ROOT)
if(ROOT_FOUND)
    option(USE_ROOT "Include ROOT support."  ON )
else()
    option(USE_ROOT "Include ROOT support."  OFF )
endif()
endif()

if(USE_ROOT)
    find_package(ROOT REQUIRED)
    add_definitions("-DUSE_ROOT")
    include_directories(${ROOT_INCLUDE_DIR})
endif()

# GDML (off for now, as xerces include to be fixed)

option( USE_GDML "Include GDML support."  OFF )
if(USE_GDML)
   add_definitions("-DUSE_GDML")
endif()

# LCDD (off for now as xml includes hardcoded and xml include needs to be fixed)

option( USE_LCDD "Include LCDD support."  OFF )
if(USE_LCDD)
   add_definitions("-DUSE_LCDD")
endif()

# GEANT4

find_package(Geant4 REQUIRED)
set(Geant4_USE_FILE UseGeant4.cmake)
include(${Geant4_USE_FILE})

GEANT4_COLLATE_APPLICATION_SOURCES(bdsim_sources)
add_library(bdsim ${bdsim_sources})
# For now hardcoded, to be fixed!!
set(G4VERSION 9)
set(G4MINORVERSION 4)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/parser/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# For now suppressing a lot of the warnings...
set(WFLAGS "-Wall  -Wno-non-virtual-dtor -Wno-write-strings -Wno-deprecated")
set(CMAKE_FLAGS "${CMAKE_FLAGS} ${CLHEP_DEFINITIONS}  ${WFLAGS}")
set(BDSIM_FLAGS "${CMAKE_FLAGS} -DG4VIS_USE -DCLHEP_VERSION=${CLHEP_VERSION} -DG4VERSION=${G4VERSION} -DG4MINORVERSION=${G4MINORVERSION} ${BDSIM_FLAGS}")

#option DG4VIS_USE

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${BDSIM_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BDSIM_FLAGS}")

add_subdirectory( parser )

add_executable(bdsimexec bdsim.cc)

set_target_properties(bdsimexec PROPERTIES OUTPUT_NAME bdsim)
target_link_libraries(bdsimexec bdsim gmad)
target_link_libraries(bdsim ${CLHEP_LIBRARIES})
target_link_libraries(bdsim ${Geant4_LIBRARIES})

# xercesc library (to be fixed):
set(XercesC_LIBRARY_DIR /afs/cern.ch/sw/lcg/external/XercesC/3.1.0/x86_64-slc5-gcc43-opt/lib)
if (XercesC_LIBRARY_DIR)
    # YIL simple array with all libraries:
    if(APPLE)
        file(GLOB XercesC_LIBRARIES ${XercesC_LIBRARY_DIR}/libxerces-c*.dylib)
    else()
        file(GLOB XercesC_LIBRARIES ${XercesC_LIBRARY_DIR}/libxerces-c*.so)
    endif()
endif()
#message("XercesC_LIBRARIES: ${XercesC_LIBRARIES}")

target_link_libraries(bdsim ${XercesC_LIBRARIES})

if(USE_ROOT)
    target_link_libraries(bdsimexec ${ROOT_LIBRARIES})
endif()
    
get_target_property(binaryname bdsimexec OUTPUT_NAME)

enable_testing()
include(CTest)