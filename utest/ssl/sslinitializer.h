#include <openssl/ssl.h>
#include <openssl/err.h>
#include <singletone.h>

struct SslInitializer
{

    SslInitializer()
    {
        SSL_library_init();
        OpenSSL_add_all_algorithms();
        SSL_load_error_strings();
        ERR_load_BIO_strings();
        //    ERR_load_crypto_strings();
    }
};
