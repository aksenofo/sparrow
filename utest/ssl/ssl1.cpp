/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include <sslctx.h>
#include "sslinitializer.h"
#include "gtest/gtest.h"

using namespace sparrow;

TEST(ssl, test0)
{
    Singletone<SslInitializer>::Instance();

    SslContext ctx(SSLv23_method());
    long options = SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3;
    ASSERT_TRUE(ctx.SetOptions(options) > 0);
    ASSERT_TRUE(ctx.GetOptions() > 0);
}
