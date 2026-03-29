#pragma once

#include<iostream>
#include<vector>
#include "utils.hpp"
#include "../thread-pool/thread-pool.hpp"

#define DEFAULT_BUFLEN 8192
#define MAX_REQUEST_SIZE 5242880 // 5MB

class ClientSession{
private:
    SOCKET socket;
public:
    std::vector<char> buff = std::vector<char>(DEFAULT_BUFLEN);

public:
    ClientSession();
    ClientSession(SOCKET& s);
    
    // delete copy constructor to implement exclusive ownership of client session socket
    ClientSession(const ClientSession& cs) = delete;
    ClientSession& operator=(const ClientSession& cs) = delete;

    // implement move assignment
    ClientSession& operator=(ClientSession&& other) noexcept;

    // implement move semantics so that ownership can be moved from one object to other
    ClientSession(ClientSession&& other) noexcept;

    ~ClientSession();
    std::string recvFromClient(); // call winsock2.h recv(this.socket,...buffer logic); buffer resize using vector to solve overflow without data being turncated
    void sendToClient(const std::string& res); // call winsock2.h send(this.socket, res.c_str(), res.length(), 0)
    void closeSession();
};