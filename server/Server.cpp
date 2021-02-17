//
// Created by Cyprien on 12/6/2020.
//

#include "Server.hpp"

Server::Server()
{
    for (int i = 0; i < 5; ++i)
        _requestsHandlers.push_back(std::make_unique<RequestHandler>(i));
}

[[noreturn]] void Server::run()
{
    LOG(INFO) << "Server Started";
    while (true) {
        for (auto &a : _requestsHandlers) {
            if (a->getState() == READY) {

            }
        }
    }
}
