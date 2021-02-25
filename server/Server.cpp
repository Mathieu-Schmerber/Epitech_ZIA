//
// Created by Cyprien on 12/6/2020.
//

#include "Server.hpp"
#include "ModuleException.hpp"
#include "ServerException.hpp"
#include <sstream>

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
                _cmdLoadModule(cmdLine);
            else if (cmdLine[0] == "startmodule")
                _cmdStartModule(cmdLine);
            else if (cmdLine[0] == "stopmodule")
                _cmdStopModule(cmdLine);
            else if (cmdLine[0] == "reloadmodule")
                _cmdReloadModule(cmdLine);
            else if (cmdLine[0] == "reloadmodules")
                _cmdReloadModules(cmdLine);
            else if (cmdLine[0] == "exit")
                _cmdExitServer(cmdLine);
            else if (cmdLine[0] == "reload" || cmdLine[0] == "r")
                _cmdLoadConfiguration(cmdLine);
            else
                throw ZiaCmdLineError("Zia command line error", "command \'" + cmdLine[0] + "\' not found.");
        } catch (const ZiaCmdLineError &e) {
            std::cerr << e.getComponent() << ": " << e.what() << std::endl;
        }
    }
    if (restart && (cmdLine.empty() || cmdLine[0] != "exit"))
        _future = std::async(std::launch::async, Server::readAsyncFunction);
}

void Server::_cmdLoadModule(const std::vector<std::string> &cmdLine)
{
    if (cmdLine.size() != 2)
        throw ZiaCmdLineError("ZiaCmdLineError", "loadmodule requires one argument.");
    _loadModule(cmdLine[1]);
}

void Server::_cmdStartModule(const std::vector<std::string> &cmdLine)
{
    if (cmdLine.size() != 2)
        throw ZiaCmdLineError("ZiaCmdLineError", "startmodule requires one argument.");
    _startModule(cmdLine[1]);
}

void Server::_cmdStopModule(const std::vector<std::string> &cmdLine)
{
    if (cmdLine.size() != 2)
        throw ZiaCmdLineError("ZiaCmdLineError", "stopmodule requires one argument.");
    _stopModule(cmdLine[1]);
}

void Server::_cmdReloadModule(const std::vector<std::string> &cmdLine)
{
    if (cmdLine.size() != 2)
        throw ZiaCmdLineError("ZiaCmdLineError", "reloadmodule requires one argument.");
    for (auto &a : _modules) {
        for (auto &b : a) {
            if (b.first == cmdLine.at(1) && b.second->get()->getStatus()) {
                b.second->stopModule();
                b.second->startModule();
            }
        }
    }
}

void Server::_cmdReloadModules(const std::vector<std::string> &cmdLine)
{
    if (cmdLine.size() != 1)
        throw ZiaCmdLineError("ZiaCmdLineError", "reloadmodule requires no argument.");
    for (auto &a : _modules) {
        for (auto &b : a) {
            if (b.second->get()->getStatus()) {
                b.second->stopModule();
                b.second->startModule();
            }
        }
    }
}

void Server::_cmdExitServer(const std::vector<std::string>& cmdLine)
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

void Server::_cmdLoadConfiguration(const std::vector<std::string> &cmdLine)
{
    std::vector<t_module> _loadedModules;
    for (auto & module : _modules)
    {
        for (auto & a : module)
        {
            if (a.second->get()->getStatus())
                a.second->stopModule();
        }
    }

    _configHandler.loadConfiguration("config.json"); ///FIXME : Change path
    _loadedModules = _configHandler.getLoadedModules();
    for (auto & _loadedModule : _loadedModules) {
        if (!dlManager.libStocked(DYNLIB(_loadedModule.name)))
            _loadModule(_loadedModule.name);
        _startModule(_loadedModule.name);
    }
}

void Server::_loadModule(const std::string &moduleName)
{
    try {
        dlManager.loadNewLib<AModule>(DYNLIB(moduleName));
        std::shared_ptr<ModuleHandler> toAdd = std::make_shared<ModuleHandler>(
                ModuleHandler(dlManager.getInstance<AModule>(DYNLIB(moduleName))));
        if (toAdd->get()->isInputData())
            _modules[MODULE_IN].insert(std::pair<std::string, std::shared_ptr<ModuleHandler>>(moduleName, toAdd));
        else
            _modules[MODULE_OUT].insert(std::pair<std::string, std::shared_ptr<ModuleHandler>>(moduleName, toAdd));
        std::cout << "Module " << DYNLIB(moduleName) << " loaded." << std::endl;
    } catch (const ModuleLoader::ModuleLoaderException &e) {
        std::cerr << e.getComponent() << ": " << e.what() << std::endl;
    }
}

void Server::_startModule(const std::string &moduleName)
{
    try {
        bool in_out[2] = {_modules[MODULE_IN].find(moduleName) == _modules[MODULE_IN].end(),
                          _modules[MODULE_OUT].find(moduleName) == _modules[MODULE_OUT].end()};
        if (in_out[0] && in_out[1])
            throw ZiaModuleError("ZiaCmdLineError", "module " + moduleName + " not loaded.");
        if (_modules[(in_out[0] ? MODULE_OUT : MODULE_IN)][moduleName]->get()->getStatus())
            throw ZiaModuleError("ZiaModuleError", "module " + moduleName + " is already started.");
        _modules[(in_out[0] ? MODULE_OUT : MODULE_IN)][moduleName]->startModule();
    } catch (const ZiaModuleError &e) {
        std::cerr << e.getErrorMessage() << std::endl;
    }
}

void Server::_stopModule(const std::string &moduleName)
{
    try {
        bool in_out[2] = {_modules[MODULE_IN].find(moduleName) == _modules[MODULE_IN].end(),
                          _modules[MODULE_OUT].find(moduleName) == _modules[MODULE_OUT].end()};
        if (in_out[0] && in_out[1])
            throw ZiaModuleError("ZiaCmdLineError", "module " + moduleName + " not loaded.");
        if (!_modules[(in_out[0] ? MODULE_OUT : MODULE_IN)][moduleName]->get()->getStatus())
            throw ZiaModuleError("ZiaModuleError", "module " + moduleName + " is not started.");
        _modules[(in_out[0] ? MODULE_OUT : MODULE_IN)][moduleName]->stopModule();
    } catch (const ZiaModuleError &e) {
        std::cerr << e.getErrorMessage() << std::endl;
    }
}

