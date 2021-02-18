//
// Created by Emilien on 18/02/2021.
//

#include "AModule.hpp"

AModule::AModule() : _running(false)
{

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
