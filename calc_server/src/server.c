/*
 * server.c
 *
 *  Created on: Jul 25, 2016
 *      Author: devteam
 */

#include "server.h"

void sem_lock(sem_t *sem_)
{
	if(sem_wait(sem_)<0)
		perror("sem_wait()");
}
void sem_unlock(sem_t *sem_)
{
	if(sem_post(sem_)<0)
		perror("sem_post()");
}

int initServer(char *addr, int port)
{
	fd_arr_size =0;
	fd_arr = malloc(sizeof(int)*(fd_arr_size+1));

	int serv_fd;

	if((serv_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return SERV_ERR;


	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	inet_pton(AF_INET,addr, &serv_addr.sin_addr);

	//serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(serv_fd, (struct sockaddr*)&serv_addr,
			sizeof(serv_addr))<0)
		return SERV_ERR;

	if(listen(serv_fd, 100)<0)
			return SERV_ERR;

	return serv_fd;
}

void *get_sharedMem_addr(const char *name, int *fd)
{
	if((*fd = shm_open(name, O_CREAT | O_RDWR,
				S_IRWXU | S_IRWXG))<0)
		return NULL;
	ftruncate(*fd, SHARED_MEM_SIZE);

	void  *addr_ = mmap(NULL,SHARED_MEM_SIZE, PROT_READ|PROT_WRITE,
			MAP_SHARED,*fd,  0);
	return addr_;
}

void add_cld(int fd)
{
	pthread_mutex_lock(&client_mutex);

	fd_arr_size++;
	fd_arr = realloc(fd_arr, sizeof(int) *fd_arr_size );
	fd_arr[fd_arr_size-1] = fd;

	pthread_mutex_unlock(&client_mutex);
}
void rm_cld(int fd)
{
	pthread_mutex_lock(&client_mutex);

	int i;
	for(i=0;i<fd_arr_size;i++)
	{
		if(fd_arr[i]==fd)
		{
			int j;
			fd_arr_size--;
			for(j=i;j<fd_arr_size;j++)
				fd_arr[j]=fd_arr[j+1];
		}
	}
	if(fd_arr_size>0)
		fd_arr = realloc(fd_arr, sizeof(int) *fd_arr_size );

	pthread_mutex_unlock(&client_mutex);
}

int get_log_size()
{
	int i, size=0;
	for(i=0;i<MAX_LOG_LINES;i++)
		size+=log_line_size[i];
	return size;
}
//must be done with semaphore!!!!!
void write_log_shm(char *buffer, int len)
{
	sem_lock(sem);
	shared_mem_size+=strlen("log: ")+len+1;
	int fd;
	void *addr_ = get_sharedMem_addr(SHARED_MEM_NAME, &fd);

	int line_size = strlen("log: ")+len+1;
	if(lines_count<MAX_LOG_LINES)
		log_line_size[lines_count++] =line_size;
	else
	{
		int i, old_size = get_log_size();

		void *old_log=malloc(old_size);
		memcpy(old_log,addr_, old_size);
		memset(addr_, 0, old_size);
		old_size-=log_line_size[0];
		memcpy(addr_, old_log+log_line_size[0], old_size);

		for(i=0;i<MAX_LOG_LINES-1;i++)
			log_line_size[i] = log_line_size[i+1];

		log_line_size[MAX_LOG_LINES-1]=line_size;
		free(old_log);
	}
	strcat(addr_,"log: ");
	strcat(addr_, buffer);
	strcat(addr_, "\n");
	close(fd);

	sem_unlock(sem);
}
void write_log_file(char *buffer, int len)
{
	logFile=fopen("server.log","a+");
	fwrite("log: ", sizeof(char), strlen("log: "), logFile);
	fwrite(buffer, sizeof(char), len, logFile);
	fputc('\n',logFile);
	fclose(logFile);
}

