/*
** EPITECH PROJECT, 2020
** Zia
** File description:
** Created by Emilien
*/

#include "RequestParser.hpp"
#include "ExceptionCore.hpp"

#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>

/** ====== Request Parser ====== **/

ZiaRequest::Request::Request(const std::string &in) : _request(in), _requestType(UNDEFINED), _correctVersion(false)
{
}

void ZiaRequest::Request::setRequestType(const std::string &requestType)
{
    if (requestType == std::string("GET"))
        _requestType = ZiaRequest::GET;
    else if (requestType == std::string("POST"))
        _requestType = ZiaRequest::POST;
    else if (requestType == std::string("OPTIONS"))
        _requestType = ZiaRequest::OPTIONS;
    else if (requestType == std::string("HEAD"))
        _requestType = ZiaRequest::HEAD;
    else if (requestType == std::string("PUT"))
        _requestType = ZiaRequest::PUT;
    else if (requestType == std::string("DELETE"))
        _requestType = ZiaRequest::DELETE;
    else if (requestType == std::string("TRACE"))
        _requestType = ZiaRequest::TRACE;
    else if (requestType == std::string("CONNECT"))
        _requestType = ZiaRequest::CONNECT;
    else
        throw ClientError("Client Error 4xx", "Bad Request", 400);
}

void ZiaRequest::Request::setRequestPath(const std::string &requestPath)
{
    _path = requestPath;
}

void ZiaRequest::Request::setRequestVersion(std::string requestVersion)
{
    RMCHAR(requestVersion, '\r')
    RMCHAR(requestVersion, '\n')
    _correctVersion = requestVersion == "HTTP/1.1";
    if (!_correctVersion)
        throw ServerError("Server Error 5xx", "HTTP Version Not Supported", 505);
}

ZiaRequest::Type ZiaRequest::Request::getRequestType() const
{
    return _requestType;
}

const std::string &ZiaRequest::Request::getRequestPath() const
{
    return _path;
}

/** ====== Request Parser ====== **/

ZiaRequest::RequestParser::RequestParser(const std::string &in) : _request(in)
{
    _toReturn = std::make_unique<Request>(in);
}

void ZiaRequest::RequestParser::parseData()
{
    std::istringstream requestToParse(_request);
    std::string out;
    int position = 0;

    while (std::getline(requestToParse, out)) {
        if (position == 0)
            _parseRequestMethod(out);
        ++position;
    }
}

std::unique_ptr<ZiaRequest::Request> &ZiaRequest::RequestParser::getRequest()
{
    return _toReturn;
}

void ZiaRequest::RequestParser::_parseRequestMethod(const std::string& out)
{
    std::vector<std::string> tokens;
    std::istringstream lineToParse(out);
    std::string each;

    while (std::getline(lineToParse, each, ' '))
        tokens.push_back(each);
    std::cout << "1" << std::endl;
    _toReturn->setRequestType(tokens.at(0));
    std::cout << "1" << std::endl;
    _toReturn->setRequestPath(tokens.at(1));
    std::cout << "1" << std::endl;
    _toReturn->setRequestVersion(tokens.at(2));
    std::cout << "1" << std::endl;
    std::cout << "Type -> " << ZiaRequest::requestTypesNames[_toReturn->getRequestType()] << std::endl;
    std::cout << "Path -> " << _toReturn->getRequestPath() << std::endl;
}
