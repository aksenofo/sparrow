/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#pragma once

#include <memory>
#include <openssl/ssl.h>
#include <utils.h>
#include <sslaux.h>

namespace sparrow
{

class SslCtxBase : public SslAux
{
public:
    SslCtxBase() = default;
    SslCtxBase(const SSL_METHOD* method);
    SslCtxBase(SSL_CTX* ctx);
    MOVEBLE_DEFAULT(SslCtxBase);
    SslCtxBase(const SslCtxBase& sslContext);
    SslCtxBase& operator=(const SslCtxBase& sslContext);

    void SetVerify(int mode, SSL_verify_cb verify_callback);
    void SetVerifyDepth(int depth);

    //int SSL_CTX_use_certificate(SSL_CTX* ctx, X509* x);
    void UseCertificateASN1(int len, unsigned char* d);
    void UseCertificateFile(const std::string& file, int type);
    void UseCertificateChainFile(const std::string& file);
    void UsePrivateKey(EVP_PKEY* pkey);
    void UsePrivateKeyASN1(int pk, unsigned char* d, long len);
    void UsePrivateKeyFile(const std::string& file, int type);

    void UseRSAPrivateKey(RSA* rsa);
    void UseRSAPrivateKeyASN1(unsigned char* d, long len);
    void UseRSAPrivateKeyFile(const std::string& file, int type);

    void CheckPrivateKey();

    long SetOptions(long options);
    long ClearOptions(long options);
    long GetOptions();

private:
    SSL_CTX* CtxPtr() noexcept { return m_ctx.get(); }
    const SSL_CTX* CtxPtr() const noexcept { return m_ctx.get(); }
    static void Deleter(SSL_CTX* p) noexcept { SSL_CTX_free(p); }

    std::unique_ptr<SSL_CTX, std::function<void(SSL_CTX*)>> m_ctx;
};

} //namespace sparrow
