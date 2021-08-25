/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#pragma once

#include "sslctxbase.h"

#include <circlebuffer.h>
#include <openssl/ssl.h>
#include <sslaux.h>
#include <string>
#include <utils.h>

namespace sparrow
{
class SslCtxBase;

class SslBase : public SslAux
{

public:
    SslBase() = default;
    MOVEBLE_DEFAULT(SslBase);

    SslBase(SSL_CTX* sslCtx);
    SslBase(SslCtxBase& sslCtx);

    SslBase(SSL* ssl);
    SslBase(const SslBase& ssl);
    SslBase& operator=(const SslBase& ssl);

    SSL* SslPtr() { return m_ssl.get(); }
    const SSL* SslPtr() const { return m_ssl.get(); }

    template<typename SslBaseT>
    SslBaseT Dup();

    bool HasObj() const { return !!m_ssl; }

private:
    static void Deleter(SSL* p) noexcept;
    std::unique_ptr<SSL, std::function<void(SSL*)>> m_ssl;
};

template<typename SslBaseT>
SslBaseT SslBase::Dup()
{
    SSL* ssl = SslPtr();
    SSL* nssl = SSL_dup(ssl);
    return SslBaseT(nssl);
}


inline void SslBase::Deleter(SSL* p) noexcept
{
    if (p)
        SSL_free(p);
}

} //namespace sparrow
