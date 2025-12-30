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
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int wsaerr;
	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr != 0)
	{
		std::cout << "Failed! Winsock DLL not found!" << std::endl;
		return 0;
	}
	else {
		std::cout << "Success!" << std::endl;
		std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
	}
	SOCKET serverSocket = INVALID_SOCKET;
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET)
	{
		std::cout << "Eror at socket(): " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}
	else {
		std::cout << "socket is OK!" << std::endl;
	}
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


	closesocket(serverSocket);
	WSACleanup();
}