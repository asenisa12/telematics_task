/*
 * message_utils.c
 *
 *  Created on: Jul 28, 2016
 *      Author: devteam
 */

#include "message_utils.h"



uint8_t *get_buff(CalcMessage *c_msg, int *size)
{
	uint8_t *buff;

	*size = calc_message__get_packed_size(c_msg);
	buff = malloc(*size);
	calc_message__pack(c_msg, buff);

	return buff;
}


ssize_t send_msg(int sock_d, CalcMessage *c_msg)
{
	int buff_size;

	uint8_t *buff = get_buff(c_msg, &buff_size);
	ssize_t wr_bytes = write(sock_d, buff, buff_size);

	free(buff);

	return wr_bytes;
}


ssize_t send_str(int sock_d, char *str)
{
	CalcMessage c_msg;
	calc_message__init(&c_msg);
	c_msg.id = MESSAGE;

	int len = strlen(str);
	c_msg.msg=malloc(sizeof(char) * (len+1));
	strcpy(c_msg.msg, str);

	ssize_t wr_bytes;
	if((wr_bytes = send_msg(sock_d, &c_msg)) < 0)
				puts("Send failed");

	free(c_msg.msg);
	return wr_bytes;
}
ssize_t send_num(int sock_d, double num)
{
	CalcMessage c_msg;
	calc_message__init(&c_msg);
	c_msg.id = NUMBER;
	c_msg.has_num=1;
	c_msg.num = num;

	ssize_t wr_bytes;
	if((wr_bytes = send_msg(sock_d, &c_msg)) < 0)
				puts("Send failed");

	return wr_bytes;
}
