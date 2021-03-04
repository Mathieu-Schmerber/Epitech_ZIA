//
// Created by Emilien on 27/02/2021.
//

#ifndef ZIA_RESPONSE_HPP
#define ZIA_RESPONSE_HPP

#include <string>
#include <utility>
#include <ctime>
#include <iostream>
#include <iomanip>

class Response {
public:
    static std::string getResponse(const std::string& content, const std::string& status, int code);
    static std::string headResponse(const std::string& content, const std::string &status, int code);
private:
    //Private methods
    static std::string _createDate();
    static std::string _createHeader(const std::string &status, int code);
    static std::string _createServerName();
    static std::string _createContentLength(const std::string &content);
};

#endif //ZIA_RESPONSE_HPP
