/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "sslctx.h"

#include "sslinternal.h"

#include <format.h>
#include <functional>
#include <log.h>


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

void SslContext::SetVerify(int mode, SSL_verify_cb verify_callback)
{
    SSL_CTX_set_verify(CtxPtr(), mode, verify_callback);
}

void SslContext::SetVerifyDepth(int depth)
{
    SSL_CTX_set_verify_depth(CtxPtr(), depth);
}

int SslContext::UseCertificateASN1(int len, unsigned char* d)
{
    return SSL_CTX_use_certificate_ASN1(CtxPtr(), len, d);
}

int SslContext::UseCertificateFile(const std::string& file, int type)
{
    return SSL_CTX_use_certificate_file(CtxPtr(), file.c_str(), type);
}

int SslContext::UseCertificateChainFile(const std::string& file)
{
    return SSL_CTX_use_certificate_chain_file(CtxPtr(), file.c_str());
}

int SslContext::UsePrivateKey(EVP_PKEY* pkey)
{
    return SSL_CTX_use_PrivateKey(CtxPtr(), pkey);
}

int SslContext::UsePrivateKeyASN1(int pk, unsigned char* d, long len)
{
    return SSL_CTX_use_PrivateKey_ASN1(pk, CtxPtr(), d, len);
}

int SslContext::UsePrivateKeyFile(const std::string& file, int type)
{
    return SSL_CTX_use_PrivateKey_file(CtxPtr(), file.c_str(), type);
}

int SslContext::UseRSAPrivateKey(RSA* rsa)
{
    return SSL_CTX_use_RSAPrivateKey(CtxPtr(), rsa);
}

int SslContext::UseRSAPrivateKeyASN1(unsigned char* d, long len)
{
    return SSL_CTX_use_RSAPrivateKey_ASN1(CtxPtr(), d, len);
}

int SslContext::UseRSAPrivateKeyFile(const std::string& file, int type)
{
    return SSL_CTX_use_RSAPrivateKey_file(CtxPtr(), file.c_str(), type);
}

int SslContext::CheckPrivateKey()
{
    return SSL_CTX_check_private_key(CtxPtr());
}

long SslContext::SetOptions(long options)
{
    return SSL_CTX_set_options(CtxPtr(), options);
}

long SslContext::ClearOptions(long options)
{
    return SSL_CTX_clear_options(CtxPtr(), options);
}

long SslContext::GetOptions()
{
    return SSL_CTX_get_options(CtxPtr());
}

} //namespace sparrow
