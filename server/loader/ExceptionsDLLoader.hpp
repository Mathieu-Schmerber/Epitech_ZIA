//
// Created by Emilien on 17/02/2021.
//

#ifndef ZIA_EXCEPTIONSDLLOADER_HPP
#define ZIA_EXCEPTIONSDLLOADER_HPP

#include <exception>
#include <string>
#include "Log.hpp"

namespace ModuleLoader {
    class ModuleLoaderException : public std::exception {
    public:
        explicit ModuleLoaderException(std::string component = "Component: ", std::string message = "An error occurred.") :
            _component(std::move(component)), _message(std::move(message)) {};

        [[nodiscard]] std::string const &getComponent() const {
            return this->_component;
        };

        [[nodiscard]] const char *what() const noexcept override {
            return this->_message.c_str();
        };
        private:
            std::string _component;
            std::string _message;
    };

    class DynamicLibError : public ModuleLoaderException {
    public:
        explicit DynamicLibError(std::string const &message) : ModuleLoaderException("DynamicLibError", message) {};
    };

    class DynamicLibAlreadyExist : public ModuleLoaderException {
    public:
        explicit DynamicLibAlreadyExist(std::string const &message) : ModuleLoaderException("DynamicLibAlreadyExist", message) {};
    };
}

#endif //ZIA_EXCEPTIONSDLLOADER_HPP
