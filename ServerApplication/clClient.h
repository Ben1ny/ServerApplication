#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <IPHlpApi.h>
#include <stdio.h>
//#include <iostream>

class clClient
{
public:
	clClient();
	clClient(const char *adress, const char *port,int recvbuflen);
	~clClient();

	int initSocket();
	int initSocketParam();
	int createSocket();
	int connectToServer();
	int sendData(char *sendbuf);
	void getData();
	int disconnectConnection();

private:
	WSADATA wsaData;
	SOCKET ConnectSocket;
	int iResult;
	int recvbuflen;
	char *recbuf;
	const char *adress;
	const char *port;

	struct addrinfo *result = NULL,
					*ptr = NULL,
					hints;

};



