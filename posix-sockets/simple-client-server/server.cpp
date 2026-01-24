#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>

int main()
{
	int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_socket == -1)
	{
		perror("socket error");
		return 0;
	}
	else
	{
		std::cout << "socket() OK!" << std::endl;
	}
	sockaddr_in sockinfo;
	sockinfo.sin_addr.s_addr = INADDR_ANY;
	sockinfo.sin_family = AF_INET;
	sockinfo.sin_port = htons(8000);
	if (bind(server_socket, (sockaddr *)&sockinfo, sizeof(sockinfo)) == -1)
	{
		perror("bind error");
		return 0;
	}
	else
	{
		std::cout << "bind() OK!" << std::endl;
	}
	if (listen(server_socket, 10) == -1)
	{
		perror("listen error");
		return 0;
	}
	else
	{
		std::cout << "listen() OK!" << std::endl;
	}
	int client_socket = accept(server_socket, NULL, NULL);
	if (client_socket == -1)
	{
		perror("accept error");
		return 0;
	}
	else
	{
		std::cout << "accept() OK!" << std::endl;
	}
	char msg[40];
	while (recv(client_socket, &msg, 40, 0) != 0)
		;
	std::cout << msg << std::endl;

	close(client_socket);
	close(server_socket);
	return 0;
}
