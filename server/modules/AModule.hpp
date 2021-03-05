/**
 * \file AModule.hpp
 * \brief Abstract for the modules
 * \author Emilien.D
**/

#ifndef ZIA_AMODULE_HPP
#define ZIA_AMODULE_HPP

#include <vector>
#include "IModule.hpp"

/**
 * \class AModule AModule.hpp "AModule.hpp"
 * \brief Abstract of the IModule interface
**/

class AModule : public IModule {
public:
    explicit AModule(const std::string &name);
    void loadConfigFile([[maybe_unused]]const std::string &configFilePath) override {};

    void dataInput(const std::string &, int id) override;
    std::pair<std::string, int> dataOutput() override;
    [[nodiscard]] std::string getFileExtension() const override { return ""; };

    bool isInputData() override { LOG(ERR) << "[AModule::isInputData] This function must be override"; return false; }; // Has to be defaulted to be able to move on with an instance of this class
    void run() final;
    bool getStatus() final;
    void startModule() override;
    void stopModule() override;

private:
    bool _running;

protected:
    const std::string _name;
    std::vector<std::pair<std::string, int>> _inQueue;
    std::vector<std::pair<std::string, int>> _outQueue;

    virtual void handleQueue() { LOG(ERR) << "[AModule::handleQueue] This function must be override";}; // Has to be defaulted to be able to move on with an instance of this class
};

class AModuleInput : public AModule {
public:
    explicit AModuleInput(const std::string &name) : AModule(name) {}

    bool isInputData() override { return true; }
};

class AModuleOutput : public AModule {
public:
    explicit AModuleOutput(const std::string &name) : AModule(name) {}

    bool isInputData() override { return false; }
    [[nodiscard]] std::string getFileExtension() const override { return ""; };
};

#endif //ZIA_AMODULE_HPP
