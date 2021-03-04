//
// Created by Emilien on 18/02/2021.
//

#include "AModule.hpp"
#include <iostream>

/**
 * \brief AModule constructor
 *
 * \param name : set the module name
 **/
AModule::AModule(const std::string &name) : _running(false), _name(name) {}

/**
 * \brief run keep calling handle queue while the module is running
 **/
void AModule::run()
{
    std::cout << "Start " << this->_name << " Module" << std::endl;
    while (getStatus()) {
        this->handleQueue();
    }
    std::cout << "End " << this->_name << " Module" << std::endl;
}

//region Queue handling
/**
 * \brief Push new request to be handled
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
        return std::pair<std::string, int>("", -1);
    std::pair<std::string, int> out = this->_outQueue.front();

    this->_outQueue.erase(this->_outQueue.begin());
    return out;
}
/// End Region

/// Region Getters and setters

/**
 * \brief Get the status module
 *
 * \return true or false according to the running status of the module
 **/
bool AModule::getStatus()
{
    return _running;
}

/**
 * \brief Start the module
 **/
void AModule::startModule()
{
    _running = true;
}

/**
 * \brief Stop the module
 **/
void AModule::stopModule()
{
    _running = false;
}
/// End Region