/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#pragma once

#include <atomic>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <singletone.h>
#include <sstream>
#include <string>
#include <utils.h>

namespace sparrow
{

using log_level_t = uint8_t;

/*!
     \brief Log level 

    Used for restrict log level                  
 */

enum class LogLevel : log_level_t {
    Nolog = 0x00,
    Trace = 0x01,
    Debug = 0x02,
    Info = 0x04,
    Warning = 0x08,
    Error = 0x10,
    Panic = 0x20,
    Always = 0x40,

};


/*!
     \brief LoggerFactory tmp 

    Not implemented correctly                  
 */

class LoggerFactory
{
public:
    LoggerFactory()
    {
        m_logLevel = static_cast<log_level_t>(LogLevel::Nolog);
    }

    template<typename Args>
    void SetLevel(Args lt)
    {
        m_logLevel |= static_cast<log_level_t>(lt);
    }

    template<typename T, typename... Args>
    void SetLevel(T first, Args... args)
    {
        SetLevel(first);
        SetLevel(args...);
    }

    //! Verify if log level has been set
    bool IsLevel(const LogLevel& level) const
    {
        return (m_logLevel & static_cast<log_level_t>(level)) == static_cast<log_level_t>(level);
    }

    //! Print log
    void Print(const std::stringstream& ss)
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        PrintSave(ss);
    }

    void SetPrintTimeStamp(bool flag = true)
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        m_printTimestamp = flag;
    }

    void SetAppPrefix(const char* argv0)
    {
        m_prefixText = GetFilename(argv0);
        m_prefixText += ":";
    }

private:
    std::string GetFilename(const char* argv0);

    //! Print exectry to ...
    virtual void PrintSave(const std::stringstream& ss);

    std::mutex m_mutex;                  //! Garding mutex
    std::atomic<log_level_t> m_logLevel; // = static_cast<log_level_t>(LogLevel::Nolog); //! log level of application

    bool m_printTimestamp = false;

    std::string m_prefixText;
};


/*!
     \brief Logger itself 

    Not public class                  
 */

class Logger
{

    static const char* LogLevelStr(const LogLevel& logLevel);

public:
    MOVEBLE_DEFAULT(Logger);

    Logger(const LogLevel& level, const char* funcName = nullptr);

    template<typename Type>
    Logger& operator<<(const Type& val)
    {
        std::lock_guard<std::mutex> guard(*m_mutex);
        *m_stringstream << val;
        return *this;
    }

    Logger& operator<<(const std::string& val)
    {
        std::lock_guard<std::mutex> guard(*m_mutex);
        *m_stringstream << val;
        return *this;
    }

    Logger& operator<<(const char* val)
    {
        std::lock_guard<std::mutex> guard(*m_mutex);
        *m_stringstream << val;
        return *this;
    }

    Logger& operator<<(std::ios_base& (*val)(std::ios_base&))
    {
        std::lock_guard<std::mutex> guard(*m_mutex);
        *m_stringstream << val;
        return *this;
    }

    virtual ~Logger()
    {
        auto& logger = Singletone<LoggerFactory>::Instance();
        std::lock_guard<std::mutex> guard(*m_mutex);
        logger.Print(*m_stringstream);
    }

private:
    std::unique_ptr<std::mutex> m_mutex;
    std::unique_ptr<std::stringstream> m_stringstream;
};

Logger WrapLogger(const LogLevel& level, const char* funcname);


#define LOG(TYPE) \
    if (sparrow::Singletone<sparrow::LoggerFactory>::Instance().IsLevel(sparrow::LogLevel::TYPE)) \
    WrapLogger(sparrow::LogLevel::TYPE, __FUNCTION__)



} // namespace sparrow
