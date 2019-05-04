#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netdb.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE	1024
#define SERVER_IP	"127.0.0.1"
#define SERVER_NAME	"localhost.localdomain"
#define SERVER_PORT	60001

//#pragma pack(push)
//#pragma pack(1)
typedef struct{
	unsigned char numOperand;	// uint8_t, 2016
	unsigned char operator;		// uint8_t, 2016
	// int testElement;
} CALC_REQ_HDR_t;

typedef struct {
	int answer;
} CALC_RES_t;
//#pragma pack(pop)


int generate_tcp_listen_socket(unsigned short);
int generate_tcp_client_socket(const char *, unsigned short);
void handle_error(char *);
