# quietly detect if we have Boost available - not formal requirement

option(USE_BOOST "Use Boost C++ library." OFF)
if (USE_BOOST)
  message(STATUS "Using Boost")
  #set(Boost_USE_STATIC_LIBS OFF)
  #set(Boost_USE_MULTITHREADED ON)
  #set(Boost_USE_STATIC_RUNTIME OFF)
  find_package(Boost 1.71 REQUIRED)
  if(Boost_FOUND)
    include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
    add_definitions("-DUSE_BOOST")
  else()
    message(FATAL_ERROR "USE_BOOST turned on, but no Boost library found.")
  endif()
endif()