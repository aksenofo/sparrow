#       Author: Alexander Ksenofontov
#       License: MIT
#       All right reserved
 
include_directories(.)
include_directories(${ROOT_PROJECT}/base)

add_library(sparrow-ssl
    sslctx.cpp
)
