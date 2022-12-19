//Nick Sells, 2022
//CSCI 3160 Final Project

#ifndef BLACKLIST_H
#define BLACKLIST_H

extern int blacklist_init(void);
extern void blacklist_print(void);
extern int isblacklisted(struct in_addr*);

#endif