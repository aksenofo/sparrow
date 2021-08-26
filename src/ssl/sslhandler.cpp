/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include <sslbio.h>
#include <sslhandler.h>

namespace sparrow
{

SslHandler::SslHandler(const SslBase& ssl)
: m_ssl(ssl)
{
}

SslHandler::SslHandler(SSL* ssl)
: m_ssl(ssl)
{
}

bool SslHandler::Handle(CircularBuffer& sendBuf, CircularBuffer& recvBuf, int socket, bool& write, bool& read)
{
    return true;
}

bool SslHandler::DoHandhakke(int socket, bool& write, bool& read)
{
    return true;
}

} //namespace sparrow
