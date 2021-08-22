/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#pragma once

#include "sslctx.h"

#include <circlebuffer.h>
#include <sslaux.h>
#include <string>
#include <utils.h>

namespace sparrow
{

class SslBase : public SslAux
{
    const size_t kEncBufferSize = 32;

public:
    SslBase() = default;
    MOVEBLE_DEFAULT(SslBase)
    SslBase(SslContext& ctx, bool serverFlag);

    virtual ~SslBase() = default;

private:
    static void DeleterSsl(void* p);
    static void SockSend(int sock, CircularBuffer& cb);
    static bool SockRecv(int sock, CircularBuffer& cb);

    std::unique_ptr<void, std::function<void(void*)>> m_ssl;
    void *m_rbio, *m_wbio;

    CircularBuffer m_sendEncBuffer = CircularBuffer(kEncBufferSize);
    CircularBuffer m_recvEncBuffer = CircularBuffer(kEncBufferSize);
};

} //namespace sparrow
