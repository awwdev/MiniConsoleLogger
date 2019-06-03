//awwdev 2019 https://github.com/awwdev
//C++17

#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <mutex>

#define LOG_WINDOWS

namespace mini::logger
{
    enum Level { Info, Err, Warn };
    enum ConsoleColorWindows : unsigned char
    {
        Green   = 0x0A,
        White   = 0x07,
        Yellow  = 0x0E,
        Red     = 0x0C,
        Aqua    = 0x0B,
    };

    extern thread_local std::string         g_threadName;
    extern std::mutex                       g_logMutex;
    extern thread_local ConsoleColorWindows g_consoleColorWindows;

    const char* const PARAMS_DELIMITER { "|*" };
    const struct SourceFile
    {
        // {
        std::string_view path;
        long line;
        // }

        inline auto getName() const -> std::string_view
        {
            return path.substr(path.find_last_of("\\") + 1); 
            //windows path, todo: unix
        }
    };

    template<typename ...Args>
    auto paramsToStr(Args... args) -> std::string
    {
        if constexpr (sizeof...(Args) > 0) {
            std::stringstream sstream;
            int expanderTrick[sizeof...(Args)] = {
                ((sstream << args << PARAMS_DELIMITER), 0) ...
                //most right of comma expression will be assigned to tmp array
                //but everything will be evaluated, so the sstream gets filled 
            };
            return sstream.str();
        }
        return "";
    }

    void log(const SourceFile src, const Level lvl, std::string_view fmt, std::string_view parameters);

}

#ifdef DEBUG
#   define LOG_INFO(fmt, ...) mini::logger::log({ __FILE__, __LINE__}, mini::logger::Info, fmt, mini::logger::paramsToStr(__VA_ARGS__));
#   define LOG_ERR(fmt, ...)  mini::logger::log({ __FILE__, __LINE__}, mini::logger::Err,  fmt, mini::logger::paramsToStr(__VA_ARGS__));
#   define LOG_WARN(fmt, ...) mini::logger::log({ __FILE__, __LINE__}, mini::logger::Warn, fmt, mini::logger::paramsToStr(__VA_ARGS__));
#else
#   define LOG_INFO(fmt, ...) 
#   define LOG_ERR(fmt, ...)
#   define LOG_WARN(fmt, ...)
#endif