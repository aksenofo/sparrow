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

using namespace sparrow;

TEST(ssl, test_SslCtxBase0)
{
    Singletone<SslInitializer>::Instance();

    SslContext ctx(SSLv23_method());
    SslContext ctx1(ctx);
    ASSERT_TRUE(ctx.HasObj());
    ASSERT_TRUE(ctx1.HasObj());
    SslContext ctx2 = std::move(ctx1);
    ASSERT_FALSE(ctx1.HasObj());
    ASSERT_TRUE(ctx2.HasObj());

    long options = SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3;
    ASSERT_TRUE(ctx.SetOptions(options) > 0);
    ASSERT_TRUE(ctx.GetOptions() > 0);
    try {
        ctx2.UseCertificateFile("", 0);
        FAIL() << "Exeption is expected";
    } catch (const std::exception& exception) {
        SUCCEED();
    }
}

namespace
{
struct Tssl : public SslBase {
    Tssl(SSL* ssl)
    : SslBase(ssl)
    {
    }
};
} // namespace

TEST(ssl, testSslBase0)
{
    Singletone<SslInitializer>::Instance();

    SslContext ctx(SSLv23_method());
    ASSERT_TRUE(ctx.HasObj());

    SslBase ssl1(ctx);
    ASSERT_TRUE(ssl1.HasObj());
    SslBase ssl2(ssl1);
    ASSERT_TRUE(ssl2.HasObj());

    SslBase ssl3 = ssl2;
    ASSERT_TRUE(ssl3.HasObj());

    SslBase ssl4 = std::move(ssl3);
    ASSERT_TRUE(ssl4.HasObj());
    ASSERT_TRUE(!ssl3.HasObj());

    Tssl tt = ssl4.Dup<Tssl>();
    ASSERT_TRUE(tt.HasObj());

    ASSERT_EQ(*tt.ContextPtr(), ctx);
}

TEST(ssl, testBio0)
{
    Singletone<SslInitializer>::Instance();

    SslContext ctx(SSLv23_method());
    ASSERT_TRUE(ctx.HasObj());

    SslBase ssl(ctx);
    ASSERT_TRUE(ssl.HasObj());

    SslBio bio1;
    ASSERT_TRUE(bio1.HasObj());
    SslBio bio2;
    ASSERT_TRUE(bio2.HasObj());

    ssl.SetBio(bio1, bio2);
}

TEST(ssl, testInitAsServer)
{
    Singletone<SslInitializer>::Instance();
    
    SslContext ctx(SSLv23_method());
    ASSERT_TRUE(ctx.HasObj());

    SslBase ssl(ctx);
    ssl.SetAcceptState();

    ssl.SetBio(SslBio(), SslBio());
}

TEST(ssl, testInitAsClient)
{
    Singletone<SslInitializer>::Instance();
    
    SslContext ctx(SSLv23_method());
    ASSERT_TRUE(ctx.HasObj());

    SslBase ssl(ctx);
    ssl.SetConnectState();
    
    ssl.SetBio(SslBio(), SslBio());
}
