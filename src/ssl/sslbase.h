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
    virtual ~SslBase();
    SslBase(SSL_CTX* sslCtx);
    SslBase(SslContext& sslCtx);
    SslBase(SSL* ssl);
    SslBase(const SslBase& ssl);
    SslBase& operator=(const SslBase& ssl);
    SSL* SslPtr() { return m_ssl.get(); }
    const SSL* SslPtr() const { return m_ssl.get(); }
    bool HasObj() const { return !!m_ssl; }
    void SetBio(const SslBio& rbio, const SslBio& wbio);
    void SetConnectState();
    void SetAcceptState();
    bool IsInitFinished() const noexcept;
    SslBio Rbio();
    SslBio Wbio();
    bool PreparedAsServer() const noexcept { return m_preparedAsServer; }
    template<typename SslBaseT>
    SslBaseT Dup();
    int Read(uint8_t* ptr, size_t size) const noexcept;
    int Write(const uint8_t* ptr, size_t size) const noexcept;

    int DoHandshake();

    std::unique_ptr<SslContext> ContextPtr();

    bool IsAcceptableReturn(const int n, const int code) const noexcept;
    bool IsCode(int n, int extraCode);

private:
    static bool DetectIfServer(const SSL* ssl);

    static void Deleter(SSL* p) noexcept;
    std::unique_ptr<SSL, std::function<void(SSL*)>> m_ssl;
    bool m_preparedAsServer = false;
};

template<typename SslBaseT>
SslBaseT SslBase::Dup()
{
    SSL* ssl = SslPtr();
    SSL* nssl = SSL_dup(ssl);
    CheckIfNullptr(nssl);
    return SslBaseT(nssl);
}

inline bool SslBase::IsInitFinished() const noexcept
{
    assert(m_ssl.get());
    return SSL_is_init_finished(m_ssl.get());
}

inline void SslBase::Deleter(SSL* p) noexcept
{
    if (p)
        SSL_free(p);
}

} //namespace sparrow
