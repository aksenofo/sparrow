/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "sslclient.h"

namespace sparrow
{

SslClient::SslClient(SslContext& context)
: SslBase(context, false)
{
}

} //namespace sparrow
