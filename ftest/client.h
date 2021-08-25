/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include <ev++.h>
#include <utils.h>
#include <tcp.h>
#include <sslclient.h>

class SslClient
{
public:
    SslClient();
    NOCOPIBLE(SslClient);

private:
    void OnCallback(ev::io& watcher, int revents);

    ev::io m_io;
    std::unique_ptr<TcpSocket> m_tcp;
//    std::unique_ptr<sparrow::SslClient> m_sslClient;
};