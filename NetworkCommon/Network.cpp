#include <Network.h>
#include <iostream>
#include <WinSock2.h>
void Network::Startup()
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int wsaerr;
	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr != 0)
	{
		std::cout << "Failed! Winsock DLL not found!" << std::endl;
	}
	else {
		std::cout << "Success!" << std::endl;
		std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
	}
}

SOCKET Network::NewSocket() {
	SOCKET output_socket = INVALID_SOCKET;
	output_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (output_socket == INVALID_SOCKET)
	{
		std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
		WSACleanup();
		return INVALID_SOCKET;
	}
	std::cout << "socket is OK!" << std::endl;
	return output_socket;
}

void Network::ServerSocketStartup(SOCKET serverSocket) {
	sockaddr_in service;
	service.sin_family = AF_INET;
	u_short port = Network::port;
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
}

SOCKET Network::Accept(SOCKET serverSocket) {
	SOCKET acceptSocket;
	acceptSocket = accept(serverSocket, NULL, NULL);
	if (acceptSocket == INVALID_SOCKET)
	{
		std::cout << "accept() failed: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return INVALID_SOCKET;
	}
	else {
		std::cout << "accept() is OK!" << std::endl;
		return acceptSocket;
	}
}