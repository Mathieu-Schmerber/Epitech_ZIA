/**
 * \file TcpProtocol.cpp
 * \brief protocol that let you easily deal with tcp protocol
 * \authors Cyprien.R, William.P
**/


#include "TcpProtocol.hpp"

/**
 * \brief TcpProtocol constructor
 *
 * \param host : ip to start the server
 * \param port : port to start the server
 * \param certificate : file to certificate to allow https communication
 * \param key : file to key for the certificate
**/
TcpProtocol::TcpProtocol(const std::string &host, unsigned short port, const std::string &certificate, const std::string &key) : _sTcp(host, port, certificate, key)
{
}

/**
 * \brief return true if at least one user has been disconnected
**/
bool TcpProtocol::userDisconnected()
{
    return _sTcp.userDisconnected();
}

/**
 * \brief return first message in queue
**/
ReceiveData TcpProtocol::getNewMessage()
{
    return _sTcp.getNewMessage();
}

/**
 * \brief send message to client id
 *
 * \param id
 * \param toSend : message to send
**/
void TcpProtocol::send(int id, const std::string& toSend)
{
    _sTcp.send(id, toSend);
}

/**
 * \brief getNewDisconnect return id of last disconnected client
**/
int TcpProtocol::getNewDisconnect()
{
    return _sTcp.getNewDisconnect();
}

TcpProtocol::~TcpProtocol()
=default;