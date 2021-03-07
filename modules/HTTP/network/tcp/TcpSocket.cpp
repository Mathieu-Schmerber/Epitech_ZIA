/**
 * \file TcpSocket.cpp
 * \brief functions of TcpSocket's and InstanceClientTCP's class
 * \author Cyprien.R
**/

#include "TcpSocket.hpp"
#include "Log.hpp"

/**
 * \brief TcpSocket constructor
 *
 * \param host : ip to start the server
 * \param port : port to start the server
**/
TcpSocket::TcpSocket(const std::string &host, unsigned short port) : _acceptor(_io_service, boost::asio::ip::tcp::endpoint(
        boost::asio::ip::address::from_string(host), port)), _socket(_io_service)
{
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
    _acceptor.close();
    _acceptor.open(endpoint.protocol());
    _acceptor.set_option(boost::asio::socket_base::keep_alive(true));
    _acceptor.set_option(boost::asio::socket_base::reuse_address (true));
    _acceptor.bind(endpoint);
    _acceptor.listen();
    startAccept();
    _tRunAccept = new std::thread([&] { _io_service.run(); });
}

/**
 * \brief startAccept accepts incoming connections and store new clients into the new clients _clients vector
**/
void TcpSocket::startAccept()
{
    auto handleAccept =
            [this](const boost::system::error_code &error) {
                if (!error) {
                    std::shared_ptr<InstanceClientTCP> newConnection = std::make_shared<InstanceClientTCP>(std::move(_socket), idCounter, _msgQueue);
                    idCounter++;
                    newConnection->startRead();
                    TcpSocket::_clients.push_back(newConnection);
                }
                startAccept();
            };
    _acceptor.async_accept(_socket, handleAccept);
}

/**
 * \brief userDisconnected : keep track of disconnected users and remove them from the client list
**/
bool TcpSocket::userDisconnected()
{
    bool toReturn = std::any_of(_clients.begin(), _clients.end(),
                                [](const std::shared_ptr<InstanceClientTCP> &i)
                                {
                                    if (!i)
                                        return true; //FIXME Mouais je suis pas sur que ce soit le meilleur truc à faire mais bon
                                    return i->getDisconnected();
                                });
    if (toReturn)
        for (int i = int(_clients.size()) - 1; i >= 0; --i) {
            if (!_clients[i]) {
                std::cerr << "Le deuxième non est ici même" << std::endl;
                _clients.erase(_clients.begin() + i);
                continue;
            }
            if (_clients[i]->getDisconnected()) {
                _idDisconnect.push_back(_clients[i]->getId());
                _clients.erase(_clients.begin() + i);
            }
        }
    return toReturn;
}

/**
 * \brief TcpSocket destructor : end threads
**/
TcpSocket::~TcpSocket()
{
    if (_tRunAccept->joinable()) {
        _io_service.stop();
        _tRunAccept->join();
    }
}

/**
 * \brief getNewMessage return receive message, remove it from the received messages queue
**/
ReceiveData TcpSocket::getNewMessage()
{
    if (!_msgQueue.empty()) {
        ReceiveData toReturn = _msgQueue.front();
        _msgQueue.pop_front();
        return toReturn;
    }
    return ReceiveData();
}

/**
 * \brief send message to a client
 *
 * \param id : client id
 * \param msg : message to send
**/
void TcpSocket::send(int id, const std::string &msg)
{
    int i = 0;

    for (const auto &client : _clients) {
        if (!client) {
            std::cerr << "potential crash" << std::endl; //FIXME
            _clients.erase(_clients.begin() + i);
            this->send(id, msg);
            return;
        }
        ++i;
        if (client->getId() == id)
            client->send(msg);
    }
}

/**
 * \brief return the id of recently disconnected client, and remove them from the diconnected clients queue
**/
int TcpSocket::getNewDisconnect()
{
    if (!_idDisconnect.empty()) {
        int toReturn = _idDisconnect.front();
        _idDisconnect.pop_front();
        return toReturn;
    }
    return (0);
}

/**
 * \brief InstanceClientTCP constructor
 *
 * \param socket : client's socket
 * \param msgQueue : client's messages received
**/
InstanceClientTCP::InstanceClientTCP(boost::asio::ip::tcp::socket socket, int id, std::deque<ReceiveData> &msgQueue) : _socket(std::move(socket)), _msgQueue(msgQueue)
{
    _id = id;
    LOG(INFO) << "User has just connected";
}

/**
 * \brief startRead read message coming from clients and store them
**/
void InstanceClientTCP::startRead()
{
    auto self(shared_from_this());
    auto handleRead =
            [this, self](const boost::system::error_code &error, size_t bytes_transferred) {
                if (error == boost::asio::error::eof || error == boost::asio::error::connection_reset) {
                    _disconnected = true;
                } else {
                    _msgQueue.emplace_back(std::string(_read, bytes_transferred), _id);
                    //LOG(DEBUG) << ("TCP : " + std::string(_read, bytes_transferred));
                    startRead();
                }
            };
    _socket.async_read_some(boost::asio::buffer(_read, MAX_SIZE), handleRead);
}

/**
 * \brief getDisconnected
 *
 * \return return true or false depending on whether the client is diconnected or not
**/
bool InstanceClientTCP::getDisconnected() const
{
    return _disconnected;
}

/**
 * \brief InstanceClientTCP destructor
**/
InstanceClientTCP::~InstanceClientTCP()
{
    LOG(INFO) << "User has just disconnected";
}

/**
 * \brief handleSend : asynchronous send (NOT IMPLEMENTED)
**/
void handleSend(const boost::system::error_code &error, size_t bytes_transferred)
{
}

/**
 * \brief send : send messae to client
**/
void InstanceClientTCP::send(const std::string &msg)
{
    size_t bufSize = 65536 - 1;
    std::string buf;
    std::string full_msg = std::string(msg);

    LOG(DEBUG) << "HTTP InstanceClientTCP::send full msg size " << full_msg.length();
    while (full_msg.length() > 0) {
        buf = full_msg.substr(0, bufSize);
        boost::asio::async_write(_socket, boost::asio::buffer(buf), &handleSend);
        full_msg.erase(0, bufSize);
    }
}

/**
 * \brief getId : get client id
**/
int InstanceClientTCP::getId() const
{
    return _id;
}