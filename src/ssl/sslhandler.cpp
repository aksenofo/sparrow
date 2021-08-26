/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include <sslhandler.h>
#include <sslbio.h>

namespace sparrow
{

SslHandler::SslHandler(const SslBase& ssl)
: m_ssl(ssl)
{
}

} //namespace sparrow
