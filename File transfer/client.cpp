#include "pch.h"
#include "transfer.h"

using namespace std;

int main(){
    SOCKET clientSocket;
    WSADATA wsadata;
    int port = 55555;
    WORD version = MAKEWORD(2,2);

    //load the dll
    int wsaerr = WSAStartup(version,&wsadata);

    if(wsaerr != 0){
        cout<<"error loading dll"<<WSAGetLastError()<<endl;
    } else{
        cout<<"successfully loaded dll"<<endl;
    }

    //create the socket
    clientSocket = socket(AF_INET,SOCK_STREAM,0);

    if(clientSocket == INVALID_SOCKET){
        cout<<"socket could not be formed"<<WSAGetLastError()<<endl;
        WSACleanup();
        return -1;
    }

    else{
        cout<<"socket successfully created"<<endl;
    }

    sockaddr_in service;
    service.sin_family = AF_INET;

    InetPton(AF_INET,_T("127.0.0.1"),&service.sin_addr.s_addr);
    
    service.sin_port = htons(port);

    if(connect(clientSocket,(SOCKADDR *) & service, sizeof(service)) == SOCKET_ERROR){
        cout<<"connection failded "<<endl;
        WSACleanup();
        return -1;
    }
    else{
        cout<<"connection established "<<endl;
        
        // char * message = new char[200];
        // cin.getline(message,200);

        // send_message(clientSocket,message);

        receiveFile(clientSocket,"receivefile.png");
    }

}