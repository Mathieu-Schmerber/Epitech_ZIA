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
#include <utility>

class ZiaError : public std::exception {
public:
    explicit ZiaError(const std::string& component, const std::string& message) : _message(std::move(component + ": " + message)) {};

    [[nodiscard]] const char *what() const noexcept override { return _message.c_str(); };
private:
    std::string _message;
};

#endif //ZIA_ZIAERROR_HPP
