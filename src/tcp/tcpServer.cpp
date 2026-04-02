#include "../../include/zen/tcp/tcpServer.hpp"

TcpServer::TcpServer(u_short port)
{
// server is in blocking mode so that whenever INVALID_SOCKET arrives the main_thread sleeps until a valid socket arrives 
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
        throw CriticalErr(e.what());
    } catch (const std::runtime_error& e){
        throw CriticalErr(e.what());
    }
    // the catch block checks error from derived to base. First it will check if the thrown error matches exactly(with the catch definition), if not then it will check if the definition it has is a direct parent of the thrown error. This is the reason we have mentione WinsockErr first so it gets identified first and the order of catch satetements is in Derived to base
}

TcpServer::~TcpServer(){
    closesocket(main_socket);
    WSACleanup();
    // exit(0);
}

SOCKET& TcpServer::getMainSocket(){
    return this->main_socket;
}