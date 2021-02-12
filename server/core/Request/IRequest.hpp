/*
** EPITECH PROJECT, 2020
** Zia
** File description:
** Created by Emilien
*/

#ifndef ZIA_IREQUEST_HPP
#define ZIA_IREQUEST_HPP

class IRequest {
public:
    virtual ~IRequest() = default;

    virtual void process() = 0;
};

#endif //ZIA_IREQUEST_HPP
