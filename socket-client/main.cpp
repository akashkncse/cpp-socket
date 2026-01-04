#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <cstring>

#pragma comment(lib, "Ws2_32.lib")
#include "Network.h"
int main()
{
	Network::Startup();
	SOCKET clientSocket = Network::NewSocket();
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	u_short port = Network::port;
	InetPton(AF_INET, L"127.0.0.1", &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(port);
	if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
	{
		std::cout << "Client: connect() - Failed to connect." << std::endl;
		WSACleanup();
		return 0;
	}
	else {
		std::cout << "Client: connect() is OK." << std::endl;
		std::cout << "Client: Can start sending and receiving data..." << std::endl;
	}
	/*char rebuf[300] = "";
	int totalBytes = 0;
	int bytesReceived = 0;
	do {
		bytesReceived = recv(clientSocket, rebuf + totalBytes, sizeof(rebuf) - totalBytes - 1, 0);

		if (bytesReceived > 0) {
			totalBytes += bytesReceived;
		}
	} while (bytesReceived > 0 && totalBytes < sizeof(rebuf) - 1);
	if (bytesReceived == SOCKET_ERROR) {
		std::cout << "recv() failed: " << WSAGetLastError() << std::endl;
	}
	std::cout << rebuf << std::endl;*/
	const int size = 100;
	char msg[size];
	std::cin.getline(msg, size);
	int msglen = strlen(msg) + 1; // for '\0' at the end
	int msgsenlen = 0;
	do {
		int x = send(clientSocket, msg + msgsenlen, msglen, 0);
		if (x == SOCKET_ERROR) {
			std::cout << "send() failed: " << WSAGetLastError() << std::endl;
			break;
		}
		else {
			msgsenlen += x;
		}
	} while (msgsenlen < msglen);

	WSACleanup();
	return 0;
}