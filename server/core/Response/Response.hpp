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
#include <map>

class Response {
public:
    enum RData {
        HEADER,
        DATE,
        SERVER_NAME,
        CONTENT_LENGTH,
        CONTENT_TYPE,
        CONTENT
    };

    static std::string getResponse(const std::string& content, const std::string& status, int code, std::map<Response::RData, std::string> other_components = {});
    static std::string headResponse(const std::string& content, const std::string &status, int code);
private:
    //Private methods
    static std::string _createDate();
    static std::string _createHeader(const std::string &status, int code);
    static std::string _createServerName();
    static std::string _createContentLength(const std::string &content);
    static std::string _createContentType(const std::string &type);
};

const std::vector<std::string> imgTypes = {"png", "ico", "jpg", "jpeg"};
const std::vector<std::string> txtTypes = {"txt", "html", "css", "php", "js"};


#endif //ZIA_RESPONSE_HPP
