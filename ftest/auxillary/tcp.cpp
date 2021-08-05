/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "tcp.h"

#include <arpa/inet.h>
#include <assert.h>
#include <format.h>
#include <log.h>
#include <netinet/in.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace sparrow
{

int TcpBase::SetNonblock(int fd)
{
    int flags;

    flags = fcntl(fd, F_GETFL);
    flags |= O_NONBLOCK;
    return fcntl(fd, F_SETFL, flags);
}

void TcpBase::SocketClose(void* ptr)
{
    int tsock = reinterpret_cast<intptr_t>(ptr);
    LOG(Debug) << sparrow::format("Socket #%1 closed.", tsock);
    shutdown(tsock, SHUT_RDWR);
    close(tsock);
}

uint32_t TcpBase::Socket() const
{
    assert(m_socket);
    return reinterpret_cast<intptr_t>(m_socket.get());
}

TcpBase::TcpBase()
{
    m_socket.reset(reinterpret_cast<void*>(socket(AF_INET, SOCK_STREAM, 0)));
    if (Socket() == -1) {
        throw std::runtime_error(sparrow::format("Cannot create socket. Code %1.", sparrow::ToECode(errno)));
    }

    int err = SetNonblock(Socket());
    if (err != 0) {
        throw std::runtime_error(sparrow::format("Failed to fcntl in client. Code %1.", sparrow::ToECode(errno)));
    }
}

TcpBase::TcpBase(int socket)
{
    m_socket.reset(reinterpret_cast<void*>(socket));
}

TcpSocket::TcpSocket(const std::string& url, int port)
: TcpBase()
{
    struct sockaddr_in sockerAddr;
    sockerAddr.sin_family = AF_INET;
    sockerAddr.sin_port = htons(port);
    sockerAddr.sin_addr.s_addr = inet_addr(url.c_str());
    memset(sockerAddr.sin_zero, '\0', sizeof(sockerAddr.sin_zero));
    int err = connect(Socket(), (struct sockaddr*)&sockerAddr, sizeof(sockerAddr));
    if (err != 0 && errno == EINPROGRESS) {
        err = 0;
        errno = 0;
        LOG(Debug) << sparrow::format("Socket #%1 connection is in progress.", Socket());
    } else if (err == 0) {
        LOG(Debug) << sparrow::format("Socket #%1 connected.", Socket());
    } else {
        std::string errorStr = sparrow::format("Failed to connect to server. Code %1.", sparrow::ToECode(errno));
        throw std::runtime_error(errorStr);
    }
}

TcpListener::TcpListener(int port, int backlog)
: TcpBase()
{
    struct sockaddr_in sockerAddr;
    sockerAddr.sin_family = AF_INET;
    sockerAddr.sin_port = htons(port);
    sockerAddr.sin_addr.s_addr = INADDR_ANY;
    memset(sockerAddr.sin_zero, '\0', sizeof(sockerAddr.sin_zero));
    int optval = 1;
    int err = setsockopt(reinterpret_cast<intptr_t>(m_socket.get()), SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));
    if (err != 0)
        throw std::runtime_error(sparrow::format("Failed to setsockopt(SOL_SOCKET, SO_REUSEADDR). Code %1.", sparrow::ToECode(errno)));

    err = bind(reinterpret_cast<intptr_t>(m_socket.get()), (struct sockaddr*)&sockerAddr, sizeof(sockerAddr));
    if (err != 0)
        throw std::runtime_error(sparrow::format("Failed to bind socket. Code %1.", sparrow::ToECode(errno)));
    err = listen(reinterpret_cast<intptr_t>(m_socket.get()), backlog);
    if (err < 0)
        throw std::runtime_error(sparrow::format("Failed to setsockopt(SOL_SOCKET, SO_REUSEADDR). Code %1.", sparrow::ToECode(errno)));
}

} // namespace sparrow
