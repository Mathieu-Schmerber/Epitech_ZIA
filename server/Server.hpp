/**
 * \file Log.hpp
 * \brief Display macros
 * \author Emilien.D
 * \author Cyprien.R
**/

#ifndef SERVER_SERVER_HPP
#define SERVER_SERVER_HPP

#include "DynamicLibManager.hpp"
#include "RequestHandler.hpp"
#include "ModuleHandler.hpp"
#include "AModule.hpp"
#include "Client.hpp"
#include "ConfigurationHandler.hpp"
#include <future>

/**
 * \class Server Server.hpp "Server.hpp"
 * \brief Main class for the server
**/

class Server {
public:
    explicit Server();
    ~Server() = default;

    void run();
    static std::string readAsyncFunction();
    std::map<std::string, std::shared_ptr<ModuleHandler>> getOutputModules();

    enum MODULE_IN_OUT {
        MODULE_IN = 0,
        MODULE_OUT = 1
    };
private:
    void _readInput();
    void _cmdLoadModule(const std::vector<std::string>& cmdLine);
    void _cmdStartModule(const std::vector<std::string>& cmdLine);
    void _cmdStopModule(const std::vector<std::string>& cmdLine);
    void _cmdReloadModule(const std::vector<std::string>& cmdLine);
    void _cmdReloadModules(const std::vector<std::string>& cmdLine);
    void _cmdExitServer(const std::vector<std::string>& cmdLine);
    void _cmdLoadConfiguration(const std::vector<std::string>& cmdLine);

    void _loadModule(const std::string &moduleName);
    void _startModule(const std::string &moduleName);
    void _stopModule(const std::string &moduleName);

    std::vector<std::unique_ptr<RequestHandler>> _requestsHandlers;
    std::map<std::string, std::shared_ptr<ModuleHandler>> _modules[2];
    ModuleLoader::DynamicLibManager dlManager;
    bool _running = true;
    std::future<std::string> _future;
    ConfigurationHandler _configHandler;
};


#endif //SERVER_SERVER_HPP
