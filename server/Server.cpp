//
// Created by Cyprien on 12/6/2020.
//

#include "Server.hpp"
#include "Utils.hpp"

[[noreturn]] void Server::run()
{
    LOG(INFO) << "Server Started";
    while (true) {
        LOG(INFO) << "Server Run";
    }
}
