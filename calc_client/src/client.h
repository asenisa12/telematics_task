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

#define MAXLINE 2000
int sockfd;

//
bool work;

//creating connection with the server
int create_con(char *addr, int port);

//receiving message from the server
void *cli_receive();

//sending message to the server
void cli_send();


#endif /* SRC_CLIENT_H_ */
