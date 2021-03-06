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
#include "Logger.hpp"

enum logType
{
    DEBUG,
    INFO,
    WARN,
    ERR,
    TRACE
};

class LOG
{
    public:
    LOG() = default;

    /**
     * \brief LOG constructor, Save log level
     * \param type : log level
    **/
    explicit LOG(logType type)
    {
        msgLevel = type;
    }

    /**
     * \brief LOG destructor : print
     *
     * call logging to print saved buffer
    **/
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

    /**
     * \brief operator << add msg to current buffer
    **/
    template<class T>
    LOG &operator<<(const T &msg)
    {
        buf << msg;
        return *this;
    }

    private:
    logType msgLevel = DEBUG;
    std::ostringstream buf;
};

#endif //ZIA_SERVER_LOG_HPP
