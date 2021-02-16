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

void ZiaRequest::Request::setRequestVersion(const std::string &requestVersion)
{
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
}

std::unique_ptr<ZiaRequest::Request> ZiaRequest::RequestParser::parseData()
{
    std::unique_ptr<Request> toReturn = std::make_unique<Request>(_request);
    std::istringstream requestToParse(_request);
    std::string out;
    int position = 0;

    while (std::getline(requestToParse, out)) {
        std::vector<std::string> tokens;
        std::istringstream lineToParse(out);
        std::string each;

        if (position == 0) {
            while (std::getline(lineToParse, each, ' '))
                tokens.push_back(each);
            toReturn->setRequestType(tokens.at(0));
            toReturn->setRequestPath(tokens.at(1));
            toReturn->setRequestVersion(tokens.at(2));
        } else {
            //std::cout << out << std::endl;
        }
        ++position;
    }
    return toReturn;
}
