
# in case you did not run cmake we assume you want to test the "old style build"
if(NOT binaryname)
    set(binary ${CMAKE_CURRENT_SOURCE_DIR}/bin/Linux-g++/./bdsim)
else()
    set(binary ${CMAKE_CURRENT_BINARY_DIR}/./${binaryname})
endif()

#add_test(NAME LHC1 COMMAND ${binary} --file=job_inj450_B1_blm.gmad --batch WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples/lhc/injection/B1/)

add_test(NAME GGMAD COMMAND ${binary} --file=line.gmad --batch WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples/ggmad/)

add_test(NAME Mokka1 COMMAND ${binary} --file=2mradIR.gmad --batch WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples/Mokka/2mradIR)

add_test(NAME SR COMMAND ${binary} --file=line.gmad --batch WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples/SR/)

# Potentially needed if binary is compiled "old style"
# set_tests_properties(LHC1 GGMAD Mokka1 SR PROPERTIES ENVIRONMENT 
#                "PATH=${CMAKE_CURRENT_SOURCE_DIR}/bin/Linux-g++/:$ENV{PATH};LD_LIBRARY_PATH=${CMAKE_CURRENT_SOURCE_DIR}/parser/:$ENV{LD_LIBRARY_PATH}")