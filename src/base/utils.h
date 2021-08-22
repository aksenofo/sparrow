/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#pragma once

#include <algorithm>
#include <cctype>
#include <chrono>
#include <functional>
#include <locale>
#include <system_error>
#include <thread>

#define COPYBLE_DEFAULT(clazz) \
    clazz(const clazz&) = default; \
    clazz& operator=(const clazz&) = default;

#define NOCOPIBLE(clazz) \
    clazz(const clazz&) = delete; \
    clazz& operator=(const clazz&) = delete;

#define MOVEBLE_DEFAULT(clazz) \
    clazz(clazz&&) noexcept = default; \
    clazz& operator=(clazz&&) noexcept = default;

namespace sparrow
{

std::string ToIso8601(const std::chrono::time_point<std::chrono::system_clock>& t);

/*
*
*/
class ScopeDeleter
{
public:
    NOCOPIBLE(ScopeDeleter);
    MOVEBLE_DEFAULT(ScopeDeleter);
    ScopeDeleter() = default;

    ScopeDeleter(std::function<void()> fn)
    : m_fn(fn)
    {
    }

    ~ScopeDeleter()
    {
        if (m_fn)
            m_fn();
    }

private:
    std::function<void()> m_fn;
};

inline std::error_code ToECode(int errno_code)
{
    return std::make_error_code(static_cast<std::errc>(errno_code));
}


// trim from start (in place)
static inline void ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        if (ch < 0)
            return false;
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        if (ch < 0)
            return false;
        return !std::isspace(ch);
    }).base(),
        s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s)
{
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s)
{
    ltrim(s);
    return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s)
{
    rtrim(s);
    return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s)
{
    trim(s);
    return s;
}

} // namespace sparrow
