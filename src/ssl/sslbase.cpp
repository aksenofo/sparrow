/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include <format.h>
#include <log.h>
#include <sslbase.h>
#include <sslbio.h>
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

    SSL_CTX_up_ref(sslCtx);
    m_context = std::make_unique<SslContext>(sslCtx);
}

SslBase::SslBase(SslContext& sslCtx)
{
    assert(sslCtx.CtxPtr());
    SSL* ssl = SSL_new(sslCtx.CtxPtr());
    m_ssl = std::unique_ptr<SSL, std::function<void(SSL*)>>(ssl, &SslBase::Deleter);
    CheckIfNullptr(m_ssl.get());
    m_context = std::make_unique<SslContext>(sslCtx);
}

SslBase::SslBase(SSL* ssl)
{
    assert(ssl);
    m_ssl = std::unique_ptr<SSL, std::function<void(SSL*)>>(ssl, &SslBase::Deleter);
    CheckIfNullptr(m_ssl.get());
    SSL_CTX* ctx = SSL_get_SSL_CTX(ssl);
    CheckIfNullptr(ctx);
    SSL_CTX_up_ref(ctx);
    m_context = std::make_unique<SslContext>(ctx);
}

SslBase::SslBase(const SslBase& ssl)
{
    SSL* tssl = const_cast<SSL*>(ssl.SslPtr());
    assert(tssl);
    SSL_up_ref(tssl);
    m_ssl = std::unique_ptr<SSL, std::function<void(SSL*)>>(tssl, &SslBase::Deleter);
    CheckIfNullptr(m_ssl.get());
    m_context = std::make_unique<SslContext>(*ssl.m_context);
}

SslBase& SslBase::operator=(const SslBase& ssl)
{
    SSL* tssl = const_cast<SSL*>(ssl.SslPtr());
    assert(tssl);
    SSL_up_ref(tssl);
    m_ssl = std::unique_ptr<SSL, std::function<void(SSL*)>>(tssl, &SslBase::Deleter);
    CheckIfNullptr(m_ssl.get());
    m_context = std::make_unique<SslContext>(*ssl.m_context);
    return *this;
}

void SslBase::SetBio(const SslBio& rbio, const SslBio& wbio)
{
    BIO* trbio = const_cast<BIO*>(rbio.BioPtr());
    BIO* twbio = const_cast<BIO*>(wbio.BioPtr());

    CheckIf_1(BIO_up_ref(trbio));
    try {
        CheckIf_1(BIO_up_ref(twbio));
    } catch (...) {
        BIO_free(trbio);
        throw;
    }

    SSL_set_bio(m_ssl.get(), trbio, twbio);
}

void SslBase::SetConnectState()
{
    m_preparedAsServer = false;
    SSL_set_connect_state(m_ssl.get());
}

void SslBase::SetAcceptState()
{
    m_preparedAsServer = true;
    SSL_set_accept_state(m_ssl.get());
}

SslBio SslBase::Rbio()
{
    BIO* bio = SSL_get_rbio(m_ssl.get());
    CheckIfNullptr(bio);
    BIO_up_ref(bio);
    return SslBio(bio);
}

SslBio SslBase::Wbio()
{
    BIO* bio = SSL_get_wbio(m_ssl.get());
    CheckIfNullptr(bio);
    BIO_up_ref(bio);
    return SslBio(bio);
}

SslBase::~SslBase() = default;

} // namespace sparrow