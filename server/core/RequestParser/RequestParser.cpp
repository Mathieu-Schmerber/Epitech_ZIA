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

void ZiaRequest::Request::setRequestVersion(const std::string& requestVersion)
{
    _correctVersion = requestVersion == "HTTP/1.1";
    if (!_correctVersion)
        throw ServerError("Server Error 5xx", "HTTP Version Not Supported", 505);
}

void ZiaRequest::Request::setRequestHeader(const std::pair<std::string, std::string>& header)
{
    _headerlist.push_back(header);
}

ZiaRequest::Type ZiaRequest::Request::getRequestType() const
{
    return _requestType;
}

const std::string &ZiaRequest::Request::getRequestPath() const
{
    return _path;
}

std::vector<std::pair<std::string, std::string>> ZiaRequest::Request::getRequestHeaders() const
{
    return _headerlist;
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
        RMCHAR(out, '\r')
        RMCHAR(out, '\n')
        if (position == 0)
            _parseRequestMethod(out);
        else
            _parseRequestHeaders(out);
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
    _toReturn->setRequestType(tokens.at(0));
    _toReturn->setRequestPath(tokens.at(1));
    _toReturn->setRequestVersion(tokens.at(2));
}

void ZiaRequest::RequestParser::_parseRequestHeaders(const std::string &out)
{
    std::istringstream lineToParse(out);
    std::string each;
    std::pair<std::string, std::string> header;
    bool first = true;

    while (std::getline(lineToParse, each, ':')) {
        if (first) {
            header.first = each;
            first = !first;
        } else {
            each.erase(0, 1);
            header.second = each;
            _toReturn->setRequestHeader(header);
            return;
        }
    }
}