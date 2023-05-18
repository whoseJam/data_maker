cmake_minimum_required(VERSION 3.15)

project(AbStructure)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include_directories(../../include)
include_directories(../../include/AbStructure)
include_directories(../../include/DtStructure)
include_directories(../../include/Utility)

# aux_source_directory(. ABSTRUCTURE_SRC_LIST)
set(ABSTRUCTURE_SRC_LIST Node.cpp Integer.cpp Format.cpp Character.cpp)

add_library(${PROJECT_NAME} ${ABSTRUCTURE_SRC_LIST})