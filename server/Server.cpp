//
// Created by Cyprien on 12/6/2020.
//

#include "Server.hpp"
#include "ModuleException.hpp"

Server::Server()
{
    for (int i = 0; i < 1; ++i)
        _requestsHandlers.push_back(std::make_unique<RequestHandler>(i));
}

[[noreturn]] void Server::run()
{
    dlManager.loadNewLib<AModule>(DYNLIB("httpModule"));
    _modules.insert(std::pair<std::string, ModuleHandler>("httpModule", ModuleHandler(dlManager.getInstance<AModule>(DYNLIB("httpModule")))));
    LOG(INFO) << "Server Started";

    try {
        _modules["httpModule"].startModule();
    } catch (const ZiaModuleError &e) {
        std::cerr << e.getErrorMessage() << std::endl;
    }
    while (true) {
        for (auto &a : _requestsHandlers) {
            if (a->getState() == READY) {

            }
        }
    }
}
