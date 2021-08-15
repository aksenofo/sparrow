/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */
#include "client.h"
#include <singletone.h>

SslClient::SslClient()
{   
    m_sslClient = std::make_unique<sparrow::SslClient>(sparrow::Singletone<sparrow::SslContext>::Instance());
    m_tcp = std::make_unique<TcpSocket>("127.0.0.1", 10000);
    m_io.set<SslClient, &SslClient::OnCallback>(this);
    m_io.start(m_tcp->Socket(), ev::WRITE | ev::READ);
}

void SslClient::OnCallback(ev::io& watcher, int revents) noexcept {

}
