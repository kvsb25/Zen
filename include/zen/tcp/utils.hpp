#pragma once
#include<winsock2.h>
#include<iostream>
#include<string>
#include<stdexcept>

class WinsockErr: public std::runtime_error{
public:
    bool cleaned;
    WinsockErr(const char* msg = ""):std::runtime_error(std::string(msg) + "Winsock error | Code: " + std::to_string(WSAGetLastError())), cleaned(false){
        WSACleanup();
        cleaned = true;
    };
    const void clean(){
        cleaned = true;
    }
};