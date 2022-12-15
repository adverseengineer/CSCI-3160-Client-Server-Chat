

#ifndef COMMON_H
#define COMMON_H

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

#endif