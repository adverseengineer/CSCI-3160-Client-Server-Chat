//Nick Sells, 2022
//CSCI 3160 Final Project

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

extern void repch(char* str, size_t len, char oldch, char newch);
extern void print_ipv4(struct in_addr* addr);
extern void print_sockaddr(struct sockaddr_in* addr);

#endif