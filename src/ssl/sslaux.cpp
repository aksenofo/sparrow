
/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include <openssl/err.h>
#include <sslaux.h>
#include <format.h>
#include <exception>

namespace sparrow
{

std::string SslAux::GetLastErrorText(char delimiter)
{
    char buf[kErrBufferSize];
    std::string ret;
    for (unsigned long code = ERR_get_error(); code; code = ERR_get_error()) {
        ERR_error_string_n(code, buf, sizeof(buf));
        ret += std::string(delimiter ? 1 : 0, delimiter) + buf;
    }
    return ret;
}

void SslAux::CheckIf_1(const int rc)
{
    if (rc != 1)
        throw std::runtime_error(format("'%1'", GetLastErrorText()));
}

void SslAux::CheckIfNullptr(const void* ptr)
{
    if (!ptr)
        throw std::runtime_error(format("'%1'", GetLastErrorText()));
}

} //namespace sparrow
