/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include <iomanip>
#include <sstream>
#include <utils.h>

namespace sparrow
{

inline std::string ToIso8601(std::chrono::time_point<std::chrono::system_clock> t)
{
    auto epoch_seconds = std::chrono::system_clock::to_time_t(t);
    std::stringstream sstream;
    sstream << std::put_time(gmtime(&epoch_seconds), "%FT%T");
    auto truncated = std::chrono::system_clock::from_time_t(epoch_seconds);
    auto delta_us = std::chrono::duration_cast<std::chrono::microseconds>(t - truncated).count();
    sstream << "." << std::fixed << std::setw(6) << std::setfill('0') << delta_us;
    return sstream.str();
}

} // namespace sparrow
