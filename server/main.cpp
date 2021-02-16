//
// Created by Cyprien on 12/6/2020.
//

#include "Server.hpp"
#include "ConfigurationHandler.hpp"
#include "RequestParser.hpp"

using namespace rapidjson;

int main() {
//    ConfigurationHandler configurationHandler;
//
//    configurationHandler.loadConfiguration("config.json");
//    Sleep(15000);
//    configurationHandler.loadConfiguration("config.json");

//    Server _server;
//    _server.run();

    ZiaRequest::RequestParser newElem(std::string("GET /hello HTTP/1.1\r\nHost: www.emiliendelevoye.fr\r\nAccept-Language: fr\r\n"));

    newElem.parseData();
    std::unique_ptr<ZiaRequest::Request> &request = newElem.getRequest();
    //std::cout << "Type -> " << ZiaRequest::requestTypesNames[request->getRequestType()] << std::endl;
    //std::cout << "Path -> " << request->getRequestPath() << std::endl;
    return (0);
}