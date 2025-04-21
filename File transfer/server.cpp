#include "pch.h"
#include "transfer.h"

using namespace std;

int main()
{
    SOCKET serverSocket, acceptSocket;
    int port = 55555;
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if (wsaerr != 0)
    {
        cout << "The Winsock dll was not found" << endl;
    }
    else
    {
        cout << "Winsock dll found" << endl;
        cout << "status is : " << wsaData.szSystemStatus << endl;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // later try with last arguement as 0

    if (serverSocket == INVALID_SOCKET)
    {
        cout << "socket not made, error " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else
    {
        cout << "socket formed" << endl;
    }

    sockaddr_in service;
    service.sin_family = AF_INET;

    InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);

    service.sin_port = htons(port);

    if (bind(serverSocket, (SOCKADDR *)&service, sizeof(service)) == SOCKET_ERROR)
    {
        cout << "error in binding " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else
    {
        cout << "binding done" << endl;
    }

    if (listen(serverSocket, 1) == SOCKET_ERROR)
    {
        cout << "error listening " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else
    {
        cout << "server starts listening" << endl;
    }

    sockaddr_in clientAddr = {};
    int clientAddrSize = sizeof(clientAddr);
    acceptSocket = accept(serverSocket, (sockaddr *) & clientAddr, &clientAddrSize);

    if (acceptSocket == INVALID_SOCKET)
    {
        cout << "error accepting socket " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else
    {
        cout << "Accepted connection" << endl;

        // char * m = receive_message(acceptSocket);

        // create_unique_image("sentfile.png",clientIp);

        sendFile(acceptSocket, "sentfile.png");

        return 0;
    }

    system("pause");

    WSACleanup();
    return 0;
}