//
// Created by William on 09/11/2020.
//

#ifndef R_TYPE_RECEIVEDATA_HPP
#define R_TYPE_RECEIVEDATA_HPP

#include <string>
#include <utility>

struct ReceiveData {
    std::string receive;
    int id;
    std::string ip;

    ReceiveData() : id(0) {}
    ReceiveData(std::string receive, int id, std::string ip) : receive(std::move(receive)), id(id), ip(std::move(ip)) {}
    ReceiveData(ReceiveData const &other) : receive(other.receive), id(other.id), ip(other.ip) {}
    ReceiveData(ReceiveData&& other) : receive(std::move(other.receive)), id(other.id), ip(std::move(other.ip)) {}
    ReceiveData& operator=(const ReceiveData& other) = default;
};

#endif //R_TYPE_RECEIVEDATA_HPP
