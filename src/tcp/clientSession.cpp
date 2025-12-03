#include "../../include/zen/tcp/clientSession.hpp"

ClientSession::ClientSession(SOCKET& s) : socket(s), buff(DEFAULT_BUFLEN){}

ClientSession::~ClientSession(){}

std::string ClientSession::recvFromClient(){
    int bytesRecv;
    int totalBytesRecv = 0;

    if(buff.empty())
        buff.resize(DEFAULT_BUFLEN);

    while(true){
        if( buff.size() == totalBytesRecv){
            buff.resize(buff.size()*2);
        }

        // call recv(for client's socket, buff starting mem addr, available space that can be filled, remote entity closes the connection)
        bytesRecv = recv(
            this->socket, 
            buff.data() + totalBytesRecv, 
            buff.size() - totalBytesRecv, 
            0
        );

        // check if all data received 
        if(bytesRecv > 0){
            totalBytesRecv += bytesRecv;
        } else if(bytesRecv == 0) {
            break;
        } else {
            std::cout << "recv failed: " << WSAGetLastError() << std::endl;
            break;
        }
    }
    
    return std::string(buff.data(), totalBytesRecv);
}