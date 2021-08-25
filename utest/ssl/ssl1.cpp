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
    
}
