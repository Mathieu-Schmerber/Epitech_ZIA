//
// Created by Cyprien on 12/6/2020.
//

#include "Server.hpp"
#include "ConfigurationHandler.hpp"

using namespace rapidjson;

int main() {
//    ConfigurationHandler configurationHandler;
//
//    configurationHandler.loadConfiguration("config.json");
//    Sleep(15000);
//    configurationHandler.loadConfiguration("config.json");

    Server _server;
    _server.run();
    return (0);
}