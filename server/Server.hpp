//
// Created by Cyprien on 12/6/2020.
//

#ifndef SERVER_SERVER_HPP
#define SERVER_SERVER_HPP


#include "TcpProtocol.hpp"
#include "Client.hpp"

class Server {
public:
    Server() = default;
    ~Server() = default;

    [[noreturn]] void run();

private:
    TcpProtocol _tcp = TcpProtocol("0.0.0.0", 4242);

    std::vector<std::unique_ptr<Client>> _clients;

};


#endif //SERVER_SERVER_HPP
