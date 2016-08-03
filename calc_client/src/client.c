/*
 * client.c
 *
 *  Created on: Jul 27, 2016
 *      Author: devteam
 */

#include "client.h"

bool check_commands(char *str)
{
	bool digit=true;
	int i,strsize=strlen(str);
	if(status==CALCULATING)
	{
		for(i=0;i<strsize;i++)
			if(!isdigit(str[i]))
			{
				digit=false;
				i=0;
				break;
			}
		if(digit)return true;
	}else i=4;

	for(;i<VALID_COMMANDS;i++)
		if(strcmp(str,valid_commands[i])==0)
			return true;

	return false;
}


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

			if(strncmp(c_msg->msg, serv_discon_msg,
				strlen(serv_discon_msg))==0)
			{
					puts("!-->server has disconnected\n");
					work=false;
					kill(getpid(),SIGINT);
			}
			if(strncmp(c_msg->msg,"log: ",
					strlen("log: "))==0)
				printf("%s\n",c_msg->msg);
			else if(status == MONITORING)
			{
				if(strncmp(c_msg->msg,"server: ",
						strlen("server: "))==0)
					printf("%s\n",c_msg->msg);
			}
			else if(status == CALCULATING)
			{
				if(strncmp(c_msg->msg,"server: ",
						strlen("server: "))!=0)
					printf("%s\n",c_msg->msg);
			}
		}

		calc_message__free_unpacked(c_msg, NULL);
	}
	pthread_exit(NULL);
}

void cli_send()
{
	char message[MAXLINE];
	puts("\n--->you are in DEFAULT mode type:\n"
			"--->'monitor'-for MONITORING mode\n"
			"--->'calculate'-for CALCULATING mode\n");

	while(work)
	{
		scanf("%s" , message);
		if(!check_commands(message))
		{
			printf("--->err: '%s' is not valid command\n\n",message);
			continue;
		}
		double m_res=atof(message);
		if(m_res>0||strcmp(message,"0")==0)
			send_num(sockfd, m_res);
		else
			send_str(sockfd, message);

		if(strcmp(message, "terminate")==0)
			kill(0,SIGINT);
		else if(strcmp(message, "monitor")==0)
		{
			puts("--->you are in monitoring mode\n");
			status=MONITORING;
		}
		else if(strcmp(message, "calculate")==0)
		{
			puts("--->you are in calculating mode\n");
			status=CALCULATING;
		}

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
	status = DEFAULT;
	return fd;
}

