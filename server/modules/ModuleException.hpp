//
// Created by Emilien on 18/02/2021.
//

#ifndef ZIA_MODULEEXCEPTION_HPP
#define ZIA_MODULEEXCEPTION_HPP

#include "ZiaHTTPError.hpp"

class ZiaModuleError : public ZiaError {
public:
    explicit ZiaModuleError(const std::string &component, const std::string& message) :
        ZiaError(component, message), _component(component), _message(message) {};

    [[nodiscard]] const char *what() const noexcept override { return _message.c_str(); };
    [[nodiscard]] std::string const &getComponent() const { return _component; };
    [[nodiscard]] std::string getErrorMessage() const { return _component + ": " + _message; };
private:
    std::string _component;
    std::string _message;
};

#endif //ZIA_MODULEEXCEPTION_HPP
