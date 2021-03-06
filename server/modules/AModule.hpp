/**
 * \file AModule.hpp
 * \brief Abstract for the modules
 * \author Emilien.D
**/

#ifndef ZIA_AMODULE_HPP
#define ZIA_AMODULE_HPP

#include <vector>
#include "IModule.hpp"
#include "ZiaError.hpp"

/**
 * \class AModule AModule.hpp "AModule.hpp"
 * \brief Abstract of the IModule interface
**/

class AModule : public IModule {
public:
    explicit AModule(const std::string &name);
    void loadConfigFile([[maybe_unused]]const std::string &configFilePath) override {};

    void dataInput(const std::string &, int id) override;
    std::string processData(const ZiaRequest::Request &request) override { LOG(ERR) << "AModule::processData This function has to be overridden"; return std::string(); };
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
    std::vector<std::pair<std::string, int>> _inQueueInput;
    ZiaRequest::Request _requestToProcess;
    std::vector<std::pair<std::string, int>> _outQueue;

    virtual void handleQueue() { LOG(ERR) << "[AModule::handleQueue] This function must be override";}; // Has to be defaulted to be able to move on with an instance of this class
};

class AModuleInput : public AModule {
public:
    explicit AModuleInput(const std::string &name) : AModule(name) {}

    std::string processData([[maybe_unused]] const ZiaRequest::Request &request) final {
        throw ZiaError("AModuleInput", "process data method must not be used in an Input Module");
    }
    bool isInputData() override { return true; }
};

class AModuleOutput : public AModule {
public:
    explicit AModuleOutput(const std::string &name) : AModule(name) {}

    void dataInput([[maybe_unused]] const std::string &str, [[maybe_unused]] int id) final {
        throw ZiaError("AModuleOutput", "dataInput method must not be used in an Output Module");
    }
    std::string processData(const ZiaRequest::Request &request) final;
    bool isInputData() override { return false; }
    [[nodiscard]] std::string getFileExtension() const override { return ""; };
protected:
    std::string _response;
};

#endif //ZIA_AMODULE_HPP
