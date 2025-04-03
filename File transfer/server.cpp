#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <tchar.h>
#include <iostream>
#include <fstream>
#define blocksize 4096

using namespace std;

void send_message(SOCKET socket, char * message){
    char * buffer = message;
    int bytes = send(socket,buffer, sizeof(buffer), 0);

    if(bytes == SOCKET_ERROR){
        cout<<"error in sending message"<<endl;
    } else{
        cout<<"message sent successfully";
    }
}

char * receive_message(SOCKET socket){
    char * buffer = new char[200];
    int bytes = recv(socket,buffer,200,0);

    if(bytes < 0){
        cout<<"error occured "<<WSAGetLastError()<<endl;
        // return "\0";
    } else{
        cout<<"message recieived is : "<<buffer<<endl;
    }

    return buffer;
}

void sendFile(SOCKET socket, string filepath){
    ifstream file(filepath,ios :: binary);

    if(!file){
        cout<<"error opening file "<<endl; 
    } else{
        char buffer[blocksize];

        cout<<"attemting to send file"<<endl;
        file.read(buffer,blocksize);
        int size;
        while((size = file.gcount())){
            cout<<"size read is : "<<size<<endl;
            int sent = 0;
            while(sent < size){
                int bytes = send(socket, buffer + sent,size - sent,0);
                cout<<"num bytes sent "<<bytes<<endl;
                sent += bytes;
            }
            file.read(buffer,blocksize);
        }
    }
}

void receiveFile(SOCKET socket, string filepath){
    ofstream file(filepath, ios :: binary);

    char buffer[blocksize];

    int bytes_received = 0;
    
    while((bytes_received = recv(socket,buffer,blocksize,0)) > 0){
        cout<<"received bits are: "<<buffer<<endl;
        file.write(buffer,bytes_received);
    }
}
int main() {
    SOCKET serverSocket,acceptSocket;
    int port = 55555;
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2,2);
    wsaerr = WSAStartup(wVersionRequested,&wsaData);
    if(wsaerr != 0){
        cout<<"The Winsock dll was not found"<<endl;
    } else{
        cout<<"Winsock dll found"<<endl;
        cout<<"status is : "<<wsaData.szSystemStatus<<endl;
    }

    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//later try with last arguement as 0

    if(serverSocket == INVALID_SOCKET){
        cout<<"socket not made, error "<<WSAGetLastError()<<endl;
        WSACleanup();
        return 0;
    } else{
        cout<<"socket formed"<<endl;
    }

    sockaddr_in service;
    service.sin_family = AF_INET;

    InetPton(AF_INET,_T("127.0.0.1"),&service.sin_addr.s_addr);

    service.sin_port = htons(port);

    if(bind(serverSocket,(SOCKADDR *) & service,sizeof(service)) == SOCKET_ERROR){
        cout<<"error in binding "<<WSAGetLastError()<<endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    } else{
        cout<<"binding done"<<endl;
    }

    if(listen(serverSocket,1) == SOCKET_ERROR){
        cout<<"error listening "<<WSAGetLastError()<<endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    } else{
        cout<<"server starts listening"<<endl;
    }

    acceptSocket = accept(serverSocket,NULL,NULL);

    if(acceptSocket == INVALID_SOCKET){
        cout<<"error accepting socket "<<WSAGetLastError()<<endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    } else{
        cout<<"Accepted connection"<<endl;

        // char * m = receive_message(acceptSocket);

        sendFile(acceptSocket,"sentfile.png");
    }

    system("pause");

    WSACleanup();
    return 0;
}