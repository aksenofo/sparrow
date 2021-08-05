

cmake_minimum_required(VERSION 3.14)

project(sparrow)

SET(ROOT_PROJECT ${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)

#===================================================================================
add_definitions(-pthread)
add_definitions(-fpic)
#===================================================================================


add_subdirectory(src)

