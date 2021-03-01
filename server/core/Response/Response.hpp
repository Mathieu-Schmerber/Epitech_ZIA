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
    explicit Response();

    std::string getResponse(const std::string& content, const std::string& status, int code);
private:
    //Private methods
    std::string _getTime();
};

#endif //ZIA_RESPONSE_HPP
