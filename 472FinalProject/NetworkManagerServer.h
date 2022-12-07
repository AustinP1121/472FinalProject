#pragma once
#pragma warning(disable:4996)
#undef UNICODE

#include <string>
#include <WinSock2.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

class NetworkManagerServer
{
public:
	NetworkManagerServer();
	NetworkManagerServer(std::string);

	~NetworkManagerServer();

	void TCPConnect();
	void ConfirmConnectionMsg();

	SOCKET ListenSocket;
	SOCKET TempClientSocket;

	int iResult;

private:
	std::string ipAddr;

};

NetworkManagerServer::NetworkManagerServer()
{
	std::cout << "Enter IP Address of this server: ";
	std::cin >> ipAddr;

	TCPConnect();
}

NetworkManagerServer::NetworkManagerServer(std::string addr)
{
	ipAddr = addr;
	TCPConnect();
}

NetworkManagerServer::~NetworkManagerServer()
{
}

void NetworkManagerServer::TCPConnect()
{
	WSADATA wsaData;

	sockaddr_in service;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	//socket for listening on port
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(ipAddr.c_str());
	//connect on port 567
	service.sin_port = htons(567);

	//bind socket to service addr
	bind(ListenSocket, (SOCKADDR*)&service, sizeof(service));

	//listen for active connection request
	listen(ListenSocket, 0);

	//Socket for the connection
	TempClientSocket = accept(ListenSocket, NULL, NULL);

	ConfirmConnectionMsg();
}

void NetworkManagerServer::ConfirmConnectionMsg()
{
	char recvbuf[56] = "Connection Established";
	int iSendResult;
	int recvbuflen = 56;

	do {
		//send a welcome packet to peer to confirm connection
		iSendResult = send(TempClientSocket, recvbuf, recvbuflen, 0);
		if (iSendResult > 0) {
			printf("Bytes sent: %d\n\n\n", iSendResult);
			break;
		}
		else
		{
			printf("send failed: %d\n\n\n", WSAGetLastError());
			break;
		}
	} while (true);
}
