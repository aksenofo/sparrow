/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include <sslaux.h>
#include <sslbase.h>
#include <utils.h>

namespace sparrow
{

class SslHandler : public SslAux {

public:

    COPYBLE_DEFAULT(SslHandler);
    MOVEBLE_DEFAULT(SslHandler);
    
    SslHandler(const SslBase& ssl);

private:
    SslBase m_ssl;
};

} //namespace sparrow
