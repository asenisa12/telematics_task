/*
 * main.c
 *
 *  Created on: Jul 25, 2016
 *      Author: devteam
 */
#include "server.h"


const char *SHARED_MEM_NAME ="/log_calc_server";
const char *SEMAPHORE_NAME="log_semaphore";
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t shared_mem_mutex = PTHREAD_MUTEX_INITIALIZER;

//called when new client has connected to the server
void new_client(int signo)
{
	print_to_cl("server: client has connected\n");
}

//called when client disconnects from server
void client_discon(int signo)
{
	print_to_cl("server: a client is disconnecting\n");
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
	if(shm_unlink(SHARED_MEM_NAME)<0)
		perror("shm_unlink()");
	sem_close(sem);
	if(sem_unlink(SEMAPHORE_NAME)<0)
		perror("sem_unlink()");
	sleep(3);

	int i;
	for(i=0;i<fd_arr_size;i++)
		close(fd_arr[i]);

	free(fd_arr);
	exit(EXIT_SUCCESS);
}

int main(void) {

	lines_count=0;
	unsigned int val=1;
	//shm_unlink(SHARED_MEM_NAME);
	sem =sem_open(SEMAPHORE_NAME, O_CREAT|O_EXCL, O_RDWR, val);
	if(sem == SEM_FAILED)
		perror("sem_open()");
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
