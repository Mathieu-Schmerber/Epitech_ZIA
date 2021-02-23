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
    explicit HTTPModule();
    void loadConfigFile(const std::string &configFilePath) override;

private:
    void handleQueue() override;
};

#endif //ZIA_HTTPMODULE_HPP
