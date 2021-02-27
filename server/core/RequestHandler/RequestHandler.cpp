/*
** EPITECH PROJECT, 2020
** Zia
** File description:
** Created by Emilien
*/

#include "RequestHandler.hpp"
#include <iostream>

RequestHandler::RequestHandler(int id) : _thread(&RequestHandler::run, this), _running(true), _requestHandlerId(id), _state(READY)
{}

RequestHandler::~RequestHandler()
{
    _running = false;
    _thread.join();
}

void RequestHandler::run()
{
    std::cout << "Thread " << _requestHandlerId << " started." << std::endl;
    while (_running) {
    }
    std::cout << "Thread " << _requestHandlerId << " stopped." << std::endl;
}

ThreadState RequestHandler::getState() const
{
    return _state;
}

std::pair<std::string, std::map<std::string, int>> RequestHandler::getProcessedRequest()
{
    std::map<std::string, int> toReturn;

    toReturn["requestID"] = _requestId;
    toReturn["moduleID"] = _moduleId;
    return std::pair<std::string, std::map<std::string, int>>(_response, toReturn);
    _state = READY;
}

void RequestHandler::setRequestToProcess(const std::pair<std::string, std::map<std::string, int>>& request)
{
    _state = PROCESSING;
}

