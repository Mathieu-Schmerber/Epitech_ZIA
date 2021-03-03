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

HTTPModule::HTTPModule() : AModuleInput("HTTP")
{
}

void HTTPModule::loadConfigFile(const std::string &configFilePath)
{
    ConfigurationHandler config = ConfigurationHandler();
    _port = config.loadHttpModule(configFilePath);
    std::cout << _port << std::endl;
}

/**
 * \brief Looping on AModule::run() while AModule::_running
 * **/
void HTTPModule::handleQueue()
{
    ReceiveData receive;
    std::pair<std::string, int> in;
    if (!(receive = _sTcp->getNewMessage()).receive.empty()) {
        _outQueue.emplace_back(receive.receive, receive.id);
    }
    if ((in = getInput()).second != -1) {
        _sTcp->send(in.second, in.first);
    }
}

std::pair<std::string, int> HTTPModule::getInput()
{
    if (_inQueue.empty())
        return {};
    std::pair<std::string, int> in = _inQueue.front();
    _inQueue.erase(_inQueue.begin());
    return in;
}

void HTTPModule::startModule()
{
    AModule::startModule();
    HTTPModule::loadConfigFile(_filePath);
    _sTcp = new TcpProtocol("0.0.0.0", _port);
}

void HTTPModule::stopModule()
{
    AModule::stopModule();
    delete _sTcp;
}
