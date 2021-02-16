/*
** EPITECH PROJECT, 2020
** Zia
** File description:
** Created by Emilien
*/

#ifndef ZIA_ZIAERROR_HPP
#define ZIA_ZIAERROR_HPP

#include <exception>
#include <string>

class ZiaError : public std::exception {
public:
    explicit ZiaError(std::string component = "ZiaError: ",
        std::string message = "An error occured.", int errorCode = 0) : _component(std::move(component)), _message(std::move(message)), _errorCode(errorCode) {};

    [[nodiscard]] const char* what() const noexcept override { return this->_message.c_str(); };
    [[nodiscard]] std::string const &getComponent() const { return this->_component; };
    [[nodiscard]] int getErrorCode() const { return this->_errorCode; };
private:
    std::string _component;
    std::string _message;
    int _errorCode;
};

#endif //ZIA_ZIAERROR_HPP
