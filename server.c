#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>

#include "common.h"
#include "config.h"

static _Atomic unsigned int numclients = 0;
static int uid = 10;

client_t* clients[MAX_CLIENTS];

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void str_overwrite_stdout(void) {
	printf("\r%s", "> ");
	fflush(stdout);
}

void print_client_addr(struct sockaddr_in* addr){
	printf(
		"%d.%d.%d.%d:%d",
		addr->sin_addr.s_addr & 0xff,
		(addr->sin_addr.s_addr & 0xff00) >> 8,
		(addr->sin_addr.s_addr & 0xff0000) >> 16,
		(addr->sin_addr.s_addr & 0xff000000) >> 24,
		addr->sin_port
	);
}

//adds a client
void queue_add(client_t* cl) {

	pthread_mutex_lock(&clients_mutex);

	for(size_t i = 0; i < MAX_CLIENTS; i++) {
		if(clients[i] == NULL) {
			clients[i] = cl;
			break;
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

//removes a client
void queue_remove(int uid) {

	pthread_mutex_lock(&clients_mutex);

	for(int i=0; i < MAX_CLIENTS; i++) {
		if(clients[i]) {
			if(clients[i]->uid == uid) {
				clients[i] = NULL;
				break;
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

//send a message from one client to all other clients
void send_message(char* msg, int uid) {

	pthread_mutex_lock(&clients_mutex);

	for(size_t i = 0; i < MAX_CLIENTS; i++) {
		if(clients[i] != NULL) {
			if(clients[i]->uid != uid) {
				ssize_t numbytes = write(clients[i]->sockfd, msg, strlen(msg));
				if(numbytes < 0) {
					perror("ERROR: write to descriptor failed");
					break;
				}
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

//get a name from the user then start an endless loop of sending and receiving
void* handle_client(void* arg) {

	char buff_out[BUFFER_MAX_CHARS];
	char name[CLIENT_NAME_MAX_CHARS];
	int leave_flag = 0;

	numclients++;
	client_t* cli = arg;

	ssize_t numbytes = recv(cli->sockfd, name, CLIENT_NAME_MAX_CHARS, 0);
	size_t namelen = strlen(name);
	//NOTE: the greater than 2 restriction here is cold code, the greater than 2 chars is enforced client-side
	if((numbytes <= 0) || (namelen < 2) || ((namelen + 1) >= CLIENT_NAME_MAX_CHARS)){
		printf("Didn't enter the name.\n");
		leave_flag = 1;
	}
	else {
		strncpy(cli->name, name, CLIENT_NAME_MAX_CHARS);
		snprintf(buff_out, BUFFER_MAX_CHARS, "%s has joined\n", cli->name);
		printf("%s", buff_out);
		send_message(buff_out, cli->uid);
	}

	memset(buff_out, '\0', BUFFER_MAX_CHARS);

	while(1) {
		
		if(leave_flag) {
			break;
		}

		ssize_t numbytes = recv(cli->sockfd, buff_out, BUFFER_MAX_CHARS, 0);
		if(numbytes > 0) {
			size_t bufflen = strlen(buff_out);
			if(bufflen > 0) {
				send_message(buff_out, cli->uid);

				striplf(buff_out, bufflen);
				printf("%s\n", buff_out);
			}
		}
		else if(numbytes == 0 || strcmp(buff_out, "exit") == 0) {
			snprintf(buff_out, BUFFER_MAX_CHARS, "%s has left\n", cli->name);
			printf("%s", buff_out);
			send_message(buff_out, cli->uid);
			leave_flag = 1;
		}
		else {
			printf("ERROR: -1\n");
			leave_flag = 1;
		}

		memset(buff_out, '\0', BUFFER_MAX_CHARS);
	}

	//if the loop exits, the thread must die, so clean up
	close(cli->sockfd);
	queue_remove(cli->uid);
	free(cli);
	numclients--;
	pthread_detach(pthread_self());

	return NULL;
}

int main(int argc, char** argv){
	
	if(argc != 2){
		fprintf(stderr, "Usage: %s <port>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char* ip = "127.0.0.1";
	int port = atoi(argv[1]);	

	//a struct to represent this machine's address
	struct sockaddr_in serv_addr;	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ip);
	serv_addr.sin_port = htons(port);
	
	//open a socket for streaming via IPv4
	ssize_t listenfd = socket(AF_INET, SOCK_STREAM, 0);

	//register a signal handler for ingnoring SIGPIPE
	signal(SIGPIPE, SIG_IGN);

	//change settings for the socket. extremely wonky API for this: we refer
	//to the setting by a macro, and the value for that setting is represented
	//by an int which we pass a pointer to. why this doesn't just take the int
	//by value is beyond me
	int option = 1;
	ssize_t status = setsockopt(
		listenfd,
		SOL_SOCKET,
		(SO_REUSEPORT | SO_REUSEADDR),
		(char*)&option,
		sizeof(option)
	);
	if(status < 0) {
		perror("ERROR: setsockopt failed");
		return EXIT_FAILURE;
	}

	//bind the socket to an address
	status = bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if(status < 0) {
		perror("ERROR: Socket binding failed");
		return EXIT_FAILURE;
	}

	//mark the socket as passive, one that can accept incoming connections
	status = listen(listenfd, MAX_PENDING_CONNECTIONS);
	if(status < 0) {
		perror("ERROR: Socket listening failed");
		return EXIT_FAILURE;
	}

	printf("chat room open, now accepting connections...\n");

	struct sockaddr_in cli_addr;
	ssize_t connfd = 0;

	while(1) {
	
		//accept an incoming connection and populate a socket fd to talk to it
		socklen_t clilen = sizeof(cli_addr);
		connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &clilen);

		//check if the server can handle another client
		if((numclients + 1) == MAX_CLIENTS) {
			printf("server at max capacity. connection rejected from ");
			print_client_addr(&cli_addr);
			putc('\n', stdout);
			close(connfd);
			continue;
		}
	
		//allocate a new client struct and set it up
		client_t* cli = malloc(sizeof(*cli));
		cli->address = cli_addr;
		cli->sockfd = connfd;
		cli->uid = uid++;

		//add the client to the list and fork the thread to handle them
		queue_add(cli);
		pthread_t tid;
		pthread_create(&tid, NULL, &handle_client, cli);

		//let the process rest for a moment before listening for more connections
		sleep(1);
	}

	return EXIT_SUCCESS;
}
