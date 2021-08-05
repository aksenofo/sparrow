/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#pragma once

#include <string>
#include <utils.h>

namespace sparrow
{

class SslBase
{

public:

    static std::string GetLastErrorText();
    virtual ~SslBase() = default;

};

} //namespace sparrow
