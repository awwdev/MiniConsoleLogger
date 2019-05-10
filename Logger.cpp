//awwdev 2019 https://github.com/awwdev
//C++17

#include "Logger.h"

#ifdef LOG_WINDOWS
#   include <Windows.h>
#endif

#include <time.h>

namespace mini::logger
{
    thread_local std::string g_threadName;
    std::mutex g_logMutex;
    thread_local ConsoleColorWindows g_consoleColorWindows { ConsoleColorWindows::White };

    inline void setColor(const ConsoleColorWindows col)
    {
    #ifdef LOG_WINDOWS
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, col);
    #endif
    }

    inline void printThreadName(const size_t fixedSize = 7u)
    {
        setColor(g_consoleColorWindows);
        auto str = g_threadName;
        str.resize(fixedSize);
        std::cout << str << " ";
    }

    inline void printTime(const size_t fixedSize = 9u)
    {
        time_t rawtime;
        struct tm timeinfo;
        time(&rawtime);
        localtime_s(&timeinfo, &rawtime);
        std::string str = 
            //std::to_string(timeinfo.tm_mday) + "/" + 
            //std::to_string(timeinfo.tm_mon   + 1) + "/" +
            //std::to_string(timeinfo.tm_year  + 1900) + " " +
            std::to_string(timeinfo.tm_hour) + ":" +
            std::to_string(timeinfo.tm_min)  + ":" + 
            std::to_string(timeinfo.tm_sec);
        str.resize(fixedSize);
        std::cout << str;
    }

    inline void printSource(const SourceFile& src)
    {
        std::string str = "[" + src.getName(12u) + ":" + src.getLine(4u);
        str.resize(18u);
        str += "] ";
        std::cout << str;
    }

    inline void printLevel(const Level lvl)
    {
        switch (lvl)
        {
            case Level::Err:  setColor(Red);     std::cout << "[ERR ] "; break;
            case Level::Warn: setColor(Yellow);  std::cout << "[WARN] "; break;
            case Level::Info: setColor(Green);   std::cout << "[INFO] "; break;
        }
        setColor(g_consoleColorWindows);
    }

    void log(const SourceFile src, const Level lvl, std::string_view fmt, std::string_view par)
    {
        std::lock_guard<std::mutex> lock(g_logMutex);

        printThreadName();
        printTime();
        printSource(src);
        printLevel(lvl);

        size_t fmtPos1 = 0;
        size_t parPos1 = 0;

        size_t fmtPos2 = fmt.find("{}");
        size_t parPos2 = par.find(",");

        //nothing to formate
        if (fmtPos2 == std::string_view::npos || parPos2 == std::string_view::npos)
        {
            std::cout << fmt << std::endl;
            return;
        }

        do
        {
            std::cout << fmt.substr(fmtPos1, fmtPos2 - fmtPos1);
            std::cout << par.substr(parPos1, parPos2 - parPos1);

            fmtPos1 = fmtPos2 + 2;
            parPos1 = parPos2 + 1;

            fmtPos2 = fmt.find("{}", fmtPos2 + 1);
            parPos2 = par.find(",",  parPos2 + 1);
        }
        while (fmtPos2 != std::string_view::npos && parPos2 != std::string_view::npos);
        std::cout << fmt.substr(fmtPos1, fmt.length() - fmtPos1);
        
        std::cout << std::endl;
    }
}