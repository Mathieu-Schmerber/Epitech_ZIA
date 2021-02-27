//
// Created by Emilien on 17/02/2021.
//

#ifndef ZIA_HTTPMODULE_HPP
#define ZIA_HTTPMODULE_HPP

#include "AModule.hpp"
#include "TcpProtocol.hpp"
#include <string>
#include <thread>

class HTTPModule : public AModule {
public:
    explicit HTTPModule();
    void loadConfigFile(const std::string &configFilePath) override;
    std::pair<std::string, int> getInput();
    bool isInputData() override {
        return true;
    }
    void startModule() override;
    void stopModule() override;

    private:
    void handleQueue() override;
    const std::string _filePath = "./http.json";
    int _port;
    TcpProtocol *_sTcp;
};

#endif //ZIA_HTTPMODULE_HPP
