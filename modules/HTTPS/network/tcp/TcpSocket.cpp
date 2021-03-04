//
// Created by Cyprien on 12/6/2020.
//

#include "TcpSocket.hpp"
#include "Log.hpp"

TcpSocket::TcpSocket(const std::string &host, unsigned short port) : _acceptor(_io_service, boost::asio::ip::tcp::endpoint(
        boost::asio::ip::address::from_string(host), port)), _socket(_io_service), _context(boost::asio::ssl::context::sslv23)
{
    _context.set_options(
            boost::asio::ssl::context::default_workarounds
            | boost::asio::ssl::context::no_sslv2
            | boost::asio::ssl::context::single_dh_use);
    _context.use_certificate_chain_file("./certs/certificate.pem");
    _context.use_private_key_file("./certs/key.pem", boost::asio::ssl::context::pem);
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
    _acceptor.close();
    _acceptor.open(endpoint.protocol());
    _acceptor.set_option(boost::asio::socket_base::keep_alive(true));
    _acceptor.bind(endpoint);
    _acceptor.listen();
    startAccept();
    _tRunAccept = new std::thread([&] { _io_service.run(); });
}

void TcpSocket::startAccept()
{
    auto handleAccept =
            [this](const boost::system::error_code &error) {
                if (!error) {
                    std::shared_ptr<InstanceClientTCP> newConnection = std::make_shared<InstanceClientTCP>(std::move(_socket), idCounter, _msgQueue, _context);
                    idCounter++;
                    newConnection->startHandshake();
                    TcpSocket::_clients.push_back(newConnection);
                }
                startAccept();
            };
    _acceptor.async_accept(_socket, handleAccept);
}

bool TcpSocket::userDisconnected()
{
    bool toReturn = std::any_of(_clients.begin(), _clients.end(),
                                [](const std::shared_ptr<InstanceClientTCP> &i) { return i->getDisconnected(); });
    if (toReturn) {
        for (int i = int(_clients.size()) - 1; i >= 0; --i) {
            if (_clients[i]->getDisconnected()) {
                _ipDisconnect.push_back(_clients[i]->getIp());
                _clients.erase(_clients.begin() + i);
            }
        }
    }
    return toReturn;
}

TcpSocket::~TcpSocket()
{
    if (_tRunAccept->joinable()) {
        _io_service.stop();
        _tRunAccept->join();
    }
}

ReceiveData TcpSocket::getNewMessage()
{
    if (!_msgQueue.empty()) {
        ReceiveData toReturn = _msgQueue.front();
        _msgQueue.pop_front();
        return toReturn;
    }
    return ReceiveData();
}

void TcpSocket::send(int id, const std::string &msg)
{
    for (const auto &client : _clients) {
        if (client->getId() == id)
            client->send(msg);
    }
}

std::string TcpSocket::getNewDisconnect()
{
    if (!_ipDisconnect.empty()) {
        std::string toReturn = _ipDisconnect.front();
        _ipDisconnect.pop_front();
        return toReturn;
    }
    return ("");
}

InstanceClientTCP::InstanceClientTCP(boost::asio::ip::tcp::socket socket, int id, std::deque<ReceiveData> &msgQueue, boost::asio::ssl::context& context) : _socket(std::move(socket), context), _msgQueue(msgQueue)
{
    _id = id;
    LOG_GREEN( "User with ip : " + _ip + " has just connected")
}

void InstanceClientTCP::startHandshake()
{
    auto self(shared_from_this());
    _socket.async_handshake(boost::asio::ssl::stream_base::server,[this, self](const boost::system::error_code& error) {
        if (!error)
            startRead();
    });
}

void InstanceClientTCP::startRead()
{
    auto self(shared_from_this());
    auto handleRead =
            [this, self](const boost::system::error_code &error, size_t bytes_transferred) {
                if (error == boost::asio::error::eof || error == boost::asio::error::connection_reset) {
                    _disconnected = true;
                } else {
                    _msgQueue.emplace_back(std::string(_read, bytes_transferred), _id, _ip);
                    // FIXME LOG_BLUE_WN("TCP : " + std::string(_read, bytes_transferred));
                    startRead();
                }
            };
    _socket.async_read_some(boost::asio::buffer(_read, MAX_SIZE), handleRead);
}

bool InstanceClientTCP::getDisconnected() const
{
    return _disconnected;
}

std::string InstanceClientTCP::getIp()
{
    return _ip;
}

InstanceClientTCP::~InstanceClientTCP()
{
    LOG_RED("User with ip : " + _ip + " has just disconnected")
}

void handleSend(const boost::system::error_code &error, size_t bytes_transferred)
{
}

void InstanceClientTCP::send(const std::string &msg)
{
    boost::asio::async_write(_socket, boost::asio::buffer(msg), &handleSend);
}

int InstanceClientTCP::getId() const
{
    return _id;
}