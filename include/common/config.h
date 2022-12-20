//Nick Sells, 2022
//CSCI 3160 Final Project

#ifndef CONFIG_H
#define CONFIG_H

#define BLACKLIST_MAX_ENTRIES 10
#define BLACKLIST_FILEPATH "data/blacklist.txt"

#define MAX_PENDING_CONNECTIONS 10
#define MAX_CLIENTS 10

#define EXITPHRASE "exit"

#define MSG_TEXT_MAX_CHARS 2016
#define CLIENT_NAME_MAX_CHARS 32
#define BUFFER_MAX_CHARS ((MSG_TEXT_MAX_CHARS) + (CLIENT_NAME_MAX_CHARS))

#endif