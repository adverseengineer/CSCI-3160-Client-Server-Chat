//Nick Sells, 2022
//CSCI 3160 Final Project

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>

#include "server/blacklist.h"
#include "common/common.h"
#include "common/config.h"

static size_t listindex = 0;
static struct in_addr blacklist[BLACKLIST_MAX_ENTRIES];

//reads the blacklist from disk and validates all ip's contained within
//TODO: put through rigorous testing, there's no way this just works without a snag
int blacklist_init(void) {

	FILE* fp = fopen(BLACKLIST_FILEPATH, "r");
	if(fp == NULL) {
		//NOTE: perror() isn't printing the colon like it should. it seems if
		//you give it any whitespace character, a null terminator, or just
		//null, it doesn't print the colon before the error. i'll just fprintf
		//it for now
		fprintf(stderr, "Unable to open blacklist file \"%s\": ", BLACKLIST_FILEPATH);
		perror(NULL);
		return EXIT_FAILURE;
	}
	
	//NOTE: as long as line is null and len is 0, getline will malloc the line,
	//make sure to free() it
	char* line = NULL;
	size_t len = 0;
	
	for(size_t i = 0; i < BLACKLIST_MAX_ENTRIES; i++) {
	
		ssize_t nread = getline(&line, &len, fp);
		if(nread == -1) break;
		
		//we must strip \n from each line, because inet_pton() hates them
		striplf(line, strlen(line));
		
		//we skip blank lines
		if(line[0] == '\0')
			continue;
		
		int status = inet_pton(AF_INET, line, &blacklist[listindex]);
		if(status == 1)
			listindex++;
		else if(status == 0)
			fprintf(stderr, "WARNING: Malformed IPv4 address at line %zu in %s\n", (i+1), BLACKLIST_FILEPATH);
	}
	
	free(line);
	fclose(fp);
	
	return EXIT_SUCCESS;
}

//prints out the entire blacklist
void blacklist_print(void) {

	for(size_t i = 0; i < listindex; i++) {
	
		printf("%zu: ", i);
		print_ipv4(&blacklist[i]);
		putchar('\n');
	}
}

//checks whether a given ip is blacklisted
//NOTE: in_addr structs store the ip in little-endian
int isblacklisted(struct in_addr* addr) {

	//if the given ip matches any of the ones in the list, it's no bueno
	for(size_t i = 0; i < listindex; i++) {
		if(addr->s_addr == blacklist[i].s_addr)
			return 1;
	}
	
	return 0;
}

//to do tomorrow:
//review the license
//work on fixing the compiler warnings about string ops overflowing. review order of header includes and 
//and add the capability for 
//comments in the blacklist file, i.e. reasons for ban, something a sysadmin would use

//OH, and RIGOROUSLY test blacklist_init(). there's no way there's nothing wrong with it

//and if you think it wise, start qualifying params with const where applicable

//near future feature to add: local files for client preferences, i.e. username read from file instead
//of having to enter it, and preferred color (for when i make it an ncurses panel front-end)

//verify that the temp variable to store the result of every system call is the same type as the return
//type of the system call, and not just a compatible type, i.e. ssize_t to int
