/**
 * \file ModuleException.hpp
 * \brief Exceptions classes for the modules
 * \author Emilien.D
**/

#ifndef ZIA_MODULEEXCEPTION_HPP
#define ZIA_MODULEEXCEPTION_HPP

#include "ZiaError.hpp"

/**
 * \class ZiaModuleError ModuleException.hpp "ModuleException.hpp"
 * \brief Class for the modules exceptions
**/

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
