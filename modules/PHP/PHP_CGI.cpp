/**
 * \file PHP_CGI.cpp
 * \brief PHP CGI module
 * \author Mathieu.S
**/

#include "PHP_CGI.hpp"
#include "Response.hpp"
#include <filesystem>
#include <iostream>
#include "Router.hpp"
#include "ExceptionCore.hpp"

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
PHP_CGI::PHP_CGI() : AModuleOutput("PHP CGI")
{
    #ifdef __unix__
        this->_cgiPath = "php-cgi";
    #elif defined(_WIN32) || defined(WIN32)
        this->_cgiPath = "php-cgi.exe";
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
    Router rooter;

    rooter.init();
    std::string cmd = PHP_CGI::getOSCmd(this->_cgiPath + " ./" + rooter.getPath(request));

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
    std::string fileContent = this->handleRequest(_requestToProcess.getRequestPath());
    std::vector<std::pair<std::string, std::string>> parameters{};
    std::istringstream toParse(fileContent);
    std::vector<std::string> lineParsed;
    std::string lineToParse;
    std::string content;
    bool isContent = false;

    while (std::getline(toParse, lineToParse)) {
        if (lineToParse.empty())
            isContent = true;
        if (isContent) {
            content += lineToParse;
        } else if (lineToParse.empty()) {
            isContent = true;
        } else {
            std::istringstream tmp(lineToParse);
            std::string param[3];

            std::getline(tmp, param[0], ':');
            std::getline(tmp, param[1], ' ');
            std::getline(tmp, param[1]);
            parameters.emplace_back(param[0], param[1]);
        }
    }
    if (parameters.empty())
#if defined(_WIN32) || defined(WIN32)
        throw ServerError("Error 500. php-cgi.exe not found. Please add it in your path.");
#else
        throw ServerError("Error 500. php-cgi not found. Please add it in your path.");
#endif
    if (parameters.at(0).first != "Status") {
        _response = Response::getResponse(content, "OK", 200, {}, parameters);
        return;
    } else {
        std::string code;
        std::string message;
        std::istringstream ssparamaters(parameters.at(0).second);

        parameters.erase(parameters.begin());
        getline(ssparamaters, code, ' ');
        getline(ssparamaters, message);
        _response = Response::getResponse(content, message, std::strtol(code.c_str(), nullptr, 10));
    }
}
