#include "pch.h"

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

