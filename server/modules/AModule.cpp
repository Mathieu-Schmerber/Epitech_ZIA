//
// Created by Emilien on 18/02/2021.
//

#include "AModule.hpp"
#include <iostream>

AModule::AModule() : _running(false)
{

}

void AModule::run()
{
    std::cout << "???" << std::endl;
}

bool AModule::getStatus()
{
    return _running;
}

void AModule::startModule()
{
    _running = true;
}

void AModule::stopModule()
{
    _running = false;
}
