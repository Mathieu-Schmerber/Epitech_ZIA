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
    size_t posBegin;
    size_t posEnd;

    if (_sTcp->userDisconnected() && (idDisconnect = _sTcp->getNewDisconnect()) != 0) {
    }
    if ((receive = _sTcp->getNewMessage()).id != 0) {
        if ((receive.receive == "\r\n" || (receive.receive.length() >= 4 && receive.receive.substr(receive.receive.size() - 4) == "\r\n\r\n")) && std::find(_readyContentLength.begin(), _readyContentLength.end(), receive.id) == _readyContentLength.end()) {
            if (receive.receive != "\r\n")
                _fullReceive[receive.id] += receive.receive;
            else if (_contentLength.find(receive.id) != _contentLength.end() &&
                     _contentLength.find(receive.id)->second > 0) {
                _readyContentLength.emplace_back(receive.id);
                _fullReceive[receive.id] += receive.receive;
                return;
            }
            _outQueue.emplace_back(_fullReceive[receive.id], receive.id);
            _fullReceive.erase(receive.id);
        } else if (std::find(_readyContentLength.begin(), _readyContentLength.end(), receive.id) == _readyContentLength.end()) {
            if ((posBegin = receive.receive.find("Content-Length:")) != std::string::npos) {
                for (posEnd = posBegin + 16; isdigit(receive.receive[posEnd]); ++posEnd);
                _contentLength[receive.id] = std::atoi(receive.receive.substr(posBegin + 16, posEnd - (posBegin + 16)).c_str());
            }
            posEnd = receive.receive.find("\r\n\r\n");
            if (posEnd != std::string::npos && _contentLength.find(receive.id) != _contentLength.end() && _contentLength.find(receive.id)->second > 0) {
                _fullReceive[receive.id] += receive.receive;
                LOG(DEBUG) << _fullReceive[receive.id];
                _outQueue.emplace_back(_fullReceive[receive.id], receive.id);
                _fullReceive.erase(receive.id);
                _contentLength[receive.id] = 0;
            }
            if (std::find(_readyContentLength.begin(), _readyContentLength.end(), receive.id) == _readyContentLength.end())
                _fullReceive[receive.id] += receive.receive;
        }
        if (std::find(_readyContentLength.begin(), _readyContentLength.end(), receive.id) != _readyContentLength.end()) {
            if (static_cast<int>(receive.receive.length()) <= _contentLength[receive.id]) {
                _fullReceive[receive.id] += receive.receive;
                _contentLength[receive.id] -= static_cast<int>(receive.receive.length());
            } else {
                _fullReceive[receive.id] += receive.receive.substr(0, _contentLength[receive.id]);
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
