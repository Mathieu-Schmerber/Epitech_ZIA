//
// Created by Cyprien on 12/6/2020.
//

#include "TcpProtocol.hpp"

TcpProtocol::TcpProtocol(const std::string &host, unsigned short port) : _sTcp(host, port)
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

std::string TcpProtocol::getNewDisconnect()
{
    return _sTcp.getNewDisconnect();
}

TcpProtocol::~TcpProtocol()
=default;