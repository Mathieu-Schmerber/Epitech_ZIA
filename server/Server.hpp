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

private:
    TcpProtocol _tcp = TcpProtocol("0.0.0.0", 4242);

    std::map<std::string, ModuleHandler> _modules;
    std::vector<std::unique_ptr<Client>> _clients;
    std::vector<std::unique_ptr<RequestHandler>> _requestsHandlers;
    ModuleLoader::DynamicLibManager dlManager;
};


#endif //SERVER_SERVER_HPP
