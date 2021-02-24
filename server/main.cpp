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
    ZiaRequest::RequestParser newElem(std::string("GET /hello HTTP/1.1\r\nHost: www.emiliendelevoye.fr\r\nAccept-Language: fr\r\npouet: coucou"));

    newElem.parseData();
    std::unique_ptr<ZiaRequest::Request> &request = newElem.getRequest();
    std::cout << "Type -> " << ZiaRequest::requestTypesNames[request->getRequestType()] << std::endl;
    std::cout << "Path -> " << request->getRequestPath() << std::endl;
    for (const auto& a : request->getRequestHeaders())
        std::cout << "[" << a.first << "] -> [" << a.second << "]" << std::endl;
}

// \TMP

#include "Router/Router.hpp"

// TMP\

int main() {
    //fct_server();
    //fct_request();


    try {
        std::cout << "testing" << std::endl;
        auto *router = new Router();

        router->init();
        std::cout << "initialized" << std::endl;
        router->addRoute("/test");
        std::cout << "?" << std::endl;
        router->addRoute("/zizi");
        //router.addRoute("/pouet", "/php_rsc");
    } catch (std::exception &err) {
        std::cerr << err.what() << std::endl;
    }
    return (0);
}