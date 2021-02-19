/*
** EPITECH PROJECT, 2020
** Zia
** File description:
** Created by Emilien
*/

#ifndef ZIA_SERVEREXCEPTION_HPP
#define ZIA_SERVEREXCEPTION_HPP

#include <string>
#include <utility>
#include "ZiaError.hpp"

class ZiaCmdLineError : public ZiaError {
public:
    explicit ZiaCmdLineError(std::string component = "ZiaCmdLineError",
        std::string message = "An error occured.") : ZiaError(component, message), _component(std::move(component)), _message(std::move(message)) {};

    [[nodiscard]] const char *what() const noexcept override { return _message.c_str(); };
    [[nodiscard]] std::string getComponent() const { return _component; };
private:
    std::string _component;
    std::string _message;
};

#endif //ZIA_SERVEREXCEPTION_HPP
