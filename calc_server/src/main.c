/*
 * main.c
 *
 *  Created on: Jul 25, 2016
 *      Author: devteam
 */
#include "server.h"


const char *SERV_MSG_QUEUE ="/serv_q";
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t shared_mem_mutex = PTHREAD_MUTEX_INITIALIZER;

//called when new client has connected to the server
void new_client(int signo)
{
	print_to_cl("client has connected\n");
}

//called when client disconnects from server
void client_discon(int signo)
{
	print_to_cl("a client is disconnecting\n");
}
//called when child process ends
void wait_child(int signo)
{
	int stat;
	pid_t pid;
	while((pid = waitpid(-1,&stat, WNOHANG))>0)
		printf("	server: child with pid:%d exited\n",pid);
}

//called when the server`s work is interrupted by signal
void interupt(int signo)
{
	server_disconn=true;
	print_to_cl("server is disconnecting\n");
	shm_unlink("/log");

	sleep(3);

	int i;
	for(i=0;i<fd_arr_size;i++)
		close(fd_arr[i]);

	free(fd_arr);
	exit(EXIT_SUCCESS);
}

int main(void) {

	lines_count=0;

	struct sockaddr_in client_addr;
	int server_fd = initServer("127.0.0.1", 9080);
	if(server_fd<0)
		perror("server Error");

	int *client_fd;
	logFile = fopen("server.log", "w");
	fclose(logFile);


	signal(SIGCHLD, wait_child);
	signal(NEW_CONN, new_client);
	signal(DEST_CONN, client_discon);
	signal(SIGTERM, interupt);
	signal(SIGINT, interupt);
	printf("starting server...\n");
	server_disconn=false;

	socklen_t addr_size= sizeof(struct sockaddr_in);
	for(;;)
	{
		client_fd = (int*)malloc(sizeof(int));

		if((*client_fd = accept(server_fd,
				(struct sockaddr*)&client_addr,&addr_size)) != -1)
		{
			pthread_t thread_id;
			if(pthread_create(&thread_id, 0,
					&client_handler, (void*)client_fd))
				perror("thread");
			pthread_detach(thread_id);

		}else{
			perror("accept()");
			return EXIT_FAILURE;
		}
	}


	return EXIT_SUCCESS;
}
