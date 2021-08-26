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

class SslContext : public SslAux
{
public:
    SslContext() = default;
    SslContext(const SSL_METHOD* method);
    SslContext(SSL_CTX* ctx);
    MOVEBLE_DEFAULT(SslContext);
    virtual ~SslContext() = default;

    SslContext(const SslContext& sslContext);
    SslContext& operator=(const SslContext& sslContext);

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

    bool HasObj() const { return !!m_ctx; }
    bool operator==(const SslContext& ctx) const noexcept;
    bool operator!=(const SslContext& ctx) const noexcept;

    SSL_CTX* CtxPtr() noexcept
    {
        return m_ctx.get();
    }
    const SSL_CTX* CtxPtr() const noexcept { return m_ctx.get(); }

private:
    static void Deleter(SSL_CTX* p) noexcept;

    std::unique_ptr<SSL_CTX, std::function<void(SSL_CTX*)>> m_ctx;
};

inline void SslContext::Deleter(SSL_CTX* p) noexcept
{
    if (p)
        SSL_CTX_free(p);
}


inline bool SslContext::operator==(const SslContext& ctx) const noexcept
{
    return m_ctx.get() == ctx.m_ctx.get();
}

inline bool SslContext::operator!=(const SslContext& ctx) const noexcept
{
    return m_ctx.get() != ctx.m_ctx.get();
}

} //namespace sparrow
