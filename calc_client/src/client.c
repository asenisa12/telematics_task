/*
 * client.c
 *
 *  Created on: Jul 27, 2016
 *      Author: devteam
 */

#include "client.h"

void* cli_receive(){
	pthread_detach(pthread_self());
	uint8_t server_reply[MAXLINE];

	char *serv_discon_msg="server is disconnecting";
	int size;
	while((size = recv(sockfd , server_reply , MAXLINE, 0)) > 0)
	{
		CalcMessage *c_msg =calc_message__unpack(NULL,
				size, server_reply);

		if(c_msg->id==MESSAGE)
		{
			printf("%s\n",c_msg->msg);
			if(strncmp(c_msg->msg, serv_discon_msg,
				strlen(serv_discon_msg))==0)
			{
					printf("bye\n");
					work=false;
					kill(getpid(),SIGINT);
			}
		}

		calc_message__free_unpacked(c_msg, NULL);
	}
	pthread_exit(NULL);
}

void cli_send()
{
	char message[MAXLINE];
	while(work)
	{
		scanf("%s" , message);
		int m_res=atoi(message);
		if(m_res>0||strcmp(message,"0")==0)
			send_num(sockfd, atof(message));
		else
			send_str(sockfd, message);

		if(strcmp(message, "terminate")==0)
			kill(0,SIGINT);
	}
}


int create_con(char *addr, int port)
{
	int fd;
	struct sockaddr_in servaddr;
	fd = socket(AF_INET,SOCK_STREAM,0);

	if(fd<0){
		perror("socket()");
		return fd;
	}

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	inet_pton(AF_INET,addr, &servaddr.sin_addr);

	socklen_t addr_size =sizeof(servaddr);
	if(connect(fd, (struct sockaddr*)&servaddr, addr_size)!=0)
	{
		perror("connect");
		return -1;
	}
	work=true;
	return fd;
}

