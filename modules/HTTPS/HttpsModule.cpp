//
// Created by Emilien on 17/02/2021.
//

#include "IModule.hpp"
#include "HttpsModule.hpp"
#include "ConfigurationHandler.hpp"
#include <iostream>

/* Make these what you want for certificate & key files */
#define CERT_FILE "./certs/certificate.pem"
#define KEY_FILE "./certs/key.pem"
/*Trusted CAs location*/
#define CA_FILE "./certs/certificate.pem"
#define CA_DIR  NULL

#include <synchapi.h>

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

HTTPSModule::HTTPSModule() : AModule("HTTPS")
{
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
    ctx = SSL_CTX_new(SSLv23_server_method());
    if (!ctx) {
        printf("Error creating the context.\n");
        exit(0);
    }
    SSL_CTX_set_ecdh_auto(ctx, 1);
    if (SSL_CTX_use_certificate_file(ctx, CERT_FILE, SSL_FILETYPE_PEM) <= 0) {
        printf("Error setting the certificate file.\n");
        exit(0);
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, KEY_FILE, SSL_FILETYPE_PEM) <= 0) {
        printf("Error setting the key file.\n");
        exit(0);
    }
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

/**
 * \brief Looping on AModule::run() while AModule::_running
 *
**/
void HTTPSModule::handleQueue()
{
    ReceiveData receive;
    std::pair<std::string, int> in;

    if (!(receive = _sTcp->getNewMessage()).receive.empty()) {
        SSL  *ssl;

//        print_buf("Message received :", reinterpret_cast<const unsigned char *>(receive.receive.data()), receive.receive.length());
        _outQueue.emplace_back(receive.receive, receive.id);


//        if (SSL_CTX_set_cipher_list(ctx, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH") <= 0) {
//            printf("Error setting the cipher list.\n");
//            exit(0);
//        }
//        if (!SSL_CTX_check_private_key(ctx)) {
//            printf("Private key does not match the certificate public key\n");
//            exit(0);
//        }
//        if (SSL_CTX_load_verify_locations(ctx, CA_FILE, CA_DIR) < 1) {
//            printf("Error setting the verify locations.\n");
//            exit(0);
//        }
        ssl=SSL_new(ctx);
        if (!ssl) {
            printf("Error creating SSL structure.\n");
            exit(0);
        }
        if (SSL_set_fd(ssl, receive.fd) <= 0) {
            printf("Error setting fd.\n");
            exit(0);
        }
        Sleep(2000);
        std::cout << "test2" << std::endl;
        int err = SSL_accept(ssl);
        std::cout << "test3" << std::endl;
        if (err < 1) {
            err = SSL_get_error(ssl,err);
            printf("SSL error #%d in SSL_accept,program terminated\n",err);
//            SSL_CTX_free(ctx);
//            exit(0);
        }
        printf("SSL connection on socket %d,Version: %s, Cipher: %s\n", receive.fd, SSL_get_version(ssl), SSL_get_cipher(ssl));
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
