//
// Created by Emilien on 17/02/2021.
//

#include "IModule.hpp"
#include "HttpModule.hpp"
#include "ConfigurationHandler.hpp"
#include <iostream>

#if defined(_WIN32) || defined(WIN32)
    #include <Windows.h>
    #include <cstdio>
#endif

extern "C" {
    #if defined(_WIN32) || defined(WIN32)
        __declspec(dllexport)
    #endif

    IModule* newInstance()
    {
        return new HTTPModule();
    }
}

HTTPModule::HTTPModule() : AModule("HTTP")
{
    HTTPModule::loadConfigFile(filePath);
}

void HTTPModule::loadConfigFile(const std::string &configFilePath)
{
    ConfigurationHandler config = ConfigurationHandler();
    int port = config.loadHttpModule(configFilePath);
    std::cout << port << std::endl;
}

/**
 * \brief Looping on AModule::run() while AModule::_running
 * **/
void HTTPModule::handleQueue()
{

}