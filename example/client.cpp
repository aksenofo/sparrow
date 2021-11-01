/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */
#include "client.h"

#include "server.h"

#include <format.h>
#include <log.h>
#include <singletone.h>
#include <sslbase.h>
#include <sslbio.h>

int portNumber = 55555;

using namespace sparrow;

SslClient::SslClient()
{
    m_tcp = std::make_unique<TcpSocket>("127.0.0.1", portNumber);
    m_io.set<SslClient, &SslClient::OnCallback>(this);
    m_io.start(m_tcp->Socket(), ev::WRITE | ev::READ);
    SslContext ctx(SSLv23_method());
    SslBase base(ctx);
    base.SetConnectState();
    base.SetBio(SslBio(), SslBio());
    m_handler = std::make_unique<SslHandler>(base);
    LOG(Always) << format("Client is started.");
}

#define MAX_USER_SIZE 1024

void SslClient::OnCallback(ev::io& watcher, int revents)
{
    bool write = revents & EV_WRITE;
    bool read = revents & EV_READ;

    if (m_sendBuffer.IsEmpty())
        m_sendBuffer.Put("Hello from client\n");

    bool rc = m_handler->Handle(m_sendBuffer, m_recvBuffer, watcher.fd, write, read);

    while(!m_recvBuffer.IsEmpty()) {
        char buffer [MAX_USER_SIZE];
        auto v = m_recvBuffer.Get(buffer, sizeof(buffer));
        if (v) {
            std::cout << std::string(buffer, v);
        }
    }

    if (rc)
        watcher.start(m_tcp->Socket(), (read ? ev::READ : 0) | (write ? ev::WRITE : 0));
    else
        watcher.stop();
}
