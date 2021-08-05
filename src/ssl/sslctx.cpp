/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <format.h>
#include <log.h>
#include "sslctx.h"

namespace mchs {

static SSL_CTX* ToSslContext(void* ptr) {
    return static_cast<SSL_CTX*>(ptr);
}

std::string SslContext::GetLastErrorText() {
    char buf[1024];
    std::string ret;
    for(unsigned long code = ERR_get_error(); code; code = ERR_get_error()) {
        ERR_error_string_n(code, buf, sizeof(buf));
        ret += std::string("\n") + buf;
    }
    return ret;
}

void SslContext::Deleter(void* p) {
    SSL_CTX_free(ToSslContext(p));
}

void SslContext::Load(const std::string& certfile, const std::string& keyfile) {
    if (SSL_CTX_use_certificate_file(ToSslContext(m_ctx.get()), certfile.c_str(),  SSL_FILETYPE_PEM) != 1) {
        throw std::runtime_error(mchs::vtformat("SSL_CTX_use_certificate_file Error '%1'.", GetLastErrorText()));
    }

    if (SSL_CTX_use_PrivateKey_file(ToSslContext(m_ctx.get()), keyfile.c_str(), SSL_FILETYPE_PEM) != 1) {
        throw std::runtime_error(mchs::vtformat("SSL_CTX_use_PrivateKey_file Error '%1'.", GetLastErrorText()));
    }

    /* Make sure the key and certificate file match. */
    if (SSL_CTX_check_private_key(ToSslContext(m_ctx.get())) != 1){
        throw std::runtime_error(mchs::vtformat("SSL_CTX_check_private_key Error '%1'.", GetLastErrorText()));
    }
    else {
        LOG(Info) << mchs::vtformat("Ssl certificate '%1' and private key '%2' loaded and verified",
                certfile, keyfile);
    }

    SSL_CTX_set_options(ToSslContext(m_ctx.get()), SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3);
}

SslContext::SslContext() {

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    auto s = SSL_CTX_new(SSLv23_method());
    m_ctx = std::unique_ptr<void, std::function<void(void*)>>(s, &SslContext::Deleter);
    if(!m_ctx) {
        throw std::runtime_error(mchs::vtformat("SSL Error '%1'.", GetLastErrorText()));
    }

    LOG(Info) << "Ssl context created";

}

}//namespace mchs {
