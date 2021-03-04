/**
 * \file Log.hpp
 * \brief Display macros
 * \author Cyprien.R
**/

#ifndef ZIA_SERVER_LOG_HPP
#define ZIA_SERVER_LOG_HPP


#include <string>
#include <iostream>
#include <mutex>
#include <thread>
#include "log_test.hpp"

#define BLUE 9
#define GREEN 10
#define RED 12
#define YELLOW 14

#ifdef _WIN32

#define LOG_RED(MESSAGE) {\
    HANDLE hConsole; \
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE); \
    FlushConsoleInputBuffer(hConsole); \
    SetConsoleTextAttribute(hConsole, RED); \
    std::cout << MESSAGE << std::endl; \
    SetConsoleTextAttribute(hConsole, 7);      \
    }

#define LOG_GREEN(MESSAGE) {\
    HANDLE hConsole; \
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE); \
    FlushConsoleInputBuffer(hConsole); \
    SetConsoleTextAttribute(hConsole, GREEN); \
    std::cout << MESSAGE << std::endl; \
    SetConsoleTextAttribute(hConsole, 7);      \
    }

#define LOG_BLUE(MESSAGE) {\
    HANDLE hConsole; \
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE); \
    FlushConsoleInputBuffer(hConsole); \
    SetConsoleTextAttribute(hConsole, BLUE); \
    std::cout << MESSAGE; \
    SetConsoleTextAttribute(hConsole, 7);      \
    }

#define LOG_BLUE_WN(MESSAGE) {\
    HANDLE hConsole; \
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE); \
    FlushConsoleInputBuffer(hConsole); \
    SetConsoleTextAttribute(hConsole, BLUE); \
    std::cout << MESSAGE; \
    SetConsoleTextAttribute(hConsole, 7);      \
    }

#define LOG_YELLOW(MESSAGE) {\
    HANDLE hConsole; \
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE); \
    FlushConsoleInputBuffer(hConsole); \
    SetConsoleTextAttribute(hConsole, YELLOW); \
    std::cout << MESSAGE << std::endl; \
    SetConsoleTextAttribute(hConsole, 7);      \
    }

#else

#define LOG_RED(MESSAGE) {std::cout << "\033[1;31m" << MESSAGE << "\033[0m" << std::endl;}
#define LOG_GREEN(MESSAGE) {std::cout << "\033[1;32m" << MESSAGE <<  "\033[0m" << std::endl;}
#define LOG_BLUE(MESSAGE) {std::cout << "\033[94m" << MESSAGE << "\033[0m" << std::endl;}
#define LOG_BLUE_WN(MESSAGE) {std::cout << "\033[94m" << MESSAGE << "\033[0m";}
#define LOG_YELLOW(MESSAGE) {std::cout << "\033[103m" << MESSAGE << "\033[0m";}


#endif


enum logType
{
    DEBUG,
    INFO,
    WARN,
    ERR, // ERROR already used as macro in .h windows files
    TRACE
};

typedef struct structLog_s
{
    bool headers = false; // FIXME if true
    logType level = DEBUG; // FIXME if debug
} structLog;

// extern structLog LOG_CFG;

class LOG
{
    public:
    LOG() = default;

    explicit LOG(logType type)
    {
        msgLevel = type;
        //if (LOG_CFG.headers) {
        //    operator<<("[" + getLabel(type) + "]");
        //}
    }

    ~LOG() {
        if (msgLevel == ERR)
            logging::ERR(buf.str());
        else if (msgLevel == WARN)
            logging::WARN(buf.str());
        else if (msgLevel == INFO)
            logging::INFO(buf.str());
        else if (msgLevel == DEBUG)
            logging::DEBUG(buf.str());
        else
            logging::TRACE(buf.str());
    }

    template<class T>
    LOG &operator<<(const T &msg)
    {
        buf << msg;
        return *this;
    }

    private:
    logType msgLevel = DEBUG;
    std::ostringstream buf;

    structLog LOG_CFG;
};

#endif //ZIA_SERVER_LOG_HPP
