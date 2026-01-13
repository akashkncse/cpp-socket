#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <iostream>
#include <vector>
#pragma comment(lib, "Ws2_32.lib")

std::vector<SOCKET> clientList;


void handleClient(SOCKET client, int id)
{	
		char buf[256];
		int n;
		while ((n = recv(client, buf, sizeof(buf) - 1, 0)) > 0) {
			buf[n] = 0;
			printf("Client %d: %s\n", id, buf);
			for (SOCKET clnt : clientList)
			{
				send(clnt, buf, n, 0);
			}
		}
		closesocket(client);
}

int main()
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int wsaerr;
	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr != 0)
	{
		std::cout << "Failed! Winsock DLL not found!" << std::endl;
		return 0;
	}
	else
	{
		std::cout << "Success!" << std::endl;
		std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
	}

	SOCKET serverSocket = INVALID_SOCKET;
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET)
	{
		std::cout << "socket() failed: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 0;
	}
	else
	{
		std::cout << "SOCKET OK!" << std::endl;
	}

	sockaddr_in service;
	service.sin_addr.s_addr = htonl(INADDR_ANY);
	service.sin_family = AF_INET;
	service.sin_port = htons(8000);
	if (bind(serverSocket, (sockaddr*)&service, sizeof(service)) == SOCKET_ERROR) {
		std::cout << "bind() failed: " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}
	else 
	{
		std::cout << "BIND OK!" << std::endl;
	}
	if (listen(serverSocket, 5) == SOCKET_ERROR) {
		std::cout << "listen() failed: " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}
	else
	{
		std::cout << "LISTEN OK!" << std::endl;
	}
	int id = 1;
	while (true) {
		SOCKET acceptSock = accept(serverSocket, nullptr, nullptr);
		if (acceptSock == INVALID_SOCKET) continue;
		clientList.push_back(acceptSock);
		std::thread(handleClient, acceptSock, id++).detach();
	}

	WSACleanup();
	return 0;
}