// ServerApplication.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "stdafx.h"
//#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <IPHlpApi.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512



void initWinSocket();



int main(int argc, const char* argv[])
{
	WSADATA wsaData;
	bool end = false;
	int recvbuflen = DEFAULT_BUFLEN;

	char *sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];

	SOCKET ConnectSocket = INVALID_SOCKET;

	int iResult;

	do
	{
		/*Initialize Winsock*/
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0)
		{
			printf("WSAStartup failed: %d\n", iResult);
			return 1;
		}

		struct addrinfo *result = NULL,
			*ptr = NULL,
			hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;		//AF_INET6 für IPv6 oder AF_INET für IPv4 
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		//Resolve the server andress and port
		iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
		if (iResult != 0)
		{
			printf("getaddrinfo failed: %d\n", iResult);
			WSACleanup();
			return 1;
		}

		/*Attempt to connect to the first address returned by
		the call to getaddrinfo*/
		ptr = result;

		//Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET)
		{
			printf("Error to socket(): %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return 1;
		}

		//Connect to server
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
		}

		/*Should really try the next address returned by getaddrinfo
		if the connect call failed
		But for this simple example we just free the resources 
		returned by getaddrinfo and print an error message*/

		freeaddrinfo(result);

		if (ConnectSocket == INVALID_SOCKET)
		{
			printf("Unable to connect to server!\n");
			WSACleanup();
			return 1;
		}

		//Send an initial buffer
		iResult = send(ConnectSocket, sendbuf, strlen(sendbuf), 0);
		if (iResult == SOCKET_ERROR)
		{
			printf("send failed: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}

		printf("Bytes Sent: %ld\n", iResult);

		/*Shutdown the connection for sending since no more data will be sent
		the client can still use the ConnectSocket for receiving data */
		iResult = shutdown(ConnectSocket, SD_SEND);
		if (iResult == SOCKET_ERROR)
		{
			printf("shutdown failed: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}
		//Receive data until the server closes the connection
		do
		{
			iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
			if (iResult > 0)
			{
				printf("Bytes received: %d\n", iResult);
			}
			else if (iResult == 0)
			{
				printf("Conenction closed\n");
			}
			else
			{
				printf("recv failed: %d\n", WSAGetLastError());
			}
		} while (iResult > 0);

		/*shutdown the send half of the connection since no more data wil be sent*/
		iResult = shutdown(ConnectSocket, SD_SEND);
		if (iResult == SOCKET_ERROR)
		{
			printf("shutdown failed: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}

		//Cleanup
		closesocket(ConnectSocket);
		WSACleanup();

		std::cout << "Wollen Sie die Konsole beenden?" << std::endl;
		std::cin >> end;

	}while (end != true);

    return 0;
}




void initWinSocket()
{
	
}