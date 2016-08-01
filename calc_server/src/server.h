/*
 * server.h
 *
 *  Created on: Jul 25, 2016
 *      Author: devteam
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <mqueue.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "message.pb-c.h"
#include "message_utils.h"
#include <sys/mman.h>
#include <stdbool.h>


#define SERV_ERR -1
#define BUFF_SIZE 2000
#define MAIN_PROC 0
#define MAX_LOG_LINES 10
static const int SHARED_MEM_SIZE = 2;

static const int NEW_CONN = 30;
static const int DEST_CONN = 16;


extern pthread_mutex_t client_mutex;
extern pthread_mutex_t shared_mem_mutex;

int fd_arr_size;
int *fd_arr;
int lines_count;
int log_line_size[MAX_LOG_LINES];


int shared_mem_size;
FILE *logFile;

//indicates that the server is disconnecting
bool server_disconn;

struct sockaddr_in serv_addr;

//initializing  function for the server
int initServer(char *addr, int port);

//adds the descriptor of a client to the fd_arr;
void add_cld(int fd);

//removes the descriptor of a client to the fd_arr;
void rm_cld(int fd);

//writes in the log file or memory for new server event
//or client operation
void write_log_shm(char *buffer, int len);
void write_log_file(char *buffer, int len);

//reads the log file or memory and sends it`s content to the client
void read_log_shm(int sock_d);
void read_log_file(int sock_d);

//print "msg" to all clients currently connected
void print_to_cl(char *msg);

//sending the result of a calculation to the client
void send_result(int sock_d, char *pattern ,  double num1,
		double num2, double result);

void calculate(int sock_d,char *msg, double num1, double num2);


//does work like calculating or saving the numbers received
//from the client
void do_work(CalcMessage *c_msg, int sockd, bool *async,
		int *current, double *numbers);

//gets the address of a shared memory
void *get_sharedMem_addr(char *name,  int *fd);

//checking if the client has sent commands for task different
//from saving or calculating numbers
bool check_for_commands(CalcMessage *c_msg,
		int client_fd, bool *async);

//the function for handling the operations of the clients
void *client_handler(void *arg);

int get_log_size();


#endif /* SERVER_H_ */
