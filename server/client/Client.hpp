/**
 * \file Client.hpp
 * \brief File for the client class
 * \author Cyprien.R
**/

#ifndef ZIA_SERVER_CLIENT_HPP
#define ZIA_SERVER_CLIENT_HPP

#ifdef _WIN32
    #define _WIN32_WINNT  0x0601
#endif

#include <boost/asio/ip/address.hpp>
#include <string>
#include <utility>
#include <iostream>

#include "Utils.hpp"
#include "Log.hpp"

/**
 * \class Client Client.hpp "Client.hpp"
 * \brief This class is to store the connected clients
**/

class Client {
public:
    Client(const boost::asio::ip::address ip, const unsigned short port) : _ip(ip), _port(port) {
    };

    bool operator==(const Client &c) const {
        return c._ip == _ip && c._port == _port;
    }
    bool operator==(const boost::asio::ip::address& ip) const {
        return ip == _ip;
    }
    bool operator==(const unsigned short port) const {
        return port == _port;
    }

    const boost::asio::ip::address _ip;
    const unsigned short _port;
};

inline bool operator==(const std::unique_ptr<Client> &client, const boost::asio::ip::address& ip) {
    return ip == client->_ip;
}

inline bool operator==(const std::unique_ptr<Client> &client, const unsigned short port) {
    return port == client->_port;
}

#endif //ZIA_SERVER_CLIENT_HPP
