

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>

#include "../include/common/config.h"

//NOTE: when i implement ncurses, this is probably gonna be replaced
typedef enum {
	BLACK = 0b0000,
	BLUE = 0b0001,
	GREEN = 0b0010,
	CYAN = 0b0011,
	RED = 0b0100,
	MAGENTA = 0b0101,
	BROWN = 0b0110,
	WHITE = 0b0111
} termcolor_t;

typedef struct {
	char name[CLIENT_NAME_MAX_CHARS];
	struct sockaddr_in server_addr;
	termcolor_t color;
	
} clientprefs_t;

#define DEFAULT_SERVER_IP "127.0.0.1"

static clientprefs_t prefs;

int clientprefs_load(char* filepath) {

	FILE* fp = fopen(filepath, "r");
	if(fp == NULL) {
		//NOTE: perror() isn't printing the colon like it should. it seems if
		//you give it any whitespace character, a null terminator, or just
		//null, it doesn't print the colon before the error. i'll just fprintf
		//it for now
		fprintf(stderr, "Unable to open client preferences file \"%s\": ", filepath);
		perror(NULL);
		return EXIT_FAILURE;
	}
	
	fread(&prefs, sizeof(clientprefs_t), 1, fp);
	
	fclose(fp);

	return EXIT_SUCCESS;
}

int main(int argc, char** argv) {

	if(argc != 2) {
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	clientprefs_load("prefs"
}