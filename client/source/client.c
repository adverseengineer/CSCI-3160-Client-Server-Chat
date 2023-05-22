//Nick Sells, 2022
//CSCI 3160 Final Project

#include "util.h"
#include "config.h"

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

volatile sig_atomic_t shouldexit = 0;
int sockfd = 0;
char name[CLIENTNAME_MAX_CHARS];

void prompt(void) {
	printf("> ");
	fflush(stdout);
}

void sigint_handler(int) {
	shouldexit = 1;
}

//runs as a thread to handle outgoing messages
void send_msg_handler(void) {
	
	size_t bufflen = MSG_TEXT_MAX_CHARS + CLIENTNAME_MAX_CHARS;
	char buffer[bufflen];
	char message[MSG_TEXT_MAX_CHARS];
	
	while(1) {
		
		memset(buffer, '\0', bufflen);
		memset(message, '\0', MSG_TEXT_MAX_CHARS);
		
		//read out message from stdin and copy into the message buffer
		prompt();
		fgets(message, MSG_TEXT_MAX_CHARS, stdin);
		repch(message, MSG_TEXT_MAX_CHARS, '\n', '\0');

		if(strcmp(message, CLIENT_EXITPHRASE) == 0) {
			break;
		}
		else {
			//copy our name and the message contents into the output buffer
			snprintf(buffer, bufflen, "%s: %s\n", name, message);
			send(sockfd, buffer, strlen(buffer), 0);
		}
	}
	
	shouldexit = 1;
}

//runs as a thread to handle incoming messages
void recv_msg_handler(void) {
	
	//buffer large enough to hold message text and sender name
	size_t bufflen = MSG_TEXT_MAX_CHARS + CLIENTNAME_MAX_CHARS;
	char buffer[bufflen];
	
	while(1) {
		
		memset(buffer, '\0', bufflen);
		
		ssize_t numbytes = recv(sockfd, buffer, bufflen, 0);
		if(numbytes > 0) { 
			printf("%s", buffer);
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
	}
}

int client_establishconnection(char* portstr) {

	char* ip = "127.0.0.1";
	int port = atoi(portstr);

	printf("Please enter your name: ");
	fgets(name, CLIENTNAME_MAX_CHARS, stdin);
	repch(name, strlen(name), '\n', '\0');

	size_t namelen = strlen(name);
	if((namelen > CLIENTNAME_MAX_CHARS) || (namelen < 2)) {
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
	send(sockfd, name, namelen, 0);

	printf("connection established, welcome to the chatroom!\n");
	return EXIT_SUCCESS;
}

void client_closeconnection(void) {
	
	close(sockfd);
}

int main(int argc, char** argv) {
	
	if(argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		return EXIT_FAILURE;
	}

	//register a signal handler to handle keyboard interrupts
	signal(SIGINT, sigint_handler);

	client_establishconnection(argv[1]);

	pthread_t send_msg_thread;
	int status = pthread_create(&send_msg_thread, NULL, (void*) send_msg_handler, NULL);
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

	client_closeconnection();

	return EXIT_SUCCESS;
}
