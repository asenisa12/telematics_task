/*
 * main.c
 *
 *  Created on: Jul 27, 2016
 *      Author: devteam
 */
#include "client.h"


void interupt(int signo)
{
	puts("INTERUPT\n");
	close(sockfd);
	exit(EXIT_SUCCESS);
}

int main()
{
	sockfd = create_con("127.0.0.1",9080);
	if(sockfd>0)
	{
		signal(SIGINT, interupt);
		pthread_t thread_id;
		if(pthread_create(&thread_id, 0,&cli_receive, NULL)!=0)
		{
			perror("thread");
			exit(EXIT_FAILURE);
		}
		else
			cli_send();
	}
	exit(EXIT_SUCCESS);
}

