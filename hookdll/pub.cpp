#include "stdafx.h"
#include <stdio.h>
#include "pub.h"

#pragma comment(lib, "Ws2_32.lib")

int init_socket()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}
	return 0;
}

void close_socket(SOCKET st)
{
	closesocket(st);
	WSACleanup();
}

SOCKET create_send_socket()
{
	SOCKET st = socket(AF_INET, SOCK_DGRAM, 0);//½¨Á¢UDP socket
	if (st ==0)
		return 0;
	return st;
}

int send_work(SOCKET st, const char *hostname, int port, const char *s)
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(hostname);

	size_t rc = sendto(st, s, strlen(s), 0, (struct sockaddr *)&addr,sizeof(addr));

	return rc;
}