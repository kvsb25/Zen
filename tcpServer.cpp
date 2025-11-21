#include <iostream>
// #include<exception>
// #include<stdexcept>
#include <winsock2.h>
using namespace std;

int main()
{
    try
    {
        // ******************Initialize Winsock******************
        WSADATA wsaData;
        int iResult;

        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0)
        {
            cout << "WSAStartup failed: " << iResult;
            return 1;
        }
        /********************************************************/

        SOCKET main_socket = INVALID_SOCKET;
        main_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // (int domain, int type, int protocol) | AF_INET -> IPv4; SOCK_STREAM -> Connection oriented; IPPROTO_TCP -> TCP IP protocol

        if (main_socket == INVALID_SOCKET)
        {
            std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
            WSACleanup(); // Clean up before exiting
            return 1;
        }

        // address structure to bind to the socket
        sockaddr_in service;
        service.sin_family = AF_INET;   // supports IPv4 -- must match the socket domain
        service.sin_port = htons(8080); // port at which the socket will exist
        // using htons to convert host byte order to network byte order
        service.sin_addr.s_addr = INADDR_ANY; // it is the IP of the network over which the socket must be active to receive and send data packets. INADDR_ANY represents all network interfaces including local host network

        int bindStatus = bind(main_socket, (SOCKADDR *)&service, sizeof(service));
        if (bindStatus == SOCKET_ERROR)
        {
            std::cout << "Bind failed with error: " << WSAGetLastError() << std::endl;
            closesocket(main_socket);
            WSACleanup();
            return 1;
        }

        while (true)
        {
        };

        closesocket(main_socket);
        WSACleanup();
    }
    catch (const std::exception &e)
    {
        cout << "ERROR: " << WSAGetLastError() << std::endl;
        cout << "ERROR e: " << e.what() << std::endl;
        WSACleanup();
    }

    return 0;
}