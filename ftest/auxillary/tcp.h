/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#pragma once

#include <utils.h>

class TcpBase
{
public:
    TcpBase();

    TcpBase(int socket);

    MOVEBLE_DEFAULT(TcpBase)

    uint32_t Socket() const;

protected:
    static int SetNonblock(int fd);
    static void SocketClose(void* ptr);

    std::unique_ptr<void, decltype(&TcpBase::SocketClose)> m_socket =
        std::unique_ptr<void, decltype(&TcpBase::SocketClose)>(nullptr, &TcpBase::SocketClose);
};

class TcpSocket : public TcpBase
{

public:
    TcpSocket() = default;
    virtual ~TcpSocket() = default;

    MOVEBLE_DEFAULT(TcpSocket)

    TcpSocket(const std::string& url, int port);

    TcpSocket(int socket)
    : TcpBase(socket)
    {
    }
};

class TcpListener : public TcpBase
{

public:
    MOVEBLE_DEFAULT(TcpListener)
    virtual ~TcpListener() = default;

    TcpListener(int port, int backlog = 5);
};

