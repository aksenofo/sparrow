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

class SslAux
{
public:
    static std::string GetLastErrorText();
};

} //namespace sparrow