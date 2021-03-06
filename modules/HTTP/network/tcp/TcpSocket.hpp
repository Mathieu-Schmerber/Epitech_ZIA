/**
 * \file TcpSocket.hpp
 * \brief Class to handle TCP message / connections / multi clients
 * \author Cyprien.R
**/


#ifndef SERVER_TCPSOCKET_HPP
#define SERVER_TCPSOCKET_HPP

#include <mutex>

#ifdef _WIN32
    #ifdef _WIN32_WINNT
        #undef _WIN32_WINNT
    #endif
    #define _WIN32_WINNT  0x0601
#endif

#include <boost/asio.hpp>
#include <string>
#include <deque>
#include "ReceiveData.hpp"

#define MAX_SIZE 1024

/**
 * \class InstanceClientTCP InstanceClientTCP.hpp "InstanceClientTCP.hpp"
 * \brief InstanceClientTCP is a class representing one client
**/
class InstanceClientTCP : public std::enable_shared_from_this<InstanceClientTCP> {
    public:
        InstanceClientTCP(boost::asio::ip::tcp::socket socket, int id, std::deque<ReceiveData> &msgQueue, std::mutex &mtxQue);
        ~InstanceClientTCP();
        void startRead();
        void send(const std::string &msg);
        bool getDisconnected() const;
        int getId() const;

    private:
        boost::asio::ip::tcp::socket _socket;
        bool _disconnected = false;
        char _read[MAX_SIZE] = {0};
        int _id;
        std::deque<ReceiveData> &_msgQueue;
        std::mutex &mtxQu;
};

/**
 * \class TcpSocket TcpSocket.hpp "TcpSocket.hpp"
 * \brief TcpSocket is a class used to manage new connections and setup new InstanceClientTCP
**/
class TcpSocket {
    public:
        TcpSocket(const std::string &host, unsigned short port);
        ~TcpSocket();
        bool userDisconnected();
        void send(int id, const std::string &msg);
        [[nodiscard]] int getNewDisconnect();
        ReceiveData getNewMessage();

    private:
        void startAccept();
        boost::asio::io_service _io_service;
        boost::asio::ip::tcp::acceptor _acceptor;
        boost::asio::ip::tcp::socket _socket;
        std::deque<std::shared_ptr<InstanceClientTCP>> _clients;
        std::deque<ReceiveData> _msgQueue;
        std::deque<int> _idDisconnect;
        std::thread *_tRunAccept = nullptr;
        int idCounter = 10000;
        std::mutex mtx;
        std::mutex mtxMq;
};


#endif //SERVER_TCPSOCKET_HPP
