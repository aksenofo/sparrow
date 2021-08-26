/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "sslinitializer.h"

#include "gtest/gtest.h"
#include <sslbase.h>
#include <sslbio.h>
#include <sslcontext.h>
#include <sslhandler.h>

using namespace sparrow;

TEST(ssl, test_SslHandler0)
{
    Singletone<SslInitializer>::Instance();

    SslContext ctx(SSLv23_method());

    ASSERT_TRUE(ctx.HasObj());
    SslBase ssl(ctx);
    ASSERT_TRUE(ssl.HasObj());
    ssl.SetAcceptState();
    auto rbio = SslBio();
    auto wbio = SslBio();

    ssl.SetBio(rbio, wbio);
    ASSERT_EQ(ssl.Rbio(), rbio);
    ASSERT_EQ(ssl.Wbio(), wbio);


    SslHandler handler1(ssl);
}
