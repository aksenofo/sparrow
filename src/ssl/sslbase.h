/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#pragma once

#include "sslcontext.h"

#include <circlebuffer.h>
#include <openssl/ssl.h>
#include <sslaux.h>
#include <string>
#include <utils.h>

namespace sparrow
{
class SslContext;
class SslBio;

class SslBase : public SslAux
{

public:
    SslBase() = default;
    MOVEBLE_DEFAULT(SslBase);
    virtual ~SslBase() = default;
    SslBase(SSL_CTX* sslCtx);
    SslBase(SslContext& sslCtx);
    SslBase(SSL* ssl);
    SslBase(const SslBase& ssl);
    SslBase& operator=(const SslBase& ssl);
    SSL* SslPtr() { return m_ssl.get(); }
    const SSL* SslPtr() const { return m_ssl.get(); }
    template<typename SslBaseT>
    SslBaseT Dup();
    bool HasObj() const { return !!m_ssl; }
    void SetBio(SslBio& rbio, SslBio& wbio); 
    void SetConnectState();
    void SetAcceptState();

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
