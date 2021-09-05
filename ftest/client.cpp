/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */
#include "client.h"

#include <singletone.h>
#include <sslbase.h>
#include <sslbio.h>
#include <log.h>
#include <format.h>

using namespace sparrow;

SslClient::SslClient()
{
    m_tcp = std::make_unique<TcpSocket>("127.0.0.1", 55555);
    m_io.set<SslClient, &SslClient::OnCallback>(this);
    m_io.start(m_tcp->Socket(), ev::WRITE | ev::READ);
    SslContext ctx(SSLv23_method());
    SslBase base(ctx);
    base.SetConnectState();
    base.SetBio(SslBio(), SslBio());
    m_handler = std::make_unique<SslHandler>(base);
    LOG(Always) << format("Client is started.");
}

void SslClient::OnCallback(ev::io& watcher, int revents)
{
    bool write = revents & EV_WRITE;
    bool read = revents & EV_READ;
    bool rc = m_handler->Handle(m_sendBuffer, m_recvBuffer, watcher.fd, write, read);
    if (rc)
        watcher.start(m_tcp->Socket(), (read ? ev::READ : 0) | (write ? ev::WRITE : 0));
    else
        watcher.stop();

    char b[1024];
    memset(b, 0, sizeof(b));
    m_recvBuffer.Get(b, sizeof(b));

    if (b[0])
        printf("%s", b);

    return;
}
