/**
 * \file RequestHandler.hpp
 * \brief Definitions for the Request Handler threads
 * \author Emilien.D
**/

#ifndef ZIA_REQUESTHANDLER_HPP
#define ZIA_REQUESTHANDLER_HPP

#include <thread>
#include <string>
#include <map>
#include "RequestParser.hpp"

class Server;

enum ThreadState {
    PROCESSING = 1,
    PROCESSED,
    READY
};

/**
 * \class RequestHandler RequestHandler.hpp "RequestHandler.hpp"
 * \brief Class for the request handlers threads
**/

class RequestHandler {
public:
    explicit RequestHandler(Server *server, int id);
    ~RequestHandler();
    void run();
    [[nodiscard]] ThreadState getState() const;
    std::pair<std::string, std::pair<std::string, int>> getProcessedRequest();
    void setRequestToProcess(const std::pair<std::string, std::pair<std::string, int>>& request);
private:
    //Private methods
    void _processRequest();
    bool _checkOutputModules(const ZiaRequest::Request& requestParsed);
    void _getRequest(const ZiaRequest::Request& requestParsed);
    void _postRequest(const ZiaRequest::Request& requestParsed);
    void _headRequest(const ZiaRequest::Request& requestParsed);

    //Request Handlers variables
    std::thread _thread;
    ThreadState _state;
    Server *_server;
    bool _running;

    //HTTP Request
    int _requestHandlerId;
    std::string _request;
    std::string _response;
    int _requestId;
    std::string _moduleName;
};

#endif //ZIA_REQUESTHANDLER_HPP
