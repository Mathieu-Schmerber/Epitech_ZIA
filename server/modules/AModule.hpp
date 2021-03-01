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
    void loadConfigFile(const std::string &configFilePath) override {};

    void dataInput(const std::string &, int id) override;
    std::pair<std::string, int> dataOutput() override;

    bool isInputData() override = 0;
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

    virtual void handleQueue();
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
    [[nodiscard]] virtual std::string getFileExtension() const = 0;
};

#endif //ZIA_AMODULE_HPP
