/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#pragma once

#include <circlebuffer.h>
#include <sslaux.h>
#include <sslbase.h>
#include <utils.h>

namespace sparrow
{

const int kEncBufferSize = 32;

class SslHandler : public SslAux
{

public:
    COPYBLE_DEFAULT(SslHandler);
    MOVEBLE_DEFAULT(SslHandler);

    SslHandler(const SslBase& ssl);

    SslHandler(SSL* ssl);

    bool Handle(CircularBuffer& sendBuf, CircularBuffer& recvBuf, int socket, bool& write, bool& read);

private:
    static void SockSend(int sock, CircularBuffer& cb);
    static bool SockRecv(int sock, CircularBuffer& mb);

    void Unencrypt(CircularBuffer& cb);
    bool Encrypt(CircularBuffer& cb);

    bool DoHandshake(int socket, bool& write, bool& read);
    bool DoDataExchande(int socket, CircularBuffer& sendBuf, CircularBuffer& recvBuf, bool& write, bool& read);
    CircularBufferConst<kEncBufferSize> m_encSendBuffer;
    CircularBufferConst<kEncBufferSize> m_encRecvBuffer;

    SslBase m_ssl;
};

} //namespace sparrow
