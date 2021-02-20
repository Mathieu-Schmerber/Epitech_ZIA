/**
 * \file ExceptionCore.hpp
 * \brief Exceptions classes for the server core
 * \author Emilien.D
**/

#ifndef ZIA_EXCEPTIONCORE_HPP
#define ZIA_EXCEPTIONCORE_HPP

#include "ZiaHTTPError.hpp"
#include <exception>
#include <string>
#include <utility>

/**
 * \class CoreError ExceptionCore.hpp "ExceptionCore.hpp"
 * \brief Exception class for all the core errors
**/

class CoreError : public ZiaHTTPError {
public:
    explicit CoreError(const std::string &errorType, const std::string &errorMessage, int errorCode) : ZiaHTTPError(errorType, errorMessage, errorCode) {};
};

/**
 * \class ClientError ExceptionCore.hpp "ExceptionCore.hpp"
 * \brief Exception class for the 4xx Client Errors
**/

class ClientError : public CoreError {
public:
    explicit ClientError(const std::string &errorType = "Client Error 4xx", const std::string &errorMessage = "", int errorCode = 400) :
        CoreError(errorType, errorMessage, errorCode) {};
};

/**
 * \class ServerError ExceptionCore.hpp "ExceptionCore.hpp"
 * \brief Exception class for the 5xx Server Errors
**/

class ServerError : public CoreError {
public:
    explicit ServerError(const std::string &errorType = "Server Error 5xx", const std::string &errorMessage = "", int errorCode = 500) :
        CoreError(errorType, errorMessage, errorCode) {};
};

#endif //ZIA_EXCEPTIONCORE_HPP
