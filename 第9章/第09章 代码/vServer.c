/**
Victim Server. Socket Server Example. 
vServer.c   By Fanping Zeng. 10/03/2013.
gcc -fno-stack-protector -z execstack -o vServer ../src/vServer.c
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#define SMALL_BUFF_LEN 64
void overflow(char Lbuffer[])
{
    char smallbuf[SMALL_BUFF_LEN];
    strcpy(smallbuf, Lbuffer);	
}

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    int sockfd = 0, n = 0;
    char recvBuff[1024];

    if(argc<2){
        printf("Usage: %s <listening port number>.\n", argv[0]);
        return 1;
    }
    if(atoi(argv[1])<1024){
        printf("Error: The listening port number of %s must be more than 1024.\n", argv[0]);
        return 1;
    }

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);
    printf("OK: %s is listening on TCP:%d\n", argv[0], atoi(argv[1]));

    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
		if(connfd==-1) continue;
		//if(fork()!=0) continue;

		// I am child process and do the job.
        if((n = read(connfd, recvBuff, sizeof(recvBuff)-1)) > 0){
            recvBuff[n] = 0;
			printf("Received %d bytes from client.\n", strlen(recvBuff));
            overflow(recvBuff);
        }

        close(connfd);
        sleep(1);
     }
}


