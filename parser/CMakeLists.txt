cmake_minimum_required(VERSION 2.6)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

bison_target(PARSER parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cc)
flex_target(SCANNER parser.l  ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cc)
add_flex_bison_dependency(SCANNER PARSER)

# You can also use file globbing, but that can cause confusion
# when we have old makefiles and cmake next to each other.
# Reason is old makefiles creates parser.cc/lex.yy.cc in source folder..
add_library(gmad gmad.cc ${BISON_PARSER_OUTPUTS} ${FLEX_SCANNER_OUTPUTS})
