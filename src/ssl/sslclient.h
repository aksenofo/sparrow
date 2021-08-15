/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#pragma once

#include <sslbase.h>
#include <utils.h>

namespace sparrow
{

class SslClient : public SslBase
{
public:
   // MOVEBLE_DEFAULT(SslClient)
    SslClient(SslContext& context);

};

} //namespace sparrow
