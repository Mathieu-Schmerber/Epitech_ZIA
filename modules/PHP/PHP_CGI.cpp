/**
 * \file PHP_CGI.cpp
 * \brief PHP CGI module
 * \author Mathieu.S
**/

#include "PHP_CGI.hpp"
#include <iostream>

#if defined(_WIN32) || defined(WIN32)
    #include <Windows.h>
    #include <cstdio>
#endif

extern "C" {
    #if defined(_WIN32) || defined(WIN32)
    __declspec(dllexport) 
    #endif
    IModule* newInstance()
    {
        return new PHP_CGI();
    }
}

PHP_CGI::PHP_CGI() : AModule("PHP CGI") {}

void PHP_CGI::loadConfigFile(const std::string &configFilePath) {}

std::string PHP_CGI::handleRequest(const std::string &request)
{
    // TODO: call php cgi with the request statement
    // TODO: return the execve result
    return request;
}

void PHP_CGI::handleQueue()
{
    std::pair<std::string, int> current;

    if (!this->_inQueue.empty()) {
        current = this->_inQueue.front();
        this->_outQueue.emplace_back(this->handleRequest(current.first), current.second);
        this->_inQueue.erase(this->_inQueue.begin());
    }
}