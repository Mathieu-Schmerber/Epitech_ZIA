//
// Created by Emilien on 17/02/2021.
//

#ifndef ZIA_HTTPSMODULE_HPP
#define ZIA_HTTPSMODULE_HPP

#include "AModule.hpp"
#include "TcpProtocol.hpp"
#include <string>
#include <thread>

#include <openssl/ssl.h>
#include <openssl/err.h>

class HTTPSModule : public AModule {
public:
    explicit HTTPSModule();
    void loadConfigFile(const std::string &configFilePath) override;
    std::pair<std::string, int> getInput();
    void startModule() override;
    void stopModule() override;


    SSL_CTX *ctx;


    private:
    void handleQueue() override;
    const std::string _filePath = "./https.json";
    int _port;
    TcpProtocol *_sTcp;
};

#endif //ZIA_HTTPSMODULE_HPP
