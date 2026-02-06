#include "../../include/zen/tcp/tcpServer.hpp"

TcpServer::TcpServer(u_short port)
{
    try{
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            throw WinsockErr("WSAStartup failed");
        }

        main_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if(main_socket == INVALID_SOCKET){
            throw WinsockErr("main_socket init failed");
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr = INADDR_ANY;

        int bindStatus = bind(main_socket, (SOCKADDR *)&server_addr, sizeof(server_addr));
        if(bindStatus == SOCKET_ERROR){
            throw WinsockErr("main_socket bind failed");
        }

        int listenStatus = listen(main_socket, 20);
        if(listenStatus == SOCKET_ERROR){
            throw WinsockErr("main_socket listen failed");
        }
    } catch (const WinsockErr& e){
        if(!e.cleaned){
            WSACleanup();   
        }
        throw;
    } catch (const std::runtime_error& e){
        throw;
    }
}

TcpServer::~TcpServer(){
    closesocket(main_socket);
    WSACleanup();
    exit(0);
}

SOCKET& TcpServer::getMainSocket(){
    return this->main_socket;
}