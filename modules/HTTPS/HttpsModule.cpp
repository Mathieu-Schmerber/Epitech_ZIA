//
// Created by Emilien on 17/02/2021.
//

#include "IModule.hpp"
#include "HttpsModule.hpp"
#include "ConfigurationHandler.hpp"
#include <iostream>

#if defined(_WIN32) || defined(WIN32)
    #include <windows.h>
    #include <cstdio>
#endif

extern "C" {
    #if defined(_WIN32) || defined(WIN32)
        __declspec(dllexport)
    #endif

    IModule* newInstance()
    {
        return new HTTPSModule();
    }
}

HTTPSModule::HTTPSModule() : AModuleInput("HTTPS"), _port(443), _sTcp(nullptr)
{
}

/**
 * \brief Load https configuration file
 *
 * \param configFilePath file path to the configuration file
 *
**/
void HTTPSModule::loadConfigFile(const std::string &configFilePath)
{
    ConfigurationHandler config = ConfigurationHandler();
    _port = config.loadHttpsModule(configFilePath);
    std::cout << _port << std::endl;
}

/**
 * \brief Looping on AModule::run() while AModule::_running
 *
**/
void HTTPSModule::handleQueue()
{
    ReceiveData receive;
    std::pair<std::string, int> in;
    std::string ipDisconnect;
    if (_sTcp->userDisconnected() && !(ipDisconnect = _sTcp->getNewDisconnect()).empty()) {
    }
    if (!(receive = _sTcp->getNewMessage()).receive.empty()) {
        if (receive.receive == "\r\n" || (receive.receive.length() >= 4 && receive.receive.substr(receive.receive.size() - 4) == "\r\n\r\n")) {
            if (receive.receive != "\r\n")
                _fullReceive[receive.id] += receive.receive;
            _outQueue.emplace_back(_fullReceive[receive.id], receive.id);
            _fullReceive[receive.id].clear();
        } else {
            _fullReceive[receive.id] += receive.receive;
        }
//        print_buf("Message received :", reinterpret_cast<const unsigned char *>(receive.receive.data()), receive.receive.length());
    }
    if ((in = getInput()).second != -1) {
        _sTcp->send(in.second, in.first);
    }
}

std::pair<std::string, int> HTTPSModule::getInput()
{
    if (_inQueue.empty())
        return {};
    std::pair<std::string, int> in = _inQueue.front();
    _inQueue.erase(_inQueue.begin());
    return in;
}

void HTTPSModule::startModule()
{
    AModule::startModule();
    HTTPSModule::loadConfigFile(_filePath);
    try {
        _sTcp = new TcpProtocol("0.0.0.0", _port);
    } catch (boost::system::system_error &error) {
        std::cerr << error.what() << std::endl;
        stopModule();
    }
}

void HTTPSModule::stopModule()
{
    AModule::stopModule();
    delete _sTcp;
}