void read_log_shm(int sock_d)
{
	int fd;
	void *addr_ = get_sharedMem_addr(SHARED_MEM_NAME, &fd);
	send_str(sock_d, addr_);
	close(fd);
}
void read_log_file(int sock_d)
{
	logFile=fopen("server.log","r");

	fseek(logFile,0L, SEEK_END);
	int size = ftell(logFile);
	rewind(logFile);

	char *msg_ = malloc(sizeof(char)*(size));
	fread(msg_, size-2, sizeof(char), logFile);
	msg_[size-2]='\n';
	msg_[size-1]='\0';
	send_str(sock_d, msg_);

	free(msg_);
	fclose(logFile);
}


void print_to_cl(char *msg)
{
	char *msg_=msg+strlen("server: ");
	puts(msg_);
	write_log_shm(msg_,strlen(msg_));
	int i;
	for(i=0;i<fd_arr_size; i++)
		send_str(fd_arr[i], msg);
}

void send_result(int sock_d, char *pattern ,  double num1,
		double num2, double result)
{
	char res_buf[BUFF_SIZE];
	sprintf(res_buf,pattern,num1, num2, result);

	write_log_shm(res_buf,strlen(res_buf));
	send_str(sock_d, res_buf);
}

void calculate(int sock_d,char *msg, double num1, double num2)
{
	if(strcmp(msg, "+")==0)
		send_result(sock_d,"%.2f+%.2f=%.2f\n",
					num1, num2,num1+num2);
	else if(strcmp(msg, "-")==0)
		send_result(sock_d,"%.2f-%.2f=%f\n",
					num1, num2,num1-num2);
	else if(strcmp(msg, "/")==0)
		send_result(sock_d,"%.2f/%.2f=%f\n",
					num1, num2,num1/num2);
	else if(strcmp(msg, "*")==0)
		send_result(sock_d,"%.2f*%.2f=%.2f\n",
					num1, num2,num1*num2);

}


void do_work(CalcMessage *c_msg, int sockd, bool *async,
		int *current, double *numbers)
{
	if(c_msg->id==MESSAGE)
	{
		if(*current>1)
		{
			if(*async)
			{
				int pid = fork();
				if(pid==0){
					sleep(3);
					calculate(sockd, c_msg->msg,
										numbers[0], numbers[1]);
					exit(EXIT_SUCCESS);
				}else *async=false;

			}else calculate(sockd, c_msg->msg,
					numbers[0], numbers[1]);
		}
	}
	else if(c_msg->id==NUMBER)
	{
		if(*current>1)
		{
			numbers[0]=numbers[1];
			(*current)--;
		}
		numbers[(*current)]=c_msg->num;

		(*current)++;
	}
}

bool check_for_commands(CalcMessage *c_msg,
		int client_fd, bool *async)
{
	if(c_msg->id!=NUMBER)
	{
		if(strcmp(c_msg->msg, "terminate")==0)
			return true;
		if(strcmp(c_msg->msg, "log")==0)
		{
			if(*async)
			{
				int pid = fork();
				if(pid==0){
					sleep(3);
					read_log_shm(client_fd);
					exit(EXIT_SUCCESS);
				}else *async=false;

			}else read_log_shm(client_fd);
			return true;
		}
		else if(strcmp(c_msg->msg, "async")==0)
		{
			*async=true;
			return true;
		}
	}

	return false;
}

void* client_handler(void *arg)
{
	int client_fd = *(int*)arg;
	add_cld(client_fd);

	kill(getpid(), NEW_CONN);

	int size;
	uint8_t buff[BUFF_SIZE];

	bool async = false;
	double numbers[2], current=0;

	while( (size = recv(client_fd , buff , BUFF_SIZE , 0)) > 0 )
	{
			CalcMessage *c_msg =calc_message__unpack(NULL, size, buff);

			if(!check_for_commands(c_msg, client_fd,&async))
					do_work(c_msg, client_fd,&async, &current, numbers);

			calc_message__free_unpacked(c_msg, NULL);
	}

	if(!server_disconn)
	{
		printf("closing connection with client %d\n", client_fd);
		rm_cld(client_fd);
		kill(getpid(), DEST_CONN);
		close(client_fd);
	}

	pthread_exit(NULL);
}
