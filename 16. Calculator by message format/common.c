#include "common.h"

int generate_tcp_listen_socket(unsigned short serverPort){
	int listenFD;
	struct sockaddr_in serverAddress;

	if((listenFD = socket(AF_INET, SOCK_STREAM, 0))<0)
		handle_error("socket() error");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr= htonl(INADDR_ANY);
	serverAddress.sin_port = htons(serverPort);

	if(bind(listenFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress))<0)
		handle_error("bind() error");

	if(listen(listenFD, 5)<0)
		handle_error("listen() error");

	return listenFD;
}

int generate_tcp_client_socket(const char *serverIP, unsigned short serverPort){
	int sockFD;
	struct sockaddr_in serverAddress;
	struct hostent *hostPtr;

	if((sockFD = socket(AF_INET, SOCK_STREAM, 0))<0)
                handle_error("socket() error");

        memset(&serverAddress, 0, sizeof(serverAddress));
        serverAddress.sin_family = AF_INET;
        hostPtr = gethostbyname(SERVER_NAME);
	serverAddress.sin_addr = *(struct in_addr *)hostPtr->h_addr_list[0];
	// inet_pton(AF_INET, serverIP, &serverAddress.sin_addr);
        serverAddress.sin_port = htons(serverPort);

	if(connect(sockFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress))<0)
		handle_error("connect() error");

	return sockFD;
}

void handle_error(char *message){
	fputs(message,stderr);
	fputs("\n", stderr);
	exit(1);
}
