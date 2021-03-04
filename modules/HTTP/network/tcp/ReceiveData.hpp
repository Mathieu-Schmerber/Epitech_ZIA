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

    ReceiveData() : id(0) {}
    ReceiveData(std::string receive, int id) : receive(std::move(receive)), id(id) {}
    ReceiveData(ReceiveData const &other) : receive(other.receive), id(other.id) {}
    ReceiveData(ReceiveData&& other) : receive(std::move(other.receive)), id(other.id) {}
    ReceiveData& operator=(const ReceiveData& other) = default;
};

#endif //R_TYPE_RECEIVEDATA_HPP
