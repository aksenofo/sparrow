#       Author: Alexander Ksenofontov
#       License: MIT
#       All right reserved

include_directories(${ROOT_PROJECT}/base)
include_directories(${ROOT_PROJECT}/ssl)
include_directories(${GOOGLETEST}/include)
include_directories(${OPENSSL}/include)

set(base_source
    base/circbuffer0.cpp
    base/circbuffer1.cpp
    base/circbuffer2.cpp
    base/circbuffer3.cpp
    base/circbuffer00.cpp
    base/circbuffer10.cpp
    base/circbuffer20.cpp
    base/circbuffer100.cpp
    base/circbuffer101.cpp
    base/circbuffer102.cpp
    base/circbuffer1000.cpp
    base/circbuffer1001.cpp
    base/circbuffer1002.cpp
)

set(ssl_source
    ssl/ssl1.cpp
    ssl/ssl2.cpp
)

add_executable(utest
    main.cpp
    ${base_source}
    ${ssl_source}
)

target_link_libraries(utest
    ${GOOGLETEST_LIB}
    sparrow-ssl 
    base
    ${OPENSSL_LIB}
    pthread
    dl
)

