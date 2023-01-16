//Nick Sells, 2022
//CSCI 3160 Final Project

#ifndef UTIL_H
#define UTIL_H

#include "config.h"
#include <netinet/in.h>

typedef struct {
	struct sockaddr_in address;
	int sockfd;
	int uid;
	char name[CLIENTNAME_MAX_CHARS];
} client_t;

extern void repch(char*, size_t, char, char);
extern void print_ipv4(struct in_addr*);
extern void print_sockaddr(struct sockaddr_in*);
extern void fix_termsize(void);

#endif
