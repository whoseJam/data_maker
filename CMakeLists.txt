cmake_minimum_required(VERSION 3.15)

project(DataMaker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# set(SRC_LIST maker/history/203.cpp)
set(SRC_LIST maker/test/test_clone.cpp)

include_directories(include)
include_directories(include/AbStructure)
include_directories(include/DtStructure)
include_directories(include/Utility)

add_subdirectory(src/AbStructure)
add_subdirectory(src/DtStructure)
add_subdirectory(src/Utility)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} PUBLIC AbStructure DtStructure Utility)
