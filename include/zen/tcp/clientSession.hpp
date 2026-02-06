#pragma once

#include<iostream>
#include<vector>
#include "utils.hpp"

#define DEFAULT_BUFLEN 8192
#define MAX_REQUEST_SIZE 5242880 // 5MB

class ClientSession{
private:
    SOCKET socket;
public:
    std::vector<char> buff;

public:
    ClientSession(SOCKET& s);
    ~ClientSession();
    std::string recvFromClient(); // call winsock2.h recv(this.socket,...buffer logic); buffer resize using vector to solve overflow without data being turncated
    void sendToClient(const std::string& res); // call winsock2.h send(this.socket, res.c_str(), res.length(), 0)
    void closeSession();
};