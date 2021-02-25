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
#include "TcpProtocol.hpp"
#include "AModule.hpp"
#include "Client.hpp"
#include "ConfigurationHandler.hpp"

#ifdef _WIN32
    #define _WIN32_WINNT  0x0601
#endif

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

    enum MODULE_IN_OUT {
        MODULE_IN = 0,
        MODULE_OUT = 1
    };
private:
    void _readInput();
    void _loadModule(const std::vector<std::string>& cmdLine);
    void _startModule(const std::vector<std::string>& cmdLine);
    void _stopModule(const std::vector<std::string>& cmdLine);
    void _stopServer(const std::vector<std::string>& cmdLine);
    void _reloadConfiguration(const std::vector<std::string>& cmdLine);

    std::vector<std::unique_ptr<RequestHandler>> _requestsHandlers;
    std::map<std::string, std::shared_ptr<ModuleHandler>> _modules[2];
    ModuleLoader::DynamicLibManager dlManager;
    bool _running = true;
    std::future<std::string> _future;
    ConfigurationHandler _configHandler;

};


#endif //SERVER_SERVER_HPP
