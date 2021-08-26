/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include <ev++.h>
#include <utils.h>
#include <tcp.h>
#include <sslbase.h>
#include <sslhandler.h>
#include <circlebuffer.h>

class SslClient
{
public:
    SslClient();
    NOCOPIBLE(SslClient);

private:
    void OnCallback(ev::io& watcher, int revents);

    ev::io m_io;
    std::unique_ptr<TcpSocket> m_tcp;
    std::unique_ptr<sparrow::SslHandler> m_handler;

    sparrow::CircularBuffer m_sendBuffer = sparrow::CircularBuffer(1024);
    sparrow::CircularBuffer m_recvBuffer = sparrow::CircularBuffer(1024);

};