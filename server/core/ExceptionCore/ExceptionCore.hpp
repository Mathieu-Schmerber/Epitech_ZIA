/*
** EPITECH PROJECT, 2020
** Zia
** File description:
** Created by Emilien
*/

#ifndef ZIA_EXCEPTIONCORE_HPP
#define ZIA_EXCEPTIONCORE_HPP

#include "ZiaError.hpp"
#include <exception>
#include <string>
#include <utility>

class CoreError : public ZiaError {
public:
    explicit CoreError(const std::string &errorType, const std::string &errorMessage, int errorCode) : ZiaError(errorType, errorMessage, errorCode) {};
};

class ClientError : public CoreError {
public:
    explicit ClientError(const std::string &errorType = "Client Error 4xx", const std::string &errorMessage = "", int errorCode = 400) :
        CoreError(errorType, errorMessage, errorCode) {};
};

class ServerError : public CoreError {
public:
    explicit ServerError(const std::string &errorType = "Server Error 5xx", const std::string &errorMessage = "", int errorCode = 500) :
        CoreError(errorType, errorMessage, errorCode) {};
};

#endif //ZIA_EXCEPTIONCORE_HPP
