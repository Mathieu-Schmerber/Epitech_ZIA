//
// Created by Emilien on 17/02/2021.
//

#ifndef ZIA_HTTPMODULE_HPP
#define ZIA_HTTPMODULE_HPP

#include "AModule.hpp"
#include <string>
#include <thread>

class HTTPModule : public AModule {
public:
    explicit HTTPModule() = default;

    void loadConfigFile(const std::string &configFilePath) override;
    void dataInput(const std::string &input, int id) override;
    std::pair<std::string, int> dataOutput() override;
    bool isInputData() override;
    void run() override;
private:
};

#endif //ZIA_HTTPMODULE_HPP
