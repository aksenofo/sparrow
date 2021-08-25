/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include <format.h>
#include <log.h>
#include <sslbase.h>
#include <unistd.h>
#include <utils.h>

namespace sparrow
{

SslBase::SslBase(SSL_CTX* sslCtx)
{
    assert(sslCtx);
    SSL* ssl = SSL_new(sslCtx);
    m_ssl = std::unique_ptr<SSL, std::function<void(SSL*)>>(ssl, &SslBase::Deleter);
    CheckIfNullptr(m_ssl.get());
}

SslBase::SslBase(SslCtxBase& sslCtx)
{
    assert(sslCtx.CtxPtr());
    SSL* ssl = SSL_new(sslCtx.CtxPtr());
    m_ssl = std::unique_ptr<SSL, std::function<void(SSL*)>>(ssl, &SslBase::Deleter);
    CheckIfNullptr(m_ssl.get());
}

SslBase::SslBase(SSL* ssl)
{
    assert(ssl);
    m_ssl = std::unique_ptr<SSL, std::function<void(SSL*)>>(ssl, &SslBase::Deleter);
    CheckIfNullptr(m_ssl.get());
}

SslBase::SslBase(const SslBase& ssl)
{
    SSL* tssl = const_cast<SSL*>(ssl.SslPtr());
    assert(tssl);
    SSL_up_ref(tssl);
    m_ssl = std::unique_ptr<SSL, std::function<void(SSL*)>>(tssl, &SslBase::Deleter);
    CheckIfNullptr(m_ssl.get());
}

SslBase& SslBase::operator=(const SslBase& ssl)
{
    SSL* tssl = const_cast<SSL*>(ssl.SslPtr());
    assert(tssl);
    SSL_up_ref(tssl);
    m_ssl = std::unique_ptr<SSL, std::function<void(SSL*)>>(tssl, &SslBase::Deleter);
    CheckIfNullptr(m_ssl.get());
    return *this;
}

} // namespace sparrow