/**
 * \file Log.hpp
 * \brief Display macros
 * \author Cyprien.R
**/

#ifndef ZIA_SERVER_LOG_HPP
#define ZIA_SERVER_LOG_HPP


#include <string>
#include <iostream>

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
    ERR // ERROR already used as macro in .h windows files
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
        if (LOG_CFG.headers) {
            operator<<("[" + getLabel(type) + "]");
        }
    }

    ~LOG()
    {
        if (opened) {
            std::cout << std::endl;
        }
        opened = false;
    }

    template<class T>
    LOG &operator<<(const T &msg)
    {
        if (msgLevel >= LOG_CFG.level) {
            if (msgLevel == WARN) LOG_YELLOW(msg)
            else if (msgLevel == INFO) LOG_BLUE(msg)
            else if (msgLevel == ERR) LOG_RED(msg)
            else std::cout << msg;
            opened = true;
        }
        return *this;
    }

    private:
    bool opened = false;
    logType msgLevel = DEBUG;

    static inline std::string getLabel(logType type)
    {
        std::string label;
        switch (type) {
            case DEBUG:
                label = "DEBUG";
                break;
            case INFO:
                label = "INFO ";
                break;
            case WARN:
                label = "WARN ";
                break;
            case ERR:
                label = "ERROR";
                break;
        }
        return label;
    }

    structLog LOG_CFG;
};

#endif //ZIA_SERVER_LOG_HPP
