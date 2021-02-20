/**
 * \file ZiaError.hpp
 * \brief Base exceptions for the Zia Project
 * \author Emilien.D
**/

#ifndef ZIA_ZIAERROR_HPP
#define ZIA_ZIAERROR_HPP

#include <exception>
#include <string>
#include <utility>

/**
 * \class ZiaError ZiaError.hpp "ZiaError.hpp"
 * \brief Class which defines the base for all Zia exceptions
**/

class ZiaError : public std::exception {
public:
    explicit ZiaError(const std::string& component, const std::string& message) : _message(std::move(component + ": " + message)) {};

    [[nodiscard]] const char *what() const noexcept override { return _message.c_str(); };
private:
    std::string _message;
};

#endif //ZIA_ZIAERROR_HPP
