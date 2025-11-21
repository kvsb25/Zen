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

    }
    catch (const std::exception &e)
    {
        cout << "ERROR: " << WSAGetLastError() << std::endl;
        cout << "ERROR e: " << e.what() << std::endl;
    }

    return 0;
}