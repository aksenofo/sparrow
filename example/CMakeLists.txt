#       Author: Alexander Ksenofontov
#       License: MIT
#       All right reserved

#libev current version does not support c++17. Downgrade c++ version.  
set(CMAKE_CXX_STANDARD 14) 

include_directories(${ROOT_PROJECT}/base)
include_directories(${ROOT_PROJECT}/ssl)
include_directories(${LIBEV}/include)
include_directories(${OPENSSL}/include)
include_directories(auxillary)

add_executable(client
    main_client.cpp
    client.cpp
    auxillary/tcp.cpp
)

target_link_libraries(client
    ${LIBEV_LIB}
    sparrow-ssl
    base
    ${OPENSSL_LIB}
    pthread
    dl
)

add_executable(server
    main_server.cpp
    server.cpp
    auxillary/tcp.cpp
)

target_link_libraries(server
    ${LIBEV_LIB}
    sparrow-ssl
    base
    ${OPENSSL_LIB}
    pthread
    dl
)
