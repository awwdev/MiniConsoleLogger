//awwdev 2019 https://github.com/awwdev
//C++17

#include "Logger.h"

#ifdef LOG_WINDOWS
#   include <Windows.h>
#endif

#include <time.h>
#include <iomanip>

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

    inline void printThreadName()
    {
        setColor(g_consoleColorWindows);
        std::cout << std::setw(7) << g_threadName.substr(0, 7) << " ";
    }

    inline void printTime()
    {
        time_t rawtime;
        struct tm timeinfo;
        time(&rawtime);
        localtime_s(&timeinfo, &rawtime);
        std::cout 
            << std::right
            << std::setw(2) << timeinfo.tm_hour << ":"
            << std::setw(2) << timeinfo.tm_min  << ":"
            << std::setw(2) << timeinfo.tm_sec  << " "
            << std::left;
    }

    inline void printSource(const SourceFile& src)
    {
        std::cout 
            << std::setw(12) << src.getName().substr(0, 12)
            << ":" 
            << std::right << std::setw(4) << src.line
            << "] " << std::left;
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

        std::cout << std::left;
        printThreadName();
        printTime();
        printSource(src);
        printLevel(lvl);

        size_t fmtPos1 = 0;
        size_t parPos1 = 0;
        size_t fmtPos2 = fmt.find("{}");
        size_t parPos2 = par.find(PARAMS_DELIMITER);

        //nothing to formate
        if (fmtPos2 == std::string_view::npos || parPos2 == std::string_view::npos)
        {
            std::cout << fmt << std::endl;
            return;
        }

        while (fmtPos2 != std::string_view::npos && parPos2 != std::string_view::npos)
        {
            std::cout << fmt.substr(fmtPos1, fmtPos2 - fmtPos1);
            std::cout << par.substr(parPos1, parPos2 - parPos1);

            fmtPos1 = fmtPos2 + 2;
            parPos1 = parPos2 + 2;

            fmtPos2 = fmt.find("{}", fmtPos2 + 1);
            parPos2 = par.find(PARAMS_DELIMITER, parPos2 + 1);
        }
        std::cout << fmt.substr(fmtPos1, fmt.length() - fmtPos1);
        std::cout << std::endl;
    }
}