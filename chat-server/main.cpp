#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <string>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>
#include <thread>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <vector>
#pragma comment(lib, "Ws2_32.lib")

std::vector<SOCKET> clientList;



void handleClient(SOCKET client, int id, std::fstream& recent)
{	
	char buf[256];
	auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	std::tm local_tm; 
	localtime_s(&local_tm, &now_time);
	int n;
	while ((n = recv(client, buf, sizeof(buf) - 1, 0)) > 0) {
		buf[n] = 0;
		std::string mwid = std::to_string(id) + "%^%" + buf;
		printf("[%02d:%02d] Client %d: %s\n",
			local_tm.tm_hour,
			local_tm.tm_min,
			id, buf);
		recent << "[" << local_tm.tm_hour << ":" << local_tm.tm_min << "]" << " Client " << id << ": " << buf << std::endl;
		for (SOCKET clnt : clientList)
		{
			send(clnt, mwid.c_str(), mwid.length(), 0);
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

	auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	std::tm local_tm;
	localtime_s(&local_tm, &now_time);
	std::stringstream fnamestream;
	fnamestream << "Chat At " << local_tm.tm_mon << local_tm.tm_wday << local_tm.tm_hour << local_tm.tm_min << local_tm.tm_sec;
	std::fstream recent(fnamestream.str(), std::ios::out);
	while (true) {
		SOCKET acceptSock = accept(serverSocket, nullptr, nullptr);
		if (acceptSock == INVALID_SOCKET) continue;
		clientList.push_back(acceptSock);
		std::thread(handleClient, acceptSock, id++, std::ref(recent)).detach();
	}

	WSACleanup();
	return 0;
}