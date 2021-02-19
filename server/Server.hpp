//
// Created by Cyprien on 12/6/2020.
//

#ifndef SERVER_SERVER_HPP
#define SERVER_SERVER_HPP

#include "DynamicLibManager.hpp"
#include "RequestHandler.hpp"
#include "ModuleHandler.hpp"
#include "TcpProtocol.hpp"
#include "AModule.hpp"
#include "Client.hpp"

class Server {
public:
    explicit Server();
    ~Server() = default;

    [[noreturn]] void run();
    static std::string readAsyncFunction();

private:
    void _readInput();
    void _loadModule(const std::vector<std::string>& cmdLine);
    void _startModule(const std::vector<std::string>& cmdLine);

    std::vector<std::unique_ptr<RequestHandler>> _requestsHandlers;
    std::map<std::string, ModuleHandler> _modules;
    ModuleLoader::DynamicLibManager dlManager;
    bool _running = true;
};


#endif //SERVER_SERVER_HPP
