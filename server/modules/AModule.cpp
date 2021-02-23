//
// Created by Emilien on 18/02/2021.
//

#include "AModule.hpp"
#include <iostream>

AModule::AModule(const std::string &name) : _running(false), _name(name) {}

void AModule::run()
{
    std::cout << "Start " << this->_name << " Module" << std::endl;
    while (getStatus())
        this->handleQueue();
    std::cout << "End " << this->_name << " Module" << std::endl;
}

//region Queue handling
/**
 * \brief Push new request to be handle
 **/
void AModule::dataInput(const std::string &str, int id)
{
    this->_inQueue.emplace_back(str, id);
}

/**
 * \brief Pop and returns the top of the handled requests list
 **/
std::pair<std::string, int> AModule::dataOutput()
{
    if (this->_outQueue.empty())
        return {};
    std::pair<std::string, int> out = this->_outQueue.front();

    this->_outQueue.erase(this->_outQueue.begin());
    return out;
}

/**
 * \brief Looping from AModule::run() method while AModule::_running
 **/
void AModule::handleQueue() {}
//endregion

//region Getters and setters
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
//endregion