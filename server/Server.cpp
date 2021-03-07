//
// Created by Cyprien on 12/6/2020.
//

// TODO Doxygen

#include "Server.hpp"
#include "ModuleException.hpp"
#include "ServerException.hpp"
#include "RequestHandler.hpp"
#include <sstream>

Server::Server(int ac, char **av)
{
    std::vector<t_module> loadedModules;
    std::string configFilePath;

    if (ac == 2) {
        _configFilePath = av[1];
    } else if (ac < 2) {
        LOG(WARN) << "No config file specified try to find one";
        _checkFolder("./");
    }

    for (int i = 0; i < 2; ++i)
        _requestsHandlers.push_back(std::make_unique<RequestHandler>(this, i));
    _future = std::async(std::launch::async, Server::readAsyncFunction);
    _configHandler.loadConfiguration(_configFilePath);
    loadedModules = _configHandler.getLoadedModules();
    for (auto &_loadedModule : loadedModules) {
        if (!dlManager.libStocked(DYNLIB(_loadedModule.name)))
            _loadModule(_loadedModule.name);
        _startModule(_loadedModule.name);
    }
}

void Server::_checkFolder(const std::filesystem::path &dirPath)
{
    std::string result;

    for (const auto &entry : std::filesystem::directory_iterator(dirPath)) {
        if (entry.path().string().find("config.json") != std::string::npos) {
            _configFilePath = entry.path().string();
            std::replace(_configFilePath.begin(), _configFilePath.end(), '\\', '/');
            LOG(INFO) << "Find a configuration file in: " << _configFilePath;
            return;
        } else if (std::filesystem::is_directory(entry.path())) {
            _checkFolder(entry.path());
        }
    }
}

void Server::run()
{
    LOG(INFO) << "Server started";

    while (_running) {
        _readInput();
        for (auto &inputModule : _modules[MODULE_IN]) {
            for (auto &a : _requestsHandlers) {
                std::pair<std::string, std::pair<std::string, int>> pRequest;
                if (a->getState() == PROCESSED) {
                    pRequest = a->getProcessedRequest();
                    _modules[MODULE_IN][pRequest.second.first]->get()->dataInput(pRequest.first,
                                                                                 pRequest.second.second);
                }
            }
            if (inputModule.second->get()->getStatus()) {
                for (auto &a : _requestsHandlers) {
                    if (a->getState() == READY) {
                        std::pair<std::string, int> requestIn = inputModule.second->get()->dataOutput();
                        std::pair<std::string, std::pair<std::string, int>> requestToProcess(requestIn.first,
                                                                                             {inputModule.first,
                                                                                              requestIn.second});
                        if (!requestToProcess.first.empty()) {
                            a->setRequestToProcess(requestToProcess);
                        }
                    }
                }
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

std::map<std::string, std::shared_ptr<ModuleHandler>> Server::getOutputModules()
{
    return _modules[MODULE_OUT];
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
            if (cmdLine.empty()) {
                _future = std::async(std::launch::async, Server::readAsyncFunction);
                return;
            }
            if (!Utils::isInMap(cmd_ptr_map, cmdLine[0]))
                throw ZiaCmdLineError("Zia command line error", "command \'" + cmdLine[0] + "\' not found.");

            std::invoke(cmd_ptr_map.at(cmdLine[0]), this, cmdLine);

        } catch (const ZiaCmdLineError &e) {
            LOG(ERR) << e.getComponent() << ": " << e.what();
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

void Server::_cmdExitServer(const std::vector<std::string> &cmdLine)
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
    for (auto &module : _modules) {
        for (auto &a : module) {
            if (a.second->get()->getStatus())
                a.second->stopModule();
        }
    }

    if (!std::filesystem::exists(_configFilePath)) { /// FIXME : Change path
        LOG(WARN) << "The configuration file has been moved or deleted trying to find him...";
        _checkFolder("./");
    }
    _configHandler.loadConfiguration(_configFilePath);
    _loadedModules = _configHandler.getLoadedModules();
    for (auto &_loadedModule : _loadedModules) {
        if (!dlManager.libStocked(DYNLIB(_loadedModule.name)))
            _loadModule(_loadedModule.name);
        _startModule(_loadedModule.name);
    }
}

void Server::_loadModule(const std::string &moduleName)
{
    try {
        dlManager.loadNewLib<AModule>(DYNLIB(moduleName));
        auto *newModule = dlManager.getInstance<AModule>(DYNLIB(moduleName));
        if (newModule->isInputData())
            _modules[MODULE_IN].insert(std::pair<std::string, std::shared_ptr<ModuleHandlerInput>>(moduleName,
                                                                                                   std::make_shared<ModuleHandlerInput>(
                                                                                                           ModuleHandlerInput(
                                                                                                                   dlManager.getInstance<AModule>(
                                                                                                                           DYNLIB(moduleName))))));
        else
            _modules[MODULE_OUT].insert(std::pair<std::string, std::shared_ptr<ModuleHandlerOutput>>(moduleName,
                                                                                                     std::make_shared<ModuleHandlerOutput>(
                                                                                                             ModuleHandlerOutput(
                                                                                                                     dlManager.getInstance<AModule>(
                                                                                                                             DYNLIB(moduleName))))));
        LOG(INFO) << "Module " << DYNLIB(moduleName) << " loaded.";
    } catch (const ModuleLoader::ModuleLoaderException &e) {
        LOG(ERR) << e.getComponent() << ": " << e.what();
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
        LOG(ERR) << e.getErrorMessage();
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
        LOG(ERR) << e.getErrorMessage();
    }
}
