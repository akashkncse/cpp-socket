#pragma once
#include <WinSock2.h>

class Network
{
public:
	static void Startup();
	static SOCKET NewSocket();
	static void ServerSocketStartup(SOCKET);
};