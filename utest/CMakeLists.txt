
#       Author: Alexander Ksenofontov
#       License: MIT
#       All right reserved

include_directories(${ROOT_PROJECT}/base)
include_directories(${GOOGLETEST}/include)

set(testbase_source
    testbase/circbuffer0.cpp
    testbase/circbuffer1.cpp
    testbase/circbuffer2.cpp
    testbase/circbuffer3.cpp
    testbase/circbuffer00.cpp
    testbase/circbuffer10.cpp
    testbase/circbuffer20.cpp
    testbase/circbuffer100.cpp
    testbase/circbuffer101.cpp
    testbase/circbuffer102.cpp
)

add_executable(utest
    main.cpp
    ${testbase_source}
)


target_link_libraries(utest
    ${GOOGLETEST_LIB}
    base
    pthread
)
