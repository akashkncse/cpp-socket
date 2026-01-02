#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <Network.h>
#pragma comment(lib, "Ws2_32.lib")


int main()
{
	Network::Startup();
	SOCKET serverSocket = Network::NewSocket();

	sockaddr_in service;
	service.sin_family = AF_INET;
	u_short port = 8000;
	//InetPton(AF_INET, _T("172.26.32.1"), &service.sin_addr.s_addr); <- Specific IP
	service.sin_addr.s_addr = htonl(INADDR_ANY);
	service.sin_port = htons(port);
	if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
		std::cout << "bind() error: " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}
	else {
		std::cout << "bind() is OK!" << std::endl;
	}
	if (listen(serverSocket, 1) == SOCKET_ERROR) {
		std::cout << "listen(): Error listening on socket " << WSAGetLastError() << std::endl;
	}
	else {
		std::cout << "listen() OK!, waiting for connections..." << std::endl;
	}
	SOCKET acceptSocket;
	acceptSocket = accept(serverSocket, NULL, NULL);
	if (acceptSocket == INVALID_SOCKET)
	{
		std::cout << "accept() failed: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return -1;
	}
	else {
		std::cout << "accept() is OK!" << std::endl;
	}
	char msg[100];
	int bytesReceived = 0;
	while (true) {
		int x = recv(acceptSocket, msg + bytesReceived, 100, 0);
		if (x == SOCKET_ERROR) {
			break;
		}
		bytesReceived += x;
		std::cout << msg << std::endl;
	}

	WSACleanup();
}