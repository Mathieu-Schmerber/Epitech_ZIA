//
// Created by Cyprien on 12/6/2020.
//

#ifndef SERVER_TCPPROTOCOL_HPP
#define SERVER_TCPPROTOCOL_HPP

#include "TcpSocket.hpp"

class TcpProtocol {
    public:
        TcpProtocol(const std::string &host, unsigned short port);
        ~TcpProtocol();
//        void setUsername(int id, const std::string& username);
//        std::string getUsername(int id);
//        [[nodiscard]] int getId(const std::string &ip) const;
        bool userDisconnected();
        ReceiveData getNewMessage();
        [[nodiscard]] std::string getNewDisconnect();
//        void joinRoom(int idToSend, int idRoom, short maxPlayerNumber, const std::vector<std::string>& username);
//        void roomInfo(int idToSend, const std::string& roomName, int idRoom, short maxPlayerNumber, const std::vector<std::string>& username);
//        void leaveRoom(int idToSend, int idRoom);
//        void launchGame(int idToSend, int playerId, unsigned short port, unsigned char color, std::pair<float, float> pos);
//        void endGame(int idToSend);
//        void roomList(int idToSend, const std::vector<int> &roomId, const std::vector<std::string> &name, const std::vector<size_t> &numberPlayer, const std::vector<int> &maxPlayerNumber);

    private:
        TcpSocket _sTcp;
};


#endif //SERVER_TCPPROTOCOL_HPP
