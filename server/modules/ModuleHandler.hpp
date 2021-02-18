//
// Created by Emilien on 18/02/2021.
//

#ifndef ZIA_MODULEHANDLER_HPP
#define ZIA_MODULEHANDLER_HPP

#include "AModule.hpp"
#include <iostream>
#include <thread>

class ModuleHandler {
public:
    explicit ModuleHandler() : _module(nullptr) {};
    explicit ModuleHandler(AModule *module);
    void startModule();
    AModule *get();
    AModule *operator->();
private:
    AModule *_module;
    std::thread _thread;
};

#endif //ZIA_MODULEHANDLER_HPP
