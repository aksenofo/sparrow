/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "tcp.h"

#include <ev++.h>
#include <memory>
#include <utils.h>
#include <sslbase.h>
#include <sslhandler.h>
#include <circlebuffer.h>

extern std::string sertificate, privateKey;

class SslInstance
{
public:
    SslInstance(int socket);
    NOCOPIBLE(SslInstance);

private:
    void OnCallback(ev::io& watcher, int revents) noexcept;

    ev::io m_io;
    std::unique_ptr<TcpSocket> m_tcp;

    std::unique_ptr<sparrow::SslHandler> m_handler;

    sparrow::CircularBuffer m_sendBuffer = sparrow::CircularBuffer(1024);
    sparrow::CircularBuffer m_recvBuffer = sparrow::CircularBuffer(1024);

};


class SslServer
{
public:
    SslServer();
    NOCOPIBLE(SslServer);

private:
    void OnAccept(ev::io& watcher, int revents) noexcept;

    ev::io m_io;
    std::unique_ptr<TcpListener> m_tcp;
    std::unique_ptr<SslInstance> m_Instance;
};
