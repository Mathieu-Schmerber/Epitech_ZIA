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
#include <filesystem>

/**
 * \class Server Server.hpp "Server.hpp"
 * \brief Main class for the server
**/

class Server {
public:
    explicit Server(int ac, char **av);
    ~Server() = default;

/**
 * \fn void run()
 * \brief Main loop of the server used to run it
**/
    void run();
/**
 * \fn static std::string readAsyncFunction()
 * \brief Function to read text asynchronously
 * \return The text read by the function
**/
    static std::string readAsyncFunction();
/**
 * \fn std::map<std::string, std::shared_ptr<ModuleHandler>> getOutputModules()
 * \brief Function that return the content of the file
 * \return Return the list of the output modules
**/
    std::map<std::string, std::shared_ptr<ModuleHandler>> getOutputModules();

    enum MODULE_IN_OUT {
        MODULE_IN = 0,
        MODULE_OUT = 1
    };
private:
    typedef void (Server::*cmd_ptr)(const std::vector<std::string>& cmdLine);

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

    void _checkFolder(const std::filesystem::path& dirPath);

    std::vector<std::unique_ptr<RequestHandler>> _requestsHandlers;
    std::map<std::string, std::shared_ptr<ModuleHandler>> _modules[2];
    ModuleLoader::DynamicLibManager dlManager;
    bool _running = true;
    std::future<std::string> _future;
    ConfigurationHandler _configHandler;
    std::string _configFilePath;

    const std::map<std::string, cmd_ptr> cmd_ptr_map {
            {"loadmodule", &Server::_cmdLoadModule},
            {"startmodule", &Server::_cmdStartModule},
            {"stopmodule", &Server::_cmdStopModule},
            {"reloadmodule", &Server::_cmdReloadModule},
            {"reloadmodules", &Server::_cmdReloadModules},
            {"exit", &Server::_cmdExitServer},
            {"reload", &Server::_cmdLoadConfiguration},
            {"r", &Server::_cmdLoadConfiguration}
    };
};


#endif //SERVER_SERVER_HPP
