#include "common.h"
#include "rwmod.h"

int main(int argc, char *argv[]){
	struct sockaddr_in serverAddress, clientAddress;
	int sockFD;
	unsigned char buffer[BUFFER_SIZE];
	CALC_REQ_HDR_t calcReqHdr;
	CALC_RES_t *calcResPtr;
	int numOperand, operator;
	int *operandPtr;
	int operandTmp;
	int messageLen;
	unsigned char *message;
	int answer;
	int i;

	sockFD = generate_tcp_client_socket(SERVER_IP, SERVER_PORT);

	printf("Connected to the calculating server.\n");

	printf("Number of operands: ");
	scanf("%d", &numOperand);
	calcReqHdr.numOperand = (unsigned char)numOperand;
	fgetc(stdin);

	printf("Operator (+, -, *): ");
	// scanf("%c", &operator);
	scanf("%lc", &operator);
	calcReqHdr.operator = (unsigned char)operator;

	printf("calcReqHdr.numOperand: %d\n", calcReqHdr.numOperand);
	printf("calcReqHdr.operator: %d %c\n", calcReqHdr.operator, calcReqHdr.operator);

	// printf("sizeof(CALC_REQ_HDR_t): %d\n", sizeof(CALC_REQ_HDR_t));
	printf("sizeof(CALC_REQ_HDR_t): %ld\n", sizeof(CALC_REQ_HDR_t));

	operandPtr = (int *)malloc(numOperand * sizeof(int));
	for(i=0; i<numOperand; i++){
		printf("Operand %d: ",i);
		scanf("%d", &operandTmp);
		operandPtr[i] = htonl(operandTmp);
		// operandPtr[i] = operandTmp;
	}

	messageLen = sizeof(CALC_REQ_HDR_t) + numOperand * sizeof(int);
	message = (unsigned char *)malloc(messageLen * sizeof(unsigned char));

	memcpy(message, (unsigned char *)&calcReqHdr, sizeof(CALC_REQ_HDR_t));
	memcpy(message + sizeof(CALC_REQ_HDR_t), (unsigned char *)operandPtr, numOperand * sizeof(int));

	printf("Message to send: ");
	for (i=0;i<messageLen;i++)
		printf("%02X ", message[i]);
	printf("\n");

	writen(sockFD, message, messageLen);

	if(readn(sockFD, buffer, sizeof(CALC_RES_t))<=0)
		handle_error("Server error");

	printf("Message received: ");
	for(i=0;i<sizeof(CALC_RES_t); i++)
		printf("%02X ", buffer[i]);
	printf("\n");

	calcResPtr = (CALC_RES_t *)buffer;
	answer = ntohl(calcResPtr->answer);

	printf("Answer: %d\n", answer);
	
	free(operandPtr);
	free(message);
	close(sockFD);

	return 0; // client
}
