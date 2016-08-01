/*
 * message_utils.h

 *
 *  Created on: Jul 28, 2016
 *      Author: devteam
 *
 */

#ifndef MESSAGE_UTILS_H_
#define MESSAGE_UTILS_H_

#include "message.pb-c.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



static const int  NUMBER = 1;
static const int  MESSAGE = 2;

//gets the protobuffer of structure
uint8_t *get_buff(CalcMessage *c_msg, int *size);

//sends to the server "CalcMessage"
ssize_t send_msg(int sock_d, CalcMessage *c_msg);

//sends string to the server
ssize_t send_str(int sock_d, char *str);
//sends double to the server
ssize_t send_num(int sock_d, double num);



#endif /* MESSAGE_UTILS_H_ */
