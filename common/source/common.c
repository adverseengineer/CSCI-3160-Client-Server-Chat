//Nick Sells, 2022
//CSCI 3160 Final Project

#include "common.h"
#include <stdio.h>

//replaces the first occcurence of oldch in str with newch, up to len chars
void repch(char* str, size_t len, char oldch, char newch) {
	for(size_t i = 0; i < len; i++) {
		if(str[i] == oldch) {
			str[i] = newch;
			break;
		}
	}
}

//pretty-prints an in_addr struct
void print_ipv4(struct in_addr* addr) {

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

//pretty-prints a sockaddr_in struct
void print_sockaddr(struct sockaddr_in* addr){
	
	print_ipv4(&addr->sin_addr);
	printf(":%d", addr->sin_port);
}
