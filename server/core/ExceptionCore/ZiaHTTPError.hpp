/**
 * \file ZiaHTTPError.hpp
 * \brief Exceptions classes for the HTTP Protocol errors
 * \author Emilien.D
**/

#ifndef ZIA_ZIAHTTPERROR_HPP
#define ZIA_ZIAHTTPERROR_HPP

#include "ZiaError.hpp"
#include <exception>

/**
 * \class ZiaHTTPError ZiaHTTPError.hpp "ZiaHTTPError.hpp"
 * \brief Class for the HTTP protocol errors
**/

class ZiaHTTPError : public ZiaError {
public:
    explicit ZiaHTTPError(std::string component = "ZiaHTTPError",
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
