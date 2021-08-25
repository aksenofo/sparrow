/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */
#include "sslbio.h"

#include <functional>

namespace sparrow
{

SslBio::SslBio(const BIO_METHOD* type)
{
    BIO* bio = BIO_new(type);
    m_bio = std::unique_ptr<BIO, std::function<void(BIO*)>>(bio, &SslBio::Deleter);
    CheckIfNullptr(m_bio.get());
}

SslBio::SslBio(BIO* bio)
{
    m_bio = std::unique_ptr<BIO, std::function<void(BIO*)>>(bio, &SslBio::Deleter);
    CheckIfNullptr(m_bio.get());
}

SslBio::SslBio(const SslBio& bio)
{
    BIO* tbio = const_cast<BIO*>(bio.BioPtr());
    BIO_up_ref(tbio);
    m_bio = std::unique_ptr<BIO, std::function<void(BIO*)>>(tbio, &SslBio::Deleter);
    CheckIfNullptr(m_bio.get());
}

SslBio& SslBio::operator=(const SslBio& bio)
{
    BIO* tbio = const_cast<BIO*>(bio.BioPtr());
    BIO_up_ref(tbio);
    m_bio = std::unique_ptr<BIO, std::function<void(BIO*)>>(tbio, &SslBio::Deleter);
    CheckIfNullptr(m_bio.get());
    return *this;
}

int SslBio::Read(void* buf, int len)
{
    return BIO_read(BioPtr(), buf, len);
}

int SslBio::Gets(char* buf, int size)
{
    return BIO_gets(BioPtr(), buf, size);
}

int SslBio::Write(const void* buf, int len)
{
    return BIO_write(BioPtr(), buf, len);
}

int SslBio::Puts(const char* buf)
{
    return BIO_puts(BioPtr(), buf);
}

} //namespace sparrow
