//
// Created by Emilien on 17/02/2021.
//

#include "IModule.hpp"
#include "HttpsModule.hpp"
#include "ConfigurationHandler.hpp"
#include <iostream>


//#include <openssl/ssl.h>
//#include <openssl/err.h>


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
        return new HTTPSModule();
    }
}

HTTPSModule::HTTPSModule() : AModule("HTTPS")
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

static void print_buf(const char *title, const unsigned char *buf, size_t buf_len)
{
    size_t i = 0;
    fprintf(stdout, "%s\n", title);
    for(i = 0; i < buf_len; ++i)
        fprintf(stdout, "%02X%s", buf[i],
                ( i + 1 ) % 16 == 0 ? "\r\n" : " " );

}

//void log_ssl()
//{
//    unsigned long err;
//    err = ERR_get_error();
//    while (err) {
//        char *str = ERR_error_string(err, nullptr);
//        if (!str)
//            return;
//        std::cout << str << std::endl;
//        fflush(stdout);
//        err = ERR_get_error();
//    }
//}

/**
 * \brief Looping on AModule::run() while AModule::_running
 *
**/
void HTTPSModule::handleQueue()
{
    ReceiveData receive;
    std::pair<std::string, int> in;


//    SSL *ssl;
//    int sock;


    if (!(receive = _sTcp->getNewMessage()).receive.empty()) {
        print_buf("Message received :", reinterpret_cast<const unsigned char *>(receive.receive.data()), receive.receive.length());
        _outQueue.emplace_back(receive.receive, receive.id);
//        SSL_library_init();
//        SSLeay_add_ssl_algorithms();
//        SSL_load_error_strings();
//        const SSL_METHOD *meth = SSLv23_server_method();
//        SSL_CTX *ctx = SSL_CTX_new(meth);
//        ssl = SSL_new(ctx);
//        if (!ssl) {
//            std::cout << "Error creating SSL." << std::endl;
//            log_ssl();
//            return;
//        }
//        sock = SSL_get_fd(ssl);
//        SSL_set_fd(ssl, s);
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
        std::cout << error.what() << std::endl;
        exit(84);
    }
}

void HTTPSModule::stopModule()
{
    AModule::stopModule();
    delete _sTcp;
}
