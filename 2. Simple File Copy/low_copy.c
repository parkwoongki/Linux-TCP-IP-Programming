#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void error_handling (char * message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main (int argc, char* argv[]){
	int readfd;
	int writefd;
	char buf[BUFSIZ];
	int buf_byte;

	if(argc!=3){
		printf("Usage: %s <The file name you want to copy><The file name you copied>", argv[0]);
		exit(1);
	}

	readfd =open(argv[1], O_RDONLY);
	if(readfd==-1)
		error_handling("File does not exist");

	writefd =open(argv[2], O_CREAT|O_WRONLY|O_TRUNC);
	if(writefd==-1)
		error_handling("open() error");
	// if there is no open() error message, it just succeed.
	
	do{
		buf_byte = read(readfd, buf, 1000);
		printf("copy buf[] bytes : %d \n", buf_byte);
		if(write(writefd, buf, buf_byte)==-1)
			error_handling("write() error");
	}while(buf_byte > 0);
	// Repeat until zero is reached. or when it returns error.
	
	if (buf_byte==-1)
		error_handling("read() error");

	close(readfd);
	close(writefd);
	
	return 0;
}
