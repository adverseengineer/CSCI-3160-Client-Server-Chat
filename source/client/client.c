#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
//#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "common/common.h"
#include "common/config.h"

volatile sig_atomic_t shouldexit = 0;
int sockfd = 0;
char name[CLIENT_NAME_MAX_CHARS];

void prompt(void) {
	printf("> ");
	fflush(stdout);
}

void sigint_handler(int) {
	shouldexit = 1;
}

void send_msg_handler(void) {
	
	char message[LENGTH] = {0};
	char buffer[LENGTH + CLIENT_NAME_MAX_CHARS] = {0};

	while(1) {
		prompt();
		fgets(message, LENGTH, stdin);
		striplf(message, LENGTH);

		if(strcmp(message, "exit") == 0) {
			break;
		}
		else {
			snprintf(buffer, BUFFER_MAX_CHARS, "%s: %s\n", name, message);
			send(sockfd, buffer, strlen(buffer), 0);
		}

		memset(message, '\0', LENGTH);
		memset(buffer, '\0', LENGTH + CLIENT_NAME_MAX_CHARS);
	}
	
	shouldexit = 1;
}

void recv_msg_handler(void) {
	
	char message[LENGTH] = {0};
	
	while(1) {
		ssize_t numbytes = recv(sockfd, message, LENGTH, 0);
		if(numbytes > 0) { 
			printf("%s", message);
			prompt();
		}
		else if(numbytes == 0) {
			//use fprintf here bc recv doesn't set an error message for 0
			fprintf(stderr, "recv returned 0\n");
			break;
		}
		else {
			//use perror here bc the system call has an error message to append
			perror("ERROR");
			break;
		}
		memset(message, '\0', sizeof(message));
	}
}

int main(int argc, char** argv) {
	
	if(argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char* ip = "127.0.0.1";
	int port = atoi(argv[1]);

	//register a signal handler to handle keyboard interrupts
	signal(SIGINT, sigint_handler);

	printf("Please enter your name: ");
	fgets(name, CLIENT_NAME_MAX_CHARS, stdin);
	striplf(name, strlen(name));

	size_t namelen = strlen(name);
	if((namelen > CLIENT_NAME_MAX_CHARS) || (namelen < 2)) {
		printf("Name must be less than 30 and more than 2 characters.\n");
		return EXIT_FAILURE;
	}

	//a struct to represent the address of the server machine
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = htons(port);

	//open the socket for streaming via IPv4
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	//attempt to connect to the server
	ssize_t status = connect(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr));
	if(status == -1) {
		perror("ERROR: Failed to establish connection");
		return EXIT_FAILURE;
	}

	//send the server our name
	send(sockfd, name, CLIENT_NAME_MAX_CHARS, 0);

	printf("connection established, welcome to the chatroom!\n");

	pthread_t send_msg_thread;
	status = pthread_create(&send_msg_thread, NULL, (void*) send_msg_handler, NULL);
	if(status != 0) {
		printf("ERROR: pthread\n");
		return EXIT_FAILURE;
	}

	pthread_t recv_msg_thread;
	status = pthread_create(&recv_msg_thread, NULL, (void*) recv_msg_handler, NULL);
	if(status != 0) {
		printf("ERROR: pthread\n");
		return EXIT_FAILURE;
	}

	//keep main thread alive to keep the other threads going too
	while(!shouldexit);
	printf("\nBye\n");

	close(sockfd);

	return EXIT_SUCCESS;
}
