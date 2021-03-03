/*
** EPITECH PROJECT, 2020
** Zia
** File description:
** Created by Emilien
*/

#include "RequestHandler.hpp"
#include "ExceptionCore.hpp"
#include "Response.hpp"
#include "Router.hpp"
#include "Server.hpp"
#include <iostream>

#undef DELETE

RequestHandler::RequestHandler(Server *server, int id) : _thread(&RequestHandler::run, this), _running(true), _requestHandlerId(id), _state(READY),
_requestId(0), _server(server)
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
    ZiaRequest::Request requestParsed;
    Response response;

    try {
        requestParsed = requestParser.parseData(_request);
        if (_checkOutputModules(requestParsed))
            throw ServerError("Not implemented", 501);
        if (requestParsed.getRequestType() == ZiaRequest::GET)
            _getRequest(requestParsed);
        else if (requestParsed.getRequestType() == ZiaRequest::POST)
            _postRequest(requestParsed);
        else if (requestParsed.getRequestType() == ZiaRequest::DELETE)
            _deleteRequest(requestParsed);
        else
            throw ServerError("Not implemented", 501);
    } catch (const CoreError &e) {
        _response = response.getResponse(e.what(), e.what(), e.getErrorCode());
    }
    _state = PROCESSED;
}

bool RequestHandler::_checkOutputModules(const ZiaRequest::Request& requestParsed)
{
    auto modulesList = _server->getOutputModules();

    for (auto &a : modulesList) {
        bool valid = true;
        std::string moduleFileExtension = a.second->get()->getFileExtension();
        if (requestParsed.getRequestPath().size() <= a.second->get()->getFileExtension().size())
            continue;
        auto itPath = requestParsed.getRequestPath().end();
        for (auto it = moduleFileExtension.end(); it != moduleFileExtension.begin(); --it) {
            if (*it != *itPath)
                valid = false;
            --itPath;
        }
        if (!valid)
            continue;
        return true;
    }
    return false;
}

void RequestHandler::_getRequest(const ZiaRequest::Request& requestParsed)
{
    Router router;
    Response response;
    std::string fileContent;

    router.init();
    fileContent = router.get("/", requestParsed.getRequestPath());
    if (fileContent.empty())
        _response = response.getResponse(fileContent, "No content", 204);
    else
        _response = response.getResponse(fileContent, "OK", 200);
}

void RequestHandler::_postRequest(const ZiaRequest::Request &requestParsed)
{
    Router router;
    Response response;
    std::string fileContent;

    router.init();
    fileContent = router.get("/", requestParsed.getRequestPath());
    if (fileContent.empty())
        _response = response.getResponse(fileContent, "No content", 204);
    else
        _response = response.getResponse(fileContent, "OK", 200);
}

void RequestHandler::_deleteRequest(const ZiaRequest::Request &requestParsed)
{
    Router router;
    Response response;

    router.init();
    std::cout << "www" + requestParsed.getRequestPath() << std::endl;
    router.remove("/", requestParsed.getRequestPath());
    _response = response.getResponse("", "OK", 200);
}
