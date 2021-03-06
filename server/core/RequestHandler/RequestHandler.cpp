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
#include "Log.hpp"
#include <iostream>

#undef DELETE

RequestHandler::RequestHandler(Server *server, int id) : _thread(&RequestHandler::run, this), _running(true), _state(READY), _server(server), _requestHandlerId(id),
_requestId(0)
{}

RequestHandler::~RequestHandler()
{
    _running = false;
    _thread.join();
}

void RequestHandler::run()
{
    LOG(INFO) << "Thread " << _requestHandlerId << " started.";
    while (_running) {
        if (_state == PROCESSING)
            _processRequest();
    }
    LOG(INFO) << "Thread " << _requestHandlerId << " stopped.";
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

    try {
        requestParsed = requestParser.parseData(_request);
        if (!Utils::isInMap(hdl_rq, requestParsed.getRequestType()))
            throw ServerError("Not implemented", 501);
        if (_checkOutputModules(requestParsed)) {
            _state = PROCESSED;
            return;
        }
        std::invoke(hdl_rq.at(requestParsed.getRequestType()), this, requestParsed);
    } catch (const CoreError &e) {
        _response = Response::getResponse(e.what(), e.what(), e.getErrorCode());
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
        if (a.second->get()->getStatus())
            _response = a.second->get()->processData(requestParsed);
        return true;
    }
    return false;
}

void RequestHandler::_getRequest(const ZiaRequest::Request& requestParsed)
{
    Router router;
    std::string fileContent;
    std::string fileExt;

    router.init();
    fileContent = router.get("/", requestParsed.getRequestPath());
    fileExt = Router::getFileExtension(requestParsed.getRequestPath());

    if (fileContent.empty())
        _response = Response::getResponse(fileContent, "No content", 204);
    else
        _response = Response::getResponse(fileContent, "OK", 200, {{Response::CONTENT_TYPE, fileExt}});
}

void RequestHandler::_postRequest(const ZiaRequest::Request &requestParsed)
{
    Router router;
    std::string fileContent;

    router.init();
    fileContent = router.get("/", requestParsed.getRequestPath());
    if (fileContent.empty())
        _response = Response::getResponse(fileContent, "No content", 204);
    else
        _response = Response::getResponse(fileContent, "OK", 200);
}

void RequestHandler::_headRequest(const ZiaRequest::Request& requestParsed)
{
    Router router;
    std::string fileContent;

    router.init();
    fileContent = router.get("/", requestParsed.getRequestPath());
    _response = Response::headResponse(fileContent, "OK", 200);
}

void RequestHandler::_deleteRequest(const ZiaRequest::Request &requestParsed)
{
    Router router;

    router.init();
    router.remove("/", requestParsed.getRequestPath());
    _response = Response::getResponse("File deleted.", "OK", 200);
}

void RequestHandler::_putRequest(const ZiaRequest::Request &requestParsed)
{
    Router router;

    router.init();
    auto res = router.create("/", requestParsed.getRequestPath(), requestParsed.getRequestBody(), true);
    if (res.second)
        _response = Response::getResponse("Content-Location: " + res.first, "OK", 200);
    else
        _response = Response::getResponse("Content-Location: " + res.first, "Created", 201);
}
// 201 (Created) : new file created
// 200 (OK) : with ltl message ? File correctly modified
// 204 (No Content) : File correctly modified