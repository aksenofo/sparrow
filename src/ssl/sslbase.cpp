/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include <openssl/err.h>
#include <openssl/ssl.h>
#include <sslbase.h>
#include <format.h>
#include <sslinternal.h>

namespace sparrow
{
using namespace internal;

void SslBase::DeleterSsl(void* p)
{
    if (p) // Delete SSL and its BIOs.
        SSL_free(ToSsl(p));
}

static void DeleterBio(void* p)
{
    if (p) // Delete SSL and its BIOs.
        BIO_free(ToBio(p));
}

SslBase::SslBase(SslContext& ctx, bool serverFlag)
{
    SSL* ssl = SSL_new(ToSslContext(ctx.Ctx()));
    m_ssl = std::unique_ptr<void, std::function<void(void*)>>(ssl, &SslBase::DeleterSsl);

    auto rbio = std::unique_ptr<void, std::function<void(void*)>>(BIO_new(BIO_s_mem()), &DeleterBio);
    if (!rbio) {
        throw std::runtime_error(format("BIO_new Error '%1'.", ctx.GetLastErrorText()));
    }

    auto wbio = std::unique_ptr<void, std::function<void(void*)>>(BIO_new(BIO_s_mem()), &DeleterBio);
    if (!wbio) {
        throw std::runtime_error(format("BIO_new Error '%1'.", ctx.GetLastErrorText()));
    }
    if (serverFlag)
        SSL_set_accept_state(ToSsl(m_ssl.get()));
    else
        SSL_set_connect_state(ToSsl(m_ssl.get()));

    m_rbio = rbio.release();
    m_wbio = wbio.release();

    SSL_set_bio(ToSsl(m_ssl.get()), ToBio(m_rbio), ToBio(m_wbio));
}

} // namespace sparrow