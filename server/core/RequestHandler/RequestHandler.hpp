/*
** EPITECH PROJECT, 2020
** Zia
** File description:
** Created by Emilien
*/

#ifndef ZIA_REQUESTHANDLER_HPP
#define ZIA_REQUESTHANDLER_HPP

#include <thread>

enum ThreadState {
    PROCESSING = 1,
    PROCESSED,
    READY
};

class RequestHandler {
public:
    explicit RequestHandler(int id);
    ~RequestHandler();
    void run();
    [[nodiscard]] ThreadState getState() const;
private:
    std::thread _thread;
    ThreadState _state;
    bool _running;
    int _id;
};

#endif //ZIA_REQUESTHANDLER_HPP
