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

	//InetPton(AF_INET, _T("172.26.32.1"), &service.sin_addr.s_addr); <- Specific IP
	Network::ServerSocketStartup(serverSocket);
	SOCKET acceptSocket = Network::Accept(serverSocket);
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