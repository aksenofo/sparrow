/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include <ev++.h>
#include <utils.h>
#include <tcp.h>

class SslClient
{
public:
    SslClient();
    NOCOPIBLE(SslClient);

private:
    void OnCallback(ev::io& watcher, int revents) noexcept;

    ev::io m_io;
    std::unique_ptr<TcpSocket> m_tcp;

};