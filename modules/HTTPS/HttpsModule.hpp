/**
 * \file HttpsModule.hpp
 * \brief Https Module
 * \author Cyprien.R
**/

#ifndef ZIA_HTTPSMODULE_HPP
#define ZIA_HTTPSMODULE_HPP

#include "AModule.hpp"
#include "TcpProtocol.hpp"
#include <string>
#include <thread>

/**
 * \class HTTPSModule HTTPSModule.hpp "HTTPSModule.hpp"
 * \brief Class HTTPSModule to receive and send https messages
**/

class HTTPSModule : public AModuleInput {
public:
    explicit HTTPSModule();
    void loadConfigFile(const std::string &configFilePath) override;
    std::pair<std::string, int> getInput();
    void startModule() override;
    void stopModule() override;

    private:
    void handleQueue() override;
    const std::string _filePath = "./https.json";
    std::string _certificate = "./certs/certificate.pem";
    std::string _key = "./certs/key.pem";
    std::map<int, std::string> _fullReceive;
    int _port;
    TcpProtocol *_sTcp;
};

#endif //ZIA_HTTPSMODULE_HPP
