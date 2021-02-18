/*
** EPITECH PROJECT, 2020
** Zia
** File description:
** Created by Emilien
*/

#ifndef ZIA_ZIAHTTPERROR_HPP
#define ZIA_ZIAHTTPERROR_HPP

#include <exception>
#include <string>
#include <utility>

class ZiaError : public std::exception {
public:
    explicit ZiaError(const std::string& component, const std::string& message) : _message(std::move(component + ": " + message)) {};

    [[nodiscard]] const char *what() const noexcept override { return _message.c_str(); };
private:
    std::string _message;
};

class ZiaHTTPError : public ZiaError {
public:
    explicit ZiaHTTPError(std::string component = "ZiaHTTPError: ",
        std::string message = "An error occured.", int errorCode = 0) : ZiaError(component, message), _component(std::move(component)), _message(std::move(message)), _errorCode(errorCode) {};

    [[nodiscard]] const char *what() const noexcept override { return this->_message.c_str(); };
    [[nodiscard]] std::string const &getComponent() const { return this->_component; };
    [[nodiscard]] int getErrorCode() const { return this->_errorCode; };
private:
    std::string _component;
    std::string _message;
    int _errorCode;
};

#endif //ZIA_ZIAHTTPERROR_HPP
