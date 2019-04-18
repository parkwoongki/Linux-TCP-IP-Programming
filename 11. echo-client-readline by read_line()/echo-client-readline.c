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

int main(int argc, char *argv[]){
	int sockfd, str_len, nCount=0;
	char message[BUF_SIZE];
	struct sockaddr_in serv_addr;

	if(argc != 3){
		printf("Usage : %s <IP> <Port>\n", argv[0]);
		exit(1);
	}

	sockfd = socket(PF_INET, SOCK_STREAM, 0); // TCP/IP
	if(sockfd == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("connect() error");
	else
		puts("Connected............");

	while(1){
		fputs("Input message(Q to quit): ",stdout);
		fgets(message, BUF_SIZE, stdin);

		if(!strcmp(message, "q\n") || !strcmp(message,"Q\n"))
			break;

		writen(sockfd, message, strlen(message));

		if((nCount = read_line(sockfd, message, BUF_SIZE))<=0)
			error_handling("Server terminated!");
				
		message[nCount]=0;
		printf("Message from server : %s", message);
	}
	close(sockfd);
	return 0;
}
