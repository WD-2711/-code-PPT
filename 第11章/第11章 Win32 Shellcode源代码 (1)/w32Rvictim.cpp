/** w32Rvictim.cpp
  Coded by Fanping Zeng(USTC).
  cl /Zi /GS- w32Rvictim.cpp
*/
//====================================================================

#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#pragma comment(lib, "wininet.lib")
#pragma comment(lib,"ws2_32.lib")

#define BUFFER_LEN 128
void overflow(char* attackStr)
{
    char buffer[BUFFER_LEN];
    strcpy(buffer,attackStr);
}

#define RECEIVE_BUFFER_LEN 2048

int main(int argc, char * argv[]) 
{
    if(argc<2){
        printf("Usage: %s TCP-Port-Number.\n", argv[0]);    return 1;
    }
    if(atoi(argv[1])<1024){
        printf("The listen port cannot less than 1024.\n"); return 1;
    }
    //----------------------
    // Initialize Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != NO_ERROR) {
        printf("Error at WSAStartup()\n");  return 1;
    }
    //----------------------
    // Create a SOCKET for listening for
    // incoming connection requests.
    SOCKET ListenSocket;
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();   return 1;
    }

    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being bound.
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(atoi(argv[1]));

    if (bind( ListenSocket, (SOCKADDR*) &service, 
        sizeof(service)) == SOCKET_ERROR) {
        printf("bind() failed.\n");
        closesocket(ListenSocket);
        WSACleanup();   return 1;
    }
    //----------------------
    // Listen for incoming connection requests.
    // on the created socket
    if (listen( ListenSocket, 1 ) == SOCKET_ERROR) {
        printf("Error listening on socket.\n");
        closesocket(ListenSocket);
        WSACleanup();   return 1;
    }
    //----------------------
    // Create a SOCKET for accepting incoming requests.
    SOCKET AcceptSocket;
    printf("Waiting for client to connect TCP:%d ...\n", atoi(argv[1]));

    //----------------------
    // Accept the connection.
    AcceptSocket = accept( ListenSocket, NULL, NULL );
    if (AcceptSocket == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();   return 1;
    } else
    printf("Client connected.\n");
    // No longer need server socket
    closesocket(ListenSocket);

    // receive from Accept socket
    char Buff[RECEIVE_BUFFER_LEN];
    long lBytesRead;

    while(1){
        lBytesRead=recv(AcceptSocket,Buff,RECEIVE_BUFFER_LEN,0);
        if(lBytesRead<=0)   break;
        printf("\nAcceptSocket ID = %x\n\tRead %d bytes\n", AcceptSocket,lBytesRead);
        Buff[lBytesRead]='\0';
        overflow(Buff);
        iResult=send(AcceptSocket,Buff,lBytesRead,0);
        if(iResult<=0)  break;
    }
    closesocket(AcceptSocket);

    WSACleanup();
    return 0;
}
