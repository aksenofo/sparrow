/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#pragma once

#include <memory>
#include <openssl/ssl.h>
#include <sslaux.h>
#include <utils.h>

namespace sparrow
{

class SslContext
{
public:
    SslContext() = default;
    SslContext(const SSL_METHOD* method);
    SslContext(SSL_CTX* ctx);
    MOVEBLE_DEFAULT(SslContext);
    SslContext(const SslContext& sslContext);
    SslContext& operator=(const SslContext& sslContext);
    
    void SetVerify(int mode, SSL_verify_cb verify_callback);
    void SetVerifyDepth(int depth);

private:
    SSL_CTX* CtxPtr() noexcept { return m_ctx.get(); }
    const SSL_CTX* CtxPtr() const noexcept { return m_ctx.get(); }
    static void Deleter(SSL_CTX* p) noexcept { SSL_CTX_free(p); }

    std::unique_ptr<SSL_CTX, std::function<void(SSL_CTX*)>> m_ctx;
};

} //namespace sparrow
