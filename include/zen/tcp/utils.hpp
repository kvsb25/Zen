#pragma once
#include<winsock2.h>
#include<iostream>
#include<string>
#include<stdexcept>

class WinsockErr: public std::runtime_error{
public:
    bool cleaned;
    // const char * because it is lighter and efficient than std::string
    WinsockErr(const char* msg = ""):std::runtime_error(std::string(msg) + "Winsock error | Code: " + std::to_string(WSAGetLastError())), cleaned(false){
        WSACleanup();
        cleaned = true;
    }
    const void clean(){
        cleaned = true;
    }
};

class ClientSockErr: public std::runtime_error{
public:
    bool cleaned;
    ClientSockErr(int msg):std::runtime_error("Client Socket Error: " + std::to_string(msg)), cleaned(false){
        // do client socket cleaning
    }
};

class CriticalErr: public std::runtime_error{
public:
    CriticalErr(const char* msg = "") : std::runtime_error(std::string(msg) + "CRITICAL ERROR, exiting..."){}
};