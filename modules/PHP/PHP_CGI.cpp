/**
 * \file PHP_CGI.cpp
 * \brief PHP CGI module
 * \author Mathieu.S
**/

#include "PHP_CGI.hpp"
#include <filesystem>
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

/**
 * \brief Gets php-cgi program depending on current OS
 * **/
PHP_CGI::PHP_CGI() : AModule("PHP CGI")
{
    #ifdef __unix__
        this->_cgiPath = "php_rsc/unix/php-cgi";
    #elif defined(_WIN32) || defined(WIN32)
        this->_cgiPath = "php_rsc/windows/php-cgi.exe";
    #endif
}

void PHP_CGI::loadConfigFile(const std::string &configFilePath) {}

/**
 * \brief Get command OS independently
 * **/
std::string PHP_CGI::getOSCmd(const std::string &cmd)
{
    std::string res = cmd;

#ifdef __unix__
    return "./" + cmd;
#elif defined(_WIN32) || defined(WIN32)
    std::replace(res.begin(), res.end(), '/', '\\');
    return "cmd.exe /C " + cmd;
#endif
}

/**
 * \brief Opens a pipe OS independently
 * **/
FILE *PHP_CGI::openOSPipe(const std::string &cmd)
{
#ifdef __unix__
    return popen(cmd.c_str(), "r");
#elif defined(_WIN32) || defined(WIN32)
    return _popen(cmd.c_str(), "r");
#endif
}

/**
 * \brief Closes a pipe OS independently
 * **/
void PHP_CGI::closeOSPipe(FILE *pipe)
{
#ifdef __unix__
    pclose(pipe);
#elif defined(_WIN32) || defined(WIN32)
    _pclose(pipe);
#endif
}

/**
 * \brief Pipes and executes a command OS independently
 * **/
std::string PHP_CGI::execute(const std::string &cmd)
{
    char buffer[BUFFER_SIZE];
    std::string result;
    FILE *pipe = PHP_CGI::openOSPipe(cmd);

    if (!pipe) throw std::runtime_error("[PHP CGI] failed to pipe.");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != nullptr)
            result += buffer;
    } catch (std::exception &error) {
        PHP_CGI::closeOSPipe(pipe);
        throw std::runtime_error(std::string("[PHP CGI] pipe error: ") + error.what());
    }
    PHP_CGI::closeOSPipe(pipe);
    return result;
}

/**
 * \brief Executes requests through php-cgi and returns its result
 * **/
std::string PHP_CGI::handleRequest(const std::string &request)
{
    std::string cmd = PHP_CGI::getOSCmd(this->_cgiPath + " " + request);

    try {
        return PHP_CGI::execute(cmd);
    } catch (std::exception &error) {
        return error.what();
    }
}

/**
 * \brief Handles requests queueing by AModule::dataInput()
 * **/
void PHP_CGI::handleQueue()
{
    std::pair<std::string, int> current;

    if (!this->_inQueue.empty()) {
        current = this->_inQueue.front();
        this->_outQueue.emplace_back(this->handleRequest(current.first), current.second);
        this->_inQueue.erase(this->_inQueue.begin());
    }
}
