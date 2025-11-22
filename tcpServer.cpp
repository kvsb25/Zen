#include <iostream>
#include <fstream>
#include <sstream>
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
                bytesRecv = recv(client_socket, buff + totalBytesRecv, (int)DEFAULT_BUFLEN - totalBytesRecv, 0); // 0 means remote host has performed an orderly shutdown of its end of the connection
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
                string version;
                int status_code;
                string status_text;
                string headers;
                string data;
            };

            Response res;

            string method = request.substr(0,first_space);
            string uri = request.substr(first_space+1, second_space -(first_space + 1));
            res.version = request.substr(second_space+1, third_space - (second_space + 1));
            res.status_code = 200;
            res.status_text = "OK";
            res.headers = "Content-Type: text/html\r\nContent-Length: 1234\r\nDate: Thu, 21 Nov 2025 19:29:07 GMT\r\n";

              // reading html file to send as data
            ifstream html("index.html");

            if(!html.is_open()){
                std::cerr << " ERROR: could not open file index.html " << std::endl;
            }

            string line;
            string fileContent;
            if(getline(html, line)){
                fileContent += line + "\n";
            }

            res.data = fileContent;

            // construct http response

            std::stringstream resStream;

            resStream << res.version << " "<<res.status_code << " "<< res.status_text << "\r\n" << res.headers << "\r\n" << res.data;
            string temp = resStream.str();
            const char* response = temp.c_str();

            // send response over client_socket

            send(client_socket, response, (int)strlen(response), 0); // 0 means remote host has performed an orderly shutdown of its end of the connection

            // shutdown receive and send on socket

            shutdown(client_socket, 2); // 2 for shutting down both, receive and send operations on socket client_socket

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