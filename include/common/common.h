

#ifndef COMMON_H
#define COMMON_H

#include <netinet/in.h>

#include "config.h"

typedef struct {
	struct sockaddr_in address;
	int sockfd;
	int uid;
	char name[CLIENT_NAME_MAX_CHARS];
} client_t;

static inline void striplf(char* arr, size_t length) {
	for(size_t i = 0; i < length; i++) {
		if(arr[i] == '\n') {
			arr[i] = '\0';
			break;
		}
	}
}

//prints out an in_addr struct nicely
static inline void print_ipv4(struct in_addr* addr) {

	//NOTE: this line should be platform-indepent. in_addr is implementation-
	//defined, the standard requires only that it contains the s_addr field
	unsigned long ip = addr->s_addr;

	printf(
		"%lu.%lu.%lu.%lu",
		ip & 0xFF,
		(ip & 0xFF00) >> 8,
		(ip & 0xFF0000) >> 16,
		(ip & 0xFF000000) >> 24
	);
}

//prints out a sockaddr_in struct nicely
static inline void print_sockaddr(struct sockaddr_in* addr){
	
	print_ipv4(&addr->sin_addr);
	printf(":%d", addr->sin_port);
}

#endif