#pragma once
#include <WinSock2.h>

class Network
{
public:
	const static u_short port = 8000;
	static void Startup();
	static SOCKET NewSocket();
	static void ServerSocketStartup(SOCKET);
	static SOCKET Accept(SOCKET);
};