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
    std::string certificate = config.getString(configFilePath, "certificate");
    std::string key = config.getString(configFilePath, "key");
    if (!certificate.empty())
        _certificate = certificate;
    if (!key.empty())
        _key = key;
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

    std::lock_guard<std::recursive_mutex> lock(_mutex);
    if (_sTcp->userDisconnected() && (idDisconnect = _sTcp->getNewDisconnect()) != 0) {
    }
    if (!(receive = _sTcp->getNewMessage()).receive.empty()) {
        std::istringstream requestToParse(receive.receive);
        std::string block;
        while (std::getline(requestToParse, block, '\n')) {
            if (std::find(_readyContentLength.begin(), _readyContentLength.end(), receive.id) == _readyContentLength.end()) {
                if (block.starts_with("Content-Length:"))
                    _contentLength[receive.id] = std::atoi(block.substr(16, block.length() - 17).c_str());
                if (block == "\r") {
                    if (_contentLength.find(receive.id) != _contentLength.end() && _contentLength[receive.id] != 0) {
                        _readyContentLength.emplace_back(receive.id);
                    } else {
                        _outQueue.emplace_back(_fullReceive[receive.id], receive.id);
                        _fullReceive.erase(receive.id);
                        break;
                    }
                }
                if (block.ends_with('\r'))
                    _fullReceive[receive.id] += (block + '\n');
                else
                    _fullReceive[receive.id] += block;
            } else {
                if (block.length() <= _contentLength[receive.id]) {
                    _fullReceive[receive.id] += block;
                    _contentLength[receive.id] -= static_cast<int>(block.length());
                } else {
                    _fullReceive[receive.id] += block.substr(0, _contentLength[receive.id]);
                    _contentLength[receive.id] = 0;
                }
                if (_contentLength[receive.id] <= 0) {
                    _outQueue.emplace_back(_fullReceive[receive.id], receive.id);
                    _fullReceive.erase(receive.id);
                    _contentLength[receive.id] = 0;
                    _readyContentLength.erase(std::remove(_readyContentLength.begin(), _readyContentLength.end(), receive.id), _readyContentLength.end());
                }
            }
        }
    }
    if ((in = getInput()).second != -1) {
        _sTcp->send(in.second, in.first);
    }
}

std::pair<std::string, int> HTTPSModule::getInput()
{
    std::lock_guard<std::recursive_mutex> locker(this->_mutex);
    if (_inQueueInput.empty())
        return {};
    std::pair<std::string, int> in = _inQueueInput.front();
    _inQueueInput.erase(_inQueueInput.begin());
    return in;
}

void HTTPSModule::startModule()
{
    HTTPSModule::loadConfigFile(_filePath);
    try {
        _sTcp = new TcpProtocol("0.0.0.0", _port, _certificate, _key);
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