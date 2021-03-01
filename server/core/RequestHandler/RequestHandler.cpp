/*
** EPITECH PROJECT, 2020
** Zia
** File description:
** Created by Emilien
*/

#include "RequestHandler.hpp"
#include "RequestParser.hpp"
#include "ExceptionCore.hpp"
#include "Response.hpp"
#include "Router.hpp"
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
        if (_state == PROCESSING)
            _processRequest();
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

void RequestHandler::_processRequest()
{
    ZiaRequest::RequestParser requestParser;
    Response response;
    Router router;
    std::string fileContent;
    ZiaRequest::Request requestParsed;

    try {
        router.init();
        requestParsed = requestParser.parseData(_request);
        fileContent = router.get("/", requestParsed.getRequestPath());
        _response = response.getResponse(fileContent, "OK", 200);
    } catch (const CoreError &e) {
        _response = response.getResponse(e.what(), e.what(), e.getErrorCode());
    }
    _state = PROCESSED;
}