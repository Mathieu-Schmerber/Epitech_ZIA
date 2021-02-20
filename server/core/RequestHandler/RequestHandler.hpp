/**
 * \file RequestHandler.hpp
 * \brief Definitions for the Request Handler threads
 * \author Emilien.D
**/

#ifndef ZIA_REQUESTHANDLER_HPP
#define ZIA_REQUESTHANDLER_HPP

#include <thread>

enum ThreadState {
    PROCESSING = 1,
    PROCESSED,
    READY
};

/**
 * \class RequestHandler RequestHandler.hpp "RequestHandler.hpp"
 * \brief Class for the request handlers threads
**/

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
