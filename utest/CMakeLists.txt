
#       Author: Alexander Ksenofontov
#       License: MIT
#       All right reserved

include_directories(${ROOT_PROJECT}/base)
include_directories(${GOOGLETEST}/include)

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
)

add_executable(utest
    main.cpp
    ${base_source}
)


target_link_libraries(utest
    ${GOOGLETEST_LIB}
    base
    pthread
)

