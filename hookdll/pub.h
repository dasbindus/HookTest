/*
 * pub.h
 *
 *  Created on: 2013Äê12ÔÂ17ÈÕ
 *      Author: zhujy
 */

#ifndef PUB_H_
#define PUB_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <WinSock2.h>

#define BUFSIZE 32768  //32k

int init_socket();
void close_socket(SOCKET st);
SOCKET create_send_socket();
int send_work(SOCKET st, const char *hostname, int port, const char *s);

#endif /* PUB_H_ */
