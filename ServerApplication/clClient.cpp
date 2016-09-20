#include "stdafx.h"
#include "clClient.h"


clClient::clClient()
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	this->adress = " ";
	this->port = " ";
	this->iResult = 0;
	this->recvbuflen = 512;
	this->recbuf[recvbuflen];

}


clClient::~clClient()
{
	closesocket(this->ConnectSocket);
	WSACleanup();
}

clClient::clClient(const char * adress, const char *port, int recvbuflen)
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	this->adress = adress;
	this->iResult = 0;
	this->recvbuflen = recvbuflen;
	this->recbuf[recvbuflen];
	this->port = port;

}

int clClient::initSocket()
{
	this->iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (this->iResult != 0)
	{
		printf("WSAStartup failed: %d\n", this->iResult);
		//cout << "WSAStartup failed: << this->iResult <<endl;
		return 1;
	}
	else
	{
		return 0;
	}
}

int clClient::initSocketParam()
{
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	this->iResult = getaddrinfo(adress, port, &hints, &result);
	if (this->iResult != 0)
	{
		printf("getaddrinfo failed: %d\n", this->iResult);
		WSACleanup();
		return 1;
	}
	else
	{
		return 0;
	}
}

int clClient::createSocket()
{
	this->ptr = this->result;

	/*Create a SOCKET for connecting to server*/
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Error at socket(): %d\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	else
	{
		return 0;
	}
}

int clClient::connectToServer()
{
	this->iResult = connect(this->ConnectSocket, this->ptr->ai_addr, (int)this->ptr->ai_addrlen);
	if (this->iResult == SOCKET_ERROR)
	{
		closesocket(this->ConnectSocket);
		this->ConnectSocket = INVALID_SOCKET;
	}

	// Should really try the next address returned by getaddrinfo
	// if the connect call failed
	// But for this simple example we just free the resources
	// returned by getaddrinfo and print an error message

	freeaddrinfo(this->result);

	if (this->ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}
	else
	{
		return 0;
	}
}

int clClient::sendData(char *sendbuf)
{
	this->iResult = send(this->ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (this->iResult == SOCKET_ERROR)
	{
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("Bytes Sent: %ld\n", iResult);

		this->iResult = shutdown(ConnectSocket, SD_SEND);
		if (this->iResult == SOCKET_ERROR) {
			printf("shutdown failed: %d\n", WSAGetLastError());
			closesocket(this->ConnectSocket);
			WSACleanup();
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

void clClient::getData()
{
	do 
	{
		this->iResult = recv(this->ConnectSocket, this->recbuf, this->recvbuflen, 0);
		if (this->iResult > 0)
		{
			printf("Bytes received: %d\n", this->iResult);
		}
		else if (this->iResult == 0)
		{
			printf("Connection closed\n");
		}
		else
		{
			printf("recv failed: %d\n", WSAGetLastError());
		}
	} while (this->iResult > 0);
}

int clClient::disconnectConnection()
{
	this->iResult = shutdown(ConnectSocket, SD_SEND);
	if (this->iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(this->ConnectSocket);
		WSACleanup();
		return 1;
	}


}