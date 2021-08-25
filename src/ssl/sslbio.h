/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#pragma once

#include <openssl/bio.h>
#include <sslaux.h>
#include <utils.h>

namespace sparrow
{

class SslBio : public SslAux
{

public:
    MOVEBLE_DEFAULT(SslBio);
    virtual ~SslBio() = default;

    SslBio(const BIO_METHOD* type);
    SslBio(BIO* bio);
    SslBio(const SslBio& bio);
    SslBio& operator=(const SslBio& bio);

    BIO* BioPtr() { return m_bio.get(); }
    const BIO* BioPtr() const { return m_bio.get(); }

    bool HasObj() const { return !!m_bio; }

private:
    static void Deleter(BIO* p) noexcept;
    std::unique_ptr<BIO, std::function<void(BIO*)>> m_bio;
};

inline void SslBio::Deleter(BIO* p) noexcept
{
    if (p)
        BIO_free(p);
}

} //namespace sparrow
