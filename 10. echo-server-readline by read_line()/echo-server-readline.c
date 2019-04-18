#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include "rwmod.h"

#define BUF_SIZE 1024

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main (int argc, char *argv[]){
	int listenfd, connfd;
	int i=0;
        int nCount=0;
	char message [BUF_SIZE];
	int str_len;

	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_adr_sz;

	if(argc != 2){
		printf("Usage : %s <Port>\n", argv[0]);
		exit(1);
	}

	listenfd = socket(PF_INET, SOCK_STREAM,0);
	if(listenfd ==-1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));
	
	if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("bind() error");

	if(listen(listenfd, 5)==-1)
		error_handling("listen() error");

	clnt_adr_sz = sizeof(clnt_addr);

	for(i=0; i< 5; i++){
		connfd = accept(listenfd, (struct sockaddr*)&clnt_addr, &clnt_adr_sz);
		if(connfd == -1)
			error_handling("accept() error");
		else
			printf("Connected client %d \n", i+1);
		
		while (1){
			if((nCount = read_line(connfd, message, BUF_SIZE))<=0)
				break;
			message[nCount]=0;
			printf("Message from client : %s", message);
			writen(connfd, message, nCount);
		}
		
		printf("Disconnected client : %d \n", i+1);
		close(connfd);
	}
	close(listenfd);

	return 0;
}
