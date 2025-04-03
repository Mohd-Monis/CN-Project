#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <tchar.h>
#include <fstream>
#define blocksize 4096

using namespace std;

int send_message(SOCKET socket, char * message){
    char * buffer = message;
    int bytes = send(socket,buffer, sizeof(buffer), 0);

    if(bytes == SOCKET_ERROR){
        cout<<"error in sending message"<<endl;
        return -1;
    } else{
        cout<<"message sent successfully";
        return 0;
    }
}

char * receive_message(SOCKET socket){
    char * buffer = new char[200];
    int bytes = recv(socket,buffer,200,0);

    if(bytes < 0){
        cout<<"error occured "<<WSAGetLastError()<<endl;
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

        file.read(buffer,blocksize);
        while(file.gcount()){
            char * message;
            strcpy(message,buffer);
            int status = send_message(socket,message);
            if(status){
                break;
            }
            file.read(buffer,blocksize);
        }
    }
}

void receiveFile(SOCKET socket, string filepath){
    ofstream file(filepath, ios :: binary);

    char buffer[blocksize];

    int bytes_received = 0;
    
    while((bytes_received = recv(socket,buffer,sizeof(buffer),0)) > 0){
        cout<<"received bits are: "<<buffer<<endl;
        file.write(buffer,bytes_received);
    }
}

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

        receiveFile(clientSocket,"received.txt");
    }

}