/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#pragma once

#include <string>

namespace sparrow
{

class SslAux
{
public:
    static std::string GetLastErrorText();
    void CheckIf_1(int rc);
    void CheckIfNullptr(void* ptr);
};

} //namespace sparrow
