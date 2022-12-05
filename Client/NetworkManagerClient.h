#pragma once
#undef UNICODE

#include <string>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <ws2def.h>
#include <iostream>


#pragma comment(lib, "Ws2_32.lib")

class NetworkManagerClient
{
public:
	NetworkManagerClient();
	~NetworkManagerClient();

	void TCPConnect();
	void Test();

	SOCKET ConnectSocket;

	int iResult;

private:

	std::string ipAddr;
};

NetworkManagerClient::NetworkManagerClient()
{
	std::cout << "Enter IP Address of the peer to connect: ";
	std::cin >> ipAddr;

	TCPConnect();
}

NetworkManagerClient::~NetworkManagerClient()
{
}

void NetworkManagerClient::TCPConnect()
{
	WSADATA wsaData;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	//create information for later socket definition, connecting at port 567
	iResult = getaddrinfo(ipAddr.c_str(), "567", &hints, &result);

	ptr = result;

	//create socket for connection
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	//connect if addr found
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
	}

	//test connection
	Test();

}

void NetworkManagerClient::Test()
{
	int recvbuflen = 56;

	char recvbuf[56];

	// Receive data until the server closes the connection
	do {
		//receive welcome paket from peer
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);
			printf(recvbuf);
			printf("\n\n");
			break;
		}
		else if (iResult == 0)
		{
			printf("Connection closed\n");
			break;
		}
		else
		{
			printf("recv failed: %d\n", WSAGetLastError());
			break;
		}
	} while (true);
}


