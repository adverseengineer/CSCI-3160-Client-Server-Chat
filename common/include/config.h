//Nick Sells, 2022
//CSCI 3160 Final Project

#ifndef CONFIG_H
#define CONFIG_H

#define TERM_LINES 80
#define TERM_COLS 120

#define MSG_TEXT_MAX_CHARS 2016
#define CLIENTNAME_MAX_CHARS 32

#define SERVER_BLACKLIST_MAX_ENTRIES 10
#define SERVER_BLACKLIST_FILEPATH "data/blacklist.txt"

#define SERVER_MAX_PENDING_CONNECTIONS 10
#define SERVER_MAX_CLIENTS 10

#define CLIENT_MAX_SERVERS 10
#define CLIENT_SERVERLIST_FILEPATH "data/servers.txt"

#define CLIENT_EXITPHRASE "exit"

#endif