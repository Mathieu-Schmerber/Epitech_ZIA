/**
 * \file TcpProtocol.cpp
 * \brief protocol that let you easily deal with tcp protocol
 * \authors Cyprien.R, William.P
**/


#include "TcpProtocol.hpp"

TcpProtocol::TcpProtocol(const std::string &host, unsigned short port, const std::string &certificate, const std::string &key) : _sTcp(host, port, certificate, key)
{
}

bool TcpProtocol::userDisconnected()
{
    return _sTcp.userDisconnected();
}

ReceiveData TcpProtocol::getNewMessage()
{
    return _sTcp.getNewMessage();
}

void TcpProtocol::send(int id, const std::string& toSend)
{
    _sTcp.send(id, toSend);
}

int TcpProtocol::getNewDisconnect()
{
    return _sTcp.getNewDisconnect();
}

TcpProtocol::~TcpProtocol()
=default;