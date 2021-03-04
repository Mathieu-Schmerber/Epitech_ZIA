/**
 * \file HttpsModule.hpp
 * \brief Http Module
 * \author Emilien.D
**/


#ifndef ZIA_HTTPMODULE_HPP
#define ZIA_HTTPMODULE_HPP

#include "AModule.hpp"
#include "TcpProtocol.hpp"
#include <string>
#include <thread>

/**
 * \class HTTPModule HTTPModule.hpp "HTTPModule.hpp"
 * \brief Class HTTPModule to receive and send http messages
**/

class HTTPModule : public AModuleInput {
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
    std::map<int, std::string> _fullReceive;
    int _port;
    TcpProtocol *_sTcp;
};

#endif //ZIA_HTTPMODULE_HPP
