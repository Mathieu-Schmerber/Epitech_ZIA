//
// Created by Emilien on 27/02/2021.
//

#include "Response.hpp"

Response::Response()
{}

std::string Response::getResponse(const std::string& content, const std::string& status, int code)
{
    std::string response;

    response += ("HTTP/1.1 " + std::to_string(code) + " " + status + "\r\n");
    response += ("Date: " + _getTime() + " UTC\r\n");
    response += ("Server: Zia\r\n");
    response += ("Content-Length: " + std::to_string(content.length()) + "\r\n");
    response += "\r\n";
    response += content;
    return response;
}

std::string Response::_getTime()
{
    std::string timeToReturn;
    std::time_t t = std::time(nullptr);
    struct tm *ti = std::gmtime(&t);
    std::string formatedTime(std::asctime(ti));
    return std::string(formatedTime.substr(0, formatedTime.size() - 1));
}
