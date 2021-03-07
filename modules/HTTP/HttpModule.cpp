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
    if (_port == 0 || _port < 1025 || _port > 49150)
        _port = 80;
    LOG(INFO) << "HTTP module use port: " << _port;
}

static void print_buf(const char *title, const unsigned char *buf, size_t buf_len)
{
    size_t i = 0;
    fprintf(stdout, "%s\n", title);
    for(i = 0; i < buf_len; ++i)
        fprintf(stdout, "%02X%s", buf[i],
                ( i + 1 ) % 16 == 0 ? "\r\n" : " " );

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

    std::lock_guard<std::recursive_mutex> lock(_mutex);
    if (_sTcp->userDisconnected() && (idDisconnect = _sTcp->getNewDisconnect()) != 0) {
    }
    if ((receive = _sTcp->getNewMessage()).id != 0) {
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
//        if (receive.receive == "\r\n" || (receive.receive.length() >= 4 && receive.receive.substr(receive.receive.size() - 4) == "\r\n\r\n")) {
//            if (receive.receive != "\r\n")
//                _fullReceive[receive.id] += receive.receive;
//            _outQueue.emplace_back(_fullReceive[receive.id], receive.id);
//            _fullReceive.erase(receive.id);
//        } else {
//            _fullReceive[receive.id] += receive.receive;
//        }
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
    std::lock_guard<std::recursive_mutex> locker(this->_mutex);
    if (_inQueueInput.empty())
        return {};
    std::pair<std::string, int> in = _inQueueInput.front();
    _inQueueInput.erase(_inQueueInput.begin());
    return in;
}

void HTTPModule::startModule()
{
    HTTPModule::loadConfigFile(_filePath);
    try {
        _sTcp = new TcpProtocol("0.0.0.0", _port);
    } catch (boost::system::system_error &error) {
        LOG(ERR) << error.what();
        return;
    } catch (std::exception &error) {
        LOG(ERR) << error.what();
        return;
    }
    AModule::startModule();
}

void HTTPModule::stopModule()
{
    delete _sTcp;
    AModule::stopModule();
}
