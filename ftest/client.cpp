/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */
#include "client.h"
#include "sslbase.h"
#include <singletone.h>

SslClient::SslClient()
{
    m_tcp = std::make_unique<TcpSocket>("127.0.0.1", 10000);
    m_io.set<SslClient, &SslClient::OnCallback>(this);
    m_io.start(m_tcp->Socket(), ev::WRITE | ev::READ);
    sparrow::SslContext ctx(SSLv23_method());
    sparrow::SslBase base (ctx);

    m_handler = std::make_unique<sparrow::SslHandler>(base);
}

void SslClient::OnCallback(ev::io& watcher, int revents)
{
    bool write = revents & EV_WRITE;
    bool read = revents & EV_READ;
    bool rc = m_handler->Handle(m_sendBuffer, m_recvBuffer, watcher.fd, write, read);
    watcher.start(m_tcp->Socket(), (read ? ev::READ : 0) | (write ? ev::WRITE : 0));
    return;
}
