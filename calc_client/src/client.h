/*
 * client.h
 *
 *  Created on: Jul 27, 2016
 *      Author: devteam
 */

#ifndef SRC_CLIENT_H_
#define SRC_CLIENT_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <ctype.h>

#define DEFAULT 0
#define MONITORING 1
#define CALCULATING 2
#define VALID_COMMANDS 9
#define COMMAND_SIZE 10
#define MAXLINE 2000
int sockfd;

extern const char
	valid_commands[VALID_COMMANDS][COMMAND_SIZE];

//
bool work;
int status;

bool check_commands(char *str);

//creating connection with the server
int create_con(char *addr, int port);

//receiving message from the server
void *cli_receive();

//sending message to the server
void cli_send();


#endif /* SRC_CLIENT_H_ */
