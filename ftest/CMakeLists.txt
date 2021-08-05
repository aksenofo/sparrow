#       Author: Alexander Ksenofontov
#       License: MIT
#       All right reserved

#libev current version is not support c++17. Downgrade version  
set(CMAKE_CXX_STANDARD 14) 

include_directories(${ROOT_PROJECT}/base)
include_directories(${LIBEV}/include)

add_executable(ftest
    main.cpp
)


target_link_libraries(ftest
    ${LIBEV_LIB}
    base
    pthread
)
