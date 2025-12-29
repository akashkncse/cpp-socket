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
	SOCKET clientSocket = INVALID_SOCKET;
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
	{
		std::cout << "Eror at socket(): " << WSAGetLastError() << std::endl;
		closesocket(clientSocket);
		WSACleanup();
		return 0;
	}
	else {
		std::cout << "socket is OK!" << std::endl;
	}
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	u_short port = 8000;
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
	WSACleanup();
	return 0;
}