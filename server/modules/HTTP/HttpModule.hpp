//
// Created by Emilien on 17/02/2021.
//

#ifndef ZIA_HTTPMODULE_HPP
#define ZIA_HTTPMODULE_HPP

#include "IModule.hpp"
#include <thread>

class HTTPModule : public IModule {
public:
    explicit HTTPModule() = default;

    void loadConfigFile(const std::string &configFilePath) override;
    void dataInput(const std::string &input) override;
    std::string dataOutput() override;
    bool isInputData() override;
    void run() override;
private:
    std::thread _thread;
};

#endif //ZIA_HTTPMODULE_HPP
