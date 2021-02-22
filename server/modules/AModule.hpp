/**
 * \file AModule.hpp
 * \brief Abstract for the modules
 * \author Emilien.D
**/

#ifndef ZIA_AMODULE_HPP
#define ZIA_AMODULE_HPP

#include "IModule.hpp"

/**
 * \class AModule AModule.hpp "AModule.hpp"
 * \brief Abstract of the IModule interface
**/

class AModule : public IModule {
public:
    explicit AModule();
    void loadConfigFile(const std::string &configFilePath) override {};
    void dataInput(const std::string &, int id) override {};
    std::pair<std::string, int> dataOutput() override {
        return std::pair<std::string, int>("", 0);
    };
    bool isInputData() override {
        return false;
    };
    void run() override;
    bool getStatus() final;
    void startModule() final;
    void stopModule() final;
private:
    bool _running;
};

#endif //ZIA_AMODULE_HPP
