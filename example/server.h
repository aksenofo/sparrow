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
extern int portNumber;

class SslInstance
{
    constexpr static size_t kBufferSize = 1024;

public:
    SslInstance(int socket);
    NOCOPIBLE(SslInstance);

private:
    void OnCallback(ev::io& watcher, int revents);

    ev::io m_io;
    std::unique_ptr<TcpSocket> m_tcp;

    std::unique_ptr<sparrow::SslHandler> m_handler;

    sparrow::CircularBuffer m_sendBuffer = sparrow::CircularBuffer(kBufferSize);
    sparrow::CircularBuffer m_recvBuffer = sparrow::CircularBuffer(kBufferSize);

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
