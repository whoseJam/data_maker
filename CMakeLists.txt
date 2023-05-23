cmake_minimum_required(VERSION 3.15)

project(DataMaker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(SRC_LIST example/history/541.cpp)
# set(SRC_LIST example/test/test_array.cpp)


# if ()
#     message(STATUS "making example/history/${TH}.cpp")
#     set(SRC_LIST example/history/${TH}.cpp)
# else()
#     set(SRC_LIST example/test/test_splay.cpp)
# endif()

include_directories(include)
include_directories(include/AbStructure)
include_directories(include/DtStructure)
include_directories(include/Utility)
include_directories(src/DtStructure)
include_directories(/usr/local/include/gtest)

add_subdirectory(src/AbStructure)
add_subdirectory(src/DtStructure)
add_subdirectory(src/Utility)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} PUBLIC AbStructure DtStructure Utility gtest)
