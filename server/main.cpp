//
// Created by Cyprien on 12/6/2020.
//

#include "Server.hpp"
//#include "ConfigurationHandler.hpp"
#include "RequestParser.hpp"

//tmp
#include "DynamicLibManager.hpp"

//using namespace rapidjson;

void fct_server()
{
    //ConfigurationHandler configurationHandler;

    //configurationHandler.loadConfiguration("config.json");
    //Sleep(15000);
    //configurationHandler.loadConfiguration("config.json");
    /**ModuleLoader::DynamicLibManager dlManager;

    dlManager.loadNewLib<AModule>(DYNLIB("httpModule"));
    auto *pouet = dlManager.getInstance<AModule>(DYNLIB("httpModule"));
    std::cout << pouet << std::endl;
    pouet->startModule();
    pouet->stopModule();**/
    Server _server;

    _server.run();
}

void fct_request()
{
    ZiaRequest::RequestParser newElem;

    ZiaRequest::Request request = newElem.parseData(std::string("GET /hello HTTP/1.1\r\nHost: www.emiliendelevoye.fr\r\nAccept-Language: fr\r\npouet: coucou"));
    std::cout << "Type -> " << ZiaRequest::requestTypesNames[request.getRequestType()] << std::endl;
    std::cout << "Path -> " << request.getRequestPath() << std::endl;
    for (const auto& a : request.getRequestHeaders())
        std::cout << "[" << a.first << "] -> [" << a.second << "]" << std::endl;
}

int main() {
    fct_server();
    //fct_request();

    return (0);
}
