/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "tcp.h"

#include <ev++.h>
#include <memory>
#include <utils.h>

class SslInstance
{
public:
    SslInstance(int socket);
    NOCOPIBLE(SslInstance);

private:
    void OnCallback(ev::io& watcher, int revents) noexcept;

    ev::io m_io;
    std::unique_ptr<TcpSocket> m_tcp;
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
