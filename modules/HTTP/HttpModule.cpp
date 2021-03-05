//
// Created by Emilien on 17/02/2021.
//

#include "IModule.hpp"
#include "HttpModule.hpp"
#include "ConfigurationHandler.hpp"
#include <iostream>

extern "C" {
    #if defined(_WIN32) || defined(WIN32)
        __declspec(dllexport)
    #endif

    IModule* newInstance()
    {
        return new HTTPModule();
    }
}

/**
 * \brief HTTP Module constructor
 *
**/
HTTPModule::HTTPModule() : AModuleInput("HTTP")
{
}

void HTTPModule::loadConfigFile(const std::string &configFilePath)
{
    ConfigurationHandler config = ConfigurationHandler();
    _port = config.getInt(configFilePath, "port");
    if (_port == 0)
        _port = 80;
    LOG(INFO) << "HTTP module use port: " << _port;
}

/**
 * \brief Looping on AModule::run() while AModule::_running
 *
**/
void HTTPModule::handleQueue()
{
    ReceiveData receive;
    std::pair<std::string, int> in;
    int idDisconnect;
    if (_sTcp->userDisconnected() && (idDisconnect = _sTcp->getNewDisconnect()) != 0) {
    }
    if ((receive = _sTcp->getNewMessage()).id != 0) {
        if (receive.receive == "\r\n" || (receive.receive.length() >= 4 && receive.receive.substr(receive.receive.size() - 4) == "\r\n\r\n")) {
            if (receive.receive != "\r\n")
                _fullReceive[receive.id] += receive.receive;
            _outQueue.emplace_back(_fullReceive[receive.id], receive.id);
            _fullReceive[receive.id].clear();
        } else {
            _fullReceive[receive.id] += receive.receive;
        }
    }
    if ((in = getInput()).second != -1) {
        _sTcp->send(in.second, in.first);
    }
}

/**
 * \brief GetInput remove received message from queue and return it
 *
 * \return pair with message and the client id
**/
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
    try {
        _sTcp = new TcpProtocol("0.0.0.0", _port);
    } catch (boost::system::system_error &error) {
        LOG(ERR) << error.what();
        stopModule();
    }
}

void HTTPModule::stopModule()
{
    AModule::stopModule();
    delete _sTcp;
}
