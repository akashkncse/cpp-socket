#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#pragma comment(lib, "Ws2_32.lib")

//void handleClient(SOCKET client, int id)
//{
//	while (true)
//	{
//		char buf[256];
//		int n;
//		while ((n = recv(client, buf, sizeof(buf) - 1, 0)) > 0) {
//			buf[n] = 0;
//			printf("Client %d: %s\n", id, buf);
//			for (SOCKET clnt : clientList)
//			{
//				send(clnt, buf, sizeof(buf), 0);
//			}
//		}
//	}
//
//	closesocket(client);
//}

int CLIENTid = -1;


void incoming(SOCKET server)
{
	while (true)
	{
		char buf[256];
		int n;
		while ((n = recv(server, buf, sizeof(buf) - 1, 0)) > 0) {
			buf[n] = 0;
			std::string buffer = std::string(buf);
			int f = buffer.find("%^%");
			int id = std::atoi(buffer.substr(0, f).c_str());
			std::string mess = buffer.substr(f + 3);
			printf("%d, %s\n",id, mess.c_str());
		}
	}
}


void getId(SOCKET server)
{
	char buf[256];
	int n;
	std::string cid;
	n = recv(server, buf, sizeof(buf) - 1, 0);
	buf[n] = 0;
	cid += std::string(buf);
	CLIENTid = std::stoi(cid);
	std::cout << "YOUR ID IS " << CLIENTid << std::endl;
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

	SOCKET clientSocket = INVALID_SOCKET;
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
	{
		std::cout << "socket() failed: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 0;
	}
	else
	{
		std::cout << "SOCKET OK!" << std::endl;
	}
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	InetPton(AF_INET, L"127.0.0.1", &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(8000);
	if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
	{
		std::cout << "Client: connect() - Failed to connect: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 0;
	}
	else {
		std::cout << "Client: connect() is OK." << std::endl;
		std::cout << "Client: Can start sending and receiving data..." << std::endl;
	}
	getId(clientSocket);
	std::string line;
	std::thread(incoming, clientSocket).detach();
	std::cout << "You can enter the messages in the terminal to chat in the room\n";
	while (true)
	{
		std::getline(std::cin, line);
		if (line == "exit") goto end;
		int sent = send(clientSocket, line.c_str(), (int)line.size(), 0);
		if (sent == SOCKET_ERROR) {
			std::cout << "send() failed: " << WSAGetLastError() << std::endl;
		}
		else {
			std::cout << "send() OK!" << std::endl;
		}
	}
	end: 
	WSACleanup();
	return 0;
}