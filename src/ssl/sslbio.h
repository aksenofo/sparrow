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

    SslBio(const BIO_METHOD* type = BIO_s_mem());
    SslBio(BIO* bio);
    SslBio(const SslBio& bio);
    SslBio& operator=(const SslBio& bio);

    BIO* BioPtr() { return m_bio.get(); }
    const BIO* BioPtr() const { return m_bio.get(); }

    bool HasObj() const { return !!m_bio; }

    int Read(void* buf, int len);
    int Gets(char* buf, int size);
    int Write(const void* buf, int len);
    int Puts(const char* buf);

    bool ShouldRead() const noexcept;
    bool ShouldWrite() const noexcept;
    bool ShouldIoSpecial() const noexcept;
    int RetryType() const noexcept;
    bool ShouldRetry() const noexcept;

    bool operator==(const SslBio& bio) const noexcept;
    bool operator!=(const SslBio& bio) const noexcept;

private:
    static void Deleter(BIO* p) noexcept;
    std::unique_ptr<BIO, std::function<void(BIO*)>> m_bio;
};

inline void SslBio::Deleter(BIO* p) noexcept
{
    if (p)
        BIO_free(p);
}

inline bool SslBio::ShouldRead() const noexcept
{
    return BIO_should_read(BioPtr());
}

inline bool SslBio::ShouldWrite() const noexcept
{
    return BIO_should_write(BioPtr());
}

inline bool SslBio::ShouldIoSpecial() const noexcept
{
    return BIO_should_io_special(BioPtr());
}

inline int SslBio::RetryType() const noexcept
{
    return BIO_retry_type(BioPtr());
}

inline bool SslBio::ShouldRetry() const noexcept
{
    return BIO_should_retry(BioPtr());
}

inline bool SslBio::operator==(const SslBio& bio) const noexcept
{
    return m_bio.get() == bio.m_bio.get();
}

inline bool SslBio::operator!=(const SslBio& bio) const noexcept
{
    return m_bio.get() != bio.m_bio.get();
}


} //namespace sparrow
