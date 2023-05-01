cmake_minimum_required(VERSION 3.15)

project(DtStructure)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include_directories(../../include)
include_directories(../../include/AbStructure)
include_directories(../../include/DtStructure)
include_directories(../../include/Utility)

aux_source_directory(. DTSTRUCTURE_SRC_LIST)

add_library(${PROJECT_NAME} ${DTSTRUCTURE_SRC_LIST})