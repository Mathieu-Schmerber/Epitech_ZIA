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
            else if (cmdLine[0] == "reload" || cmdLine[0] == "r")
                _reloadConfiguration(cmdLine);
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
        std::shared_ptr<ModuleHandler> toAdd = std::make_shared<ModuleHandler>(ModuleHandler(dlManager.getInstance<AModule>(DYNLIB(cmdLine[1]))));
        if (toAdd->get()->isInputData())
            _modules[MODULE_IN].insert(std::pair<std::string, std::shared_ptr<ModuleHandler>>(cmdLine[1], toAdd));
        else
            _modules[MODULE_OUT].insert(std::pair<std::string, std::shared_ptr<ModuleHandler>>(cmdLine[1], toAdd));
        std::cout << "Module " << DYNLIB(cmdLine[1]) << " loaded." << std::endl;
    } catch (const ModuleLoader::ModuleLoaderException &e) {
        std::cerr << e.getComponent() << ": " << e.what() << std::endl;
    }
}

void Server::_startModule(const std::vector<std::string> &cmdLine)
{
    if (cmdLine.size() != 2)
        throw ZiaCmdLineError("ZiaCmdLineError", "startmodule requires one argument.");
    try {
        bool in_out[2] = {_modules[MODULE_IN].find(cmdLine[1]) == _modules[MODULE_IN].end(), _modules[MODULE_OUT].find(cmdLine[1]) == _modules[MODULE_OUT].end()};
        if (in_out[0] && in_out[1])
            throw ZiaModuleError("ZiaCmdLineError", "module " + cmdLine[1] + " not loaded.");
        if (_modules[(in_out[0] ? MODULE_OUT : MODULE_IN)][cmdLine[1]]->get()->getStatus())
            throw ZiaModuleError("ZiaModuleError", "module " + cmdLine[1] + " is already started.");
        _modules[(in_out[0] ? MODULE_OUT : MODULE_IN)][cmdLine[1]]->startModule();
    } catch (const ZiaModuleError &e) {
        std::cerr << e.getErrorMessage() << std::endl;
    }
}

void Server::_stopModule(const std::vector<std::string> &cmdLine)
{
    if (cmdLine.size() != 2)
        throw ZiaCmdLineError("ZiaCmdLineError", "stopmodule requires one argument.");
    try {
        bool in_out[2] = {_modules[MODULE_IN].find(cmdLine[1]) == _modules[MODULE_IN].end(), _modules[MODULE_OUT].find(cmdLine[1]) == _modules[MODULE_OUT].end()};
        if (in_out[0] && in_out[1])
            throw ZiaModuleError("ZiaCmdLineError", "module " + cmdLine[1] + " not loaded.");
        if (!_modules[(in_out[0] ? MODULE_OUT : MODULE_IN)][cmdLine[1]]->get()->getStatus())
            throw ZiaModuleError("ZiaModuleError", "module " + cmdLine[1] + " is not started.");
        _modules[(in_out[0] ? MODULE_OUT : MODULE_IN)][cmdLine[1]]->stopModule();
    } catch (const ZiaModuleError &e) {
        std::cerr << e.getErrorMessage() << std::endl;
    }
}

void Server::_stopServer(const std::vector<std::string>& cmdLine)
{
    if (cmdLine.size() != 1)
        throw ZiaCmdLineError("ZiaCmdLineError", "stopserver requires no argument.");
    for (auto &b : _modules) {
        for (auto &a : b) {
            if (a.second->get()->getStatus())
                a.second->stopModule();
        }
    }
    _running = false;
}

void Server::_reloadConfiguration(const std::vector<std::string> &)
{
    _configHandler.loadConfiguration("config.json");
}
