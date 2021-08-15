/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "sslaux.h"
#include <openssl/err.h>

namespace sparrow
{

std::string SslAux::GetLastErrorText()
{
    char buf[1024];
    std::string ret;
    for (unsigned long code = ERR_get_error(); code; code = ERR_get_error()) {
        ERR_error_string_n(code, buf, sizeof(buf));
        ret += std::string("\n") + buf;
    }
    return ret;
}

} //namespace sparrow
