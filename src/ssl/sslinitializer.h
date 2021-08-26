/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */


#include <openssl/err.h>
#include <openssl/ssl.h>
#include <singletone.h>

namespace sparrow
{

struct SslInitializer {

    SslInitializer()
    {
        SSL_library_init();
        OpenSSL_add_all_algorithms();
        SSL_load_error_strings();
        ERR_load_BIO_strings();
        //    ERR_load_crypto_strings();
    }
};

} // namespace sparrow