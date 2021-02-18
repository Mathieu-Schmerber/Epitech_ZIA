//
// Created by Emilien on 18/02/2021.
//

#include "ModuleHandler.hpp"
#include "ModuleException.hpp"
#include <thread>

ModuleHandler::ModuleHandler(AModule *module) : _module(module)
{
}

void ModuleHandler::startModule()
{
    if (!_module)
        throw ZiaModuleError("ModuleHandler", "Module not loaded in ModuleHandler");
    _module->startModule();
    _thread = std::thread(&AModule::run, _module);
}

AModule *ModuleHandler::get()
{
    if (!_module)
        throw ZiaModuleError("ModuleHandler", "Module not loaded in ModuleHandler");
    return _module;
}

AModule *ModuleHandler::operator->()
{
    if (!_module)
        throw ZiaModuleError("ModuleHandler", "Module not loaded in ModuleHandler");
    return this->get();
}