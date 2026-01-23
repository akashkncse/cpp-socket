#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>

int main()
{
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		perror("socket");
		return 1;
	}
	sockaddr_in addr{};
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);

	if (bind(server_fd, (sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("bind error");
		return 1;
	}

	if (listen(server_fd, 10) < 0)
	{
		perror("listen error");
		return 1;
	}

	std::cout << "server listening on 8000";
}
