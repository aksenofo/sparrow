/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "server.h"

#include "log.h"

#include <format.h>
#include <netinet/in.h>
#include <sslbio.h>
#include <sys/socket.h>
#include <tcp.h>

using namespace sparrow;

SslInstance::SslInstance(int socket)
{
    m_tcp = std::make_unique<TcpSocket>(socket);

    m_io.set<SslInstance, &SslInstance::OnCallback>(this);
    m_io.start(m_tcp->Socket(), ev::READ);

    SslContext ctx(SSLv23_method());
    SslBase base(ctx);
    base.SetAcceptState();
    base.SetBio(SslBio(), SslBio());
    m_handler = std::make_unique<SslHandler>(base);
}

void SslInstance::OnCallback(ev::io& watcher, int revents) noexcept
{
    bool write = revents & EV_WRITE;
    bool read = revents & EV_READ;
    bool rc = m_handler->Handle(m_sendBuffer, m_recvBuffer, watcher.fd, write, read);
    if (rc)
        watcher.start(m_tcp->Socket(), (read ? ev::READ : 0) | (write ? ev::WRITE : 0));
    return;
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
