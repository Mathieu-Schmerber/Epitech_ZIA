//
// Created by Emilien on 17/02/2021.
//

#include "IModule.hpp"
#include "HttpsModule.hpp"
#include "ConfigurationHandler.hpp"
#include <iostream>

extern "C" {
    #if defined(_WIN32) || defined(WIN32)
        __declspec(dllexport)
    #endif

    IModule* newInstance()
    {
        return new HTTPSModule();
    }
}

/**
 * \brief HTTPS Module constructor
 *
**/
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
    _port = config.getInt(configFilePath, "port");
    if (_port == 0 || _port < 1025 || _port > 49150)
            _port = 443;
    LOG(INFO) << "HTTPS module use port: " << _port;
}

/**
 * \brief Looping on AModule::run() while AModule::_running
 *
**/
void HTTPSModule::handleQueue()
{
    ReceiveData receive;
    std::pair<std::string, int> in;
    int idDisconnect;
    if (_sTcp->userDisconnected() && (idDisconnect = _sTcp->getNewDisconnect()) != 0) {
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
    HTTPSModule::loadConfigFile(_filePath);
    try {
        LOG(DEBUG) << "port " << _port;
        _sTcp = new TcpProtocol("0.0.0.0", _port);
        LOG(DEBUG) << "_sTcp created";
    } catch (boost::system::system_error &error) {
        LOG(ERR) << error.what();
        return;
    } catch (std::exception &error) {
        LOG(ERR) << error.what();
        return;
    }
    AModule::startModule();
}

void HTTPSModule::stopModule()
{
    delete _sTcp;
    AModule::stopModule();
}