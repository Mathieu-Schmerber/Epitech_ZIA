//
// Created by Emilien on 27/02/2021.
//

#include <vector>
#include "Response.hpp"
#include "Utils.hpp"

std::string Response::getResponse(const std::string& content, const std::string& status, int code, std::map<Response::RData, std::string> other_components, std::vector<std::pair<std::string, std::string>> params)
{
    std::string response;

    response += _createHeader(status, code);
    response += _createDate();
    response += _createServerName();
    response += _createContentLength(content);
    if (Utils::isInMap(other_components, CONTENT_TYPE))
        response += _createContentType(other_components[CONTENT_TYPE]);
    for (auto &a : params)
        response += a.first + ": " + a.second + "\r\n";
    response += "\r\n";
    response += content;
    return response;
}

std::string Response::headResponse(const std::string& content, const std::string &status, int code)
{
    std::string response;

    response += _createHeader(status, code);
    response += _createDate();
    response += _createServerName();
    response += _createContentLength(content);
    response += "\r\n";
    return response;
}

std::string Response::_createDate()
{
    std::string timeToReturn;
    std::time_t t = std::time(nullptr);
    struct tm *ti = std::gmtime(&t);
    std::string formatedTime(std::asctime(ti));
    return ("Date: " + std::string(formatedTime.substr(0, formatedTime.size() - 1)) + " UTC\r\n");
}

std::string Response::_createHeader(const std::string &status, int code)
{
    return ("HTTP/1.1 " + std::to_string(code) + " " + status + "\r\n");
}

std::string Response::_createServerName()
{
    return ("Server: Zia\r\n");
}

std::string Response::_createContentLength(const std::string &content)
{
    return ("Content-Length: " + std::to_string(content.length()) + "\r\n");
}

std::string Response::_createContentType(const std::string &extension)
{
    std::string file_type = "image";  // Default

    if (Utils::isInVector(imgTypes, extension))
        file_type = "image";
    if (Utils::isInVector(txtTypes, extension))
        file_type = "text";

    return ("Content-Type: " + file_type + '/' + extension + "\r\n");
}