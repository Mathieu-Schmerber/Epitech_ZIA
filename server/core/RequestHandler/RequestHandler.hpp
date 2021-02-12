/*
** EPITECH PROJECT, 2020
** Zia
** File description:
** Created by Emilien
*/

#ifndef ZIA_REQUESTHANDLER_HPP
#define ZIA_REQUESTHANDLER_HPP

#include <thread>

class RequestHandler {
public:
    explicit RequestHandler(int id);
    ~RequestHandler();
    void run();
private:
    std::thread _thread;
    bool _running;
    int _id;
};

#endif //ZIA_REQUESTHANDLER_HPP
