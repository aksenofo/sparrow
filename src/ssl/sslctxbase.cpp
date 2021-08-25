/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "sslctxbase.h"

#include <format.h>
#include <functional>
#include <log.h>


namespace sparrow
{

SslCtxBase::SslCtxBase(const SSL_METHOD* method)
{
    SSL_CTX* ctx = SSL_CTX_new(method);
    m_ctx = std::unique_ptr<SSL_CTX, std::function<void(SSL_CTX*)>>(ctx, &SslCtxBase::Deleter);
}

SslCtxBase::SslCtxBase(SSL_CTX* ctx)
{
    SSL_CTX_up_ref(ctx);
    m_ctx = std::unique_ptr<SSL_CTX, std::function<void(SSL_CTX*)>>(ctx, &SslCtxBase::Deleter);
}

SslCtxBase::SslCtxBase(const SslCtxBase& sslContext)
{
    SSL_CTX* ctx = const_cast<SSL_CTX*>(sslContext.CtxPtr());
    SSL_CTX_up_ref(ctx);
    m_ctx = std::unique_ptr<SSL_CTX, std::function<void(SSL_CTX*)>>(ctx, &SslCtxBase::Deleter);
}

SslCtxBase& SslCtxBase::operator=(const SslCtxBase& sslContext)
{
    SSL_CTX* ctx = const_cast<SSL_CTX*>(sslContext.CtxPtr());
    SSL_CTX_up_ref(ctx);
    m_ctx = std::unique_ptr<SSL_CTX, std::function<void(SSL_CTX*)>>(ctx, &SslCtxBase::Deleter);
    return *this;
}

void SslCtxBase::SetVerify(int mode, SSL_verify_cb verify_callback)
{
    SSL_CTX_set_verify(CtxPtr(), mode, verify_callback);
}

void SslCtxBase::SetVerifyDepth(int depth)
{
    SSL_CTX_set_verify_depth(CtxPtr(), depth);
}

void SslCtxBase::UseCertificateASN1(int len, unsigned char* d)
{
    CheckIf_1(SSL_CTX_use_certificate_ASN1(CtxPtr(), len, d));
}

void SslCtxBase::UseCertificateFile(const std::string& file, int type)
{
    CheckIf_1(SSL_CTX_use_certificate_file(CtxPtr(), file.c_str(), type));
}

void SslCtxBase::UseCertificateChainFile(const std::string& file)
{
    CheckIf_1(SSL_CTX_use_certificate_chain_file(CtxPtr(), file.c_str()));
}

void SslCtxBase::UsePrivateKey(EVP_PKEY* pkey)
{
    CheckIf_1(SSL_CTX_use_PrivateKey(CtxPtr(), pkey));
}

void SslCtxBase::UsePrivateKeyASN1(int pk, unsigned char* d, long len)
{
    CheckIf_1(SSL_CTX_use_PrivateKey_ASN1(pk, CtxPtr(), d, len));
}

void SslCtxBase::UsePrivateKeyFile(const std::string& file, int type)
{
    CheckIf_1(SSL_CTX_use_PrivateKey_file(CtxPtr(), file.c_str(), type));
}

void SslCtxBase::UseRSAPrivateKey(RSA* rsa)
{
    CheckIf_1(SSL_CTX_use_RSAPrivateKey(CtxPtr(), rsa));
}

void SslCtxBase::UseRSAPrivateKeyASN1(unsigned char* d, long len)
{
    CheckIf_1(SSL_CTX_use_RSAPrivateKey_ASN1(CtxPtr(), d, len));
}

void SslCtxBase::UseRSAPrivateKeyFile(const std::string& file, int type)
{
    CheckIf_1(SSL_CTX_use_RSAPrivateKey_file(CtxPtr(), file.c_str(), type));
}

void SslCtxBase::CheckPrivateKey()
{
    CheckIf_1(SSL_CTX_check_private_key(CtxPtr()));
}

long SslCtxBase::SetOptions(long options)
{
    return SSL_CTX_set_options(CtxPtr(), options);
}

long SslCtxBase::ClearOptions(long options)
{
    return SSL_CTX_clear_options(CtxPtr(), options);
}

long SslCtxBase::GetOptions()
{
    return SSL_CTX_get_options(CtxPtr());
}

} //namespace sparrow
