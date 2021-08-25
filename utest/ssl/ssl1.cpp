/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "sslinitializer.h"

#include "gtest/gtest.h"
#include <sslctxbase.h>

using namespace sparrow;

TEST(ssl, test0)
{
    Singletone<SslInitializer>::Instance();

    SslCtxBase ctx(SSLv23_method());
    long options = SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3;
    ASSERT_TRUE(ctx.SetOptions(options) > 0);
    ASSERT_TRUE(ctx.GetOptions() > 0);
    try {
        ctx.UseCertificateFile("", 0);
        FAIL() << "Exeption is expected";
    } catch (const std::exception& exception) {
        SUCCEED();
    }
}
