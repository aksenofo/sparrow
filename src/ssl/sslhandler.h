/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include <sslaux.h>
#include <sslbase.h>
#include <utils.h>
#include <circlebuffer.h>

namespace sparrow
{

class SslHandler : public SslAux {

public:

    COPYBLE_DEFAULT(SslHandler);
    MOVEBLE_DEFAULT(SslHandler);
    
    SslHandler(const SslBase& ssl);

    bool Handle(CircularBuffer& sendBuf, CircularBuffer& recvBuf, int socket, bool& write, bool& read);

private:
    bool DoHandhakke(int socket, bool& write, bool& read);

    SslBase m_ssl;
};

} //namespace sparrow
