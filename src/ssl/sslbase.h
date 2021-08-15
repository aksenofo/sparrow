/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#pragma once

#include "sslctx.h"

#include <sslaux.h>
#include <string>
#include <utils.h>

namespace sparrow
{

class SslBase : public SslAux
{

public:
    SslBase() = default;
    MOVEBLE_DEFAULT(SslBase)
    SslBase(SslContext& ctx, bool serverFlag);

    virtual ~SslBase() = default;

private:
    static void DeleterSsl(void* p);

    std::unique_ptr<void, std::function<void(void*)>> m_ssl;
    void *m_rbio, *m_wbio;
};

} //namespace sparrow
