#pragma once

#include <chrono>
#include <functional>
#include <thread>
#include <system_error>
#include <algorithm> 
#include <cctype>
#include <locale>


#define CONSTRUCT_DEFAULT(clazz) clazz() = default;

#define COPYBLE_DEFAULT(clazz) \
    clazz(const clazz&) = default; \
    clazz& operator=(const clazz&) = default;

#define NOCOPIBLE(clazz) \
    clazz(const clazz&) = delete; \
    clazz& operator=(const clazz&) = delete;

#define MOVEBLE_DEFAULT(clazz) \
    clazz(clazz&&) noexcept = default; \
    clazz& operator=(clazz&&) noexcept = default;



namespace mchs {

std::string ToIso8601(const std::chrono::time_point<std::chrono::system_clock>& t);

/*
*
*/
class ScopeDeleter
{
public:
    NOCOPIBLE(ScopeDeleter);
    MOVEBLE_DEFAULT(ScopeDeleter);
    CONSTRUCT_DEFAULT(ScopeDeleter)

	ScopeDeleter(std::function<void()> fn)
    : m_fn(fn)
    {}

    ~ScopeDeleter()
    {
        if (m_fn)
			m_fn();
    }

private:
    std::function<void()> m_fn;
};

inline uint64_t NowMicrosecs()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
}

template<typename T>
void ClockForEvery(const uint64_t& period, T fn)
{
    auto now = []() {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = now.time_since_epoch();
        return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    };

    int64_t nextStopTime = now();
    while (fn()) {
        nextStopTime += period;
        int64_t nextDel = nextStopTime - now();
        if (nextDel < 0)
            nextDel = 0;
	
        std::this_thread::sleep_for(std::chrono::microseconds(nextDel));
    }
}

inline std::error_code ToECode(int errno_code) {
  return std::make_error_code(static_cast<std::errc>(errno_code));
}

template <typename T>
T ReverseBits(T n) {
    short bits = sizeof(n) * 8; 
    T mask = ~T(0);
    while (bits >>= 1) {
        mask ^= mask << (bits);
        n = (n & ~mask) >> bits | (n & mask) << bits;
    }

    return n;
}


inline
uint16_t ToBitmack(uint8_t number)
{
    uint16_t rc = 0x1;
    return rc << number;
}

inline uint8_t FromBitmack(uint16_t mack)
{
    for (uint8_t t = 0; t < 10; t++) {
        if ((mack & 0x1) == 0x1)
            return t;
        mack >>= 1;
	}
    return 0;
}

std::string BitsToStr(uint64_t value, size_t size);

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        if (ch < 0)
            return false;
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        if (ch < 0)
            return false;
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}


} // namespace mchs
