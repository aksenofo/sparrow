/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include <format.h>
#include <sslbio.h>
#include <sslhandler.h>
#include <unistd.h>

namespace sparrow
{

SslHandler::SslHandler(const SslBase& ssl)
: m_ssl(ssl)
{
}

SslHandler::SslHandler(SSL* ssl)
: m_ssl(ssl)
{
}

bool SslHandler::Handle(CircularBuffer& sendBuf, CircularBuffer& recvBuf, int socket, bool& write, bool& read)
{
    if (!m_ssl.IsInitFinished())
        if (!DoHandshake(socket, write, read))
            return false;

    if (m_ssl.IsInitFinished())
        return DoDataExchande(socket, sendBuf, recvBuf, write, read);
    else
        return true;
}

bool SslHandler::DoDataExchande(int socket, CircularBuffer& sendBuf, CircularBuffer& recvBuf, bool& write, bool& read)
{
    return true;
}

bool SslHandler::DoHandshake(int socket, bool& write, bool& read)
{
    int n = m_ssl.DoHandshake();
    if (!m_ssl.IsAcceptableReturn(n, SSL_ERROR_WANT_READ)) {
        throw std::runtime_error(format("Cannot SSL_do_handshake. %1", m_ssl.GetLastErrorText()));
    }
    bool needRead = m_ssl.IsCode(n, SSL_ERROR_WANT_READ);
    if (needRead) {
        const SslBio wbio = m_ssl.Wbio();
        Populator populator;
        populator(m_encSendBuffer, [&wbio](uint8_t* ptr, size_t size) {
            return wbio.Read(ptr, size);
        });
    }
    SockSend(socket, m_encSendBuffer);
    bool socketNotClosed = true;
    uint32_t consumedNumber = 0;
    if (read) {

        socketNotClosed = SockRecv(socket, m_encRecvBuffer);

        SslBio rbio = m_ssl.Rbio();
        Consumer consumer;
        consumedNumber = consumer(m_encRecvBuffer, [&rbio](const uint8_t* ptr, size_t size) {
            return rbio.Write(ptr, size);
        });
    }

    write = !m_encSendBuffer.IsEmpty() || consumedNumber > 0;
    read = true;
    return socketNotClosed;
}

void SslHandler::Unencrypt(CircularBuffer& cb)
{
    bool needRead = false;
    int nWrite;

    do {
        if (cb.IsFull()) {
            throw std::runtime_error(format("Read buffer has no space. Capacity: %1", cb.Reserved()));
        }

    } while (needRead && nWrite);
}

void SslHandler::SockSend(int sock, CircularBuffer& cb)
{
    Consumer consumer;
    consumer(cb, [&sock](const uint8_t* ptr, const size_t size) {
        return write(sock, ptr, size);
    });
}

bool SslHandler::SockRecv(int sock, CircularBuffer& cb)
{
    Populator populator;
    auto s = populator(cb, [&sock](uint8_t* ptr, const size_t size) {
        return read(sock, ptr, size);
    });
    return s;
}

} //namespace sparrow
