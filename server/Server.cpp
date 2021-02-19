//
// Created by Cyprien on 12/6/2020.
//

#include "Server.hpp"
#include "ModuleException.hpp"

Server::Server()
{
    for (int i = 0; i < 0; ++i)
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

    while (_running) {
        _readInput();
        for (auto &a : _requestsHandlers) {
            if (a->getState() == READY) {

            }
        }
    }


    try {
        _modules["httpModule"].stopModule();
    } catch (const ZiaModuleError &e) {
        std::cerr << e.getErrorMessage() << std::endl;
    }
}

std::string Server::readAsyncFunction()
{
    std::string line;

    std::getline(std::cin, line);
    return line;
}

void Server::_readInput()
{
    static auto future = std::async(std::launch::async, Server::readAsyncFunction);

    if (future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
        auto line = future.get();
        future = std::async(std::launch::async, Server::readAsyncFunction);
        std::istringstream lineToParse(line);
        std::string block;
        std::vector<std::string> cmdLine;

        while (std::getline(lineToParse, block, ' '))
            cmdLine.push_back(block);
        for (auto &a : cmdLine) {
            std::cout << "block : " << a << std::endl;
        }
    }
}
