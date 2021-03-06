//
// Created by Cyprien on 12/6/2020.
//

#ifndef SERVER_TCPPROTOCOL_HPP
#define SERVER_TCPPROTOCOL_HPP

#include "TcpSocket.hpp"

class TcpProtocol {
    public:
        TcpProtocol(const std::string &host, unsigned short port, const std::string &certificate, const std::string &key);
        ~TcpProtocol();
        bool userDisconnected();
        ReceiveData getNewMessage();
        [[nodiscard]] int getNewDisconnect();
        void send(int id, const std::string& toSend);

    private:
        TcpSocket _sTcp;
};


#endif //SERVER_TCPPROTOCOL_HPP
