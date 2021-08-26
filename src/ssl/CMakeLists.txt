#       Author: Alexander Ksenofontov
#       License: MIT
#       All right reserved
 
include_directories(.)
include_directories(${ROOT_PROJECT}/base)
include_directories(${OPENSSL}/include)

add_library(sparrow-ssl
    sslbase.cpp
    sslcontext.cpp
    sslbio.cpp
    sslaux.cpp
    sslhandler.cpp
)
