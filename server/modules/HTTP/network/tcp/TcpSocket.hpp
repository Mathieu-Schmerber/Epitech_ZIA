//
// Created by Cyprien on 12/6/2020.
//

#ifndef SERVER_TCPSOCKET_HPP
#define SERVER_TCPSOCKET_HPP

#include <string>
#include <deque>
#include <boost/asio.hpp>
#include "ReceiveData.hpp"

#define MAX_SIZE 1024

class InstanceClientTCP : public std::enable_shared_from_this<InstanceClientTCP> {
    public:
        InstanceClientTCP(boost::asio::ip::tcp::socket socket, int id, std::deque<ReceiveData> &msgQueue);
        ~InstanceClientTCP();
        void startRead();
        void send(const std::string &msg);
        bool getDisconnected() const;
        std::string getIp();
        int getId() const;
        void setUsername(std::string username);
        std::string getUsername();

    private:
        boost::asio::ip::tcp::socket _socket;
        std::string _ip;
        bool _disconnected = false;
        char _read[MAX_SIZE] = {0};
        int _id;
        std::string _username;
        std::deque<ReceiveData> &_msgQueue;
};

class TcpSocket {
    public:
        TcpSocket(const std::string &host, unsigned short port);
        ~TcpSocket();
        bool userDisconnected();
        void send(int id, const std::string &msg);
        void sendToEveryone(const std::string &msg, int id);
        void setUsername(int id, const std::string& username);
        std::string getUsername(int id);
        [[nodiscard]] int getId(const std::string &ip) const;
        [[nodiscard]] std::string getNewDisconnect();
        ReceiveData getNewMessage();

    private:
        void startAccept();
        boost::asio::io_service _io_service;
        boost::asio::ip::tcp::acceptor _acceptor;
        boost::asio::ip::tcp::socket _socket;
        std::deque<std::shared_ptr<InstanceClientTCP>> _clients;
        std::deque<ReceiveData> _msgQueue;
        std::deque<std::string> _ipDisconnect;
        std::thread *_tRunAccept = nullptr;
        int idCounter = 10000;
};


#endif //SERVER_TCPSOCKET_HPP
