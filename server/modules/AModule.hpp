//
// Created by Emilien on 18/02/2021.
//

#ifndef ZIA_AMODULE_HPP
#define ZIA_AMODULE_HPP

#include "IModule.hpp"

class AModule : public IModule {
public:
    explicit AModule();
    void loadConfigFile(const std::string &configFilePath) override = 0;
    void dataInput(const std::string &) override = 0;
    std::string dataOutput() override = 0;
    bool isInputData() override = 0;
    void run() override;
    bool getStatus() final;
    void startModule() final;
    void stopModule() final;
private:
    bool _running;
};

#endif //ZIA_AMODULE_HPP
