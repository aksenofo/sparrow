#       Author: Alexander Ksenofontov
#       License: MIT
#       All right reserved
 

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")

    SET(dirpostfix debug)
    SET(namepostfix d)

elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")

    SET(dirpostfix release)

elseif(${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")

    SET(dirpostfix release)

elseif(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")

    SET(dirpostfix release)

else ()

    message(FATAL_ERROR, "Unknown build type:" ${CMAKE_BUILD_TYPE} ".")

endif()

##############################################################################################################
SET(GOOGLETEST ${PROJECT_SOURCE_DIR}/3th/googletest-${dirpostfix})
SET(GOOGLETEST_LIB ${GOOGLETEST}/lib/libgtest_main${namepostfix}.a ${GOOGLETEST}/lib/libgtest${namepostfix}.a)


SET(LIBEV ${PROJECT_SOURCE_DIR}/3th/libev-release)
SET(LIBEV_LIB ${LIBEV}/lib/libev.a)

SET(OPENSSL ${PROJECT_SOURCE_DIR}/3th/openssl-${dirpostfix})

