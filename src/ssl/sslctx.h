
#pragma once

#include <utils.h>

namespace mchs {

class SslContext {
public:
    SslContext();

    NOCOPIBLE(SslContext)

    void Load(const std::string& certfile, const std::string& keyfile);
    
    static void Deleter(void* p);

    std::string GetLastErrorText();

    const void* Ctx() const { return m_ctx.get(); }
    void* Ctx() { return m_ctx.get(); }

private:
    void CreateSsl(bool serverFlag);

private:
    std::unique_ptr<void, std::function<void(void*)>> m_ctx;

}; 

}//namespace mchs {
