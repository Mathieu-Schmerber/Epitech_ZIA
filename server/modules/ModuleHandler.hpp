/**
 * \file ModuleHandler.hpp
 * \brief ModuleHandler classes
 * \author Emilien.D
**/

#ifndef ZIA_MODULEHANDLER_HPP
#define ZIA_MODULEHANDLER_HPP

#include "AModule.hpp"
#include <iostream>
#include <thread>

/**
 * \class ModuleHandler ModuleHandler.hpp "ModuleHandler.hpp"
 * \brief Class which contains the modules loaded by the server
**/

class ModuleHandler {
public:
    explicit ModuleHandler() : _module(nullptr) {};
    explicit ModuleHandler(AModule *module);
    void startModule();
    void stopModule();
    AModule *get();
    AModule *operator->();
private:
    AModule *_module;
    std::thread _thread;
};

#endif //ZIA_MODULEHANDLER_HPP
