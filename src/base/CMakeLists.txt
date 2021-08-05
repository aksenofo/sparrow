#       Author: Alexander Ksenofontov
#       License: MIT
#       All right reserved
 
include_directories(.)

add_library(base
    format.cpp
    circlebuffer.cpp
    utils.cpp
    log.cpp
)
