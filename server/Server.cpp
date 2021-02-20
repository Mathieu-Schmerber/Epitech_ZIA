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
    _future = std::async(std::launch::async, Server::readAsyncFunction);
}

void Server::run()
{
    LOG(INFO) << "Server started";

    while (_running) {
        _readInput();
        for (auto &a : _requestsHandlers) {
            if (a->getState() == READY) {

            }
        }
    }
    LOG(INFO) << "Server stopped";
}

std::string Server::readAsyncFunction()
{
    std::string line;

    std::getline(std::cin, line);
    return line;
}

void Server::_readInput()
{
    std::vector<std::string> cmdLine;
    bool restart = false;

    if (_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
        auto line = _future.get();
        std::istringstream lineToParse(line);
        std::string block;

        while (std::getline(lineToParse, block, ' '))
            cmdLine.push_back(block);
        restart = true;
        try {
            if (cmdLine.empty())
                return;
            if (cmdLine[0] == "loadmodule")
                _loadModule(cmdLine);
            else if (cmdLine[0] == "startmodule")
                _startModule(cmdLine);
            else if (cmdLine[0] == "stopmodule")
                _stopModule(cmdLine);
            else if (cmdLine[0] == "stopserver")
                _stopServer(cmdLine);
            else
                throw ZiaCmdLineError("Zia command line error", "command \'" + cmdLine[0] + "\' not found.");
        } catch (const ZiaCmdLineError &e) {
            std::cerr << e.getComponent() << ": " << e.what() << std::endl;
        }
    }
    if (restart && (cmdLine.empty() || cmdLine[0] != "stopserver"))
        _future = std::async(std::launch::async, Server::readAsyncFunction);
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
        if (_modules[cmdLine[1]]->getStatus())
            throw ZiaModuleError("ZiaModuleError", "module " + cmdLine[1] + " is already started.");
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
        if (!_modules[cmdLine[1]]->getStatus())
            throw ZiaModuleError("ZiaModuleError", "module " + cmdLine[1] + " is not started.");
        _modules[cmdLine[1]].stopModule();
    } catch (const ZiaModuleError &e) {
        std::cerr << e.getErrorMessage() << std::endl;
    }
}

void Server::_stopServer(const std::vector<std::string>& cmdLine)
{
    if (cmdLine.size() != 1)
        throw ZiaCmdLineError("ZiaCmdLineError", "stopserver requires no argument.");
    for (auto &a : _modules) {
        if (a.second->getStatus())
            a.second.stopModule();
    }
    _running = false;
}