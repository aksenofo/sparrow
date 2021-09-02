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

/*! \brief SslBio is BIO wrapper.
 *
 *  SslBio is BIO wrapper class.
 */
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

    //! Read() attempts to read len bytes from BIO b and places the data in buf.
    /*!
      \param buf - is a pointer of buffer
      \param len - is a size if buffer
      \return This function returns the amount of data successfully read (if the return value is positive) 
              or that no data was successfully read if the result is 0 or -1. 
              If the return value is -2 then the operation is not implemented in the specific BIO type.
    */
    int Read(void* buf, int len) const noexcept;

    //! Gets() performs the BIOs "gets" operation and places the data in buf. 
    /*!
        Usually this operation will attempt to read a line of data from the BIO of maximum length len. 
        There are exceptions to this however, for example Gets() on a digest BIO will 
        calculate and return the digest and other BIOs may not support Gets() at all.

      \param buf - is a pointer of buffer
      \param len - is a size if buffer
      \return This function returns the amount of data successfully read (if the return value is positive) 
              or that no data was successfully read if the result is 0 or -1. 
              If the return value is -2 then the operation is not implemented in the specific BIO type.
    */
    int Gets(char* buf, int size) const noexcept;

    //! Write() attempts to write len bytes from buf to BIO b.
    /*!
      \param buf - is a pointer of buffer
      \param len - is a size if buffer
      \return This function returns the amount of data successfully written (if the return value is positive) 
              or that no data was successfully written if the result is 0 or -1. 
              If the return value is -2 then the operation is not implemented in the specific BIO type.
    */
    int Write(const void* buf, int len) const noexcept;

    //! Puts() attempts to write a null terminated string buf to BIO b.
    /*!
      \param buf - is a pointer of buffer
      \param len - is a size if buffer
      \return This function returns the amount of data successfully written (if the return value is positive) 
              or that no data was successfully written if the result is 0 or -1. 
              If the return value is -2 then the operation is not implemented in the specific BIO type.
    */
    int Puts(const char* buf) const noexcept;

    //! ShouldRead() is true if the cause of the condition is that a BIO needs to read data.
    /*!
      \return true if the cause of the condition is that a BIO needs to read data.
    */
    bool ShouldRead() const noexcept;

    //! ShouldWrite() is true if the cause of the condition is that a BIO needs to read data.
    /*!
      \return true if the cause of the condition is that a BIO needs to read data.
    */
    bool ShouldWrite() const noexcept;

    //! ShouldIoSpecial() is true if some "special" condition.
    /*!
      \return true if some "special" condition, that is a reason other than reading 
              or writing is the cause of the condition.
    */
    bool ShouldIoSpecial() const noexcept;
  
    //! RetryType() returns a mask of the cause of a retry condition.
    /*!
      \return a mask of the cause of a retry condition consisting of the 
              values BIO_FLAGS_READ, BIO_FLAGS_WRITE, BIO_FLAGS_IO_SPECIAL though current BIO types 
              will only set one of these.
    */
    int RetryType() const noexcept;

    //! ShouldRetry() is true if the call that produced this condition should then be retried at a later time.
    /*!
      \return is true if the call that produced this condition should then be retried at a later time.
    */
    bool ShouldRetry() const noexcept;

    //! Compare two BIO objects
    /*!
      \return true if this is the same objects.
    */
    bool operator==(const SslBio& bio) const noexcept;

    //! Compare two BIO objects
    /*!
      \return true if this is not the same objects.
    */
    bool operator!=(const SslBio& bio) const noexcept;

private:
    //! Internal BIO deleter
    /*!
      \param p - pointer to BIO object.
    */
    static void Deleter(BIO* p) noexcept;

    std::unique_ptr<BIO, std::function<void(BIO*)>> m_bio; /*!< unique pointer to BIO object */
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
