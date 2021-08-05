
include_directories(.)
include_directories(${ROOT_PROJECT}/base)

add_library(sparrow-ssl
    sslctx.cpp
)
