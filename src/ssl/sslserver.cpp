/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "sslserver.h"

namespace sparrow
{

SslServer::SslServer(SslContext& context)
: SslBase(context, true)
{
}

} //namespace sparrow
