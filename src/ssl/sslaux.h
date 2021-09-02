/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#pragma once

#include <string>

namespace sparrow
{
/*! \brief SslAux is ssl auxillary class.
 *
 *  SslAux is ssl auxillary class.
 */
class SslAux
{
    constexpr static size_t kErrBufferSize = 2048; /*!< buffer size for single error string */

public:
    //! Get last error stack.
    /*!
      \param Delimier is used after every single string in error stack. If this is == 0 than there is no delimiter at all.
      \return Error stack in printable format. 
    */
    static std::string GetLastErrorText(char delimiter = '\n');

    //! Verify if parameter is 1.
    /*!
      \param Validate if parameter is 1, otherwise throw an exception. 
    */
    static void CheckIf_1(const int rc);

    //! Verify if parameter is no null.
    /*!
      \param Validate if parameter is not null, otherwise throw an exception. 
    */
    static void CheckIfNullptr(const void* ptr);
};

} //namespace sparrow
