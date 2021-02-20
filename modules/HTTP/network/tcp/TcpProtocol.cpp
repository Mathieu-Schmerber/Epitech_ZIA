//
// Created by Cyprien on 12/6/2020.
//

#include "TcpProtocol.hpp"

TcpProtocol::TcpProtocol(const std::string &host, unsigned short port) : _sTcp(host, port)
{
}

//void TcpProtocol::setUsername(int id, const std::string& username)
//{
//    _sTcp.setUsername(id, username);
//}

bool TcpProtocol::userDisconnected()
{
    return _sTcp.userDisconnected();
}

ReceiveData TcpProtocol::getNewMessage()
{
    return _sTcp.getNewMessage();
}

//void TcpProtocol::joinRoom(int idToSend, int idRoom, short maxPlayerNumber, const std::vector<std::string>& username)
//{
//    std::string toSend = std::string(char(JoinRoom) + std::to_string(idRoom) + ':' + std::to_string(username.size()) + ',' + std::to_string(maxPlayerNumber));
//    for (const auto &user : username) {
//        toSend += ',';
//        toSend += user;
//    }
//    _sTcp.send(idToSend, toSend);
//}

//std::string TcpProtocol::getUsername(int id)
//{
//    return _sTcp.getUsername(id);
//}

//void TcpProtocol::leaveRoom(int idToSend, int idRoom)
//{
//    _sTcp.send(idToSend, std::string(char(LeaveRoom) + std::to_string(idRoom)));
//}
//
//void TcpProtocol::launchGame(int idToSend, int playerId, unsigned short port, unsigned char color, std::pair<float, float> pos)
//{
//    _sTcp.send(idToSend, std::string(char(LaunchGame) + std::to_string(port) + ';' + std::to_string(playerId) + std::string(":") + char(color) + std::string(",") + std::to_string(int(pos.first)) + ',' + std::to_string(int(pos.second))));
//}
//
//void TcpProtocol::endGame(int idToSend)
//{
//    std::string toSend;
//    toSend.push_back(char(EndGame));
//    _sTcp.send(idToSend, toSend);
//}

//int TcpProtocol::getId(const std::string &ip) const
//{
//    return _sTcp.getId(ip);
//}

//void TcpProtocol::roomList(int idToSend, const std::vector<int> &roomId, const std::vector<std::string> &name, const std::vector<size_t> &numberPlayer, const std::vector<int> &maxPlayerNumber)
//{
//    std::string toSend;
//    toSend.push_back(char(RoomList));
//    for (int i = 0; i < roomId.size(); ++i) {
//        if (i != 0)
//            toSend += ';';
//        toSend += std::to_string(roomId[0]) + ':' + name[0] + ',' + std::to_string(numberPlayer[0]) + ',' + std::to_string(maxPlayerNumber[0]);
//    }
//    _sTcp.send(idToSend, toSend);
//}

std::string TcpProtocol::getNewDisconnect()
{
    return _sTcp.getNewDisconnect();
}

//void TcpProtocol::roomInfo(int idToSend, const std::string &roomName, int idRoom, short maxPlayerNumber, const std::vector<std::string> &username)
//{
//    _sTcp.sendToEveryone(std::string(char(RoomInfo) + std::to_string(idRoom) + ':' + roomName + ',' + std::to_string(username.size()) + ',' + std::to_string(maxPlayerNumber)), idToSend);
//}

TcpProtocol::~TcpProtocol()
=default;