#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")


int main()
{
	// Winsock stuff
	WSADATA wsaData;
	WORD wVersionReq = MAKEWORD(2, 2);
	int wsaerr;
	wsaerr = WSAStartup(wVersionReq, &wsaData);
	if (wsaerr != 0) {
		std::cout << "Failed to launch server! " << WSAGetLastError() << std::endl;
	}
	else {
		std::cout << "Success!" << std::endl;
		std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
	}

	// Creating the actual socket that will listen
	SOCKET serverSocket = INVALID_SOCKET;
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) {
		std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
		return -1;
	}
	else {
		std::cout << "Success! serversocket is OK!" << std::endl;
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	u_short port = 8000;
	service.sin_addr.s_addr = htonl(INADDR_ANY);
	service.sin_port = htons(port);
	if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
		std::cout << "bind() error: " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
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

	while (true) {
		
	}

}