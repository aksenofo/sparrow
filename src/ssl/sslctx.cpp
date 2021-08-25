/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "sslctx.h"

#include "sslinternal.h"

#include <format.h>
#include <log.h>
#include <functional>


namespace sparrow
{

SslContext::SslContext(const SSL_METHOD* method)
{
    SSL_CTX* ctx = SSL_CTX_new(method);
    m_ctx = std::unique_ptr<SSL_CTX, std::function<void(SSL_CTX*)>>(ctx, &SslContext::Deleter);
}

SslContext::SslContext(SSL_CTX* ctx)
{
    SSL_CTX_up_ref(ctx);
    m_ctx = std::unique_ptr<SSL_CTX, std::function<void(SSL_CTX*)>>(ctx, &SslContext::Deleter);
}

SslContext::SslContext(const SslContext& sslContext)
{
    SSL_CTX* ctx = const_cast<SSL_CTX*>(sslContext.CtxPtr());
    SSL_CTX_up_ref(ctx);
    m_ctx = std::unique_ptr<SSL_CTX, std::function<void(SSL_CTX*)>>(ctx, &SslContext::Deleter);
}

SslContext& SslContext::operator=(const SslContext& sslContext)
{
    SSL_CTX* ctx = const_cast<SSL_CTX*>(sslContext.CtxPtr());
    SSL_CTX_up_ref(ctx);
    m_ctx = std::unique_ptr<SSL_CTX, std::function<void(SSL_CTX*)>>(ctx, &SslContext::Deleter);
    return *this;
}

void SslContext::SetVerify(int mode, SSL_verify_cb verify_callback) {
    SSL_CTX_set_verify(CtxPtr(), mode, verify_callback);
}

void SslContext::SetVerifyDepth(int depth) {
    SSL_CTX_set_verify_depth(CtxPtr(), depth);
}

} //namespace sparrow
