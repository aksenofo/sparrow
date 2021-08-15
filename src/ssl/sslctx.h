/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#pragma once

#include <sslaux.h>
#include <utils.h>

namespace sparrow
{

class SslContext : public SslAux
{
public:
    SslContext();

    NOCOPIBLE(SslContext)

    void Load(const std::string& certfile, const std::string& keyfile);

    static void Deleter(void* p);

    const void* Ctx() const { return m_ctx.get(); }
    void* Ctx() { return m_ctx.get(); }

private:
    void CreateSsl(bool serverFlag);

private:
    std::unique_ptr<void, std::function<void(void*)>> m_ctx;
};

} // namespace sparrow
