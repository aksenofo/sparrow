/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "server.h"
#include "client.h"

#include "log.h"

#include <format.h>
#include <netinet/in.h>
#include <sslbio.h>
#include <sys/socket.h>
#include <tcp.h>

using namespace sparrow;

std::string sertificate, privateKey;
int portNumber = 55555;


SslInstance::SslInstance(int socket)
{
    m_tcp = std::make_unique<TcpSocket>(socket);

    m_io.set<SslInstance, &SslInstance::OnCallback>(this);
    m_io.start(m_tcp->Socket(), ev::READ);

    SslContext ctx(SSLv23_method());
    ctx.UseCertificateFile(sertificate, SSL_FILETYPE_PEM);
    ctx.UsePrivateKeyFile(privateKey, SSL_FILETYPE_PEM);
    ctx.CheckPrivateKey();
    ctx.SetOptions(SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3);

    SslBase base(ctx);
    base.SetAcceptState();
    base.SetBio(SslBio(), SslBio());
    m_handler = std::make_unique<SslHandler>(base);
}

void SslInstance::OnCallback(ev::io& watcher, int revents)
{
    bool write = revents & EV_WRITE;
    bool read = revents & EV_READ;
    bool rc = m_handler->Handle(m_sendBuffer, m_recvBuffer, watcher.fd, write, read);

    while(!m_recvBuffer.IsEmpty()) {
        auto v = m_recvBuffer.Get();
        if(v != static_cast<uint8_t>(m_clientToServerByteCounter)) {
            throw std::runtime_error(format("Invalid value. Imcoming %1, expected: %2", v, static_cast<uint8_t>(m_clientToServerByteCounter)));
        }
        m_clientToServerByteCounter ++;
    }

    if (rc)
        watcher.start(m_tcp->Socket(), (read ? ev::READ : 0) | (write ? ev::WRITE : 0));
    else {
        watcher.stop();
        LOG(Debug) << format("Connection closed for socket #%1", watcher.fd);
    }

    return;
}

SslServer::SslServer()
{
    m_tcp = std::make_unique<TcpListener>(portNumber, 5);
    m_io.set<SslServer, &SslServer::OnAccept>(this);
    m_io.start(m_tcp->Socket(), ev::READ);

     LOG(Always) << sparrow::format("Server is started.");
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
