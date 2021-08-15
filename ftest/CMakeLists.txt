#       Author: Alexander Ksenofontov
#       License: MIT
#       All right reserved

#libev current version is not support c++17. Downgrade version  
set(CMAKE_CXX_STANDARD 14) 

include_directories(${ROOT_PROJECT}/base)
include_directories(${ROOT_PROJECT}/ssl)
include_directories(${LIBEV}/include)
include_directories(${OPENSSL}/include)
include_directories(auxillary)

add_executable(ftest
    main.cpp
    client.cpp
    server.cpp
    auxillary/tcp.cpp
)


target_link_libraries(ftest
    ${LIBEV_LIB}
    sparrow-ssl
    base
    ${OPENSSL_LIB}
    pthread
    dl
)
