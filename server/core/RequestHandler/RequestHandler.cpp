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
        if (_state == PROCESSING) {
            _response = _request;
            std::cout << "Request " << _requestId << " processed (" << _moduleName << ")" << std::endl;
            _state = PROCESSED;
        }
    }
    std::cout << "Thread " << _requestHandlerId << " stopped." << std::endl;
}

ThreadState RequestHandler::getState() const
{
    return _state;
}

std::pair<std::string, std::pair<std::string, int>> RequestHandler::getProcessedRequest()
{
    _state = READY;
    return std::pair<std::string, std::pair<std::string, int>>(_response, {_moduleName, _requestId});
}

void RequestHandler::setRequestToProcess(const std::pair<std::string, std::pair<std::string, int>>& request)
{
    _state = PROCESSING;
    _request = request.first;
    _moduleName = request.second.first;
    _requestId = request.second.second;
}

