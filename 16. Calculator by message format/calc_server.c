#include "common.h"
#include "rwmod.h"

int main(int argc, char *argv[]){
	struct sockaddr_in clientAddress;
	socklen_t clientAddressLen;
	int listenFD, connFD;
	unsigned char buffer[BUFFER_SIZE];
	CALC_REQ_HDR_t *calcReqHdrPtr;
	int messageLen;
	unsigned char *message;
	unsigned char numOperand, operator;
	int *operandPtr;
	int answer=0;
	int i=0;

	listenFD = generate_tcp_listen_socket(SERVER_PORT);

	for(;;){
		clientAddressLen = sizeof(clientAddress);
		
		if((connFD =accept(listenFD, (struct sockaddr *)&clientAddress, &clientAddressLen))<0)
			handle_error("accept() error");

		if(readn(connFD, buffer, sizeof(CALC_REQ_HDR_t))<=0){
			close(connFD);
			continue;
		}

		calcReqHdrPtr = (CALC_REQ_HDR_t *)buffer;

		numOperand = calcReqHdrPtr->numOperand;
		operator = calcReqHdrPtr->operator;

		printf("numOperand: %d\n", numOperand);
		printf("operator: %d %c\n", operator, operator);

		if(readn(connFD, buffer, numOperand*sizeof(int))<=0){
			close(connFD);
			continue;
		}

		printf("Message (operands) received: ");
		for(i=0;i<numOperand*sizeof(int);i++)
			printf("%02X ",buffer[i]);
		printf("\n");

		operandPtr = (int *)malloc(numOperand * sizeof(int));
		for(i=0; i<numOperand;i++){
			operandPtr[i] = ntohl(*(int *)&buffer[i*sizeof(int)]);
			printf("operatorPtr[%d]: %d\n", i, operandPtr[i]);
		}
		answer = operandPtr[0];

		switch(operator){
		case '+':
			printf("+ operation.\n");
			for(i=1;i<numOperand;i++)
				answer+=operandPtr[i];
			break;
		case '-':
                        printf("- operation.\n");
                        for(i=1;i<numOperand;i++)
                                answer-=operandPtr[i];
                        break;
		case '*':
                        printf("* operation.\n");
                        for(i=1;i<numOperand;i++)
                                answer*=operandPtr[i];
                        break;
		default:
                        printf("Unavailable operation.\n");
                        break;
		}
			
		printf("Answer: %d\n", answer);
		answer = htonl(answer);

		messageLen = sizeof(CALC_RES_t);
		message = (unsigned char *)malloc(messageLen * sizeof(unsigned char));
		memcpy(message, (unsigned char *)&answer, messageLen);

		printf("Message to send: ");
		for(i=0; i<messageLen;i++)
			printf("%02X ", message[i]);
		printf("\n");

		//answer = htonl(answer);
		writen(connFD, &answer, sizeof(answer));

		close(connFD);
	}

	free(operandPtr);
	free(message);
	close(listenFD);

	return 0; // server
}
