/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#pragma once

#include <openssl/ssl.h>

namespace sparrow::internal
{

inline SSL_CTX* ToSslContext(void* ptr) noexcept
{
    return static_cast<SSL_CTX*>(ptr);
}

inline SSL* ToSsl(void* ptr) noexcept
{
    return static_cast<SSL*>(ptr);
}

inline BIO* ToBio(void* ptr) noexcept
{
    return static_cast<BIO*>(ptr);
}

} //namespace sparrow::internal