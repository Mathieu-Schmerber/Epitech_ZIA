//
// Created by Cyprien on 12/6/2020.
//

#include "Server.hpp"
#include "ModuleException.hpp"
#include "ServerException.hpp"

Server::Server()
{
    for (int i = 0; i < 0; ++i)
        _requestsHandlers.push_back(std::make_unique<RequestHandler>(i));
}

[[noreturn]] void Server::run()
{
    LOG(INFO) << "Server Started";

    while (_running) {
        _readInput();
        for (auto &a : _requestsHandlers) {
            if (a->getState() == READY) {

            }
        }
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
        try {
            if (cmdLine.empty())
                return;
            if (cmdLine[0] == "loadmodule")
                _loadModule(cmdLine);
            else if (cmdLine[0] == "startmodule")
                _startModule(cmdLine);
            else if (cmdLine[0] == "stopmodule")
                _stopModule(cmdLine);
            else
                throw ZiaCmdLineError("Zia command line error", "command \'" + cmdLine[0] + "\' not found.");
        } catch (const ZiaCmdLineError &e) {
            std::cerr << e.getComponent() << ": " << e.what() << std::endl;
        }
    }
}

void Server::_loadModule(const std::vector<std::string>& cmdLine)
{
    if (cmdLine.size() != 2)
        throw ZiaCmdLineError("ZiaCmdLineError", "loadmodule requires one argument.");
    try {
        dlManager.loadNewLib<AModule>(DYNLIB(cmdLine[1]));
        _modules.insert(std::pair<std::string, ModuleHandler>(cmdLine[1],
            ModuleHandler(dlManager.getInstance<AModule>(DYNLIB(cmdLine[1])))));
        std::cout << "Module " << DYNLIB(cmdLine[1]) << " loaded." << std::endl;
    } catch (const ModuleLoader::ModuleLoaderException &e) {
        for (auto &a : _modules)
            std::cout << a.first << " & " << a.second.get() << std::endl;
        std::cerr << e.getComponent() << ": " << e.what() << std::endl;
    }
}

void Server::_startModule(const std::vector<std::string> &cmdLine)
{
    if (cmdLine.size() != 2)
        throw ZiaCmdLineError("ZiaCmdLineError", "startmodule requires one argument.");
    try {
        if (_modules.find(cmdLine[1]) == _modules.end())
            throw ZiaModuleError("ZiaCmdLineError", "module " + cmdLine[1] + " not loaded.");
        _modules[cmdLine[1]].startModule();
    } catch (const ZiaModuleError &e) {
        std::cerr << e.getErrorMessage() << std::endl;
    }
}

void Server::_stopModule(const std::vector<std::string> &cmdLine)
{
    if (cmdLine.size() != 2)
        throw ZiaCmdLineError("ZiaCmdLineError", "stopmodule requires one argument.");
    try {
        if (_modules.find(cmdLine[1]) == _modules.end())
            throw ZiaModuleError("ZiaCmdLineError", "module " + cmdLine[1] + " not loaded.");
        _modules[cmdLine[1]].stopModule();
    } catch (const ZiaModuleError &e) {
        std::cerr << e.getErrorMessage() << std::endl;
    }
}