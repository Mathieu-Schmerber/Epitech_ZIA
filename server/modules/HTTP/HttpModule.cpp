//
// Created by Emilien on 17/02/2021.
//

#include "IModule.hpp"
#include "HttpModule.hpp"
#include <iostream>

#if defined(_WIN32) || defined(WIN32)
    #include <Windows.h>
    #include <cstdio>
#endif

extern "C" {
    #if defined(_WIN32) || defined(WIN32)
        __declspec(dllexport)
    #endif

    IModule* newInstance()
    {
        return new HTTPModule();
    }
}

void HTTPModule::loadConfigFile(const std::string &configFilePath)
{
}

void HTTPModule::dataInput(const std::string &input)
{
}

std::string HTTPModule::dataOutput()
{
    return std::string("");
}

bool HTTPModule::isInputData()
{
    return true;
}

void HTTPModule::run()
{
    std::cout << "Start HTTP Module" << std::endl;
    while (getStatus()) {

    }
    std::cout << "End HTTP Module" << std::endl;
}