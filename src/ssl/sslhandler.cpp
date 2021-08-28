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
    int nRead = 0;
    int nWrite = 0;

    SslBio rbio = m_ssl.Rbio();

    do {
        if (cb.IsFull()) {
            throw std::runtime_error(format("Read buffer has no space. Capacity: %1", cb.Capacity()));
        }
        Populator populator;
        nRead = populator(cb, [this](uint8_t* buf, size_t size) {
            int nRead = m_ssl.Read(buf, size);
            if (!m_ssl.IsAcceptableReturn(nRead, SSL_ERROR_WANT_READ))
                throw std::runtime_error(format("Cannot SSL_read. %1", GetLastErrorText()));
            return nRead;
        });

        Consumer consumer;
        nWrite = consumer(m_encRecvBuffer, [&rbio, this](const uint8_t* buf, size_t size) {
            int nWrite = rbio.Write(buf, size);
            if (!m_ssl.IsAcceptableReturn(nWrite, SSL_ERROR_WANT_WRITE))
                throw std::runtime_error(format("Cannot BIO_write. %1", GetLastErrorText()));
            return nWrite;
        });

    } while (nRead && nWrite);
}

bool SslHandler::Encrypt(CircularBuffer& cb)
{
    SslBio wbio = m_ssl.Wbio();
    Consumer consumer;
    Populator populator;

    do {
        consumer(cb, [this](const uint8_t* ptr, size_t size) {
            int nWrite = m_ssl.Write(ptr, size);
            if (!m_ssl.IsAcceptableReturn(nWrite, 0))
                throw std::runtime_error(format("Cannot SSL_write. %1", GetLastErrorText()));

            return nWrite;
        });

        if (!m_ssl.IsInitFinished()) // TODO
            throw std::runtime_error(format("Needs to re-negotiate. Not implemented"));

        populator(m_encSendBuffer, [&wbio](uint8_t* ptr, size_t size) {
            int nRead = wbio.Read(ptr, size);
            if (nRead == -1 && !wbio.ShouldRetry())
                throw std::runtime_error(format("Cannot BIO_read. %1", GetLastErrorText()));

            return nRead;
        });

    } while (!cb.IsEmpty() && !m_encSendBuffer.IsFull());
    
    return !m_encSendBuffer.IsFull();
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
