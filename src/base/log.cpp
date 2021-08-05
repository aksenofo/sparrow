/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "log.h"

namespace sparrow
{

const char* Logger::LogLevelStr(const LogLevel& logLevel)
{
    switch (logLevel) {
    case LogLevel::Nolog: return "NOLOG";
    case LogLevel::Trace: return "TRACE";
    case LogLevel::Debug: return "DEBUG";
    case LogLevel::Info: return "INFO";
    case LogLevel::Warning: return "WARNING";
    case LogLevel::Error: return "ERROR";
    case LogLevel::Panic: return "PANIC";
    case LogLevel::Always: return "***";
    default: return "UNKNOWN-LEVEL";
    }
}

Logger WrapLogger(const LogLevel& level, const char* funcname)
{
    if (level == LogLevel::Trace || level == LogLevel::Debug)
        return Logger(level, funcname);
    else
        return Logger(level);
}


Logger::Logger(const LogLevel& level, const char* funcName)
{
    m_mutex = std::make_unique<std::mutex>();
    m_stringstream = std::make_unique<std::stringstream>();

    std::lock_guard<std::mutex> guard(*m_mutex);
    *m_stringstream << LogLevelStr(level) << ":";
    if (funcName)
        *m_stringstream << funcName << ": ";
    else
        *m_stringstream << " ";
}

std::string LoggerFactory::GetFilename(const char* argv0)
{
    std::string pathname(argv0);

    struct MatchPathSeparator {
        bool operator()(char ch) const
        {
            return ch == '\\' || ch == '/';
        }
    };


    return std::string(
        std::find_if(pathname.rbegin(), pathname.rend(),
            MatchPathSeparator())
            .base(),
        pathname.end());
}

    void LoggerFactory::PrintSave(const std::stringstream& ss)
    {
        if (m_printTimestamp)
            std::cout << sparrow::ToIso8601(std::chrono::system_clock::now()) << ">> " << m_prefixText << ss.str() << std::endl;
        else
            std::cout << m_prefixText << ss.str() << std::endl;
    }

} // namespace sparrow
