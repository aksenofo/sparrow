/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "server.h"

#include "log.h"

#include <format.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <tcp.h>

SslInstance::SslInstance(int socket)
{
    m_sslServer = std::make_unique<sparrow::SslServer>(sparrow::Singletone<sparrow::SslContext>::Instance());
    m_tcp = std::make_unique<TcpSocket>(socket);

    m_io.set<SslInstance, &SslInstance::OnCallback>(this);
    m_io.start(m_tcp->Socket(), ev::READ);
}

void SslInstance::OnCallback(ev::io& watcher, int revents) noexcept
{
}

SslServer::SslServer()
{
    m_tcp = std::make_unique<TcpListener>(10000, 5);
    m_io.set<SslServer, &SslServer::OnAccept>(this);
    m_io.start(m_tcp->Socket(), ev::READ);
}

void SslServer::OnAccept(ev::io& watcher, int revents) noexcept
{
    if (EV_ERROR & revents) {
        LOG(Error) << "Got invalid event";
        return;
    }

    int client = accept(watcher.fd, nullptr, nullptr);
    if (client < 0) {
        LOG(Error) << sparrow::format("Accept error. Code %1.", sparrow::ToECode(errno));
        return;
    }

    LOG(Info) << sparrow::format("Accepted Instance");

    m_Instance = std::make_unique<SslInstance>(client);
}
