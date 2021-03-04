//
// Created by Emilien on 17/02/2021.
//

#ifndef ZIA_HTTPSMODULE_HPP
#define ZIA_HTTPSMODULE_HPP

#include "AModule.hpp"
#include "TcpProtocol.hpp"
#include <string>
#include <thread>

class HTTPSModule : public AModule {
public:
    explicit HTTPSModule();
    void loadConfigFile(const std::string &configFilePath) override;
    std::pair<std::string, int> getInput();
    void startModule() override;
    void stopModule() override;

    private:
    void handleQueue() override;
    const std::string _filePath = "./https.json";
    std::map<int, std::string> _fullReceive;
    int _port;
    TcpProtocol *_sTcp;
};

#endif //ZIA_HTTPSMODULE_HPP
