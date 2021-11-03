#       Author: Alexander Ksenofontov
#       License: MIT
#       All right reserved
 

cmake_minimum_required(VERSION 3.14)

project(sparrow)

SET(ROOT_PROJECT ${PROJECT_SOURCE_DIR}/src)

set(CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
            "Default build type: Debug" FORCE)
endif()

include(3th/3th.cmake)

add_definitions(-Wno-deprecated)
add_definitions(-Wno-deprecated-declarations)
add_definitions(-pthread)
add_definitions(-fpic)

add_subdirectory(src)
add_subdirectory(utest)
add_subdirectory(ftest)
add_subdirectory(example)

#############################################################
