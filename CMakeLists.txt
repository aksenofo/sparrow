cmake_minimum_required(VERSION 3.13.4)

project(mchs)

SET(ROOT_PROJECT ${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)

#===================================================================================
add_definitions(-pthread)
add_definitions(-fpic)
#===================================================================================


#if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
#    SET(GOOGLE_TEST_INCLUDE ${PROJECT_SOURCE_DIR}/3th/googletest-debug/include)
#    SET(GOOGLE_TEST_LIB ${PROJECT_SOURCE_DIR}/3th/googletest-debug/lib/libgtestd.a)
#else ( ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
#    SET(GOOGLE_TEST_INCLUDE ${PROJECT_SOURCE_DIR}/3th/googletest-release/include)
#    SET(GOOGLE_TEST_LIB ${PROJECT_SOURCE_DIR}/3th/googletest-release/lib/libgtest.a)
#endif ()

add_subdirectory(src)

