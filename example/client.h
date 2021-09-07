/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include <circlebuffer.h>
#include <ev++.h>
#include <sslbase.h>
#include <sslhandler.h>
#include <tcp.h>
#include <utils.h>

extern int portNumber;

class SslClient
{
    constexpr static size_t kBufferSize = 1024;

public:
    SslClient();
    NOCOPIBLE(SslClient);

private:
    void OnCallback(ev::io& watcher, int revents);

    ev::io m_io;
    std::unique_ptr<TcpSocket> m_tcp;
    std::unique_ptr<sparrow::SslHandler> m_handler;

    sparrow::CircularBuffer m_sendBuffer = sparrow::CircularBuffer(kBufferSize);
    sparrow::CircularBuffer m_recvBuffer = sparrow::CircularBuffer(kBufferSize);

};