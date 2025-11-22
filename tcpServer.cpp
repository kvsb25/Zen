#include <iostream>
// #include<exception>
// #include<stdexcept>
#include <winsock2.h>
using namespace std;

#define DEFAULT_PORT 8080
#define DEFAULT_BUFLEN 512

int main()
{
    try
    {
        // ******************Initialize Winsock******************
        WSADATA wsaData;
        int iResult;
        char *buff = new char[DEFAULT_BUFLEN];
        // int buffLen = DEFAULT_BUFLEN;

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
            std::cout << "Error at main_socket: " << WSAGetLastError() << std::endl;
            WSACleanup(); // Clean up before exiting
            return 1;
        }

        // address structure to bind to the socket
        sockaddr_in service;
        service.sin_family = AF_INET;           // supports IPv4 -- must match the socket domain
        service.sin_port = htons(DEFAULT_PORT); // port at which the socket will exist
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

        // listen
        if (listen(main_socket, 20) == SOCKET_ERROR)
        {
            std::cout << "Listen failed with error: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return 1;
        }

        // main loop
        while (true)
        {
            // accepting client connection, and assigning a socket for data exchange
            SOCKET client_socket = accept(main_socket, NULL, NULL); //
            if (client_socket == INVALID_SOCKET)
            {
                std::cout << "Error at client_socket: " << WSAGetLastError() << std::endl;
                WSACleanup(); // Clean up before exiting
                return 1;
            }

            int bytesRecv;
            int totalBytesRecv = 0;
            do
            {
                if((int)DEFAULT_BUFLEN - totalBytesRecv <= 0){
                    cout << "recv buffer full " << std::endl;
                    break;
                }

                // preventing overflow this way. But it might cause data loss on the application layer, so will implement vector buffer later
                bytesRecv = recv(client_socket, buff + totalBytesRecv, (int)DEFAULT_BUFLEN - totalBytesRecv, 0);
                if (bytesRecv > 0)
                {
                    totalBytesRecv += bytesRecv;
                    cout << "bytes received: " << bytesRecv << std::endl;
                } else {
                    cout << "recv failed: " << WSAGetLastError() << std::endl;
                    WSACleanup();
                    return 1;
                }

            } while(bytesRecv > 0);

            //***************************************** Process data *****************************************
            string request(buff); // converting char* to std::string

            int first_space = request.find(' ');
            int second_space = request.find(' ', first_space + 1);
            int third_space = request.find('\r\n', second_space + 1);
            struct Response{
                string method;
                string uri;
                string version;
                string data;
            };

            Response res;

            res.method = request.substr(0,first_space);
            res.uri = request.substr(first_space+1, second_space -(first_space + 1));
            res.version = request.substr(second_space+1, third_space - (second_space + 1));
            res.data = "this is a test response from tcpServer";

            cout<<res.method<< " " << res.uri << " " << res.version << " ";

            closesocket(client_socket);
        };

        iResult = closesocket(main_socket);
        if (iResult == SOCKET_ERROR)
        {
            std::cout << "socket close failed with error: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return 1;
        }
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