/**
 * \file TcpSocket.cpp
 * \brief functions of TcpSocket's and InstanceClientTCP's class (https version)
 * \authors Cyprien.R, William.P
**/


#include "TcpSocket.hpp"
#include "Log.hpp"

/**
 * \brief TcpSocket constructor
 *
 * \param host : ip to start the server
 * \param port : port to start the server
**/
TcpSocket::TcpSocket(const std::string &host, unsigned short port, const std::string &certificate, const std::string &key) :
_acceptor(_io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(host), port)),
_socket(_io_service),
_context(boost::asio::ssl::context::sslv23)
{
    _context.set_options(
            boost::asio::ssl::context::default_workarounds
            | boost::asio::ssl::context::no_sslv2);
    _context.use_certificate_chain_file(certificate);
    _context.use_private_key_file(key, boost::asio::ssl::context::pem);

    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
    _acceptor.close();
    _acceptor.open(endpoint.protocol());
    _acceptor.set_option(boost::asio::socket_base::keep_alive(true));
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
                    std::shared_ptr<InstanceClientTCP> newConnection = std::make_shared<InstanceClientTCP>(std::move(_socket), idCounter, _msgQueue, _context);
                    idCounter++;
                    newConnection->startHandshake();
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
                                [](const std::shared_ptr<InstanceClientTCP> &i) { return i->getDisconnected(); });
    if (toReturn) {
        for (int i = int(_clients.size()) - 1; i >= 0; --i) {
            if (_clients[i]->getDisconnected()) {
                _idDisconnect.push_back(_clients[i]->getId());
                _clients.erase(_clients.begin() + i);
            }
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
    for (const auto &client : _clients) {
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
InstanceClientTCP::InstanceClientTCP(boost::asio::ip::tcp::socket socket, int id, std::deque<ReceiveData> &msgQueue, boost::asio::ssl::context& context) :
_socket(std::move(socket), context), _msgQueue(msgQueue)
{
    _id = id;
    LOG(INFO) << "User has just connected";
}

/**
 * \brief startHandshake, handle https handshake
**/
void InstanceClientTCP::startHandshake()
{
    auto self(shared_from_this());
    _socket.async_handshake(boost::asio::ssl::stream_base::server,[this, self](const boost::system::error_code& error) {
        if (!error)
            startRead();
        else
            _disconnected = true;
    });
}

/**
 * \brief startRead read message coming from clients and store them
**/
void InstanceClientTCP::startRead()
{
    auto self(shared_from_this());
    auto handleRead =
            [this, self](const boost::system::error_code &error, size_t bytes_transferred) {
                if (error == boost::asio::ssl::error::stream_truncated) {
                    _socket.lowest_layer().cancel();
                    _socket.async_shutdown([this](...){_socket.lowest_layer().close();});
                    _disconnected = true;
                } else if (error == boost::asio::error::eof || error == boost::asio::error::connection_reset) {
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
void handleSend([[maybe_unused]]const boost::system::error_code &error, [[maybe_unused]]size_t bytes_transferred)
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