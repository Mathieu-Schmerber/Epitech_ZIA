/**
 * \file ModuleHandler.hpp
 * \brief ModuleHandler classes
 * \author Emilien.D
**/

#ifndef ZIA_MODULEHANDLER_HPP
#define ZIA_MODULEHANDLER_HPP

#include "AModule.hpp"
#include "ModuleException.hpp"
#include <iostream>
#include <thread>

/**
 * \class ModuleHandler ModuleHandler.hpp "ModuleHandler.hpp"
 * \brief Class which contains the modules loaded by the server
**/

class ModuleHandler {
public:
    virtual void startModule() = 0;
    virtual void stopModule() = 0;
    virtual AModule *get() = 0;
    virtual AModule *operator->() = 0;
};

class ModuleHandlerInput : public ModuleHandler {
public:
    explicit ModuleHandlerInput() : _module(nullptr) {};
    explicit ModuleHandlerInput(AModule *module) : _module(module) {}
    ~ModuleHandlerInput() {
        if (_thread && _thread->joinable())
            _thread->join();
    }
    void startModule() override {
        if (!_module)
            throw ZiaModuleError("ModuleHandler", "Module not loaded in ModuleHandler");
        _module->startModule();
        _thread = new std::thread(&AModule::run, _module);
    }
    void stopModule() override {
        if (!_module)
            throw ZiaModuleError("ModuleHandler", "Module not loaded in ModuleHandler");
        _module->stopModule();
        if (_thread && _thread->joinable())
            _thread->join();
    }
    AModule *get() override {
        if (!_module)
            throw ZiaModuleError("ModuleHandler", "Module not loaded in ModuleHandler");
        return _module;
    }
    AModule *operator->() override {
        if (!_module)
            throw ZiaModuleError("ModuleHandler", "Module not loaded in ModuleHandler");
        return this->get();
    }
private:
    AModule *_module = nullptr;
    std::thread *_thread = nullptr;
};

class ModuleHandlerOutput : public ModuleHandler {
public:
    explicit ModuleHandlerOutput() : _module(nullptr) {};
    explicit ModuleHandlerOutput(AModule *module) : _module(module) {}
    void startModule() override {
        if (!_module)
            throw ZiaModuleError("ModuleHandler", "Module not loaded in ModuleHandler");
        _module->startModule();
    }
    void stopModule() override {
        if (!_module)
            throw ZiaModuleError("ModuleHandler", "Module not loaded in ModuleHandler");
        _module->stopModule();
    }
    AModule *get() override {
        if (!_module)
            throw ZiaModuleError("ModuleHandler", "Module not loaded in ModuleHandler");
        return _module;
    }
    AModule *operator->() override {
        if (!_module)
            throw ZiaModuleError("ModuleHandler", "Module not loaded in ModuleHandler");
        return this->get();
    }
private:
    AModule *_module;
};

#endif //ZIA_MODULEHANDLER_HPP
